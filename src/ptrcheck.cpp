//
// Copyright goblinhack@gmail.com
//

#include "my_backtrace.hpp"
#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_sprintf.hpp"
#include "my_time.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <mutex>
#include <print>
#include <vector>

static std::mutex ptrcheck_mutex;

class PtrcheckHistory;
static std::vector< class PtrcheckHistory * > all_Ptrcheck_history {};

//
// A single event in the life of a pointer.
//
class PtrcheckHistory
{
public:
  const char *file {};
  const char *func {};
  char        ts[ MY_TIMESTAMP_SIZE ] {};
  int         line {};
  std::string bt;

  PtrcheckHistory()
  {
    ts[ 0 ] = '\0';
    all_Ptrcheck_history.push_back(this);
  }
  PtrcheckHistory(const PtrcheckHistory &other) : file(other.file), func(other.func), line(other.line), bt(other.bt)
  {
    all_Ptrcheck_history.push_back(this);

    strcpy(ts, other.ts);
  }
  ~PtrcheckHistory() = default;
};

//
// The life of a single pointer.
//
class Ptrcheck
{
public:
  Ptrcheck() = default;

  //
  // For sanity, the pointer itself.
  //
  void *ptr {};

  //
  // The type of memory.
  //
  const char *what {};

  //
  // How much memory it is using.
  //
  int size {};

  //
  // Where did the pointer get allocated/freed/last_seen from?
  //
  PtrcheckHistory *allocated_by {};
  PtrcheckHistory *freed_by {};
#ifdef ENABLE_PTRCHECK_HISTORY
  std::array< PtrcheckHistory *, ENABLE_PTRCHECK_HISTORY > last_seen {};
#endif

  //
  // Where in the history buffer we are.
  //
  int last_seen_at {};
  int last_seen_size {};
};

#ifndef CROAK
static void die(void)
{
  std::cerr << "exit(1) error" << std::endl;
  exit(1);
}

static void cleanup_wrapper_(const char *fmt, va_list args)
{
  auto err = "\n" + timestamp();
  err += ": PTRCHECK: FATAL ERROR: ";
  err += string_sprintf(fmt, args);
  std::cerr << err << std::endl;
  die();
}

void CLEANUP(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  cleanup_wrapper_(fmt, args);
  va_end(args);
}

static void error_(const char *fmt, va_list args)
{
  auto err = timestamp();
  err += ": PTRCHECK: ERROR: ";
  err += string_sprintf(fmt, args);
  std::cerr << err << std::endl;
}

void ERROR(const char *fmt, ...) CHECK_FORMAT_STR(printf, 1, 2);
void ERROR(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  error_(fmt, args);
  va_end(args);
}

#define CROAK(args...)                                                                                                                     \
  std::cerr << std::format("Died at {}:{} line {}", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM);                                           \
  CLEANUP(args);

#define ERR(args...)                                                                                                                       \
  std::cerr << std::format("Error at {}:{} line {}", SRC_FILE_NAME, SRC_FUNC_NAME, SRC_LINE_NUM);                                          \
  ERROR(args);

#endif

//
// A hash table so we can check pointers fast.
//
using hash_elem_t = struct HashElemT {
  struct HashElemT *next;
  Ptrcheck         *pc;
};

using hash_t = struct HashT {
  int           hash_size;
  hash_elem_t **elements;
};

static hash_t *ptrcheck_hash[ MTYPE_MAX ];

//
// How many old/freed pointers do we keep track of. We use this when we find
// an unknown pointer to find when it last lived.
//
static const int                                      ringbuf_max_size = 100000;
static int                                            ringbuf_current_size[ MTYPE_MAX ];
static std::array< class Ptrcheck, ringbuf_max_size > ringbuf[ MTYPE_MAX ];
static Ptrcheck                                      *ringbuf_next[ MTYPE_MAX ];
static Ptrcheck                                      *ringbuf_base[ MTYPE_MAX ];

//
// Wrapper for calloc.
//
static auto local_zalloc(int size) -> void *
{
  void *p = nullptr; // NOLINT
  p       = calloc(1, size);
  return p;
}

//
// World a pointer to a hash slot.
//
static auto ptr2hash(hash_t *hash_table, void *ptr) -> hash_elem_t **
{
  int slot = 0;

  //
  // Knock lower 2 bits off of pointer - these are always 0.
  //
  slot = static_cast< int >((((uintptr_t) (ptr)) >> 2) % hash_table->hash_size);

  return &hash_table->elements[ slot ];
}

