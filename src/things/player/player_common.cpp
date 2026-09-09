//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_game_popups.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_random.hpp"
#include "my_sound.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

auto tp_player_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  if (thing_is_dead(me)) {
    return "dead you";
  }
  return "You";
}

auto tp_player_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR "You: a luckless wizard hoping to reclaim forgotten skills.";
}

void tp_player_on_moved(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  THING_DBG(g, v, l, me, "player moved");
  TRACE_INDENT();

  //
  // If we fell into another chasm, don't make an oof sound
  //
  if (level_is_chasm_bool(g, v, l, thing_at(g, v, l, me))) {
    return;
  }

  if (thing_is_submergible(g, v, l, me) && level_is_water_deep_bool(g, v, l, thing_at(g, v, l, me))) {
    //
    // Allow some items to drift away
    //
    THING_DBG(g, v, l, me, "in deep water, check to see if things float away");
    TRACE_INDENT();

    thing_inventory_dump(g, v, l, me);

    FOR_ALL_INVENTORY_ITEMS(g, v, l, me, item)
    {
      if (thing_is_worn(item)) {
        continue;
      }

      if (g_opt_tests) {
        //
        // Always drop
        //
      } else if (d100() > 1) {
        //
        // No drop
        //
        continue;
      } else {
        //
        // Random drop
        //
      }

      ThingEvent e {
          .reason     = "drifted away",     //
          .event_type = THING_EVENT_THROWN, //
          .source     = me,                 //
      };

      THING_DBG(g, v, l, item, "item drifted away, drop");

      if (thing_drop(g, v, l, me, item, e)) {
        THING_DBG(g, v, l, item, "item drifted away, completed drop");
        TRACE_INDENT();

        topcon(UI_WARN_FMT_STR "You feel lighter..." UI_RESET_FMT);
        break;
      }
    }
  } else if (thing_is_submergible(g, v, l, me) && level_is_water_shallow_bool(g, v, l, thing_at(g, v, l, me))) {
    //
    // Ripple where we used to be
    //
    if (d100() < 50) {
      if (thing_noise_incr(g, v, l, me, THING_NOISE_SPLASH)) {
        auto at = thing_old_at(me);

        if (! level_is_water_shallow_bool(g, v, l, at)) {
          at = thing_at(g, v, l, me);
        }

        if (d100() < 50) {
          game_popup_text_add(g, at.x, at.y, std::string("Splosh!"));
        } else {
          game_popup_text_add(g, at.x, at.y, std::string("Splash!"));
        }

        (void) thing_spawn(g, v, l, tp_first(is_effect_ripple), at);
        thing_sound_play(g, v, l, me, "splash");
      }
    }
  } else if (level_is_foliage_bool(g, v, l, thing_at(g, v, l, me))) {
    //
    // Allow bushes to rustle if levitating, but not ethereal
    //
    if (! thing_is_ethereal(g, v, l, me)) {
      thing_sound_play(g, v, l, me, "footstep_foliage");
      (void) thing_noise_incr(g, v, l, me, THING_NOISE_FOLIAGE);
    }
  } else if (level_is_grass_bool(g, v, l, thing_at(g, v, l, me))) {
    if (! thing_is_ethereal(g, v, l, me) && ! thing_is_levitating(g, v, l, me)) {
      (void) thing_noise_incr(g, v, l, me, THING_NOISE_GRASS);
    }
  } else {
    //
    // No footsteps if levitating
    //
    if (! thing_is_ethereal(g, v, l, me) && ! thing_is_levitating(g, v, l, me)) {
      thing_sound_play(g, v, l, me, "footstep");

      //
      // Noisy bridge?
      //
      if (level_is_bridge_bool(g, v, l, thing_at(g, v, l, me))) {
        (void) thing_noise_incr(g, v, l, me, THING_NOISE_FOOTSTEP_BRIDGE);
      } else {
        (void) thing_noise_incr(g, v, l, me, THING_NOISE_FOOTSTEP);
      }
    }
  }

  auto at                   = thing_at(g, v, l, me);
  v->cursor[ at.x ][ at.y ] = CURSOR_NONE;
}

void tp_player_on_moving_to(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &to)
{
  THING_DBG(g, v, l, me, "player moving");
  TRACE_INDENT();

  if (! l->player_has_walked_tile[ to.x ][ to.y ]) {
    (void) thing_score_incr(g, v, l, me, 1);
  }
}

bool tp_player_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  //
  // If hit, clear the cursor path and stop moving.
  //
  player_state_change(g, v, l, PLAYER_STATE_NORMAL);

  (void) thing_spawn(g, v, l, tp_first(is_effect_blood), me);

  thing_sound_play(g, v, l, me, "player_hit");

  (void) thing_noise_incr(g, v, l, me, THING_NOISE_PLAYER_HIT);

  return true; // allow the damage to be applied
}

