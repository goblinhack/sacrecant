//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <cstdint>
#include <limits>

[[nodiscard]] auto thing_weight(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_weight;
}

[[nodiscard]] auto thing_weight_set(Gamep g, Levelsp v, Levelp l, Thingp t, uint32_t val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }

  if (val > std::numeric_limits< decltype(t->_weight) >::max()) {
    thing_err(g, v, l, t, "value overflow: %d", val);
    return 0;
  }

  return t->_weight = val;
}

[[nodiscard]] auto thing_is_on_map(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return t->_is_on_map;
}

void thing_is_on_map_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }
  t->_is_on_map = val;
}

void thing_is_on_map_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_on_map_set(g, v, l, t, false);
}

[[nodiscard]] auto thing_is_animated_can_hflip(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_animated_can_hflip) != 0;
}

[[nodiscard]] auto thing_is_animated_no_dir(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_animated_no_dir) != 0;
}

[[nodiscard]] auto thing_is_animated_sync_first(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_animated_sync_first) != 0;
}

[[nodiscard]] auto thing_is_animated(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_animated) != 0;
}

[[nodiscard]] auto thing_is_barrel(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_barrel) != 0;
}

[[nodiscard]] auto thing_is_blit_centered(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_centered) != 0;
}

[[nodiscard]] auto thing_is_blit_flush_per_line(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_flush_per_line) != 0;
}

[[nodiscard]] auto thing_is_blit_outlined(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_outlined) != 0;
}

[[nodiscard]] auto thing_is_blit_square_outlined(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_square_outlined) != 0;
}

[[nodiscard]] auto thing_is_brazier(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_brazier) != 0;
}

[[nodiscard]] auto thing_is_bridge(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_bridge) != 0;
}

[[nodiscard]] auto thing_is_chasm(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_chasm) != 0;
}

[[nodiscard]] auto thing_is_corridor(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_corridor) != 0;
}

[[nodiscard]] auto thing_is_obs_to_cursor_path(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_cursor_path) != 0;
}

[[nodiscard]] auto thing_is_cursor_path_hazard(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_cursor_path_hazard) != 0;
}

[[nodiscard]] auto thing_is_cursor_path(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_cursor_path) != 0;
}

[[nodiscard]] auto thing_is_cursor(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_cursor) != 0;
}

[[nodiscard]] auto thing_is_water_deep(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_water_deep) != 0;
}

[[nodiscard]] auto thing_is_described_cursor(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_described_cursor) != 0;
}

[[nodiscard]] auto thing_is_dirt(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_dirt) != 0;
}

[[nodiscard]] auto thing_is_door_unlocked(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_door_unlocked) != 0;
}

[[nodiscard]] auto thing_is_dungeon_entrance(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_dungeon_entrance) != 0;
}

[[nodiscard]] auto thing_is_entrance(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_entrance) != 0;
}

[[nodiscard]] auto thing_is_exit(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_exit) != 0;
}

[[nodiscard]] auto thing_is_ghost(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_ghost) != 0;
}

[[nodiscard]] auto thing_is_kobalos(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_kobalos) != 0;
}

[[nodiscard]] auto thing_is_grass(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_grass) != 0;
}

[[nodiscard]] auto thing_is_key(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_key) != 0;
}

[[nodiscard]] auto thing_is_usable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_usable) != 0;
}

[[nodiscard]] auto thing_is_level_curr(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_curr) != 0;
}

[[nodiscard]] auto thing_is_critical_to_dungeon_design(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_critical_to_dungeon_design) != 0;
}

[[nodiscard]] auto thing_is_level_final_icon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_final_icon) != 0;
}

[[nodiscard]] auto thing_is_level_next_icon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_next_icon) != 0;
}

[[nodiscard]] auto thing_is_level_locked_icon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_locked_icon) != 0;
}

[[nodiscard]] auto thing_is_level_closed_icon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_closed_icon) != 0;
}

[[nodiscard]] auto thing_is_pillar(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_pillar) != 0;
}

[[nodiscard]] auto thing_is_rock(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_rock) != 0;
}

[[nodiscard]] auto thing_is_door_secret(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_door_secret) != 0;
}

[[nodiscard]] auto thing_is_slime(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_slime) != 0;
}

[[nodiscard]] auto thing_is_tickable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_tickable) != 0;
}

[[nodiscard]] auto thing_is_tiled(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_tiled) != 0;
}

[[nodiscard]] auto thing_is_trap(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_trap) != 0;
}

