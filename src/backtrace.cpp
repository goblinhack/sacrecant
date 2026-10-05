//
// Copyright goblinhack@gmail.com
//
//
#include "my_backtrace.hpp" // NOLINT
#include "my_main.hpp"      // NOLINT
#include "my_string.hpp"    // NOLINT
#include "my_wids.hpp"

#include <cstdio>
#include <cstdlib>
#include <format>
#include <string>

#ifdef _WIN32
// clang-format off
#include <stdio.h>   
#include <windows.h> 
#include <shlwapi.h> 
#include <winbase.h> 
#include <dbghelp.h>
// clang-format on
#endif

#ifdef __has_include
#if __has_include(<execinfo.h>)
#include <execinfo.h> // NOLINT
#endif
#endif

#ifdef __has_include
#if __has_include(<libunwind.h>)
#include <libunwind.h> // NOLINT
#define HAVE_LIBUNWIND
#endif
#endif

#include <array>
#include <cerrno>
#include <cstring>
#include <cxxabi.h>
#include <mutex>
#include <print>

static std::recursive_mutex backtrace_mutex;

#ifndef __EMSCRIPTEN__
//
// Inspired from https://github.com/nico/demumble/issues
//
[[nodiscard]] static auto starts_with(const char *s, const char *prefix) -> bool { return strncmp(s, prefix, strlen(prefix)) == 0; }

[[nodiscard]] static auto is_mangle_char_win(char c) -> bool
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (strchr("?_@$", c) != nullptr);
}

[[nodiscard]] static auto is_mangle_char_posix(char c) -> bool
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

[[nodiscard]] static auto is_plausible_posix_prefix(char *s) -> bool
{
  // POSIX symbols start with 1-4 underscores followed by Z.
  // strnstr() is BSD, so use a small local buffer and strstr().
  const int N = 5; // == strlen("____Z")
  char      prefix[ N + 1 ];
  (void) my_strlcpy(prefix, s, N);
  prefix[ N ] = '\0';
  return strstr(prefix, "_Z") != nullptr;
}

//
// See
// https://stackoverflow.com/questions/4939636/function-to-mangle-demangle-functions
//
// See also c++filt e.g.:
//
// c++filt -n _ZNK3MapI10StringName3RefI8GDScriptE10ComparatorIS0_E16DefaultAllocatorE3hasERKS0_
// Map<StringName, Ref<GDScript>, Comparator<StringName>, DefaultAllocator>::has(StringName const&) const
//
static auto demangle(const char *name) -> std::string
{

  int status = -4; // some arbitrary value to eliminate the compiler warning

  auto *p = abi::__cxa_demangle(name, nullptr, nullptr, &status);

  if (status == 0) {
    auto ret = std::string(p);
    free(p);
    return ret;
  }

  free(p);
  return "";
}

static auto demangle_symbol(char *name) -> std::string
{
  std::string sout;
  char       *p         = name;
  char       *cur       = p;
  char const *end       = p + strlen(cur);
  bool        demangled = false;

  while (cur < end) {
    size_t const special = strcspn(cur, "_?");
    cur += special;

    if (cur >= end) {
      break;
    }

    size_t n_sym = 0;
    if (*cur == '?') {
      while (cur + n_sym != end && is_mangle_char_win(cur[ n_sym ])) {
        ++n_sym;
      }
    } else if (is_plausible_posix_prefix(cur)) {
      while (cur + n_sym != end && is_mangle_char_posix(cur[ n_sym ])) {
        ++n_sym;
      }
    } else {
      ++cur;
      continue;
    }

    char const tmp = cur[ n_sym ];
    cur[ n_sym ]   = '\0';

    if (starts_with(cur, "__Z")) {
      cur++;
    }

    auto was_demangled = demangle(cur);
    if (! was_demangled.empty()) {
      sout += std::format("{}", was_demangled);
      demangled = true;
      break;
    }

    cur[ n_sym ] = tmp;
    cur += n_sym;
  }

  if (! demangled) {
    //
    // Not demangled
    //
    sout += std::format("{}", p);
  }

  return sout;
}
#endif