//
// Create a hash table for all pointers.
//
static auto hash_init(int hash_size) -> hash_t *
{
  hash_t *hash_table = nullptr;

  hash_table = static_cast< __typeof__(hash_table) >(local_zalloc(SIZEOF(hash_t)));

  hash_table->hash_size = hash_size;

  hash_table->elements = static_cast< __typeof__(hash_table->elements) >(local_zalloc(hash_size * SIZEOF(hash_elem_t *)));

  return hash_table;
}

//
// Store a pointer in our hash.
//
static void hash_add(hash_t *hash_table, Ptrcheck *pc)
{
  hash_elem_t **slot = nullptr;
  hash_elem_t  *elem = nullptr;

  if (! g_ptrcheck_inited) {
    if (g_memory_allocated > 0) {
      CROAK("memory has already been allocated prior to invoking ptrcheck (%d times)", g_memory_allocated);
    }
    g_ptrcheck_inited = true;
  }

  if (pc == nullptr) {
    return;
  }

  if (hash_table == nullptr) {
    delete pc;
    return;
  }

  slot = ptr2hash(hash_table, pc->ptr);
  elem = *slot;
  while ((elem != nullptr) && (elem->pc->ptr != pc->ptr)) {
    elem = elem->next;
  }

  if (elem != nullptr) {
    delete pc;
    return;
  }

  elem       = static_cast< __typeof__(elem) >(local_zalloc(SIZEOF(*elem)));
  elem->pc   = pc;
  elem->next = *slot;
  *slot      = elem;
}

//
// Find a pointer in our hash.
//
static auto hash_find(hash_t *hash_table, void *ptr) -> hash_elem_t *
{
  hash_elem_t *const *slot = nullptr;
  hash_elem_t        *elem = nullptr; // NOLINT

  if (hash_table == nullptr) {
    return nullptr;
  }

  slot = ptr2hash(hash_table, ptr);
  elem = *slot;
  while ((elem != nullptr) && (elem->pc->ptr != ptr)) {
    elem = elem->next;
  }

  return elem;
}

//
// Free a pointer from our hash.
//
static void hash_free(hash_t *hash_table, void *ptr)
{
  hash_elem_t **slot = nullptr;
  hash_elem_t  *prev = nullptr;
  hash_elem_t  *elem = nullptr;

  if (hash_table == nullptr) {
    return;
  }

  if (ptr == nullptr) {
    return;
  }

  slot = ptr2hash(hash_table, ptr);
  elem = *slot;
  prev = nullptr;

  while ((elem != nullptr) && (elem->pc->ptr != ptr)) {
    prev = elem;
    elem = elem->next;
  }

  if (elem == nullptr) {
    return;
  }

  if (prev != nullptr) {
    prev->next = elem->next;
  } else {
    *slot = elem->next;
  }

  delete elem->pc;
  free(elem);
}