[[nodiscard]] auto thing_is_treasure(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_treasure) != 0;
}

[[nodiscard]] auto thing_is_unused1(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused1) != 0;
}

[[nodiscard]] auto thing_is_unused2(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused2) != 0;
}

[[nodiscard]] auto thing_is_unused3(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused3) != 0;
}

[[nodiscard]] auto thing_is_unused4(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused4) != 0;
}

[[nodiscard]] auto thing_is_unused5(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused5) != 0;
}

[[nodiscard]] auto thing_is_unused6(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused6) != 0;
}

[[nodiscard]] auto thing_is_unused7(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused7) != 0;
}

[[nodiscard]] auto thing_is_unused8(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused8) != 0;
}

[[nodiscard]] auto thing_is_unused9(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused9) != 0;
}

[[nodiscard]] auto thing_is_unused10(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused10) != 0;
}

[[nodiscard]] auto thing_is_unused11(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused11) != 0;
}

[[nodiscard]] auto thing_is_unused12(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused12) != 0;
}

[[nodiscard]] auto thing_is_unused13(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused13) != 0;
}

[[nodiscard]] auto thing_is_unused14(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused14) != 0;
}

[[nodiscard]] auto thing_is_unused15(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused15) != 0;
}

[[nodiscard]] auto thing_is_unused16(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused16) != 0;
}

[[nodiscard]] auto thing_is_unused17(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused17) != 0;
}

[[nodiscard]] auto thing_is_unused18(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused18) != 0;
}

[[nodiscard]] auto thing_is_unused19(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused19) != 0;
}

[[nodiscard]] auto thing_is_unused20(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused20) != 0;
}

[[nodiscard]] auto thing_is_unused21(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused21) != 0;
}

[[nodiscard]] auto thing_is_unused22(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused22) != 0;
}

[[nodiscard]] auto thing_is_unused23(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused23) != 0;
}

[[nodiscard]] auto thing_is_unused24(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused24) != 0;
}

[[nodiscard]] auto thing_is_unused25(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused25) != 0;
}

[[nodiscard]] auto thing_is_unused26(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused26) != 0;
}

[[nodiscard]] auto thing_is_unused27(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused27) != 0;
}

[[nodiscard]] auto thing_is_unused28(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused28) != 0;
}

[[nodiscard]] auto thing_is_unused29(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused29) != 0;
}

[[nodiscard]] auto thing_is_unused30(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused30) != 0;
}

[[nodiscard]] auto thing_is_unused31(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused31) != 0;
}

[[nodiscard]] auto thing_is_unused32(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused32) != 0;
}

[[nodiscard]] auto thing_is_unused33(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused33) != 0;
}

[[nodiscard]] auto thing_is_unused34(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused34) != 0;
}

[[nodiscard]] auto thing_is_unused35(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused35) != 0;
}

[[nodiscard]] auto thing_is_unused36(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused36) != 0;
}

[[nodiscard]] auto thing_is_unused37(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused37) != 0;
}

[[nodiscard]] auto thing_is_unused38(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused38) != 0;
}

[[nodiscard]] auto thing_is_unused39(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused39) != 0;
}

[[nodiscard]] auto thing_is_unused40(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused40) != 0;
}

[[nodiscard]] auto thing_is_unused41(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused41) != 0;
}

[[nodiscard]] auto thing_is_unused42(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused42) != 0;
}

[[nodiscard]] auto thing_is_unused43(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused43) != 0;
}

[[nodiscard]] auto thing_is_unused44(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused44) != 0;
}

[[nodiscard]] auto thing_is_unused45(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused45) != 0;
}

[[nodiscard]] auto thing_is_unused46(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused46) != 0;
}

[[nodiscard]] auto thing_is_unused47(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused47) != 0;
}

[[nodiscard]] auto thing_is_unused48(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused48) != 0;
}

[[nodiscard]] auto thing_is_unused49(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused49) != 0;
}

[[nodiscard]] auto thing_is_unused50(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused50) != 0;
}

[[nodiscard]] auto thing_is_unused51(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused51) != 0;
}

[[nodiscard]] auto thing_is_unused52(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused52) != 0;
}

[[nodiscard]] auto thing_is_unused53(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused53) != 0;
}

[[nodiscard]] auto thing_is_unused54(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused54) != 0;
}

[[nodiscard]] auto thing_is_unused55(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused55) != 0;
}

[[nodiscard]] auto thing_is_unused56(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused56) != 0;
}