#ifdef _WIN32
static void PrintLastError(const char *msg)
{
  DWORD errCode = GetLastError();
  char *err;
  if (! FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errCode,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
                      (LPTSTR) &err, 0, NULL))
    return;

  static char buffer[ 1024 * 32 ];
  snprintf(buffer, sizeof(buffer), "ERROR: %s: %s\n", msg, err);
  OutputDebugString(buffer);
  LocalFree(err);
}

std::string backtrace_string(void)
{
  backtrace_mutex.lock();
  std::string out = "stack trace\n===========\n";

  HANDLE        thread            = GetCurrentThread();
  HANDLE        handle            = GetCurrentProcess();
  constexpr int frames_to_capture = 16;

  if (! SymInitialize(handle, nullptr, true)) {
    backtrace_mutex.unlock();
    PrintLastError("SymInitialize failed");
    return "<failed to collect backtrace>";
  }

  SymSetOptions(SymGetOptions() | SYMOPT_LOAD_LINES);

  bool do_stackwalk = false;
  if (do_stackwalk) {
    CONTEXT context;
    memset(&context, 0, sizeof(CONTEXT));
    context.ContextFlags = CONTEXT_FULL;
    RtlCaptureContext(&context);

    DWORD        imageType;
    STACKFRAME64 stackframe;
    memset(&stackframe, 0, sizeof(stackframe));

#ifdef _M_IX86
    // normally, call ImageNtHeader() and use machine info from PE header
    imageType                   = IMAGE_FILE_MACHINE_I386;
    stackframe.AddrPC.Offset    = context.Eip;
    stackframe.AddrPC.Mode      = AddrModeFlat;
    stackframe.AddrFrame.Offset = context.Ebp;
    stackframe.AddrFrame.Mode   = AddrModeFlat;
    stackframe.AddrStack.Offset = context.Esp;
    stackframe.AddrStack.Mode   = AddrModeFlat;
#elif _M_X64
    imageType                   = IMAGE_FILE_MACHINE_AMD64;
    stackframe.AddrPC.Offset    = context.Rip;
    stackframe.AddrPC.Mode      = AddrModeFlat;
    stackframe.AddrFrame.Offset = context.Rsp;
    stackframe.AddrFrame.Mode   = AddrModeFlat;
    stackframe.AddrStack.Offset = context.Rsp;
    stackframe.AddrStack.Mode   = AddrModeFlat;
#elif _M_IA64
    imageType                    = IMAGE_FILE_MACHINE_IA64;
    stackframe.AddrPC.Offset     = context.StIIP;
    stackframe.AddrPC.Mode       = AddrModeFlat;
    stackframe.AddrFrame.Offset  = context.IntSp;
    stackframe.AddrFrame.Mode    = AddrModeFlat;
    stackframe.AddrBStore.Offset = context.RsBSP;
    stackframe.AddrBStore.Mode   = AddrModeFlat;
    stackframe.AddrStack.Offset  = context.IntSp;
    stackframe.AddrStack.Mode    = AddrModeFlat;
#elif _M_ARM64
    imageType                   = IMAGE_FILE_MACHINE_ARM64;
    stackframe.AddrPC.Offset    = context.Pc;
    stackframe.AddrPC.Mode      = AddrModeFlat;
    stackframe.AddrFrame.Offset = context.Fp;
    stackframe.AddrFrame.Mode   = AddrModeFlat;
    stackframe.AddrStack.Offset = context.Sp;
    stackframe.AddrStack.Mode   = AddrModeFlat;
#else
#error "Platform not supported!"
#endif

    for (int i = 0; i < frames_to_capture; i++) {
      BOOL result = StackWalk64(imageType, handle, thread, &stackframe, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL);
      if (! result) {
        out += "StackWalk[end]\n";
        break;
      }

      char         buffer[ sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR) ];
      PSYMBOL_INFO symbol  = (PSYMBOL_INFO) buffer;
      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
      symbol->MaxNameLen   = MAX_SYM_NAME;

      DWORD64 displacement = 0;
      if (SymFromAddr(handle, stackframe.AddrPC.Offset, &displacement, symbol)) {
        out += std::format("StackWalk[{}]: sym:{}\n", i, symbol->Name);
      }
    }
  }