static auto ptrcheck_describe_pointer(int mtype, const void *ptr) -> Ptrcheck *
{
  int ring_ptr_size = 0;

  //
  // Currently active pointer?
  //
  auto *elem = hash_find(ptrcheck_hash[ mtype ], const_cast< void * >(ptr));
  if (elem != nullptr) {
    auto *pc = elem->pc;

    auto *a = pc->allocated_by;
    if (a != nullptr) {
      std::println(stderr, "PTRCHECK: Currently allocated at {} \"{}\" ({} bytes) at {}:{} line {} at {}", pc->ptr, pc->what, pc->size,
                   a->file, a->func, a->line, a->ts);

      std::print(stderr, "{}", a->bt);
    }

    //
    // Dump the pointer history.
    //
#ifdef ENABLE_PTRCHECK_HISTORY
    int h = pc->last_seen_at;
    for (auto j = 0; j < pc->last_seen_size; j++) {
      if (--h < 0) {
        h = ENABLE_PTRCHECK_HISTORY - 1;
      }

      auto *H = pc->last_seen[ h ];
      if (H != nullptr) {
        std::println(stderr, "PTRCHECK: Last seen at [{}] at {}:{} line {} at {}", j, H->file, H->func, H->line, H->ts);
        std::print(stderr, "{}", H->bt);
      }
    }
#endif
  }

  //
  // Check the ring buffer to see if we've seen this pointer before.
  //
  auto *pc = &ringbuf_next[ mtype ][ 0 ];
  pc--;

  if (pc < ringbuf_base[ mtype ]) {
    pc = ringbuf_base[ mtype ] + ringbuf_max_size - 1;
  }

  ring_ptr_size = ringbuf_current_size[ mtype ];

  std::print(stderr, "vvvvv Pointer history for {} vvvvv (max {} ptrs saved)", ptr, ring_ptr_size);

  //
  // Walk back through the ring buffer.
  //
  while (ring_ptr_size != 0) {
    //
    // Found a match?
    //
    if (pc->ptr == ptr) {
      auto *a = pc->allocated_by;
      if (a != nullptr) {
        std::println(stderr, "PTRCHECK: {} allocated at \"{}\" ({} bytes) at {}:{} line {} at {}\n{}", ptr, pc->what, pc->size, a->file,
                     a->func, a->line, a->ts, a->bt);
      }

      auto *f = pc->freed_by;
      if (f != nullptr) {
        std::println(stderr, "PTRCHECK: {} freed at {}:{} line {} at {}\n{}", ptr, f->file, f->func, f->line, f->ts, f->bt);
      }

      //
      // Dump the pointer history.
      //
#ifdef ENABLE_PTRCHECK_HISTORY
      int h = pc->last_seen_at;
      for (auto i = 0; i < pc->last_seen_size; i++) {
        if (--h < 0) {
          h = ENABLE_PTRCHECK_HISTORY - 1;
        }

        auto *H = pc->last_seen[ h ];
        if (H != nullptr) {
          std::println(stderr, "PTRCHECK: {} last seen at [{}] at {}:{} line {} at {}\n{}", ptr, i, H->file, H->func, H->line, H->ts,
                       H->bt);
        }
      }
#endif
      //
      // Memory reuse can cause a lot of false hits, so stop after
      // the first match.
      //
      break;
    }

    ring_ptr_size--;
    pc--;

    //
    // Handle wraps.
    //
    if (pc < ringbuf_base[ mtype ]) {
      pc = ringbuf_base[ mtype ] + ringbuf_max_size - 1;
    }
  }

  std::print(stderr, "^^^^^ End of pointer history for {} ^^^^^", ptr);
  return nullptr;
}

//
// Check a pointer for validity.
//
static auto ptrcheck_verify_pointer(int mtype, const void *ptr, const char *func, const char *file, int line, int dont_store) -> Ptrcheck *
{
  static const char *unknown_ptr_warning  = "** UNKNOWN POINTER ** ";
  static const char *null_pointer_warning = "** NULL POINTER ** ";
  Ptrcheck          *pc                   = nullptr;
  hash_elem_t const *e                    = nullptr;

  //
  // Check the robust handle is valid.
  //
  e = hash_find(ptrcheck_hash[ mtype ], const_cast< void * >(ptr));
  if (e != nullptr) {
    pc = e->pc;

    if (dont_store != 0) {
#if 0
#ifdef ENABLE_DEBUG_PTRCHECK
      std::cerr << std::format("PTRCHECK: {} verified at \"{}\" ({} bytes) at {}:{} line {} (do not store)\n", ptr,
                                  pc->what, pc->size, file, func, line);
#endif
#endif
      return pc;
    }

    //
    // Add some free information that we know the pointer is safe at this
    // point in time.
    //
#ifdef ENABLE_PTRCHECK_HISTORY
    IF_DEBUG
    {
      auto *l = pc->last_seen[ pc->last_seen_at ];
      if (l == nullptr) {
        l = pc->last_seen[ pc->last_seen_at ] = new PtrcheckHistory();
      }
      l->file = file;
      l->func = func;
      l->line = line;

      l->bt = callstack_string();
      timestamp(l->ts, SIZEOF(l->ts));

#ifdef ENABLE_DEBUG_PTRCHECK
      std::cerr << std::format("PTRCHECK: {} verified at \"{}\" ({} bytes) at {}:{} line {} at {}\n", ptr, pc->what, pc->size, file, func,
                               line, l->ts);
#endif

      pc->last_seen_at++;
      pc->last_seen_size++;

      if (pc->last_seen_at >= ENABLE_PTRCHECK_HISTORY) {
        pc->last_seen_at = 0;
      }

      pc->last_seen_size = std::min(pc->last_seen_size, ENABLE_PTRCHECK_HISTORY);
    }
#endif
    return pc;
  }
  if (ptr == nullptr) {
    ERR("%s%p NULL pointer %s:%s line %u", null_pointer_warning, ptr, file, func, line);
    return nullptr;
  }

  //
  // We may be about to crash. Complain!
  //
  std::print(stderr, "{}{} {}:{} line {}, see below logs", unknown_ptr_warning, ptr, file, func, line);
  ptrcheck_describe_pointer(mtype, ptr);
  CROAK("%s%p %s:%s line %u, see above logs", unknown_ptr_warning, ptr, file, func, line);
  return nullptr;
}

