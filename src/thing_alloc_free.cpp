//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <cinttypes>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <mutex>

static std::mutex thing_mutex;

static uint32_t thing_id_last_per_level_id[ LEVEL_ARR_IDX_MAX ];
static uint32_t thing_id_from_end_id = THING_ID_MAX - 1;
static uint16_t thing_id_entropy;

static uint32_t last_ext_id[ LEVEL_ARR_IDX_MAX ];
static uint32_t last_dmap_id[ LEVEL_ARR_IDX_MAX ];
static uint32_t last_light_id[ LEVEL_ARR_IDX_MAX ];

#ifdef ENABLE_PER_THING_MEMORY
static bool memory_test = true;
#endif

[[nodiscard]] static auto thing_ext_alloc(Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  //
  // Continue from the last successful allocation
  //
  int max_tries;

  if (g_thread_id == MAIN_THREAD) {
    max_tries = THING_EXT_ID_MAX;
  } else {
    max_tries = THING_EXT_ID_PER_LEVEL_MAX;
  }

  auto level_arr_idx = l->level_arr_idx;
  if (! last_ext_id[ level_arr_idx ]) {
    last_ext_id[ level_arr_idx ] = level_arr_idx * THING_EXT_ID_PER_LEVEL_MAX;
  }

  for (auto tries = 0; tries < max_tries; tries++) {
    ThingExtId ext_id = last_ext_id[ level_arr_idx ] + tries;
    ext_id %= THING_EXT_ID_MAX;
    if (ext_id == 0U) [[unlikely]] {
      continue;
    }

    if ((v->thing_ext[ ext_id ].in_use) != 0U) [[unlikely]] {
      continue;
    }

    v->thing_ext[ ext_id ].in_use = true;
    me->ext_id                    = ext_id;
    last_ext_id[ level_arr_idx ]  = ext_id;

    return true;
  }

  if (g_thread_id == MAIN_THREAD) {
    warn("out of thing ext IDs: max is %08" PRIX32 "", THING_EXT_ID_MAX);
  } else {
    CROAK("out of thing ext IDs: max is %08" PRIX32 "", THING_EXT_ID_MAX);
  }

  return false;
}

static void thing_ext_free(Levelsp v, Thingp me)
{
  TRACE();

  auto ext_id = me->ext_id;
  if (ext_id == 0U) {
    return;
  }

  if (! v->thing_ext[ ext_id ].in_use) {
    CROAK("freeing unused thing ext ID is not in use, %" PRIX32 "", ext_id);
  }

  v->thing_ext[ ext_id ] = {};
  me->ext_id             = 0;
}

[[nodiscard]] static auto thing_dmap_alloc(Gamep g, Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  //
  // Continue from the last successful allocation
  //
  int max_tries;

  if (g_thread_id == MAIN_THREAD) {
    max_tries = THING_DMAP_ID_MAX;
  } else {
    max_tries = THING_DMAP_ID_PER_LEVEL_MAX;
  }

  auto level_arr_idx = l->level_arr_idx;
  if (! last_dmap_id[ level_arr_idx ]) {
    last_dmap_id[ level_arr_idx ] = level_arr_idx * THING_DMAP_ID_PER_LEVEL_MAX;
  }

  for (auto tries = 0; tries < max_tries; tries++) {
    ThingDmapId dmap_id = last_dmap_id[ level_arr_idx ] + tries;
    dmap_id %= THING_DMAP_ID_MAX;
    if (dmap_id == 0U) [[unlikely]] {
      continue;
    }

    if ((v->thing_dmap[ dmap_id ].in_use) != 0U) [[unlikely]] {
      continue;
    }

    v->thing_dmap[ dmap_id ].in_use = true;
    me->dmap_id                     = dmap_id;
    if (compiler_unused) {
      THING_DBG(g, v, l, me, "alloc dmap id: %08" PRIX32 "", me->dmap_id);
    }
    last_dmap_id[ level_arr_idx ] = dmap_id;

    return true;
  }

  if (g_thread_id == MAIN_THREAD) {
    warn("out of thing dmap IDs: max is %08" PRIX32 "", THING_DMAP_ID_MAX);
  } else {
    CROAK("out of thing dmap IDs: max is %08" PRIX32 "", THING_DMAP_ID_MAX);
  }

  return false;
}

