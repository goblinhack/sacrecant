//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_types.hpp"

#include <cstdlib>
#include <cstring>

auto MYZALLOC_(int size, const char *what, const char *file, const char *func, int line) -> void *
{
  TRACE();

  void *ptr = calloc(1, size); // NOLINT

  if (ptr == nullptr) [[unlikely]] {
    CROAK("no memory, %s:%s:%u, size %u", file, func, line, size);
  }

  IF_DEBUG2
  { //
    ptrcheck_alloc(MTYPE_MISC, ptr, what, size, file, func, line);
  }

  // callstack_dump();
  g_memory_allocated++;

  return ptr;
}

auto MYMALLOC_(int size, const char *what, const char *file, const char *func, int line) -> void *
{
  TRACE();

  void *ptr = malloc(size); // NOLINT

  if (ptr == nullptr) [[unlikely]] {
    CROAK("no memory, %s:%s:%u", file, func, line);
  }

  IF_DEBUG2
  { //
    ptrcheck_alloc(MTYPE_MISC, ptr, what, size, file, func, line);
  }

  // callstack_dump();
  g_memory_allocated++;

  return ptr;
}

auto MYREALLOC_(void *ptr, int size, const char *what, const char *file, const char *func, int line) -> void *
{
  TRACE();
  IF_DEBUG2
  { //
    ptrcheck_free(MTYPE_MISC, ptr, file, func, line);
  }

  ptr = realloc(ptr, size);
  if (ptr == nullptr) [[unlikely]] {
    CROAK("no memory, %s:%s:%u", file, func, line);
  }

  IF_DEBUG2
  { //
    ptrcheck_alloc(MTYPE_MISC, ptr, what, size, file, func, line);
  }

  // callstack_dump();
  g_memory_allocated++;

  return ptr;
}

void MYFREE_(void *ptr, const char *file, const char *func, int line)
{
  TRACE();

  IF_DEBUG2
  { //
    ptrcheck_free(MTYPE_MISC, ptr, file, func, line);
  }

  // callstack_dump();
  g_memory_allocated++;

  free(ptr);
}

auto MYDUPSTR_(const char *in, const char *what, const char *file, const char *func, int line) -> char *
{
  TRACE();

  if (in == nullptr) [[unlikely]] {
    ERR("no string to duplicate");
    return nullptr;
  }

  char *ptr = strdup(in); // NOLINT
  if (ptr == nullptr) [[unlikely]] {
    CROAK("no memory, %s:%s:%u", file, func, line);
  }

  IF_DEBUG2
  {
    auto const size = strlen(in);

    ptrcheck_alloc(MTYPE_MISC, ptr, what, size, file, func, line);
  }

  // callstack_dump();
  g_memory_allocated++;

  return ptr;
}