[[nodiscard]] auto thing_is_unused57(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused57) != 0;
}

[[nodiscard]] auto thing_is_unused58(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused58) != 0;
}

[[nodiscard]] auto thing_is_unused59(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused59) != 0;
}

[[nodiscard]] auto thing_is_unused60(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused60) != 0;
}

[[nodiscard]] auto thing_is_unused61(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused61) != 0;
}

[[nodiscard]] auto thing_is_unused62(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused62) != 0;
}

[[nodiscard]] auto thing_is_unused63(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused63) != 0;
}

[[nodiscard]] auto thing_is_unused64(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused64) != 0;
}

[[nodiscard]] auto thing_is_unused65(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused65) != 0;
}

[[nodiscard]] auto thing_is_unused66(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused66) != 0;
}

[[nodiscard]] auto thing_is_unused67(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused67) != 0;
}

[[nodiscard]] auto thing_is_unused68(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused68) != 0;
}

[[nodiscard]] auto thing_is_unused69(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused69) != 0;
}

[[nodiscard]] auto thing_is_unused70(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused70) != 0;
}

[[nodiscard]] auto thing_is_unused71(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused71) != 0;
}

[[nodiscard]] auto thing_is_unused72(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused72) != 0;
}

[[nodiscard]] auto thing_is_unused73(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused73) != 0;
}

[[nodiscard]] auto thing_is_unused74(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused74) != 0;
}

[[nodiscard]] auto thing_is_unused75(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused75) != 0;
}

[[nodiscard]] auto thing_is_unused76(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused76) != 0;
}

[[nodiscard]] auto thing_is_unused77(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused77) != 0;
}

[[nodiscard]] auto thing_is_unused78(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused78) != 0;
}

[[nodiscard]] auto thing_is_unused79(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused79) != 0;
}

[[nodiscard]] auto thing_is_unused80(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused80) != 0;
}

[[nodiscard]] auto thing_is_unused81(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused81) != 0;
}

[[nodiscard]] auto thing_is_unused82(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused82) != 0;
}

[[nodiscard]] auto thing_is_unused83(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused83) != 0;
}

[[nodiscard]] auto thing_is_unused84(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused84) != 0;
}

[[nodiscard]] auto thing_is_unused85(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused85) != 0;
}

[[nodiscard]] auto thing_is_unused86(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused86) != 0;
}

[[nodiscard]] auto thing_is_unused87(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused87) != 0;
}

[[nodiscard]] auto thing_is_unused88(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused88) != 0;
}

[[nodiscard]] auto thing_is_unused89(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused89) != 0;
}

[[nodiscard]] auto thing_is_unused90(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused90) != 0;
}

[[nodiscard]] auto thing_is_unused91(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused91) != 0;
}

[[nodiscard]] auto thing_is_unused92(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused92) != 0;
}

[[nodiscard]] auto thing_is_unused93(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused93) != 0;
}

[[nodiscard]] auto thing_is_unused94(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused94) != 0;
}

[[nodiscard]] auto thing_is_unused95(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused95) != 0;
}

[[nodiscard]] auto thing_is_unused96(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused96) != 0;
}

[[nodiscard]] auto thing_is_unused97(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused97) != 0;
}

[[nodiscard]] auto thing_is_unused98(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused98) != 0;
}

[[nodiscard]] auto thing_is_unused99(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused99) != 0;
}

[[nodiscard]] auto thing_is_unused100(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused100) != 0;
}

[[nodiscard]] auto thing_is_unused101(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused101) != 0;
}

[[nodiscard]] auto thing_is_unused102(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused102) != 0;
}

[[nodiscard]] auto thing_is_unused103(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused103) != 0;
}

[[nodiscard]] auto thing_is_unused104(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused104) != 0;
}

[[nodiscard]] auto thing_is_unused105(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused105) != 0;
}

[[nodiscard]] auto thing_is_unused106(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused106) != 0;
}

[[nodiscard]] auto thing_is_unused107(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused107) != 0;
}

[[nodiscard]] auto thing_is_unused108(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused108) != 0;
}

[[nodiscard]] auto thing_is_unused109(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused109) != 0;
}

[[nodiscard]] auto thing_is_unused110(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused110) != 0;
}

[[nodiscard]] auto thing_is_unused111(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused111) != 0;
}

[[nodiscard]] auto thing_is_unused112(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused112) != 0;
}

[[nodiscard]] auto thing_is_unused113(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused113) != 0;
}

