//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_cpp_template.hpp"
#include "../../my_dice_rolls.hpp"
#include "../../my_level_inlines.hpp"
#include "../../my_math.hpp"
#include "../../my_random.hpp"
#include "../../my_sound.hpp"
#include "../../my_thing.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tile.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_types.hpp"
#include "../../my_ui.hpp"
#include "my_boss_common.hpp"

static const std::initializer_list< bpoint > multi_tile_tiles = {
    bpoint(-1, -2), bpoint(0, -2), bpoint(1, -2), //
    bpoint(-1, -1), bpoint(0, -1), bpoint(1, -1), //
    bpoint(-1, 0),  bpoint(1, 0),                 //
    bpoint(-1, 1),  bpoint(0, 1),  bpoint(1, 1),  //
    bpoint(-1, 2),  bpoint(0, 2),  bpoint(1, 2),  //
};

//
// Approximate where the source is
//
const std::initializer_list< fpoint > source_list = {
    fpoint(0, 0),
};

static auto tp_boss5_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_dead(me)) {
    return "dead lord batcat";
  }
  return "lord batcat";
}

static auto tp_boss5_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return //
      UI_INFO1_FMT_STR
      "Lord Batcat III. Tremble all ye at the might and majesty before ye.\n" //
      UI_INFO2_FMT_STR
      "Are you up to this final challenge and to reclaim the throne and title of the darkest dark lord in these here parts?\n" //                                                               //
      UI_INFO3_FMT_STR "Probably not...\n" //                                                               //
      UI_INFO4_FMT_STR
      "Expect Lord Batcat to summon help and use every trick in his dark book in order to succeed in vanquishing you, you peasant!\n"; //
}

static void tp_boss5_on_death(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  (void) thing_spawn(g, v, l, tp_first(is_effect_blood), me);

  thing_sound_play(g, v, l, me, "monster_roar_epic");

  tp_boss_on_death(g, v, l, me, e);
}

static void tp_boss5_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto at = thing_at(g, v, l, me);

  if (! me->tick) {
    (void) sound_play(g, v, l, "monster_roar_epic");
  }

  //
  // Spawn a spider!
  //
  if (l->info.monst_count < 10) {
    (void) thing_spawn(g, v, l, tp_first(is_skullferno), me);

    if (! level_is_gas_bool(g, v, l, at)) {
      (void) thing_spawn(g, v, l, tp_first(is_gas_death), at);
    }
  }

  if (d100() < 2) {
    topcon("Lord Batcat creates a cloud of poison gas!");

    const std::initializer_list< bpoint > points = {
        bpoint(-1, -1), bpoint(1, -1), bpoint(0, -1), bpoint(-1, 0), bpoint(1, 0), bpoint(0, 0), bpoint(-1, 1), bpoint(1, 1), bpoint(0, 1),
    };

    for (auto delta : points) {
      auto p = at + delta;
      if (! is_oob(p)) {
        if (level_is_obs_to_gas(g, v, l, p) == nullptr) {
          if (! level_is_gas_bool(g, v, l, p)) {
            (void) thing_spawn(g, v, l, tp_first(is_gas_death), p);
          }
        }
      }
    }
  }
}