void tp_player_on_jump_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (thing_is_falling(me) != 0) {
    return;
  }

  if (level_is_water_shallow_bool(g, v, l, thing_at(g, v, l, me))) {
    //
    // We already have a splash noise
    //
    if (thing_noise_incr(g, v, l, me, THING_NOISE_SPLASH)) {
      auto at = thing_at(g, v, l, me);
      game_popup_text_add(g, at.x, at.y, std::string("Splash!"));
      (void) thing_spawn(g, v, l, tp_first(is_effect_ripple), me);
    }
  } else {
    thing_sound_play(g, v, l, me, "player_oof");
    (void) thing_noise_incr(g, v, l, me, THING_NOISE_PLAYER_JUMP);
    auto at = thing_at(g, v, l, me);
    game_popup_text_add(g, at.x, at.y, std::string("Oof!"));
  }
}

void tp_player_on_fall_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto at = thing_at(g, v, l, me);
  game_popup_text_add(g, at.x, at.y, std::string("Aargh!"));
}

void tp_player_on_fall_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  //
  // If we fell into another chasm, don't make an oof sound
  //
  if (level_is_chasm_bool(g, v, l, thing_at(g, v, l, me))) {
    return;
  }

  if (level_is_water_shallow_bool(g, v, l, thing_at(g, v, l, me))) {
    if (thing_noise_incr(g, v, l, me, THING_NOISE_SPLASH)) {
      auto at = thing_at(g, v, l, me);
      game_popup_text_add(g, at.x, at.y, std::string("Splash!"));
      (void) thing_spawn(g, v, l, tp_first(is_effect_ripple), me);
      thing_sound_play(g, v, l, me, "splash");
    }
  } else {
    thing_sound_play(g, v, l, me, "player_oof");
    (void) thing_noise_incr(g, v, l, me, THING_NOISE_PLAYER_FALL);
  }
}

void tp_player_level_leave(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  THING_DBG(g, v, l, me, "level leave");

  thing_vision_reset(g, v, l, me);

  //
  // If in level select we might choose another level than level 1, so only fade out
  // when leaving a level that we spent time in.
  //
  if (v->tick) {
    sound_fade_out(g);
  }
}

void tp_player_level_enter(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  THING_DBG(g, v, l, me, "level enter");

  thing_vision_reset(g, v, l, me);

  //
  // Ambience loops
  //
  auto loops = 1000;
  auto scale = 1;

  //
  // Needed to make sure the light resets as it looks for pixel changes.
  //
  thing_prev_pix_at_set(g, v, l, me, spoint(-1, -1));

  if (g_opt_tests) {
    return;
  }

  switch (level_to_biome(g, v, l)) {
    case BIOME_DUNGEON :    (void) sound_play(g, v, l, "dungeon_ambience", scale, loops); break;
    case BIOME_BOGLAND :    (void) sound_play(g, v, l, "bogland_ambience", scale, loops); break;
    case BIOME_NETHERVOID : (void) sound_play(g, v, l, "nethervoid_ambience", scale, loops); break;
    case BIOME_GRAVEYARD :  (void) sound_play(g, v, l, "graveyard_ambience", scale, loops); break;
    case BIOME_UNDERHELL :  (void) sound_play(g, v, l, "underhell_ambience", scale, loops); break;
    default :               ERR("unhandled biome"); break;
  }

  if (level_is_boss_level(g, v, l)) {
    switch (level_to_biome(g, v, l)) {
      case BIOME_DUNGEON :    (void) music_play(g, "dungeon.boss"); break;
      case BIOME_BOGLAND :    (void) music_play(g, "bogland.boss"); break;
      case BIOME_NETHERVOID : (void) music_play(g, "nethervoid.boss"); break;
      case BIOME_GRAVEYARD :  (void) music_play(g, "graveyard.boss"); break;
      case BIOME_UNDERHELL :  (void) music_play(g, "underhell.boss"); break;
      default :               ERR("unhandled biome"); break;
    }
  } else {
    switch (level_to_biome(g, v, l)) {
      case BIOME_DUNGEON :
        switch (PCG_RANDOM_RANGE_INCLUSIVE(1, 4)) {
          case 1 :  (void) music_play(g, "dungeon.1"); break;
          case 2 :  (void) music_play(g, "dungeon.2"); break;
          case 3 :  (void) music_play(g, "dungeon.3"); break;
          case 4 :  (void) music_play(g, "dungeon.4"); break;
          default : ERR("unhandled biome music"); break;
        }
        break;

      case BIOME_BOGLAND :
        switch (PCG_RANDOM_RANGE_INCLUSIVE(1, 4)) {
          case 1 :  (void) music_play(g, "bogland.1"); break;
          case 2 :  (void) music_play(g, "bogland.2"); break;
          case 3 :  (void) music_play(g, "bogland.3"); break;
          case 4 :  (void) music_play(g, "bogland.4"); break;
          default : ERR("unhandled biome music"); break;
        }
        break;
      case BIOME_NETHERVOID :
        switch (PCG_RANDOM_RANGE_INCLUSIVE(1, 4)) {
          case 1 :  (void) music_play(g, "nethervoid.1"); break;
          case 2 :  (void) music_play(g, "nethervoid.2"); break;
          case 3 :  (void) music_play(g, "nethervoid.3"); break;
          case 4 :  (void) music_play(g, "nethervoid.4"); break;
          default : ERR("unhandled biome music"); break;
        }
        break;
      case BIOME_GRAVEYARD :
        switch (PCG_RANDOM_RANGE_INCLUSIVE(1, 4)) {
          case 1 :  (void) music_play(g, "graveyard.1"); break;
          case 2 :  (void) music_play(g, "graveyard.2"); break;
          case 3 :  (void) music_play(g, "graveyard.3"); break;
          case 4 :  (void) music_play(g, "graveyard.4"); break;
          default : ERR("unhandled biome music"); break;
        }
        break;
      case BIOME_UNDERHELL :
        switch (PCG_RANDOM_RANGE_INCLUSIVE(1, 4)) {
          case 1 :  (void) music_play(g, "underhell.1"); break;
          case 2 :  (void) music_play(g, "underhell.2"); break;
          case 3 :  (void) music_play(g, "underhell.3"); break;
          case 4 :  (void) music_play(g, "underhell.4"); break;
          default : ERR("unhandled biome music"); break;
        }
        break;
      default : ERR("unhandled biome"); break;
    }
  }
}