[[nodiscard]] auto thing_is_unused114(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused114) != 0;
}

[[nodiscard]] auto thing_is_unused115(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused115) != 0;
}

[[nodiscard]] auto thing_is_unused116(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused116) != 0;
}

[[nodiscard]] auto thing_is_unused117(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused117) != 0;
}

[[nodiscard]] auto thing_is_unused118(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused118) != 0;
}

[[nodiscard]] auto thing_is_unused119(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused119) != 0;
}

[[nodiscard]] auto thing_is_unused120(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused120) != 0;
}

[[nodiscard]] auto thing_is_unused121(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused121) != 0;
}

[[nodiscard]] auto thing_is_unused122(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused122) != 0;
}

[[nodiscard]] auto thing_is_unused123(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused123) != 0;
}

[[nodiscard]] auto thing_is_unused124(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused124) != 0;
}

[[nodiscard]] auto thing_is_unused125(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused125) != 0;
}

[[nodiscard]] auto thing_is_unused126(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused126) != 0;
}

[[nodiscard]] auto thing_is_unused127(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused127) != 0;
}

[[nodiscard]] auto thing_is_unused128(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused128) != 0;
}

[[nodiscard]] auto thing_is_unused129(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused129) != 0;
}

[[nodiscard]] auto thing_is_unused130(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused130) != 0;
}

[[nodiscard]] auto thing_is_unused131(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused131) != 0;
}

[[nodiscard]] auto thing_is_unused132(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused132) != 0;
}

[[nodiscard]] auto thing_is_unused133(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused133) != 0;
}

[[nodiscard]] auto thing_is_unused134(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused134) != 0;
}

[[nodiscard]] auto thing_is_unused135(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused135) != 0;
}

[[nodiscard]] auto thing_is_unused136(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused136) != 0;
}

[[nodiscard]] auto thing_is_unused137(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused137) != 0;
}

[[nodiscard]] auto thing_is_unused138(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused138) != 0;
}

[[nodiscard]] auto thing_is_unused139(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused139) != 0;
}

[[nodiscard]] auto thing_is_unused140(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused140) != 0;
}

[[nodiscard]] auto thing_is_unused141(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused141) != 0;
}

[[nodiscard]] auto thing_is_unused142(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused142) != 0;
}

[[nodiscard]] auto thing_is_unused143(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused143) != 0;
}

[[nodiscard]] auto thing_is_unused144(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused144) != 0;
}

[[nodiscard]] auto thing_is_unused145(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused145) != 0;
}

[[nodiscard]] auto thing_is_unused146(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused146) != 0;
}

[[nodiscard]] auto thing_is_unused147(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused147) != 0;
}

[[nodiscard]] auto thing_is_unused148(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused148) != 0;
}

[[nodiscard]] auto thing_is_unused149(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused149) != 0;
}

[[nodiscard]] auto thing_is_unused150(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused150) != 0;
}

[[nodiscard]] auto thing_is_unused151(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused151) != 0;
}

[[nodiscard]] auto thing_is_unused152(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused152) != 0;
}

[[nodiscard]] auto thing_is_unused153(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused153) != 0;
}

[[nodiscard]] auto thing_is_weaponless_on_spawn(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_weaponless_on_spawn) != 0;
}

[[nodiscard]] auto thing_is_hook(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_hook) != 0;
}

[[nodiscard]] auto thing_is_sacrifice(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_sacrifice) != 0;
}

[[nodiscard]] auto thing_is_debuff(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_debuff) != 0;
}

[[nodiscard]] auto thing_is_venomous(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_venomous) != 0;
}

[[nodiscard]] auto thing_is_able_to_see_invisible(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_see_invisible) != 0;
}

[[nodiscard]] auto thing_is_critical_to_gameplay(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_critical_to_gameplay) != 0;
}

[[nodiscard]] auto thing_is_fish(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_fish) != 0;
}

[[nodiscard]] auto thing_is_unlucky(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unlucky) != 0;
}

[[nodiscard]] auto thing_is_cursed(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_cursed) != 0;
}

[[nodiscard]] auto thing_is_chocolate(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_chocolate) != 0;
}

[[nodiscard]] auto thing_is_tick_on_eaten(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_tick_on_eaten) != 0;
}

[[nodiscard]] auto thing_is_able_to_eat_food(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_eat_food) != 0;
}

[[nodiscard]] auto thing_is_edible(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_edible) != 0;
}

[[nodiscard]] auto thing_is_able_to_be_engulfed_blocked(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_be_engulfed_blocked) != 0;
}