static void thing_dmap_free(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto dmap_id = me->dmap_id;
  if (dmap_id == 0U) {
    return;
  }

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "free dmap id: %08" PRIX32 "", me->dmap_id);
  }

  if (! v->thing_dmap[ dmap_id ].in_use) {
    CROAK("freeing unused thing dmap ID is not in use, %" PRIX32 "", dmap_id);
  }

  v->thing_dmap[ dmap_id ] = {};
  me->dmap_id              = 0;
}

[[nodiscard]] static auto thing_light_alloc(Levelsp v, Levelp l, Thingp me) -> bool
{
  TRACE();

  //
  // Continue from the last successful allocation
  //
  int max_tries;

  if (g_thread_id == MAIN_THREAD) {
    max_tries = THING_LIGHT_ID_MAX;
  } else {
    max_tries = THING_LIGHT_ID_PER_LEVEL_MAX;
  }

  auto level_arr_idx = l->level_arr_idx;
  if (! last_light_id[ level_arr_idx ]) {
    last_light_id[ level_arr_idx ] = level_arr_idx * THING_LIGHT_ID_PER_LEVEL_MAX;
  }

  for (auto tries = 0; tries < max_tries; tries++) {
    ThingLightId light_id = last_light_id[ level_arr_idx ] + tries;
    light_id %= THING_LIGHT_ID_MAX;
    if (light_id == 0U) [[unlikely]] {
      continue;
    }

    if ((v->thing_light[ light_id ].in_use) != 0U) [[unlikely]] {
      continue;
    }

    v->thing_light[ light_id ].in_use = true;
    me->light_id                      = light_id;
    last_light_id[ level_arr_idx ]    = light_id;

    return true;
  }

  if (g_thread_id == MAIN_THREAD) {
    warn("out of thing light IDs: max is %08" PRIX32 "", THING_LIGHT_ID_MAX);
  } else {
    CROAK("out of thing light IDs on level %08" PRIX32 ": max is %08" PRIX32 "", level_arr_idx, THING_LIGHT_ID_MAX);
  }

  return false;
}

static void thing_light_free(Levelsp v, Thingp me)
{
  TRACE();

  auto light_id = me->light_id;
  if (light_id == 0U) {
    return;
  }

  if (! v->thing_light[ light_id ].in_use) {
    CROAK("freeing unused thing light ID is not in use, %" PRIX32 "", light_id);
  }

  v->thing_light[ light_id ] = {};
  me->light_id               = 0;
}

static auto thing_alloc_do(Gamep g, Levelsp v, Levelp l, Tpp tp, ThingIdPacked id,
                           bool needs_ext_memory,   //
                           bool needs_dmap_memory,  //
                           bool needs_light_memory, //
                           bool need_mutex) -> Thingp
{
  TRACE();

  const auto tp_id = tp_id_get(tp);

  //
  // Use the full range of IDs. If we can, we will use part of this as the level as
  // it allows threads to avoid mutexes.
  //
  // Intentionally using OS rand here, as if we had a conflict with another thread
  // we would end up with multiple rand calls - and that would make level gen inconsistent
  // across seeds.
  //

  //
  // Check if there is anything at this index
  //
  auto  arr_index = id.c.arr_index;
  auto *me        = &v->thing_body[ arr_index ];
  if ((me->tp_id) != 0U) [[unlikely]] {
    //
    // Some other thread grabbed it already?
    //
    return nullptr;
  }

  //
  // If we need a mutex, lock the thing population for this slot
  //
  if ((need_mutex)) [[unlikely]] {
    thing_mutex.lock();

    //
    // Just in case someone else grabbed it while locking...
    //
    if ((me->tp_id) != 0U) [[unlikely]] {
      thing_mutex.unlock();
      return nullptr;
    }

    //
    // We safely have this slot
    //
    me->tp_id = tp_id;
    thing_mutex.unlock();
  } else {
    //
    // No need to worry about other threads
    //
    if ((me->tp_id) != 0U) [[unlikely]] {
      return nullptr;
    }

    me->tp_id = tp_id;
  }

  //
  // Zero the thing out. TAKE CARE not to zero tp_id
  //
  if (g_opt_tests || g_opt_do_level_gen || g_opt_do_level_select_gen || g_opt_do_room_gen || DEBUG) {
    //
    // Should be no need to do this as we zero on thing destruction
    //
    char *p = reinterpret_cast< char * >(me) + SIZEOF(me->tp_id);
    for (auto i = 0; i < SIZEOF(*me) - SIZEOF(me->tp_id); i++, p++) {
      if (*p) {
        CROAK("expected memory to be zeroed");
      }
    }

    memset(reinterpret_cast< char * >(me) + SIZEOF(me->tp_id), 0, SIZEOF(*me) - SIZEOF(me->tp_id));
  }

  //
  // Create the final ID with some rando entropy
  //
  thing_id_entropy++;
  if (thing_id_entropy == 0U) {
    thing_id_entropy++;
  }
  id.c.entropy = ++thing_id_entropy;

  //
  // The thing has the full ID, including entropy
  //
  me->id = id.a.val;

  if (compiler_unused) {
    log("thing alloc %08" PRIX32 //
        " (level: %" PRIu32      //
        " id: %08" PRIX32        //
        " entropy: %08" PRIX32   //
        ")",                     //
        me->id,                  //
        id.b.level_arr_idx,      //
        id.b.per_level_thing_id, //
        id.b.entropy);
  }

  if (needs_ext_memory) {
    if (! thing_ext_alloc(v, l, me)) {
      //
      // Try to carry on
      //
    }
  }

  if (needs_dmap_memory) {
    if (! thing_dmap_alloc(g, v, l, me)) {
      //
      // Try to carry on
      //
    }
  }

  if (needs_light_memory) {
    if (! thing_light_alloc(v, l, me)) {
      //
      // Try to carry on
      //
    }
  }

  if (compiler_unused) {
    THING_DBG(g, v, l, me, "alloced ext id   %08x", me->ext_id);
    THING_DBG(g, v, l, me, "alloced dmap id  %08x", me->dmap_id);
    THING_DBG(g, v, l, me, "alloced light id %08x", me->light_id);
  }

#ifdef ENABLE_PER_THING_MEMORY
  if (memory_test) {
    auto tmp = malloc(sizeof(*me));
    memcpy(tmp, me, sizeof(*me));
    me                               = (Thingp) tmp;
    v->thing_body_debug[ arr_index ] = me;
  }
#endif

  return me;
}

