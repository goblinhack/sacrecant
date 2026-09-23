//
// Copyright goblinhack@gmail.com
//

#include "../../../my_callstack.hpp"
#include "../../../my_level.hpp"
#include "../../../my_main.hpp"
#include "../../../my_thing.hpp"
#include "../../../my_thing_callbacks.hpp"
#include "../../../my_thing_inlines.hpp"
#include "../../../my_tile.hpp"
#include "../../../my_tp.hpp"
#include "../../../my_tps.hpp"
#include "../../../my_types.hpp"

[[nodiscard]] auto tp_load_spell_effect() -> bool
{
  auto *tp   = tp_load("spell_effect"); // keep as string for scripts
  auto  name = tp_name(tp);
  // begin sort marker1 {
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_shown_in_overlay);
  tp_flag_set(tp, is_internal);
  // end sort marker1 }

  auto *tile = tile_find_mand("spell_effect");
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);

  return true;
}