[[nodiscard]] auto thing_is_able_to_be_ensnared_blocked(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_be_ensnared_blocked) != 0;
}

[[nodiscard]] auto thing_is_active_when_worn(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_active_when_worn) != 0;
}

[[nodiscard]] auto thing_is_leather(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_leather) != 0;
}

[[nodiscard]] auto thing_is_boots(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_boots) != 0;
}

[[nodiscard]] auto thing_is_spider_giant(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_spider_giant) != 0;
}

[[nodiscard]] auto thing_is_spider_baby(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_spider_baby) != 0;
}

[[nodiscard]] auto thing_is_able_to_choose_targets(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_choose_targets) != 0;
}

[[nodiscard]] auto thing_is_flesh_eater(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_flesh_eater) != 0;
}

[[nodiscard]] auto thing_is_able_to_move(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_move) != 0;
}

[[nodiscard]] auto thing_is_spider(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_spider) != 0;
}

[[nodiscard]] auto thing_is_spiderweb(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_spiderweb) != 0;
}

[[nodiscard]] auto thing_is_rubble(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_rubble) != 0;
}

[[nodiscard]] auto thing_is_unused188(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused188) != 0;
}

[[nodiscard]] auto thing_is_skeleton(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_skeleton) != 0;
}

[[nodiscard]] auto thing_is_water_bound(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_water_bound) != 0;
}

[[nodiscard]] auto thing_is_amphibious(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_amphibious) != 0;
}

[[nodiscard]] auto thing_is_soft_landing(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_soft_landing) != 0;
}

[[nodiscard]] auto thing_is_gelatinous(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_gelatinous) != 0;
}

[[nodiscard]] auto thing_is_potion(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_potion) != 0;
}

[[nodiscard]] auto thing_is_teleport_hazard(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_teleport_hazard) != 0;
}

[[nodiscard]] auto thing_is_active_when_carried(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_active_when_carried) != 0;
}

[[nodiscard]] auto thing_is_wearable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_wearable) != 0;
}

[[nodiscard]] auto thing_is_ring(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_ring) != 0;
}

[[nodiscard]] auto thing_is_lava_bg(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_lava_bg) != 0;
}

[[nodiscard]] auto thing_is_light_flicker(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_light_flicker) != 0;
}

[[nodiscard]] auto thing_is_able_to_drop_all_items_on_death(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_drop_all_items_on_death) != 0;
}

[[nodiscard]] auto thing_is_effect_ripple(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_effect_ripple) != 0;
}

[[nodiscard]] auto thing_is_shown_noise(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_shown_noise) != 0;
}

[[nodiscard]] auto thing_is_obs_to_hearing(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_hearing) != 0;
}

[[nodiscard]] auto thing_is_throwable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_throwable) != 0;
}

[[nodiscard]] auto thing_is_obs_to_throwing_over(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_throwing_over) != 0;
}

[[nodiscard]] auto thing_is_obs_to_throwing_onto(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_throwing_onto) != 0;
}

[[nodiscard]] auto thing_is_physics_trap(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_physics_trap) != 0;
}

[[nodiscard]] auto thing_is_tick_on_strip(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_tick_on_strip) != 0;
}

[[nodiscard]] auto thing_is_tick_on_worn(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_tick_on_worn) != 0;
}

[[nodiscard]] auto thing_is_buff(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_buff) != 0;
}

[[nodiscard]] auto thing_is_able_to_be_buffed(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(t)) {
    return false;
  }

  return tp_flag(thing_tp(t), is_able_to_be_buffed) != 0;
}

[[nodiscard]] auto thing_is_tick_on_drop(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_tick_on_drop) != 0;
}

[[nodiscard]] auto thing_is_tick_on_use(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_tick_on_use) != 0;
}

[[nodiscard]] auto thing_is_able_to_throw(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(t)) {
    return false;
  }

  return tp_flag(thing_tp(t), is_able_to_throw) != 0;
}

[[nodiscard]] auto thing_is_chest(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_chest) != 0;
}

[[nodiscard]] auto thing_is_flammable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_flammable) != 0;
}

[[nodiscard]] auto thing_is_auto_wear(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_auto_wear) != 0;
}

[[nodiscard]] auto thing_is_able_to_fire_weapons(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  return tp_flag(thing_tp(t), is_able_to_fire_weapons) != 0;
}

[[nodiscard]] auto thing_is_weapon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_weapon) != 0;
}