//
// Record this pointer.
//
static auto ptrcheck_alloc_(int mtype, const void *ptr, const char *what, int size, const char *func, const char *file, int line) -> void *
{
  Ptrcheck *pc = nullptr;

#ifdef ENABLE_DEBUG_PTRCHECK
  char tmp[ MY_TIMESTAMP_SIZE ];
  auto ts = timestamp(tmp, SIZEOF(tmp));
  fprintf(stderr, "%s: PTRCHECK: Alloc %p \"%s\" (%u bytes) at %s:%s line %u\n", ts, ptr, what, size, file, func, line);
#endif

  if (ptr == nullptr) {
    ERR("NULL pointer");
  }

  //
  // Create a hash table to store pointers.
  //
  if (ptrcheck_hash[ mtype ] == nullptr) {
    //
    // Create enough space for lots of pointers.
    //
    ptrcheck_hash[ mtype ] = hash_init(1046527 /* prime */);

    if (ptrcheck_hash[ mtype ] == nullptr) {
      return const_cast< void * >(ptr);
    }

    //
    // And a ring buffer to store old pointer into.
    //
    ringbuf_next[ mtype ]         = ringbuf[ mtype ].data();
    ringbuf_base[ mtype ]         = ringbuf[ mtype ].data();
    ringbuf_current_size[ mtype ] = 0;
  }

  //
  // Missing an earlier free?
  //
  if (hash_find(ptrcheck_hash[ mtype ], const_cast< void * >(ptr)) != nullptr) {
    ERR("Pointer %p already exists and attempting to add again", ptr);
    ptrcheck_describe_pointer(mtype, ptr);
    return const_cast< void * >(ptr);
  }

  //
  // Allocate a block of data to describe the pointer and owner.
  //
  pc = new Ptrcheck();

  //
  // Populate the data block.
  //
  pc->ptr  = const_cast< void * >(ptr);
  pc->what = what;
  pc->size = size;

  auto *a = pc->allocated_by = new PtrcheckHistory();

  a->func = func;
  a->file = file;
  a->line = line;
  timestamp(a->ts, SIZEOF(a->ts));
  a->bt = callstack_string();

  //
  // Add it to the hash. Not the ring buffer (only when freed).
  //
  hash_add(ptrcheck_hash[ mtype ], pc);

  return const_cast< void * >(ptr);
}

auto ptrcheck_alloc(int mtype, const void *ptr, const char *what, int size, const char *func, const char *file, int line) -> void *
{
  ptrcheck_mutex.lock();
  TRACE();
  auto *ret = ptrcheck_alloc_(mtype, ptr, what, size, func, file, line);
  ptrcheck_mutex.unlock();

  return ret;
}

//
// Check a pointer is valid and if so add it to the ring buffer. If not,
// return false and avert the MYFREE(), just in case.
//
static auto ptrcheck_free_(int mtype, void *ptr, const char *func, const char *file, int line) -> int
{
  Ptrcheck *pc = nullptr;

#ifdef ENABLE_DEBUG_PTRCHECK
  char tmp[ MY_TIMESTAMP_SIZE ];
  auto ts = timestamp(tmp, SIZEOF(tmp));
  fprintf(stderr, "%s: PTRCHECK: Free %p at %s:%s line %u ringbuf_current_size %u\n", ts, ptr, file, func, line,
          ringbuf_current_size[ mtype ]);
#endif

  if (ptr == nullptr) {
    ERR("NULL pointer");
    return 0;
  }

  pc = ptrcheck_verify_pointer(mtype, ptr, file, func, line, 1 /* don't store */);
  if (pc == nullptr) {
    CROAK("failed to save pointer history");
    return 0;
  }

  //
  // Add some free information that we know the pointer is safe at this
  // point in time.
  //
  auto *f = pc->freed_by = new PtrcheckHistory();

  f->file = file;
  f->func = func;
  f->line = line;
  f->bt   = callstack_string();
  timestamp(f->ts, SIZEOF(f->ts));

  //
  // Add the free info to the ring buffer.
  //
  *ringbuf_next[ mtype ] = *pc;

  //
  // Take care of wraps.
  //
  ringbuf_next[ mtype ]++;
  if (ringbuf_next[ mtype ] >= ringbuf_base[ mtype ] + ringbuf_max_size) {
    ringbuf_next[ mtype ] = ringbuf_base[ mtype ];
  }

  //
  // Increment the ring buffer used size up to the limit.
  //
  if (ringbuf_current_size[ mtype ] < ringbuf_max_size) {
    ringbuf_current_size[ mtype ]++;
  }

  hash_free(ptrcheck_hash[ mtype ], ptr);

  return 1;
}

