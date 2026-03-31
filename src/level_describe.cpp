//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_ui.hpp"
#include "my_wids.hpp"

//
// Update the count of things being described
//
void level_cursor_describe_update(Gamep g, Levelsp v)
{
  TRACE();

  if (g == nullptr) {
    err("no game pointer");
    return;
  }

  if (v == nullptr) {
    err("no levels pointer");
    return;
  }

  v->describe_count = 0;

  for (uint32_t const i : v->describe) {
    if (i != 0U) {
      v->describe_count++;
    }
  }
}

//
// Add a thing to be described
//
auto level_cursor_describe_add(Gamep g, Levelsp v, Thingp t) -> bool
{
  TRACE();

  if (g == nullptr) {
    err("no game pointer");
    return false;
  }

  if (v == nullptr) {
    err("no levels pointer");
    return false;
  }

  if (t == nullptr) {
    err("no thing pointer");
    return false;
  }

  for (uint32_t const i : v->describe) {
    auto *cand = thing_find_optional(g, v, i);
    if (cand == t) {
      return true;
    }
  }

  for (uint32_t &i : v->describe) {
    auto *cand = thing_find_optional(g, v, i);
    if (cand == nullptr) {
      i = t->id;
      game_request_to_remake_ui_set(g);
      level_cursor_describe_update(g, v);
      return true;
    }
  }

  return false;
}

//
// Remove a thing from the describe array
//
auto level_cursor_describe_remove(Gamep g, Levelsp v, Thingp t) -> bool
{
  TRACE();

  if (g == nullptr) {
    err("no game pointer");
    return false;
  }

  if (v == nullptr) {
    err("no levels pointer");
    return false;
  }

  if (t == nullptr) {
    err("no thing pointer");
    return false;
  }

  for (uint32_t &i : v->describe) {
    auto *cand = thing_find_optional(g, v, t->id);
    if (cand == t) {
      i = 0;
      game_request_to_remake_ui_set(g);
      level_cursor_describe_update(g, v);
      return true;
    }
  }
  return false;
}

//
// Reset what things are being described
//
void level_cursor_describe_clear(Gamep g, Levelsp v)
{
  TRACE();

  if (g == nullptr) {
    err("no game pointer");
    return;
  }

  if (v == nullptr) {
    err("no levels pointer");
    return;
  }

  if (v->describe_count != 0U) {
    memset(v->describe, 0, sizeof(v->describe));
    game_request_to_remake_ui_set(g);
    level_cursor_describe_update(g, v);
  }
}

//
// Describe what is under the cursor
//
void level_cursor_describe(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  if (g == nullptr) {
    err("no game pointer");
    return;
  }

  if (v == nullptr) {
    err("no levels pointer");
    return;
  }

  if (l == nullptr) {
    err("no level pointer");
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    err("no player pointer");
    return;
  }

  //
  // Only if over the map
  //
  if (! level_cursor_is_valid(g, v)) {
    return;
  }

  //
  // Only if seen
  //
  auto at = v->cursor_at;
  if (! thing_vision_can_see_tile(g, v, l, player, at)) {
    botcon_newline();
    botcon_newline();
    if (compiler_unused) {
      botcon("cannot see here.");
    }
    return;
  }

  std::string all_things_description;

  level_cursor_describe_clear(g, v);

  FOR_ALL_THINGS_AT(g, v, l, it, at)
  {
    if (! thing_is_described_cursor(it)) {
      continue;
    }

    auto one_desc = thing_description_get(g, v, l, it);
    if (one_desc.empty()) {
      continue;
    }

    auto one_detail = thing_detail_get(g, v, l, it);
    if (! one_detail.empty()) {
      (void) level_cursor_describe_add(g, v, it);
    }

    //
    // Water and deep water exist at the same tile. Don't describe twice.
    //
    if (thing_is_water(it) && (level_is_deep_water(g, v, l, at) != nullptr)) {
      continue;
    }

    one_desc = capitalize_first(one_desc);

    if (all_things_description.empty()) {
      all_things_description = one_desc;
      continue;
    }

    all_things_description += ". ";
    all_things_description += one_desc;
  }

  botcon_newline();

  if ((static_cast< int >(all_things_description.empty())) != 0) {
    if (compiler_unused) {
      botcon("Nothing here.");
    }
    botcon_newline();
    return;
  }

  if (static_cast< int >(all_things_description.size()) > UI_BOTCON_WIDTH - 1) {
    botcon("Multiple items here.");
    return;
  }

  all_things_description += ".";
  wid_botcon_log(all_things_description);
}