[[nodiscard]] auto thing_is_able_to_wear_items(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(t)) {
    return false;
  }

  return tp_flag(thing_tp(t), is_able_to_wear_items) != 0;
}

[[nodiscard]] auto thing_is_wand(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_wand) != 0;
}

[[nodiscard]] auto thing_is_staff(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_staff) != 0;
}

[[nodiscard]] auto thing_is_able_to_eat_treasure(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_eat_treasure) != 0;
}

[[nodiscard]] auto thing_is_wait_on_anim(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_wait_on_anim) != 0;
}

[[nodiscard]] auto thing_is_flat(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_flat) != 0;
}

[[nodiscard]] auto thing_is_obs_to_beam(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_beam) != 0;
}

[[nodiscard]] auto thing_is_beam_weapon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_beam_weapon) != 0;
}

[[nodiscard]] auto thing_is_vault(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_vault) != 0;
}

[[nodiscard]] auto thing_is_biome_underhell(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_biome_underhell) != 0;
}

[[nodiscard]] auto thing_is_biome_graveyard(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_biome_graveyard) != 0;
}

[[nodiscard]] auto thing_is_biome_nethervoid(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_biome_nethervoid) != 0;
}

[[nodiscard]] auto thing_is_biome_bogland(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_biome_bogland) != 0;
}

[[nodiscard]] auto thing_is_biome_dungeon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_biome_dungeon) != 0;
}

[[nodiscard]] auto thing_is_level_open_icon(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_open_icon) != 0;
}

[[nodiscard]] auto thing_is_blit_bg(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_bg) != 0;
}

[[nodiscard]] auto thing_is_level_select_bg(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_level_select_bg) != 0;
}

[[nodiscard]] auto thing_is_blit_hit_outline_w_black_inside(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_hit_outline_w_black_inside) != 0;
}

[[nodiscard]] auto thing_is_blit_hit_outline_w_invis_inside(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_hit_outline_w_invis_inside) != 0;
}

[[nodiscard]] auto thing_is_blit_when_obscured_outline(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_when_obscured_outline) != 0;
}

[[nodiscard]] auto thing_is_effect(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_effect) != 0;
}

[[nodiscard]] auto thing_is_effect_attack(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_effect_attack) != 0;
}

[[nodiscard]] auto thing_is_effect_blood(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_effect_blood) != 0;
}

[[nodiscard]] auto thing_is_grouped_thing(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_grouped_thing) != 0;
}

[[nodiscard]] auto thing_is_able_to_lunge(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(t)) {
    return false;
  }

  return tp_flag(thing_tp(t), is_able_to_lunge) != 0;
}

[[nodiscard]] auto thing_is_obs_to_paths(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_paths) != 0;
}

[[nodiscard]] auto thing_is_blit_on_ground(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_on_ground) != 0;
}

[[nodiscard]] auto thing_is_insectoid(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_insectoid) != 0;
}

[[nodiscard]] auto thing_is_able_to_eat_items(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_eat_items) != 0;
}

[[nodiscard]] auto thing_is_dmap(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_dmap) != 0;
}

[[nodiscard]] auto thing_is_obs_to_spawning(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_spawning) != 0;
}

[[nodiscard]] auto thing_is_border(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_border) != 0;
}

[[nodiscard]] auto thing_is_gold(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_gold) != 0;
}

[[nodiscard]] auto thing_is_glass(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_glass) != 0;
}

[[nodiscard]] auto thing_is_plant(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_plant) != 0;
}

[[nodiscard]] auto thing_is_wood(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_wood) != 0;
}

[[nodiscard]] auto thing_is_metal(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_metal) != 0;
}

[[nodiscard]] auto thing_is_flesh(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_flesh) != 0;
}

[[nodiscard]] auto thing_is_stone(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_stone) != 0;
}

[[nodiscard]] auto thing_is_meltable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_meltable) != 0;
}

[[nodiscard]] auto thing_is_blit_per_pixel_lighting(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_per_pixel_lighting) != 0;
}

[[nodiscard]] auto thing_is_cursor_path_warning(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_cursor_path_warning) != 0;
}

[[nodiscard]] auto thing_is_lucky(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_lucky) != 0;
}

[[nodiscard]] auto thing_is_projectile(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_projectile) != 0;
}

[[nodiscard]] auto thing_is_item_mergeable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_item_mergeable) != 0;
}

[[nodiscard]] auto thing_is_door_locked(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_door_locked) != 0;
}

