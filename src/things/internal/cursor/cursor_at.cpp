//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

static auto tp_cursor_at_display_get_tile_info(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t_maybe_null) -> Tilep
{
  TRACE();

  //
  // Default tile
  //
  Tilep tile = tp_tiles_get(tp, THING_ANIM_CURSOR_NOPATH, 0);

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return tile;
  }

  if (! level_has_seen(g, v, l, p)) {
    return tile;
  }

  //
  // Non zero cursor path, change the cursor to a positive color
  //
  auto *ext_struct = thing_ext_struct(g, player);
  if (ext_struct == nullptr) {
    return tile;
  }

  if (level_is_cursor_path_warning(g, v, l, p) != nullptr) {
    return tp_tiles_get(tp, THING_ANIM_CURSOR_WARNING, 0);
  }

  if (level_is_cursor_path_hazard(g, v, l, p) != nullptr) {
    return tp_tiles_get(tp, THING_ANIM_CURSOR_HAZARD, 0);
  }

  //
  // If we have a path, show the cursor highlighted
  //
  if ((level_cursor_path_size(g) != 0) || (thing_move_path_size(g, v, l, player) != 0)) {
    return tp_tiles_get(tp, THING_ANIM_CURSOR_NORMAL, 0);
  }

  //
  // If we are over a level, show the cursor highlighted
  //
  if (level_is_level_select(g, v, l)) {
    auto *s = level_select_get_level_at_tile_coords(g, v, p);
    if (s != nullptr) {
      return tp_tiles_get(tp, THING_ANIM_CURSOR_NORMAL, 0);
    }
  }

  return tile;
}

auto tp_load_cursor_at() -> bool
{
  auto *tp   = tp_load("cursor_at"); // keep as string for scripts
  auto  name = tp_name(tp);
  // begin sort marker1 {
  thing_display_get_tile_info_set(tp, tp_cursor_at_display_get_tile_info);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_shown_in_overlay);
  tp_flag_set(tp, is_cursor);
  // end sort marker1 }

  auto *tile = tile_find_mand("cursor_at.nopath");
  tp_tiles_push_back(tp, THING_ANIM_CURSOR_NOPATH, tile);
  tile = tile_find_mand("cursor_at.normal");
  tp_tiles_push_back(tp, THING_ANIM_CURSOR_NORMAL, tile);
  tile = tile_find_mand("cursor_at.warning");
  tp_tiles_push_back(tp, THING_ANIM_CURSOR_WARNING, tile);
  tile = tile_find_mand("cursor_at.hazard");
  tp_tiles_push_back(tp, THING_ANIM_CURSOR_HAZARD, tile);

  return true;
}
