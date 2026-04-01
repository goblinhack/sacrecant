//
// Copyright goblinhack@gmail.com
//

#include "my_backtrace.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_string.hpp"
#include "my_wids.hpp"

#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <strings.h>
#include <unistd.h>

#include <cassert>
#include <print>
#ifndef _WIN32
#include <sys/wait.h>
#endif

#ifdef __linux__
#include <sys/prctl.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX 512
#endif

#ifdef __linux__
//
// I can only really get this working with linux
//
static void debug_crash_handler(int sig)
{
  std::string pid_str(std::to_string(getpid()));

  const size_t max_path = PATH_MAX + 1;
  char         prog_name[ max_path ];

#if defined __APPLE__
  auto pid_num = getpid();
  auto ret     = proc_pidpath(pid_num, prog_name, SIZEOF(prog_name));
  if (ret <= 0) {
    fprintf(stderr, "PID %d: proc_pidpath ();\n", pid_num);
    fprintf(stderr, "    %s\n", strerror(errno));
  } else {
    printf("proc %d: %s\n", pid_num, prog_name);
  }
#elif defined(__WIN64__) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(_WIN32) || defined(WIN32)                   \
    || defined(__WINDOWS_386__) || defined(__CYGWIN__)
  HMODULE module = GetModuleHandleA(nullptr);
  GetModuleFileNameA(module, prog_name, max_path);
#elif defined __linux__
  int ret = readlink("/proc/self/exe", prog_name, max_path - 1);
  if (ret == -1) {
    ERR("debug_crash_handler: read process name failed");
    return;
  }

  if ((size_t) ret >= max_path) {
    ERR("debug_crash_handler: symlink too long");
    return;
  }
#endif

#if defined __linux__
  //
  // Permissions for GDB
  //
  prctl(PR_SET_PTRACER, PR_SET_PTRACER_ANY, 0, 0, 0);

  auto child = fork();
  if (! child) {
    //
    // Start GDB
    //
#ifdef __APPLE__
    execl("/usr/bin/lldb", "lldb", "-p", pid_str.c_str(), nullptr);
#else
    execl("/usr/bin/gdb", "gdb", "--batch", "-n", "-ex", "thread apply all bt", prog_name, pid_str.c_str(), nullptr);
#endif
    if (system("which gdb")) {
      // ignore
    }
    ERR("debugger failed to exec");
    assert(false && "Debugger failed to exec");
  } else {
    //
    // Wait for the child to stop
    //
    waitpid(child, nullptr, 0);
  }
#endif
}
#endif

static auto signal_str(int sig) -> const char *
{
  switch (sig) {
#ifdef SIGHUP
    case SIGHUP : return "SIGHUP";
#endif
#ifdef SIGINT
    case SIGINT : return "SIGINT";
#endif
#ifdef SIGQUIT
    case SIGQUIT : return "SIGQUIT";
#endif
#ifdef SIGILL
    case SIGILL : return "SIGILL";
#endif
#ifdef SIGTRAP
    case SIGTRAP : return "SIGTRAP";
#endif
#ifdef SIGABRT
    case SIGABRT : return "SIGABRT";
#endif
#ifdef SIGEMT
    case SIGEMT : return "SIGEMT";
#endif
#ifdef SIGFPE
    case SIGFPE : return "SIGFPE";
#endif
#ifdef SIGKILL
    case SIGKILL : return "SIGKILL";
#endif
#ifdef SIGBUS
    case SIGBUS : return "SIGBUS";
#endif
#ifdef SIGSEGV
    case SIGSEGV : return "SIGSEGV";
#endif
#ifdef SIGSYS
    case SIGSYS : return "SIGSYS";
#endif
#ifdef SIGPIPE
    case SIGPIPE : return "SIGPIPE";
#endif
#ifdef SIGALRM
    case SIGALRM : return "SIGALRM";
#endif
#ifdef SIGTERM
    case SIGTERM : return "SIGTERM";
#endif
#ifdef SIGURG
    case SIGURG : return "SIGURG";
#endif
#ifdef SIGSTOP
    case SIGSTOP : return "SIGSTOP";
#endif
#ifdef SIGTSTP
    case SIGTSTP : return "SIGTSTP";
#endif
#ifdef SIGCONT
    case SIGCONT : return "SIGCONT";
#endif
#ifdef SIGCHLD
    case SIGCHLD : return "SIGCHLD";
#endif
#ifdef SIGTTIN
    case SIGTTIN : return "SIGTTIN";
#endif
#ifdef SIGTTOU
    case SIGTTOU : return "SIGTTOU";
#endif
#ifdef SIGIO
    case SIGIO : return "SIGIO";
#endif
#ifdef SIGXCPU
    case SIGXCPU : return "SIGXCPU";
#endif
#ifdef SIGXFSZ
    case SIGXFSZ : return "SIGXFSZ";
#endif
#ifdef SIGVTALRM
    case SIGVTALRM : return "SIGVTALRM";
#endif
#ifdef SIGPROF
    case SIGPROF : return "SIGPROF";
#endif
#ifdef SIGWINCH
    case SIGWINCH : return "SIGWINCH";
#endif
#ifdef SIGINFO
    case SIGINFO : return "SIGINFO";
#endif
#ifdef SIGUSR1
    case SIGUSR1 : return "SIGUSR1";
#endif
#ifdef SIGUSR2
    case SIGUSR2 : return "SIGUSR2";
#endif
  }

  return "unknown signal";
}

void crash_handler(int sig)
{
  static bool crashed;

  if (crashed) {
    std::println(stderr, "\nNested crash. Signal {}({})", sig, signal_str(sig));
    callstack_dump(stderr);
    backtrace_dump(stderr);
    return;
  }

  crashed = true;

  std::println(stderr, "\nCrashed. Signal {}({}). Disabling signal handlers...", sig, signal_str(sig));

#ifdef SIGSEGV
  signal(SIGSEGV, nullptr);
#endif
#ifdef SIGBUS
  signal(SIGBUS, nullptr);
#endif
#ifdef SIGABRT
  signal(SIGABRT, nullptr);
#endif
#ifdef SIGFPE
  signal(SIGFPE, nullptr);
#endif
#ifdef SIGILL
  signal(SIGILL, nullptr);
#endif
#ifdef SIGPIPE
  signal(SIGPIPE, nullptr);
#endif
#ifdef SIGINT
  signal(SIGINT, nullptr);
#endif

#ifdef __linux__
  debug_crash_handler(sig);
#endif

  CROAK("crashed due to signal %d(%s)\n", sig, signal_str(sig));
}

void ctrlc_handler(int sig)
{
  std::println(stderr, "\n\nInterrupted. Signal {}({}). Disabling signal handlers...", sig, signal_str(sig));
  callstack_dump(stderr);
  backtrace_dump(stderr);

#ifdef SIGSEGV
  signal(SIGSEGV, nullptr);
#endif
#ifdef SIGBUS
  signal(SIGBUS, nullptr);
#endif
#ifdef SIGABRT
  signal(SIGABRT, nullptr);
#endif
#ifdef SIGFPE
  signal(SIGFPE, nullptr);
#endif
#ifdef SIGILL
  signal(SIGILL, nullptr);
#endif
#ifdef SIGPIPE
  signal(SIGPIPE, nullptr);
#endif
#ifdef SIGINT
  signal(SIGINT, nullptr);
#endif

  std::println(stderr, "\nInterrupted. Cleaning up...");
  std::println(stderr, "---------------------------");

  DIE_CLEAN("Interrupted");
}