[[nodiscard]] auto thing_alloc(Gamep g, Levelsp v, Levelp l, Tpp tp, bpoint /*p*/) -> Thingp
{
  TRACE();

  if (tp == nullptr) [[unlikely]] {
    CROAK("no template set for thing allocation");
  }

  //
  // Check we cannot overflow on monsters
  //
  auto needs_ext_memory   = tp_is_memory_ext(tp);
  auto needs_dmap_memory  = tp_is_dmap(tp);
  auto needs_light_memory = tp_is_light_source(tp) || tp_is_player(tp);

  //
  // The level select level doesn't have a dedicated bit slot so just use a level that
  // is not initialized
  //
  auto level_arr_idx = l->level_arr_idx;
  if ((level_arr_idx == LEVEL_ARR_IDX_LEVEL_SELECT) || (level_arr_idx == LEVEL_ARR_IDX_LEVEL_SECRET)) {
    if (v->is_generating_levels) {
      CROAK("cannot allocate level select while mutexes are off");
    }

    for (auto i = 0; i < LEVEL_ARR_IDX_MAX; i++) {
      if (! v->level[ i ].is_initialized) {
        level_arr_idx = i;
        break;
      }
    }
  }

  //
  // Sequentially try to allocate an ID from the last ID allocated.
  // Use the level_arr_idx as scoping so that we do not need to use a mutex as different
  // levels will allocate from different regions of the array.
  //
  for (uint32_t tries = 0; tries < (1 << THING_PER_LEVEL_THING_ID_BITS); tries++) {
    uint32_t per_level_id = tries + thing_id_last_per_level_id[ level_arr_idx ] + 1;
    if (per_level_id == 0U) {
      per_level_id = 1;
    }

    ThingIdPacked id        = {};
    id.b.level_arr_idx      = level_arr_idx;
    id.b.per_level_thing_id = per_level_id;

    auto *me = thing_alloc_do(g, v, l, tp, id, needs_ext_memory, needs_dmap_memory, needs_light_memory, false /* mutex */);
    if (me != nullptr) {
      thing_id_last_per_level_id[ level_arr_idx ] = per_level_id;
      return me;
    }
  }

  //
  // If we hit this, it turns out we should have been using mutexes. We have no idea
  // if this will collide with something else not using a mutex.
  //
  if (v->is_generating_levels) {
    CROAK("ran out of thing IDs while doing multi threaded level gen");
  }

  //
  // Last resort, allocate from the end backwards.
  //
  for (uint32_t arr_index = thing_id_from_end_id - 1; arr_index > 0; arr_index--) {
    ThingIdPacked id = {};
    id.c.arr_index   = arr_index;

    auto *me = thing_alloc_do(g, v, l, tp, id, needs_ext_memory, needs_dmap_memory, needs_light_memory, true /* mutex */);
    if (me != nullptr) {
      thing_id_from_end_id = arr_index;
      return me;
    }
  }

  CROAK("out of thing IDs: max is THING_ID_MAX %08" PRIX32 "", THING_ID_MAX);
  return nullptr;
}

