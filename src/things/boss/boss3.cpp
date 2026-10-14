//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_cpp_template.hpp"
#include "../../my_dice_rolls.hpp"
#include "../../my_level_inlines.hpp"
#include "../../my_random.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"
#include "my_boss_common.hpp"

static const std::initializer_list< bpoint > additional_body_tiles = {
    bpoint(0, -2), bpoint(1, -2),                                           //
    bpoint(0, -1), bpoint(1, -1),                                           //
    bpoint(0, 0),  bpoint(1, 0),                                            //
    bpoint(-2, 1), bpoint(-1, 1), bpoint(0, 1), bpoint(1, 1),               //
    bpoint(-2, 1), bpoint(-1, 1), bpoint(0, 1), bpoint(1, 1), bpoint(2, 1), //
    bpoint(-2, 2), bpoint(-1, 2), bpoint(0, 2), bpoint(1, 2), bpoint(2, 2), //
};

//
// Approximate where the stinger is
//
const std::initializer_list< fpoint > stinger_list = {
    fpoint(-1, -1),
    fpoint(-2, -1),
};

static auto tp_boss3_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_dead(me)) {
    return "dead scorpios";
  }
  return "scorpios";
}

static auto tp_boss3_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return //
      UI_INFO1_FMT_STR
      "Scorpios, the God. This is no mere giant scorpion and is more ethereal than natural in form.\n" //
      UI_INFO2_FMT_STR
      "Beware it's deadly stinger and be ready for surprises. This Scorpion will not sit and wait to be hit..\n"; //                                                               //
}

static auto tp_boss3_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  if (tp_is_chasm(tp)) {
    return THING_ENVIRON_HATES;
  }

  return THING_ENVIRON_NEUTRAL;
}

static auto tp_boss3_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  if (level_is_chasm(g, v, l, at)) {
    return THING_ENVIRON_HATES;
  }

  return THING_ENVIRON_NEUTRAL;
}

static void tp_boss3_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_blood), me);

  thing_sound_play(g, v, l, me, "monster_roar_epic");

  tp_boss_on_death(g, v, l, me, e);
}

static void tp_boss3_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto target = thing_monst_target(g, v, l, me);
  if (level_is_player(g, v, l, target)) {
    (void) thing_attack_at(g, v, l, me, target);
  }
}

static bool tp_boss3_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  THING_DBG(g, v, l, attacker, "boss attack");
  TRACE_INDENT();

  TpSpecialAttack d;

  std::vector< fpoint > stingers(stinger_list);

  if (thing_special_attack_get_random(g, v, l, attacker, target, d)) {
    THING_DBG(g, v, l, attacker, "boss special attack");

    e.special_attack = d;
    e.event_type     = d.event_type;
    e.damage         = d.dice.roll();

    if (d.what != "") {
      auto target_at = thing_at(g, v, l, target);
      auto fire_what = tp_find_mand(d.what);

      //
      // Keep track of which stalks fired
      //
      std::map< int, bool > stinger_fired_already = {};

      for (auto beam = 0; beam < 2; beam++) {
        //
        // Get a stinger
        //
        auto which_stinger  = PCG_RANDOM_RANGE(0, stingers.size());
        auto stinger_offset = stingers[ which_stinger ];

        //
        // Check it has not fired already
        //
        int tries = 0;
        for (;;) {

          if (stinger_fired_already.find(which_stinger) == stinger_fired_already.end()) {
            //
            // This is the first time this one has fired this loop
            //
            stinger_fired_already[ which_stinger ] = true;
            break;
          }

          //
          // Try another
          //
          which_stinger = PCG_RANDOM_RANGE(0, stingers.size());

          if (tries++ > 10000) {
            CROAK("failed stinger fire");
          }
        }

        //
        // Swap the position if the boss is facing a different direction
        //
        if (thing_is_dir_left(attacker) || thing_is_dir_tl(attacker) || thing_is_dir_bl(attacker)) {
          std::swap(stinger_offset.x, stinger_offset.x);
        }

        THING_DBG(g, v, l, attacker, "stinger #%d fire", beam);
        TRACE_INDENT();

        (void) thing_proj_launch_at(g, v, l, attacker, fire_what, target_at, stinger_offset);
      }

      thing_sound_play(g, v, l, attacker, "monster_roar");

      THING_DBG(g, v, l, attacker, "prevent melee attack as fired weapon");
      return false; // prevent melee attack
    }
  }

  if (! adjacent(thing_at(g, v, l, attacker), thing_at(g, v, l, target))) {
    THING_DBG(g, v, l, attacker, "prevent melee attack as not adjacent");
    thing_sound_play(g, v, l, attacker, "monster_roar");
    return false; // prevent melee attack
  }

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);
  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

