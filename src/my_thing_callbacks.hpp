//
// Copyright goblinhack@gmail.com
//

#ifndef MY_THING_CALLBACKS_HPP
#define MY_THING_CALLBACKS_HPP

#include "my_bpoint.hpp"
#include "my_thing.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"

#include <string>

using thing_display_get_tile_info_t = Tilep (*)(Gamep, Levelsp, Levelp, const bpoint &p, Tpp tp, Thingp t_maybe_null);
auto               thing_display_get_tile_info_set(Tpp tp, thing_display_get_tile_info_t callback) -> void;
[[nodiscard]] auto thing_display_get_tile_info(Gamep g, Levelsp v, Levelp l, const bpoint &p, Tpp tp, Thingp t_maybe_null) -> Tilep;

using thing_assess_tile_t = ThingEnvironType (*)(Gamep, Levelsp, Levelp l, const bpoint &p, Thingp me);
auto               thing_assess_tile_set(Tpp tp, thing_assess_tile_t callback) -> void;
[[nodiscard]] auto thing_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &p, Thingp me) -> ThingEnvironType;

using thing_assess_tp_t = ThingEnvironType (*)(Gamep, Levelsp, Levelp l, Tpp tp, Thingp me);
auto               thing_assess_tp_set(Tpp tp, thing_assess_tp_t callback) -> void;
[[nodiscard]] auto thing_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType;

using thing_description_get_t = std::string (*)(Gamep, Levelsp, Levelp, Thingp me);
auto               thing_description_set(Tpp tp, thing_description_get_t callback) -> void;
[[nodiscard]] auto thing_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string;

using thing_z_depth_get_t = MapZDepthType (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_z_depth_set(Tpp tp, thing_z_depth_get_t callback) -> void;

using thing_detail_get_t = std::string (*)(Gamep, Levelsp, Levelp, Thingp me);
auto               thing_detail_set(Tpp tp, thing_detail_get_t callback) -> void;
[[nodiscard]] auto thing_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string;

using thing_mouse_down_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, int x, int y, int button);
auto               thing_mouse_down_set(Tpp tp, thing_mouse_down_t callback) -> void;
[[nodiscard]] auto thing_mouse_down(Gamep g, Levelsp v, Levelp l, Thingp me, int x, int y, int button) -> bool;

using thing_on_spawned_t = void (*)(Gamep, Levelsp, Levelp, Thingp me, ThingEvent *e);
auto thing_on_spawned_set(Tpp tp, thing_on_spawned_t callback) -> void;
auto thing_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent *e = nullptr) -> void;

using thing_on_levitated_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_levitated_set(Tpp tp, thing_on_levitated_t callback) -> void;
auto thing_on_levitated(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_invisible_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_invisible_set(Tpp tp, thing_on_invisible_t callback) -> void;
auto thing_on_invisible(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_level_enter_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_level_enter_set(Tpp tp, thing_on_level_enter_t callback) -> void;
auto thing_on_level_enter(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_level_leave_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_level_leave_set(Tpp tp, thing_on_level_leave_t callback) -> void;
auto thing_on_level_leave(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_level_populated_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_level_populated_set(Tpp tp, thing_on_level_populated_t callback) -> void;
auto thing_on_level_populated(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_engulf_request_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp engulfer);
auto               thing_on_engulf_request_set(Tpp tp, thing_on_engulf_request_t callback) -> void;
[[nodiscard]] auto thing_on_engulf_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp engulfer) -> bool;

using thing_on_carry_request_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp owner, ThingEvent &);
auto               thing_on_carry_request_set(Tpp tp, thing_on_carry_request_t callback) -> void;
[[nodiscard]] auto thing_on_carry_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp user, ThingEvent & /*e*/) -> bool;

using thing_on_drop_request_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp dropper, ThingEvent &);
auto               thing_on_drop_request_set(Tpp tp, thing_on_drop_request_t callback) -> void;
[[nodiscard]] auto thing_on_drop_request(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp user, ThingEvent & /*e*/) -> bool;

using thing_on_carry_success_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp owner, ThingEvent &);
auto               thing_on_carry_success_set(Tpp tp, thing_on_carry_success_t callback) -> void;
[[nodiscard]] auto thing_on_carry_success(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp user, ThingEvent & /*e*/) -> bool;

using thing_on_drop_success_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp dropper, ThingEvent &);
auto               thing_on_drop_success_set(Tpp tp, thing_on_drop_success_t callback) -> void;
[[nodiscard]] auto thing_on_drop_success(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp user, ThingEvent & /*e*/) -> bool;

using thing_on_use_weapon_request_t = Tpp (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp user);
auto               thing_on_use_weapon_request_set(Tpp tp, thing_on_use_weapon_request_t callback) -> void;
[[nodiscard]] auto thing_on_use_weapon_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp user) -> Tpp;

using thing_on_use_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp user);
auto               thing_on_use_set(Tpp tp, thing_on_use_t callback) -> void;
[[nodiscard]] auto thing_on_use(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp user) -> bool;

using thing_on_eaten_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp eatenr);
auto               thing_on_eaten_set(Tpp tp, thing_on_eaten_t callback) -> void;
[[nodiscard]] auto thing_on_eaten(Gamep g, Levelsp v, Levelp l, Thingp item, Thingp eater) -> bool;