void thing_free(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  if (compiler_unused) {
    THING_DBG(g, v, l, me, "free");
    THING_DBG(g, v, l, me, "free ext id   %08x", me->ext_id);
    THING_DBG(g, v, l, me, "free dmap id  %08x", me->dmap_id);
    THING_DBG(g, v, l, me, "free light id %08x", me->light_id);
  }
  TRACE();

  auto *o = thing_find(g, v, me->id);
  if (me != o) {
    CROAK("thing_free: thing mismatch found for id, %p %08" PRIX32 "", (void *) me, me->id);
  }

  auto *tp = thing_tp(me);

  //
  // Clean up references
  //
  if (tp_is_player(tp)) {
    v->player_id = 0;
  }

  if (tp_is_level_curr_icon(tp)) {
    v->level_select_id = 0;
  }

  if (v->is_generating_levels) {
    CROAK("unexpected to be freeing things during level creation");
  }

  //
  // Remove from descriptions
  //
  (void) level_cursor_describe_remove(g, v, me);

  (void) thing_pop(g, v, me);

  thing_ext_free(v, me);
  thing_dmap_free(g, v, l, me);
  thing_light_free(v, me);
  memset((void *) me, 0, SIZEOF(*me));

#ifdef ENABLE_PER_THING_MEMORY
  if (memory_test) {
    free(me);
  }
#endif
}

void thing_alloc_free_fini(Gamep g)
{
  TRACE();

  memset(thing_id_last_per_level_id, 0, sizeof(thing_id_last_per_level_id));
  thing_id_from_end_id = THING_ID_MAX - 1;

  memset(last_ext_id, 0, sizeof(last_ext_id));
  memset(last_dmap_id, 0, sizeof(last_dmap_id));
  memset(last_light_id, 0, sizeof(last_light_id));
}

[[nodiscard]] auto thing_is_removable_when_dead_on_err(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    CROAK("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(me), is_removable_when_dead_on_err) != 0;
}

[[nodiscard]] auto thing_is_removable_on_err(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    CROAK("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(me), is_removable_on_err) != 0;
}

[[nodiscard]] auto thing_is_scheduled_for_cleanup(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    CROAK("no thing pointer");
    return false;
  }

  return me->_is_scheduled_for_cleanup;
}

void thing_is_scheduled_for_cleanup_set(Gamep g, Levelsp v, Levelp l, Thingp me, bool val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    CROAK("no thing pointer");
    return;
  }

  if (me->_is_scheduled_for_cleanup == static_cast< int >(val)) {
    return;
  }
  me->_is_scheduled_for_cleanup = val;

  if (val) {
    THING_DBG(g, v, l, me, "is scheduled for cleanup set");
  }

  level_request_to_cleanup_things_set(g, v, l);
}

void thing_is_scheduled_for_cleanup_unset(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE_DEBUG();

  thing_is_scheduled_for_cleanup_set(g, v, l, me, false);
}

[[nodiscard]] auto thing_is_scheduled_for_worklist(Thingp me) -> bool
{
  TRACE_DEBUG();

  if (me == nullptr) {
    CROAK("no thing pointer");
    return false;
  }

  return me->_is_scheduled_for_worklist;
}

void thing_is_scheduled_for_worklist_set(Thingp me, bool val)
{
  TRACE_DEBUG();

  if (me == nullptr) {
    CROAK("no thing pointer");
    return;
  }

  if (me->_is_scheduled_for_worklist == static_cast< int >(val)) {
    return;
  }
  me->_is_scheduled_for_worklist = val;
}

void thing_is_scheduled_for_worklist_unset(Thingp me)
{
  TRACE_DEBUG();

  thing_is_scheduled_for_worklist_set(me, false);
}