void tp_player_level_populated(Gamep g, Levelsp v, Levelp l, Thingp me) { TRACE(); }

void tp_player_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me) { TRACE(); }

void tp_player_on_teleported(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  auto at = thing_at(g, v, l, me);
  game_popup_text_add(g, at.x, at.y, std::string("Urgh"));
}

void tp_player_tick_idle(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  //
  // If asked to follow the mouse path, start walking
  //
  (void) player_move_to_next(g, v, l, me);
}

void tp_player_tick_end(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  //
  // If asked to follow the mouse path, start walking
  //
  (void) player_move_to_next(g, v, l, me);
}

void tp_player_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent *e_maybe_null)
{
  TRACE();
  THING_DBG(g, v, l, me, "player spawned");

  if (g_opt_tests) {
    return;
  }

  static std::initializer_list< std::string > items = {
      "wand_fire", //
  };

  if (! thing_carry(g, v, l, me, items)) {
    thing_err(g, v, l, me, "failed to carry");
  }
}

bool tp_player_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  if (e.event_type == THING_EVENT_MELEE_DAMAGE) {
    (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

    thing_sound_play(g, v, l, attacker, "player_punch");
  }

  return true;
}

bool tp_player_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e)
{
  TRACE();

  if (e.event_type == THING_EVENT_MELEE_DAMAGE) {
    (void) thing_spawn(g, v, l, tp_first(is_effect_attack), target);

    thing_sound_play(g, v, l, attacker, "player_punch");
  }

  return true;
}