auto ptrcheck_free(int mtype, void *ptr, const char *func, const char *file, int line) -> int
{
  ptrcheck_mutex.lock();
  TRACE();
  auto ret = ptrcheck_free_(mtype, ptr, func, file, line);
  ptrcheck_mutex.unlock();

  return ret;
}

//
// Check a pointer for validity with no recording of history.
//
auto ptrcheck_verify(int mtype, const void *ptr, const char *func, const char *file, int line) -> int
{
  ptrcheck_mutex.lock();
  TRACE();

  //
  // Handy if things get too slow, to see what is firing most
  //
  // fprintf(stderr, "PTRCHECK %s %s %d\n", file, func, line);
  auto ret = ptrcheck_verify_pointer(mtype, ptr, file, func, line, 0 /* don't store */) != nullptr;

  ptrcheck_mutex.unlock();

  return static_cast< int >(ret);
}

//
// Show any leaks
//
void ptrcheck_leak_print(int mtype)
{
  hash_elem_t *const *slot = nullptr;
  hash_elem_t const  *elem = nullptr;
  Ptrcheck           *pc   = nullptr;
  int                 i    = 0;

  if (ptrcheck_hash[ mtype ] == nullptr) {
    return;
  }

  for (i = 0; i < ptrcheck_hash[ mtype ]->hash_size; i++) {
    slot = &ptrcheck_hash[ mtype ]->elements[ i ];
    elem = *slot;

    while (elem != nullptr) {
      pc = elem->pc;

      auto *a = pc->allocated_by;
      if (a != nullptr) {
        std::println(stderr, "PTRCHECK: Leak {} \"{}\" ({} bytes) at {}:{} line {} at {}\n{}", pc->ptr, pc->what, pc->size, a->file,
                     a->func, a->line, a->ts, a->bt);
      } else {
        std::println(stderr, "PTRCHECK: Leak \"{}\" ({} bytes)", pc->what, pc->size);
      }

      //
      // Dump the pointer history.
      //
#ifdef ENABLE_PTRCHECK_HISTORY
      int h = pc->last_seen_at;
      for (auto j = 0; j < pc->last_seen_size; j++) {
        if (--h < 0) {
          h = ENABLE_PTRCHECK_HISTORY - 1;
        }

        auto *H = pc->last_seen[ h ];
        if (H != nullptr) {
          std::println(stderr, "PTRCHECK: Last seen at [{}] at {}:{} line {} at {}\n{}", j, H->file, H->func, H->line, H->ts, H->bt);
        }
      }
#endif
      if (elem->next == elem) {
        ERR("Hash table corruption");
      }

      elem = elem->next;
    }
  }
}

void ptrcheck_leak_print()
{
  for (int mtype = 0; mtype < MTYPE_MAX; mtype++) {
    ptrcheck_leak_print(mtype);
  }
}

static void ptrcheck_fini(int mtype)
{
  hash_elem_t *const *slot = nullptr;
  hash_elem_t        *elem = nullptr;
  int                 i    = 0;

  if (ptrcheck_hash[ mtype ] == nullptr) {
    return;
  }

  for (auto *p : all_Ptrcheck_history) {
    delete p;
  }
  all_Ptrcheck_history.clear();

  for (i = 0; i < ptrcheck_hash[ mtype ]->hash_size; i++) {
    slot = &ptrcheck_hash[ mtype ]->elements[ i ];
    elem = *slot;

    while (elem != nullptr) {
      auto *next = elem->next;
      delete elem->pc;
      free(elem);
      elem = next;
    }
  }

  free(ptrcheck_hash[ mtype ]->elements);
  free(ptrcheck_hash[ mtype ]);

  ptrcheck_hash[ mtype ] = nullptr;
}

void ptrcheck_fini()
{
  for (int mtype = 0; mtype < MTYPE_MAX; mtype++) {
    ptrcheck_fini(mtype);
  }
}
