//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"

static auto tp_cursor_path_display_get_tile_info(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t_maybe_null) -> Tilep
{
  TRACE();

  //
  // Default tile
  //
  Tilep tile = tp_tiles_get(tp, THING_ANIM_CURSOR_NOPATH, 0);

  auto *player = thing_player(g);
  if (player) {
    if (! level_has_seen(g, v, l, p)) {
      return tile;
    }
  }

  if (level_is_cursor_path_warning(g, v, l, p) != nullptr) {
    return tp_tiles_get(tp, THING_ANIM_CURSOR_WARNING, 0);
  }

  if (level_is_cursor_path_hazard(g, v, l, p) != nullptr) {
    return tp_tiles_get(tp, THING_ANIM_CURSOR_HAZARD, 0);
  }

  tile = tp_tiles_get(tp, THING_ANIM_CURSOR_NORMAL, 0);

  return tile;
}

auto tp_load_cursor_path() -> bool
{
  auto *tp   = tp_load("cursor_path"); // keep as string for scripts
  auto  name = tp_name(tp);
  // begin sort marker1 {
  thing_display_get_tile_info_set(tp, tp_cursor_path_display_get_tile_info);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_shown_in_overlay);
  tp_flag_set(tp, is_cursor_path);
  // end sort marker1 }

  auto *tile = tile_find_mand("cursor_path.nopath");
  tp_tiles_push_back(tp, THING_ANIM_CURSOR_NOPATH, tile);
  tile = tile_find_mand("cursor_path.normal");
  tp_tiles_push_back(tp, THING_ANIM_CURSOR_NORMAL, tile);
  tile = tile_find_mand("cursor_path.warning");
  tp_tiles_push_back(tp, THING_ANIM_CURSOR_WARNING, tile);
  tile = tile_find_mand("cursor_path.hazard");
  tp_tiles_push_back(tp, THING_ANIM_CURSOR_HAZARD, tile);

  return true;
}