void tp_load_player_common(Tpp tp)
{
  // begin sort marker1 {
  thing_description_set(tp, tp_player_description_get);
  thing_detail_set(tp, tp_player_detail_get);
  thing_on_attacking_set(tp, tp_player_on_attacking);
  thing_on_damage_set(tp, tp_player_on_damage);
  thing_on_fall_begin_set(tp, tp_player_on_fall_begin);
  thing_on_fall_end_set(tp, tp_player_on_fall_end);
  thing_on_jump_end_set(tp, tp_player_on_jump_end);
  thing_on_level_enter_set(tp, tp_player_level_enter);
  thing_on_level_leave_set(tp, tp_player_level_leave);
  thing_on_level_populated_set(tp, tp_player_level_populated);
  thing_on_missing_set(tp, tp_player_on_missing);
  thing_on_moved_set(tp, tp_player_on_moved);
  thing_on_moving_to_set(tp, tp_player_on_moving_to);
  thing_on_spawned_set(tp, tp_player_on_spawned);
  thing_on_teleported_set(tp, tp_player_on_teleported);
  thing_on_tick_begin_set(tp, tp_player_tick_begin);
  thing_on_tick_end_set(tp, tp_player_tick_end);
  thing_on_tick_idle_set(tp, tp_player_tick_idle);
  tp_attack_count_max_per_tick_set(tp, 1);
  tp_chance_set(tp, THING_CHANCE_CONTINUE_TO_BURN, "1d6"); // fumble => intensify / keep burning / crit => stop burning
  tp_chance_set(tp, THING_CHANCE_START_BURNING, "1d100");  // fumble => flames spread to you
  tp_damage_set(tp, THING_EVENT_MELEE_DAMAGE, "1d2");
  tp_distance_jump_set(tp, 3);
  tp_distance_throw_set(tp, 6);
  tp_distance_vision_set(tp, MAP_WIDTH); // tiles
  tp_flag_set(tp, is_able_to_be_buffed);
  tp_flag_set(tp, is_able_to_be_engulfed);
  tp_flag_set(tp, is_able_to_be_ensnared);
  tp_flag_set(tp, is_able_to_be_invisible);
  tp_flag_set(tp, is_able_to_be_levitated);
  tp_flag_set(tp, is_able_to_be_teleported);
  tp_flag_set(tp, is_able_to_collect_items);
  tp_flag_set(tp, is_able_to_collect_keys);
  tp_flag_set(tp, is_able_to_crush_grass);
  tp_flag_set(tp, is_able_to_eat_food);
  tp_flag_set(tp, is_able_to_fall_repeatedly);
  tp_flag_set(tp, is_able_to_fall_sound);
  tp_flag_set(tp, is_able_to_fall);
  tp_flag_set(tp, is_able_to_fire_weapons);
  tp_flag_set(tp, is_able_to_jump);
  tp_flag_set(tp, is_able_to_lunge);
  tp_flag_set(tp, is_able_to_move_diagonally);
  tp_flag_set(tp, is_able_to_open_things);
  tp_flag_set(tp, is_able_to_shove);
  tp_flag_set(tp, is_able_to_throw);
  tp_flag_set(tp, is_able_to_wear_items);
  tp_flag_set(tp, is_animated_can_hflip);
  tp_flag_set(tp, is_animated);
  tp_flag_set(tp, is_attackable_by_monst);
  tp_flag_set(tp, is_blit_centered);
  tp_flag_set(tp, is_blit_hit_outline_w_invis_inside);
  tp_flag_set(tp, is_blit_shown_in_chasms);
  tp_flag_set(tp, is_blit_when_obscured_outline);
  tp_flag_set(tp, is_burnable); // is capable of being burned by fire
  tp_flag_set(tp, is_collision_circle_large);
  tp_flag_set(tp, is_combustible);
  tp_flag_set(tp, is_corpse_on_death);
  tp_flag_set(tp, is_critical_to_gameplay);
  tp_flag_set(tp, is_damage_capped);
  tp_flag_set(tp, is_described_cursor);
  tp_flag_set(tp, is_dmap);
  tp_flag_set(tp, is_flesh);
  tp_flag_set(tp, is_light_flicker);
  tp_flag_set(tp, is_light_source, MAP_WIDTH / 2);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_obs_to_beam);
  tp_flag_set(tp, is_obs_to_jumping_onto);
  tp_flag_set(tp, is_obs_to_movement);
  tp_flag_set(tp, is_physics_explosion);
  tp_flag_set(tp, is_physics_temperature);
  tp_flag_set(tp, is_physics_trap);
  tp_flag_set(tp, is_player);
  tp_flag_set(tp, is_shown_health);
  tp_flag_set(tp, is_shown_noise);
  tp_flag_set(tp, is_shown_stamina);
  tp_flag_set(tp, is_submergible); // is seen submerged when in water
  tp_flag_set(tp, is_tickable);
  tp_health_set(tp, "100");
  tp_hearing_threshold_set(tp, 10); // smaller values => better hearing
  tp_light_color_set(tp, "white");
  tp_missile_count_max_set(tp, THING_MISSILE_MAX);
  tp_name_long_set(tp, "easier wizard");
  tp_priority_set(tp, THING_PRIORITY_PLAYER);
  tp_speed_set(tp, 100);
  tp_stamina_set(tp, "100");
  tp_temperature_burns_at_set(tp, 50);  // celsius
  tp_temperature_damage_at_set(tp, 35); // celsius
  tp_temperature_initial_set(tp, 20);   // celsius
  tp_weight_set(tp, WEIGHT_HUMAN);      // grams
  tp_z_depth_set(tp, MAP_Z_DEPTH_PLAYER);
  // end sort marker1 }

  auto delay = 1000;

  auto name = tp_name(tp);

  for (auto frame = 0; frame < 2; frame++) {
    auto *tile = tile_find_mand(name + std::string(".idle.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);
  }

  for (auto frame = 0; frame < 1; frame++) {
    auto *tile = tile_find_mand(name + std::string(".dead.") + std::to_string(frame));
    tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
    tile_delay_ms_set(tile, delay);
    tp_tiles_push_back(tp, THING_ANIM_DEAD, tile);
  }
}