static bool tp_boss5_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  THING_DBG(g, v, l, attacker, "boss attack");
  TRACE_INDENT();

  TpSpecialAttack d;

  std::vector< fpoint > sources(source_list);

  if (thing_special_attack_get_random(g, v, l, attacker, target, d)) {
    THING_DBG(g, v, l, attacker, "boss special attack");
    THING_DBG(g, v, l, target, "boss special attack target");

    e.special_attack = d;
    e.event_type     = d.event_type;
    e.damage         = d.dice.roll();

    if (d.what != "") {
      //
      // Do not always fire at things we cannot see. We can get hear from hearing.
      //
      auto target_at = thing_at(g, v, l, target);
      if (! thing_vision_can_see_tile(g, v, l, attacker, target_at)) {
        if (d100() < 50) {
          return false; // prevent melee attack
        }
      }

      fpoint target_real_at   = thing_real_at(g, v, l, target);
      fpoint attacker_real_at = thing_real_at(g, v, l, attacker);
      auto   fire_what        = tp_find_mand(d.what);

      //
      // Keep track of which stalks fired
      //
      std::map< int, bool > source_fired_already = {};

      for (auto beam = 0; beam < 1; beam++) {
        //
        // Get a source
        //
        auto which_source  = PCG_RANDOM_RANGE(0, sources.size());
        auto source_offset = sources[ which_source ];

        //
        // Check it has not fired already
        //
        int tries = 0;
        for (;;) {

          if (source_fired_already.find(which_source) == source_fired_already.end()) {
            //
            // This is the first time this one has fired this loop
            //
            source_fired_already[ which_source ] = true;
            break;
          }

          //
          // Try another
          //
          which_source = PCG_RANDOM_RANGE(0, sources.size());

          if (tries++ > 10000) {
            warn("failed source fire");
            break;
          }
        }

        //
        // Swap the position if the boss is facing a different direction
        //
        if (thing_is_dir_left(attacker) || thing_is_dir_tl(attacker) || thing_is_dir_bl(attacker)) {
          std::swap(source_offset.x, source_offset.x);
        }

        THING_DBG(g, v, l, attacker, "source #%d fire", beam);
        TRACE_INDENT();

        //
        // Fire a spread of fireballs
        //
        fpoint target_to_origin        = target_real_at - attacker_real_at;
        auto   current_angle_to_target = angle_radians(target_to_origin);
        auto   distance_to_target      = distance(target_real_at, attacker_real_at);
        auto   spread_count            = 0; // how many fireballs either side of the target
        float  angle_step              = (float) RAD_360 / 32;

        //
        // A first fireball at the target directly
        //
        {
          fpoint p(attacker_real_at.x + distance_to_target * cosf(current_angle_to_target), //
                   attacker_real_at.y + distance_to_target * sinf(current_angle_to_target));
          (void) thing_proj_launch_at(g, v, l, attacker, fire_what, p, source_offset);
        }

        for (float angle_delta = angle_step; angle_delta < (float) RAD_360 / 2; angle_delta += angle_step) {
          if (spread_count++ > 5) {
            break;
          }

          {
            fpoint p(attacker_real_at.x + distance_to_target * cosf(current_angle_to_target + angle_delta), //
                     attacker_real_at.y + distance_to_target * sinf(current_angle_to_target + angle_delta));
            (void) thing_proj_launch_at(g, v, l, attacker, fire_what, p, source_offset);
          }
          {
            fpoint p(attacker_real_at.x + distance_to_target * cosf(current_angle_to_target - angle_delta), //
                     attacker_real_at.y + distance_to_target * sinf(current_angle_to_target - angle_delta));
            (void) thing_proj_launch_at(g, v, l, attacker, fire_what, p, source_offset);
          }
        }
      }

      thing_set_dir_from_target(g, v, l, attacker, make_bpoint(target_real_at));
      thing_sound_play(g, v, l, attacker, "hiss");

      THING_DBG(g, v, l, attacker, "prevent melee attack as fired weapon");
      return false; // prevent melee attack
    }
  }

  if (! adjacent(thing_at(g, v, l, attacker), thing_at(g, v, l, target))) {
    THING_DBG(g, v, l, attacker, "prevent melee attack as not adjacent");
    thing_sound_play(g, v, l, attacker, "hiss");
    return false; // prevent melee attack
  }

  (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);
  thing_sound_play(g, v, l, attacker, "hiss");

  return true;
}

static void tp_boss5_on_pushed(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto at = thing_at(g, v, l, me);

  for (auto p : multi_tile_tiles) {
    (void) thing_push_additional(g, v, l, me, at + p);
  }
}

static void tp_boss5_on_popped(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto at = thing_at(g, v, l, me);

  for (auto p : multi_tile_tiles) {
    (void) thing_pop_additional(g, v, l, me, at + p);
  }
}

static bool tp_boss5_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE_INDENT();

  if (adjacent(thing_at(g, v, l, attacker), thing_at(g, v, l, target))) {
    thing_sound_play(g, v, l, attacker, "hiss");
  }

  return true;
}

bool tp_boss5_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  return true;
}

static auto tp_boss5_assess_tp(Gamep g, Levelsp v, Levelp l, Tpp tp, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  return THING_ENVIRON_NEUTRAL;
}