using thing_on_worn_request_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp owner);
auto               thing_on_worn_request_set(Tpp tp, thing_on_worn_request_t callback) -> void;
[[nodiscard]] auto thing_on_worn_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp owner) -> bool;

using thing_on_strip_request_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp owner);
auto               thing_on_strip_request_set(Tpp tp, thing_on_strip_request_t callback) -> void;
[[nodiscard]] auto thing_on_strip_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp owner) -> bool;

using thing_on_open_request_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp opener);
auto               thing_on_open_request_set(Tpp tp, thing_on_open_request_t callback) -> void;
[[nodiscard]] auto thing_on_open_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp opener) -> bool;

using thing_on_close_request_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp closer);
auto               thing_on_close_request_set(Tpp tp, thing_on_close_request_t callback) -> void;
[[nodiscard]] auto thing_on_close_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp closer) -> bool;

using thing_on_tick_idle_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_tick_idle_set(Tpp tp, thing_on_tick_idle_t callback) -> void;
auto thing_on_tick_idle(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_tick_begin_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_tick_begin_set(Tpp tp, thing_on_tick_begin_t callback) -> void;
auto thing_on_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_tick_end_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_tick_end_set(Tpp tp, thing_on_tick_end_t callback) -> void;
auto thing_on_tick_end(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_death_t = void (*)(Gamep, Levelsp, Levelp, Thingp me, ThingEvent &);
auto thing_on_death_set(Tpp tp, thing_on_death_t callback) -> void;
auto thing_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e) -> void;

using thing_on_damage_t = bool (*)(Gamep, Levelsp, Levelp, Thingp me, ThingEvent &);
auto               thing_on_damage_set(Tpp tp, thing_on_damage_t callback) -> void;
[[nodiscard]] auto thing_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e) -> bool;

using thing_on_attacking_t = bool (*)(Gamep, Levelsp, Levelp, Thingp attacker, Thingp target, ThingEvent &);
auto               thing_on_attacking_set(Tpp tp, thing_on_attacking_t callback) -> void;
[[nodiscard]] auto thing_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e) -> bool;

using thing_on_killing_t = bool (*)(Gamep, Levelsp, Levelp, Thingp attacker, Thingp target, ThingEvent &);
auto               thing_on_killing_set(Tpp tp, thing_on_killing_t callback) -> void;
[[nodiscard]] auto thing_on_killing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e) -> bool;

using thing_on_missing_t = bool (*)(Gamep, Levelsp, Levelp, Thingp attacker, Thingp target, ThingEvent &);
auto               thing_on_missing_set(Tpp tp, thing_on_missing_t callback) -> void;
[[nodiscard]] auto thing_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e) -> bool;

using thing_on_moved_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_moved_set(Tpp tp, thing_on_moved_t callback) -> void;
auto thing_on_moved(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_moving_to_t = void (*)(Gamep, Levelsp, Levelp, Thingp me, const bpoint &to);
auto thing_on_moving_to_set(Tpp tp, thing_on_moving_to_t callback) -> void;
auto thing_on_moving_to(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &to) -> void;

using thing_on_activated_t = bool (*)(Gamep, Levelsp, Levelp, Thingp trap, Thingp sucker);
auto thing_on_activated_set(Tpp tp, thing_on_activated_t callback) -> void;
auto thing_on_activated(Gamep g, Levelsp v, Levelp l, Thingp trap, Thingp user) -> bool;

using thing_on_teleported_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_teleported_set(Tpp tp, thing_on_teleported_t callback) -> void;
auto thing_on_teleported(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_shoved_t = void (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp shover);
auto thing_on_shoved_set(Tpp tp, thing_on_shoved_t callback) -> void;
auto thing_on_shoved(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp shover) -> void;

using thing_on_jump_begin_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_jump_begin_set(Tpp tp, thing_on_jump_begin_t callback) -> void;
auto thing_on_jump_begin(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_jump_end_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_jump_end_set(Tpp tp, thing_on_jump_end_t callback) -> void;
auto thing_on_jump_end(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_thrown_begin_t = void (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp thrower);
auto thing_on_thrown_begin_set(Tpp tp, thing_on_thrown_begin_t callback) -> void;
auto thing_on_thrown_begin(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp thrower) -> void;

using thing_on_thrown_end_t = void (*)(Gamep, Levelsp, Levelp, Thingp me, Thingp thrower);
auto thing_on_thrown_end_set(Tpp tp, thing_on_thrown_end_t callback) -> void;
auto thing_on_thrown_end(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp thrower) -> void;

using thing_on_fall_begin_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_fall_begin_set(Tpp tp, thing_on_fall_begin_t callback) -> void;
auto thing_on_fall_begin(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_melt_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_melt_set(Tpp tp, thing_on_melt_t callback) -> void;
auto thing_on_melt(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

using thing_on_fall_end_t = void (*)(Gamep, Levelsp, Levelp, Thingp me);
auto thing_on_fall_end_set(Tpp tp, thing_on_fall_end_t callback) -> void;
auto thing_on_fall_end(Gamep g, Levelsp v, Levelp l, Thingp me) -> void;

#endif