#define MAX_SYMBOL_LEN 1024

  char         symbol_mem[ SIZEOF(SYMBOL_INFO) + MAX_SYMBOL_LEN * SIZEOF(TCHAR) ];
  SYMBOL_INFO *symbol  = (SYMBOL_INFO *) symbol_mem;
  symbol->MaxNameLen   = MAX_SYMBOL_LEN;
  symbol->SizeOfStruct = SIZEOF(SYMBOL_INFO);

  IMAGEHLP_LINE64 line = {};
  line.SizeOfStruct    = sizeof(IMAGEHLP_LINE64);

  std::array< void *, frames_to_capture > frames;
  int                                     frames_to_skip = 0;
  int                                     frame_count    = CaptureStackBackTrace(frames_to_skip, frames_to_capture, frames.data(), NULL);

  for (int i = 0; i < frame_count; i++) {
    DWORD64 addr = (DWORD64) frames[ i ];

    const char *file         = "<no-file>";
    int         line_number  = -1;
    DWORD       displacement = 0;

    if (SymGetLineFromAddr64(handle, addr, &displacement, &line)) {
      file        = line.FileName;
      line_number = (int) line.LineNumber;
    }

    if (SymFromAddr(handle, addr, nullptr, symbol)) {
      char *function_name = symbol->Name;
      auto  sym           = demangle_symbol(function_name);
      out += std::format("CaptureStackBackTrace[{}]: {}() {}:{}\n", i - frames_to_skip, sym.c_str(), file, line_number);
    } else {
      out += std::format("CaptureStackBackTrace[{}]: {}:{}\n", i - frames_to_skip, file, line_number);
    }
  }
  backtrace_mutex.unlock();

  PrintLastError(out.c_str());
  // __debugbreak();

  SymCleanup(handle);
  CloseHandle(thread);

  return out;
}
#elif __EMSCRIPTEN__
[[nodiscard]] auto backtrace_string() -> std::string { return "<na>"; }
#else
[[nodiscard]] auto backtrace_string() -> std::string
{
  backtrace_mutex.lock();

  static const int                    max_backtrace = 63;
  std::array< void *, max_backtrace > bt {};
  int                                 size {};

#ifdef HAVE_LIBUNWIND

#ifdef LIBUNWIND_HAS_UNW_BACKTRACE
  size               = unw_backtrace(&bt[ 0 ], max_backtrace);
  const char *prefix = "(unw_backtrace) ";
#else
  size               = backtrace(bt.data(), max_backtrace);
  const char *prefix = "(backtrace) ";
#endif

#else
  size               = backtrace(&bt[ 0 ], max_backtrace);
  const char *prefix = "(backtrace) ";
#endif

  auto       *addrlist = bt.data();
  std::string sout     = "stack trace\n===========\n";

  if (size == 0) {
    sout += "  <empty, possibly corrupt>\n";
    backtrace_mutex.unlock();
    return sout;
  }

  // resolve addresses into strings containing "filename(function+address)",
  // this array must be free()-ed
  char **symbollist = backtrace_symbols(addrlist, size);

  // address of this function.
  for (int i = size - 1; i >= 0; i--) {
    sout += prefix + demangle_symbol(symbollist[ i ]) + "\n";
  }

  free(symbollist);

  backtrace_mutex.unlock();
  return sout;
}
#endif

void backtrace_dump(FILE *fp)
{
  backtrace_mutex.lock();
  auto bt = backtrace_string();
  std::print(fp, "{}", bt);
  backtrace_mutex.unlock();
}

void backtrace_dump()
{
  backtrace_mutex.lock();
  auto bt = backtrace_string();
  std::print(MY_STDERR, "{}", bt);
  wid_console_log(bt);
  backtrace_mutex.unlock();
}