[[nodiscard]] auto thing_is_inventory_item(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_inventory_item) != 0;
}

[[nodiscard]] auto thing_is_able_to_open_things(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(t)) {
    return false;
  }

  return tp_flag(thing_tp(t), is_able_to_open_things) != 0;
}

[[nodiscard]] auto thing_is_item(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_item) != 0;
}

[[nodiscard]] auto thing_is_able_to_crush_grass(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(t)) {
    return false;
  }

  return tp_flag(thing_tp(t), is_able_to_crush_grass) != 0;
}

[[nodiscard]] auto thing_is_able_to_collect_keys(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(t)) {
    return false;
  }

  return tp_flag(thing_tp(t), is_able_to_collect_keys) != 0;
}

[[nodiscard]] auto thing_is_able_to_collect_items(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (thing_is_dead(t)) {
    return false;
  }

  return tp_flag(thing_tp(t), is_able_to_collect_items) != 0;
}

[[nodiscard]] auto thing_is_collectable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_collectable) != 0;
}

[[nodiscard]] auto thing_is_blit_when_obscured_faded(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_when_obscured_faded) != 0;
}

[[nodiscard]] auto thing_is_openable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_openable) != 0;
}

[[nodiscard]] auto thing_is_tick_end_delay(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_tick_end_delay) != 0;
}

[[nodiscard]] auto thing_is_physics_explosion(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_physics_explosion) != 0;
}

[[nodiscard]] auto thing_is_obs_to_explosion(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_explosion) != 0;
}

[[nodiscard]] auto thing_is_cursor_path_none(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_cursor_path_none) != 0;
}

[[nodiscard]] auto thing_is_blit_obscures(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_obscures) != 0;
}

[[nodiscard]] auto thing_is_obs_to_ethereal(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_to_ethereal) != 0;
}

[[nodiscard]] auto thing_is_physics_water(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_physics_water) != 0;
}

[[nodiscard]] auto thing_is_indestructible(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_indestructible) != 0;
}

[[nodiscard]] auto thing_is_obs_to_fire(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  //
  // Unless open
  //
  if (thing_is_openable(t)) {
    if (thing_is_open(t)) {
      return false;
    }
  }

  return tp_flag(thing_tp(t), is_obs_to_fire) != 0;
}

[[nodiscard]] auto thing_is_combustible(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_combustible) != 0;
}

[[nodiscard]] auto thing_is_blit_shown_in_chasms(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_shown_in_chasms) != 0;
}

[[nodiscard]] auto thing_is_gaseous(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_gaseous) != 0;
}

[[nodiscard]] auto thing_is_blit_if_has_seen(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_if_has_seen) != 0;
}

[[nodiscard]] auto thing_is_smoke(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_smoke) != 0;
}

[[nodiscard]] auto thing_is_steam(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_steam) != 0;
}

[[nodiscard]] auto thing_is_burnable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_burnable) != 0;
}

[[nodiscard]] auto thing_is_loggable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }

  if (g_opt_tests) {
    //
    // Everything is logged
    //
    return true;
  }

  IF_DEBUG2
  {
    //
    // Everything is logged
    //
    return true;
  }

  return tp_flag(thing_tp(t), is_loggable) != 0;
}

[[nodiscard]] auto thing_is_crushable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_crushable) != 0;
}

[[nodiscard]] auto thing_is_ethereal(Gamep g, Levelsp v, Levelp l, Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_ethereal) != 0;
}

[[nodiscard]] auto thing_is_blit_shown_in_overlay(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_blit_shown_in_overlay) != 0;
}

[[nodiscard]] auto thing_is_unused183(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_unused183) != 0;
}

[[nodiscard]] auto thing_is_droppable(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_droppable) != 0;
}

[[nodiscard]] auto thing_is_able_to_see_through_walls(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_able_to_see_through_walls) != 0;
}

[[nodiscard]] auto thing_value1(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value1;
}

[[nodiscard]] auto thing_value1_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value1 = val;
}

[[nodiscard]] auto thing_value1_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value1 += val;
}

[[nodiscard]] auto thing_value1_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value1) - val <= 0) {
    return t->_value1 = 0;
  }
  return t->_value1 -= val;
}
[[nodiscard]] auto thing_value2(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value2;
}

[[nodiscard]] auto thing_value2_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value2 = val;
}

[[nodiscard]] auto thing_value2_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value2 += val;
}

