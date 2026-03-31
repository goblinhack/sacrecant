//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_thing_inlines.hpp"

#include <iostream>

auto thing_get(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot) -> Thingp
{
  TRACE_DEBUG();

  if (l == nullptr) {
    err("trying to get a thing on a null level");
    return nullptr;
  }

  if (is_oob(p)) [[unlikely]] {
    err("out of bounds thing_get %d,%d, slot %d", p.x, p.y, slot);
    return nullptr;
  }

  if ((slot < 0) || (slot >= MAP_SLOTS)) {
    err("slot out of bounds thing_get %d,%d, slot %d", p.x, p.y, slot);
    return nullptr;
  }

  ThingId const id = l->thing_id[ p.x ][ p.y ][ slot ];
  if (id == 0U) {
    return nullptr;
  }

  auto *t = thing_find(g, v, id);
  if (t == nullptr) {
    return nullptr;
  }

  return t;
}

auto thing_get_at_safe(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot) -> Thingp
{
  TRACE_DEBUG();

  ThingId const id = l->thing_id[ p.x ][ p.y ][ slot ];

  if (id == 0U) {
    return nullptr;
  }

  auto *t = thing_find(g, v, id);
  if (t == nullptr) {
    return nullptr;
  }

  return t;
}

auto thing_and_tp_get_at_safe(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot, Tpp *out) -> Thingp
{
  TRACE_DEBUG();

  ThingId const id = l->thing_id[ p.x ][ p.y ][ slot ];

  if (id == 0U) {
    *out = nullptr;
    return nullptr;
  }

  auto *t = thing_find(g, v, id);
  if (t == nullptr) {
    *out = nullptr;
    return nullptr;
  }

  *out = tp_find(t->tp_id);

  return t;
}

auto thing_and_tp_get_at(Gamep g, Levelsp v, Levelp l, const bpoint &p, int slot, Tpp *out) -> Thingp
{
  TRACE_DEBUG();

  *out = nullptr;

  if (l == nullptr) {
    err("trying to get a thing on a null level");
    return nullptr;
  }

  if (is_oob(p)) [[unlikely]] {
    err("out of bounds thing_and_tp_get_at %d,%d, slot %d", p.x, p.y, slot);
    return nullptr;
  }

  if ((slot < 0) || (slot >= MAP_SLOTS)) {
    err("slot out of bounds thing_and_tp_get_at %d,%d, slot %d", p.x, p.y, slot);
    return nullptr;
  }

  ThingId const id = l->thing_id[ p.x ][ p.y ][ slot ];

  if (id == 0U) {
    return nullptr;
  }

  auto *t = thing_find(g, v, id);
  if (t == nullptr) {
    return nullptr;
  }

  *out = tp_find(t->tp_id);

  return t;
}

void thing_stats_dump(Gamep g, Levelsp v)
{
  TRACE();

  int in_use_things = 0;
  int free_things   = 0;

  for (auto &index : v->thing_body) {
    auto *t = &index;

#ifdef ENABLE_PER_THING_MEMORY
    if (v->thing_body_debug[ index ]) {
      t = v->thing_body_debug[ index ];
    }
#endif

    if (t->id != 0U) {
      in_use_things++;
    } else {
      free_things++;
    }
  }

  log("thing stats:");
  log("- Total things        %u out of max %u", in_use_things + free_things, THING_ID_MAX);
  log("- In use things       %u", in_use_things);
  log("- Free things         %u", free_things);
  log("- Ext mem things      %u out of max %u", v->thing_ext_count, THING_EXT_MAX);
}

auto thing_ext_struct(Gamep g, Thingp t) -> ThingExtp
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return nullptr;
  }

  auto ext_id = t->ext_id;
  if (ext_id == 0U) {
    return nullptr;
  }

  return &v->thing_ext[ ext_id ];
}

auto thing_light_struct(Gamep g, Thingp t) -> ThingLightp
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return nullptr;
  }

  auto light_id = t->light_id;
  if (light_id == 0U) {
    return nullptr;
  }

  return &v->thing_light[ light_id ];
}

auto thing_player_struct(Gamep g) -> ThingPlayerp
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) {
    return nullptr;
  }

  return &v->thing_player;
}