static void tp_boss3_on_pushed(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto at = thing_at(g, v, l, me);

  for (auto p : additional_body_tiles) {
    (void) thing_push_additional(g, v, l, me, at + p);
  }
}

static void tp_boss3_on_popped(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto at = thing_at(g, v, l, me);

  for (auto p : additional_body_tiles) {
    (void) thing_pop_additional(g, v, l, me, at + p);
  }
}

static bool tp_boss3_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE_INDENT();

  if (adjacent(thing_at(g, v, l, attacker), thing_at(g, v, l, target))) {
    (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);
    thing_sound_play(g, v, l, attacker, "hiss");
  }

  return true;
}

bool tp_boss3_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  // auto attacker = thing_get_attacker(g, v, l, e.source);

  return true;
}

[[nodiscard]] auto tp_load_boss3() -> bool
{
  auto *tp   = tp_load("boss3"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_boss3_assess_tile);
  thing_assess_tp_set(tp, tp_boss3_assess_tp);
  thing_description_set(tp, tp_boss3_description_get);
  thing_detail_set(tp, tp_boss3_detail_get);
  thing_on_attacking_set(tp, tp_boss3_on_attacking);
  thing_on_damage_set(tp, tp_boss3_on_damage);
  thing_on_death_set(tp, tp_boss3_on_death);
  thing_on_missing_set(tp, tp_boss3_on_missing);
  tp_missile_count_max_set(tp, THING_MISSILE_MAX);
  thing_on_popped_set(tp, tp_boss3_on_popped);
  thing_on_pushed_set(tp, tp_boss3_on_pushed);
  thing_on_tick_begin_set(tp, tp_boss3_tick_begin);
  tp_additional_body_tiles_set(tp, additional_body_tiles);
  tp_attack_count_max_per_tick_set(tp, 1);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d6"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d2");    // fumble => flames spread to you
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "1d4");
  tp_distance_vision_set(tp, MAP_WIDTH / 2);
  tp_distance_jump_set(tp, 5);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_breathe);
  tp_flag_set(tp, is_able_to_jump);
  tp_flag_set(tp, is_able_to_choose_targets);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_able_to_fire_weapons);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_blasted_when_dead);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_solid_white);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_boss);
  tp_flag_set(tp, is_boss3);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_described_when_killed);
  tp_flag_set(tp, is_flesh);
  tp_flag_set(tp, is_insectoid);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_memory_ext);
  tp_flag_set(tp, is_monst);
  tp_flag_set(tp, is_multi_tile);
  tp_flag_set(tp, is_obs_to_beam);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_obs_to_wall_walker);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_shown_health);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_vision_360_degrees);
  tp_health_set(tp, "100+12d8");
  tp_hearing_threshold_set(tp, 2); // smaller values => better hearing
  tp_name_a_or_an_set(tp, "scorpios, the god");
  tp_name_apostrophize_set(tp, "scorpios, the god's");
  tp_name_long_set(tp, "scorpios, the god");
  tp_name_pluralize_set(tp, "scorpios, the god");
  tp_name_short_set(tp, "scorpios, god");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_rarity_set(tp, THING_RARITY_COMMON);
  tp_score_value_set(tp, 3000);
  tp_speed_set(tp, 25);
  tp_stat_set(tp, THING_STAT_ATT, "1d4+14");
  tp_stat_set(tp, THING_STAT_DEF, "1d4+14");
  tp_stat_set(tp, THING_STAT_DMG, "1d4+14");
  tp_temperature_damage_at_set(tp, 200); // celsius
  tp_temperature_initial_set(tp, 20);    // celsius
  tp_temperature_melts_at_set(tp, 250);  // celsius
  tp_weight_set(tp, WEIGHT_VVHEAVY);     // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_MONST);
  // end sort marker1 }

  tp_special_attack_add(tp,
                        TpSpecialAttack {
                            .type          = "1",                      //
                            .event_type    = THING_EVENT_CRUSH_DAMAGE, //
                            .name          = "crush attack",           //
                            .roll          = "8d8",                    //
                            .d100          = 100,
                            .when_adjacent = true,
                        });

  tp_special_attack_add(tp,
                        TpSpecialAttack {
                            .type         = "2",             //
                            .name         = "stinger blast", //
                            .what         = "proj_fire",     //
                            .d100         = 100,
                            .when_distant = true,
                        });

  auto delay = 1000;

  for (auto frame = 0; frame < 4; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, BOSS3_TILE_WIDTH, BOSS3_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  for (auto frame = 0; frame < 4; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_size_set(tile, BOSS3_TILE_WIDTH, BOSS3_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);

    //
    // We want the boss to vanish and not all into a chasm
    //
    if (frame == 3) {
      tile_is_cleanup_on_end_of_anim_set(tile);
    }
  }

  return true;
}
