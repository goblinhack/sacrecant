//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_ui.hpp"

//
// The player has been attacked
//
static void thing_killed_player(Gamep g, Levelsp v, Levelp l, ThingEvent &e)
{
  TRACE();

  auto *it = e.source;

  if (it != nullptr) {
    auto by_the_thing = thing_name_long_the(g, v, l, it);

    switch (e.event_type) {
      case THING_EVENT_SHOVED : //
        topcon(UI_IMPORTANT_FMT_STR "You are shoved by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_CRUSH : //
        topcon(UI_IMPORTANT_FMT_STR "You are crushed to death by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_MELEE_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You are killed by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_HEAT_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You die in the unsuffereble heat from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_WATER_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You die in the undrinkable depths from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You die in blast from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_FIRE_DAMAGE : //
        if (thing_is_lava(it)) {
          topcon(UI_IMPORTANT_FMT_STR "You are consumed by lava!" UI_RESET_FMT);
        } else if (thing_is_fire(it)) {
          topcon(UI_IMPORTANT_FMT_STR "You are consumed in flames!" UI_RESET_FMT);
        } else if (thing_is_water(it)) {
          topcon(UI_IMPORTANT_FMT_STR "You are boiled to death!" UI_RESET_FMT);
        } else if (thing_is_steam(it)) {
          topcon(UI_IMPORTANT_FMT_STR "You are steamed to death!" UI_RESET_FMT);
        } else {
          topcon(UI_IMPORTANT_FMT_STR "You are burnt by %s." UI_RESET_FMT, by_the_thing.c_str());
        }
        break;
      case THING_EVENT_OPEN :             //
      case THING_EVENT_NONE :             //
      case THING_EVENT_FALL :             //
      case THING_EVENT_LIFESPAN_EXPIRED : //
      case THING_EVENT_CARRIED :          //
      case THING_EVENT_CARRIED_MERGED :   //
      case THING_EVENT_MELT :             //
      case THING_EVENT_ENUM_MAX :         //
        err("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
        break;
    }
  } else {
    switch (e.event_type) {
      case THING_EVENT_NONE : break;
      case THING_EVENT_FALL : //
        topcon(UI_IMPORTANT_FMT_STR "You fall to your death." UI_RESET_FMT);
        break;
      case THING_EVENT_LIFESPAN_EXPIRED : //
        topcon(UI_IMPORTANT_FMT_STR "You die of old age." UI_RESET_FMT);
        break;
      case THING_EVENT_SHOVED : //
        topcon(UI_IMPORTANT_FMT_STR "You are shoved and die." UI_RESET_FMT);
        break;
      case THING_EVENT_CRUSH : //
        topcon(UI_IMPORTANT_FMT_STR "You are crushed and die." UI_RESET_FMT);
        break;
      case THING_EVENT_MELEE_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You are beaten to death." UI_RESET_FMT);
        break;
      case THING_EVENT_HEAT_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You die in the unsuffereble heat." UI_RESET_FMT);
        break;
      case THING_EVENT_WATER_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You die in the undrinkable depths." UI_RESET_FMT);
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You die in the blast." UI_RESET_FMT);
        break;
      case THING_EVENT_FIRE_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You are burnt to death." UI_RESET_FMT);
        break;
      case THING_EVENT_OPEN :           //
      case THING_EVENT_CARRIED :        //
      case THING_EVENT_CARRIED_MERGED : //
      case THING_EVENT_MELT :           //
      case THING_EVENT_ENUM_MAX :       //
        err("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
        break;
    }
  }
}

//
// The player has attacked
//
static void thing_killed_by_player(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent &e)
{
  TRACE();
  auto *it = e.source;

  if ((it != nullptr) && thing_is_loggable(t)) {
    auto the_thing = capitalize_first(thing_name_long_the(g, v, l, t));
    auto by_player = thing_name_long(g, v, l, it);

    switch (e.event_type) {
      case THING_EVENT_SHOVED : //
        topcon("%s is knocked over by %s.", the_thing.c_str(), by_player.c_str());
        break;
      case THING_EVENT_CRUSH : //
        topcon("%s is crushed by %s.", the_thing.c_str(), by_player.c_str());
        break;
      case THING_EVENT_MELEE_DAMAGE : //
        topcon("%s is killed by %s.", the_thing.c_str(), by_player.c_str());
        break;
      case THING_EVENT_HEAT_DAMAGE : //
        topcon("%s is killed by heat damage from %s.", the_thing.c_str(), by_player.c_str());
        break;
      case THING_EVENT_WATER_DAMAGE : //
        topcon("%s is killed by water damage from %s.", the_thing.c_str(), by_player.c_str());
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        topcon("%s is killed by blast damage from %s.", the_thing.c_str(), by_player.c_str());
        break;
      case THING_EVENT_FIRE_DAMAGE : //
        topcon("%s is burnt to death by %s.", the_thing.c_str(), by_player.c_str());
        break;
      case THING_EVENT_CARRIED :        //
      case THING_EVENT_CARRIED_MERGED : //
        topcon("%s is carried by %s.", the_thing.c_str(), by_player.c_str());
        break;
      case THING_EVENT_OPEN :             //
      case THING_EVENT_NONE :             //
      case THING_EVENT_FALL :             //
      case THING_EVENT_LIFESPAN_EXPIRED : //
      case THING_EVENT_MELT :             //
      case THING_EVENT_ENUM_MAX :         //
        err("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
        break;
    }
  }
}

//
// Initiate the death process
//
void thing_dead(Gamep g, Levelsp v, Levelp l, Thingp t, ThingEvent &e)
{
  TRACE();

  THING_DBG(t, "thing_dead");

  if (thing_is_dead(t)) {
    return;
  }

  auto *tp = thing_tp(t);

  //
  // Where did the thing die? Might not be on the current level.
  //
  auto *t_level = game_level_get(g, v, t->level_num);
  if (t_level != nullptr) {
    l = t_level;
  }

  //
  // Log the reason of demise?
  //
  if (thing_is_loggable(t)) {
    THING_DBG(t, "%s: dead", to_string(g, v, l, e).c_str());
  }

  //
  // Call this prior to setting death, else we are told that we killed an already dead thing
  //
  if (thing_is_player(t)) {
    thing_killed_player(g, v, l, e);
  } else if ((e.source != nullptr) && thing_is_player(e.source)) {
    thing_killed_by_player(g, v, l, t, e);
  }

  thing_is_dead_set(g, v, l, t);

  //
  // Update the animation, for example, flattened grass
  //
  if (thing_is_burning(t) && (tp_tiles_size(tp, THING_ANIM_BURNT) != 0)) {
    //
    // If it has burnt anim frames
    //
    t->anim_type = THING_ANIM_BURNT;
    //
    // Restart the animation if we have burnt frames
    //
    thing_anim_init(g, v, l, t, THING_ANIM_BURNT);
  } else if (tp_tiles_size(tp, THING_ANIM_DEAD) != 0) {
    //
    // Restart the animation if we have dead frames
    //
    thing_anim_init(g, v, l, t, THING_ANIM_DEAD);
  }

  //
  // Stop it moving
  //
  thing_move_or_jump_finish(g, v, l, t);

  //
  // Do adjacent tiles need updating due to the destruction of this tiled thing?
  //
  if (thing_is_dmap(t) || thing_is_tiled(t)) {
    level_update_paths_set(g, v, l, thing_at(t));
  }

  t->tick_dead = v->tick;

  //
  // Leaves a corpse?
  //
  if (thing_corpse_allowed(g, v, l, t)) {
    //
    // Keep the thing on the map, but in dead state.
    //
    thing_is_corpse_set(g, v, l, t);
  } else {
    //
    // Schedule for removal from the map and freeing
    //
    thing_is_scheduled_for_cleanup_set(g, v, l, t);
  }

  //
  // Request end of game if this is the player
  //
  if (thing_is_player(t)) {
    //
    // No more following the cursor if dead...
    //
    player_state_change(g, v, l, PLAYER_STATE_DEAD);

    auto death_reason = to_death_reason_string(g, v, l, t, e);

    //
    // Hiscore?
    //
    auto score = 666;
    if (game_is_new_hiscore(g, score)) {
      topcon(UI_GOOD_FMT_STR "New high score, %s place!" UI_RESET_FMT, game_place_str(g, score));
      game_add_new_hiscore(g, score, l->level_num, game_player_name_get(g), death_reason.c_str());
    }

    //
    // Request the dead menu at end of tick
    //
    game_request_to_end_game_set(g);
    game_request_to_end_game_reason_set(g, death_reason);
  }

  //
  // Per thing callback
  //
  thing_on_death(g, v, l, t, e);

  //
  // If the mob dies, unleash or kill minions
  //
  if (thing_is_mob(t)) {
    if (thing_is_mob_kill_minions_on_death(t)) {
      (void) thing_mob_kill_all_minions(g, v, l, t, e);
    } else {
      (void) thing_mob_detach_all_minions(g, v, l, t);
    }
  }

  //
  // Unleash minions from mobs
  //
  if (thing_is_minion(t)) {
    (void) thing_minion_detach_me_from_mob(g, v, l, t);
  }

  //
  // Not sure if we kill or just detach projectiles
  //
  if (thing_is_able_to_fire_projectiles(t)) {
    (void) thing_projectile_detach_all_fired(g, v, l, t);
  }

  if (thing_is_projectile(t)) {
    (void) thing_projectile_detach_me_from_firer(g, v, l, t);
  }

  //
  // Bridges need to leave their group now
  //
  // There is a problem here potentially if the group leader dies, leaving the members
  // detached.
  //
  thing_group_leave(g, v, l, t);

  //
  // Unset various flags so the dead thing is not still described as "sleeping" when dead
  //
  thing_is_sleeping_unset(g, v, l, t);
}
