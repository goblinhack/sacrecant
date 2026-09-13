//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"

static auto tp_staff_fire_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return "staff_fire";
}

static auto tp_staff_fire_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                          //
      UI_INFO1_FMT_STR "A fiery red staff. Burning sparks drip from it.\n"                                        //
      UI_INFO2_FMT_STR "Tip: staves fire a single blast at a time, unavoidable by monsters.\n"                    //
      UI_INFO3_FMT_STR "Tip: blast can pass through multiple monsters, so aim to create a conga line of death.\n" //
      UI_INFO4_FMT_STR "Info: staves are more powerful than wands, but have fewer charges and are worn with a move penalty.\n";
}

[[nodiscard]] static auto tp_staff_fire_on_use_weapon_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp user) -> Tpp
{
  TRACE();

  Tpp             what {};
  TpSpecialAttack d;

  if (thing_special_attack_get_random(g, v, l, me, nullptr, d)) {
    if (! d.what.empty()) {
      what = tp_find_mand(d.what);
    }
  }

  if (user && thing_is_player(user)) {
    (void) thing_noise_incr(g, v, l, user, THING_NOISE_STAFF);
  }

  return what;
}

[[nodiscard]] auto tp_load_staff_fire() -> bool
{
  TRACE();

  auto *tp   = tp_load("staff_fire"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_description_set(tp, tp_staff_fire_description_get);
  thing_detail_set(tp, tp_staff_fire_detail_get);
  thing_on_use_weapon_request_set(tp, tp_staff_fire_on_use_weapon_request);
  tp_charge_count_set(tp, 5000);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_auto_wear);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_black_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_collectable);
  tp_flag_set(tp, is_collision_circle_small);
  tp_flag_set(tp, is_combustible); // will continue to burn once on fire
  tp_flag_set(tp, is_dead_when_discharged);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_droppable);
  tp_flag_set(tp, is_inventory_item);
  tp_flag_set(tp, is_item);
  tp_flag_set(tp, is_light_flicker);
  tp_flag_set(tp, is_light_source, 2);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_physics_trap);
  tp_flag_set(tp, is_physics_water);
  tp_flag_set(tp, is_staff);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_throwable);
  tp_flag_set(tp, is_tick_on_drop);
  tp_flag_set(tp, is_tick_on_strip);
  tp_flag_set(tp, is_tick_on_worn);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_treasure);
  tp_flag_set(tp, is_weapon);
  tp_flag_set(tp, is_wood);
  tp_health_set(tp, "1d4");
  tp_is_immune_to_add(tp, THING_EVENT_FIRE_DAMAGE);
  tp_light_color_set(tp, "red");
  tp_name_a_or_an_set(tp, "a staff of fire");
  tp_name_apostrophize_set(tp, "staff of fire's");
  tp_name_long_set(tp, "staff of fire");
  tp_name_pluralize_set(tp, "staves of fire");
  tp_name_short_set(tp, "staff of fire");
  tp_priority_set(tp, THING_PRIORITY_OBJECT);
  tp_rarity_set(tp, THING_RARITY_UNCOMMON);
  tp_temperature_burns_at_set(tp, 30);  // celsius
  tp_temperature_damage_at_set(tp, 30); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_weight_set(tp, WEIGHT_MEDIUM);     // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_OBJ);
  // end sort marker1 }

  tp_special_attack_add(tp,
                        TpSpecialAttack {
                            .type = "1",            //
                            .name = "beam weapon",  //
                            .what = "beam_of_fire", //
                        });
  auto delay = 20;

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + std::string(".") + std::to_string(frame));
    tile_size_set(tile, TILE_WIDTH, TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  return true;
}