static auto tp_boss5_assess_tile(Gamep g, Levelsp v, Levelp l, const bpoint &at, Thingp me) -> ThingEnvironType
{
  TRACE_DEBUG();

  return THING_ENVIRON_NEUTRAL;
}

[[nodiscard]] auto tp_load_boss5() -> bool
{
  auto *tp   = tp_load("boss5"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_assess_tile_set(tp, tp_boss5_assess_tile);
  thing_assess_tp_set(tp, tp_boss5_assess_tp);
  thing_description_set(tp, tp_boss5_description_get);
  thing_detail_set(tp, tp_boss5_detail_get);
  thing_on_attacking_set(tp, tp_boss5_on_attacking);
  thing_on_damage_set(tp, tp_boss5_on_damage);
  thing_on_death_set(tp, tp_boss5_on_death);
  thing_on_missing_set(tp, tp_boss5_on_missing);
  thing_on_popped_set(tp, tp_boss5_on_popped);
  thing_on_pushed_set(tp, tp_boss5_on_pushed);
  thing_on_tick_begin_set(tp, tp_boss5_tick_begin);
  tp_attack_count_max_per_tick_set(tp, 1);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d6"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d2");    // fumble => flames spread to you
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "1d4");
  tp_distance_vision_set(tp, MAP_WIDTH / 2);
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_fire_weapons);
  tp_flag_set(tp, is_able_to_move_diagonally);
  tp_flag_set(tp, is_able_to_move);
  tp_flag_set(tp, is_able_to_teleport_safely);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_player);
  tp_flag_set(tp, is_blasted_when_dead);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_solid_white);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_blit_shown_in_overlay);
  tp_flag_set(tp, is_boss);
  tp_flag_set(tp, is_boss5);
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_described_when_killed);
  tp_flag_set(tp, is_levitating);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_memory_ext);
  tp_flag_set(tp, is_monst_overlap_allowed);
  tp_flag_set(tp, is_monst);
  tp_flag_set(tp, is_multi_tile);
  tp_flag_set(tp, is_obs_to_beam);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_obs_to_teleporting_onto);
  tp_flag_set(tp, is_obs_to_wall_walker);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_removable_when_dead_on_err);
  tp_flag_set(tp, is_shown_health);
  tp_flag_set(tp, is_tickable);
  tp_flag_set(tp, is_vision_360_degrees);
  tp_health_set(tp, "100+12d8");
  tp_hearing_threshold_set(tp, 2); // smaller values => better hearing
  tp_is_immune_to_add(tp, THING_EVENT_FIRE_DAMAGE);
  tp_missile_count_max_set(tp, THING_MISSILE_MAX);
  tp_multi_tile_tiles_set(tp, multi_tile_tiles);
  tp_name_a_or_an_set(tp, "lord batcat");
  tp_name_apostrophize_set(tp, "lord batcat's");
  tp_name_long_set(tp, "lord batcat");
  tp_name_pluralize_set(tp, "lord batcat");
  tp_name_short_set(tp, "lord batcat");
  tp_priority_set(tp, THING_PRIORITY_MONST);
  tp_rarity_set(tp, THING_RARITY_COMMON);
  tp_score_value_set(tp, 10000);
  tp_speed_set(tp, 50);
  tp_stat_set(tp, THING_STAT_ATT, "1d4+14");
  tp_stat_set(tp, THING_STAT_DEF, "1d4+14");
  tp_stat_set(tp, THING_STAT_DMG, "1d4+14");
  tp_temperature_damage_at_set(tp, 200); // celsius
  tp_temperature_initial_set(tp, 20);    // celsius
  tp_temperature_melts_at_set(tp, 250);  // celsius
  tp_weight_set(tp, WEIGHT_VVHEAVY);     // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_FLOATING_MONST);
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
                            .type         = "2",            //
                            .name         = "source blast", //
                            .what         = "proj_fire",    //
                            .d100         = 100,
                            .when_distant = true,
                        });

  auto delay = 1000;

  for (auto frame = 0; frame < 4; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, BOSS5_TILE_WIDTH, BOSS5_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  for (auto frame = 0; frame < 4; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_size_set(tile, BOSS5_TILE_WIDTH, BOSS5_TILE_HEIGHT);
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