[[nodiscard]] auto thing_value2_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value2) - val <= 0) {
    return t->_value2 = 0;
  }
  return t->_value2 -= val;
}
[[nodiscard]] auto thing_value3(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value3;
}

[[nodiscard]] auto thing_value3_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value3 = val;
}

[[nodiscard]] auto thing_value3_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value3 += val;
}

[[nodiscard]] auto thing_value3_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value3) - val <= 0) {
    return t->_value3 = 0;
  }
  return t->_value3 -= val;
}
[[nodiscard]] auto thing_value4(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value4;
}

[[nodiscard]] auto thing_value4_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value4 = val;
}

[[nodiscard]] auto thing_value4_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value4 += val;
}

[[nodiscard]] auto thing_value4_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value4) - val <= 0) {
    return t->_value4 = 0;
  }
  return t->_value4 -= val;
}
[[nodiscard]] auto thing_value5(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value5;
}

[[nodiscard]] auto thing_value5_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value5 = val;
}

[[nodiscard]] auto thing_value5_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value5 += val;
}

[[nodiscard]] auto thing_value5_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value5) - val <= 0) {
    return t->_value5 = 0;
  }
  return t->_value5 -= val;
}
[[nodiscard]] auto thing_value6(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value6;
}

[[nodiscard]] auto thing_value6_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value6 = val;
}

[[nodiscard]] auto thing_value6_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value6 += val;
}

[[nodiscard]] auto thing_value6_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value6) - val <= 0) {
    return t->_value6 = 0;
  }
  return t->_value6 -= val;
}
[[nodiscard]] auto thing_value7(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value7;
}

[[nodiscard]] auto thing_value7_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value7 = val;
}

[[nodiscard]] auto thing_value7_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_value7 += val;
}

[[nodiscard]] auto thing_value7_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_value7) - val <= 0) {
    return t->_value7 = 0;
  }
  return t->_value7 -= val;
}
[[nodiscard]] auto thing_difficulty(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_difficulty;
}

[[nodiscard]] auto thing_difficulty_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_difficulty = val;
}

[[nodiscard]] auto thing_difficulty_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_difficulty += val;
}

[[nodiscard]] auto thing_difficulty_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_difficulty) - val <= 0) {
    return t->_difficulty = 0;
  }
  return t->_difficulty -= val;
}

[[nodiscard]] auto thing_items_collected_max(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_items_collected_max;
}

[[nodiscard]] auto thing_items_collected_max_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_items_collected_max = val;
}

[[nodiscard]] auto thing_items_collected_max_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_items_collected_max += val;
}

[[nodiscard]] auto thing_items_collected_max_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_items_collected_max) - val <= 0) {
    return t->_items_collected_max = 0;
  }
  return t->_items_collected_max -= val;
}
[[nodiscard]] auto thing_hearing_threshold(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_hearing_threshold;
}

[[nodiscard]] auto thing_hearing_threshold_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_hearing_threshold = val;
}

[[nodiscard]] auto thing_hearing_threshold_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_hearing_threshold += val;
}

[[nodiscard]] auto thing_hearing_threshold_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_hearing_threshold) - val <= 0) {
    return t->_hearing_threshold = 0;
  }
  return t->_hearing_threshold -= val;
}
[[nodiscard]] auto thing_distance_avoid_target(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_avoid_target;
}

[[nodiscard]] auto thing_distance_avoid_target_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_avoid_target = val;
}

[[nodiscard]] auto thing_distance_avoid_target_incr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_distance_avoid_target += val;
}

[[nodiscard]] auto thing_distance_avoid_target_decr(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  if (static_cast< int >(t->_distance_avoid_target) - val <= 0) {
    return t->_distance_avoid_target = 0;
  }
  return t->_distance_avoid_target -= val;
}

[[nodiscard]] auto thing_variant(Thingp t) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_variant;
}

[[nodiscard]] auto thing_variant_set(Gamep g, Levelsp v, Levelp l, Thingp t, int val) -> int
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return 0;
  }
  return t->_variant = val;
}

[[nodiscard]] auto thing_priority(Thingp t) -> ThingPriorityType
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return THING_PRIORITY_LOWEST;
  }
  if (thing_is_dead(t)) {
    return THING_PRIORITY_DEAD;
  }
  return t->_priority;
}

[[nodiscard]] auto thing_priority_set(Gamep g, Levelsp v, Levelp l, Thingp t, ThingPriorityType val) -> ThingPriorityType
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return THING_PRIORITY_LOWEST;
  }
  return t->_priority = val;
}
