//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_inlines.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wids.hpp"

#include <string>

//
// The player has been attacked
//
static void thing_killed_player(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  auto *it = e.source;

  if (it != nullptr) {
    std::string by_the_thing;
    auto       *fired_by = thing_missile_fired_by_get(g, v, l, it);
    if (fired_by != nullptr) {
      if (fired_by == me) {
        by_the_thing = "your " + thing_name_long(g, v, l, it);
      } else {
        by_the_thing = thing_name_apostrophize_the(g, v, l, fired_by) + " " + thing_name_long(g, v, l, it);
      }
    } else {
      by_the_thing = thing_name_long_the(g, v, l, it);
    }

    //
    // Ensure we're showing the detail of whatever did the deed
    //
    level_cursor_describe_clear(g, v);
    (void) level_cursor_describe_add(g, v, it);
    (void) wid_rightbar_init(g);

    switch (e.event_type) {
      case THING_EVENT_THROWN : //
        topcon(UI_IMPORTANT_FMT_STR "You are thrown by %s!" UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_SHOVED : //
        topcon(UI_IMPORTANT_FMT_STR "You are shoved by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_CRUSH_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You are crushed to death by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_ENGULF_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You are digested to death by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_POISON_DAMAGE : topcon(UI_IMPORTANT_FMT_STR "You are killed by poison." UI_RESET_FMT); break;
      case THING_EVENT_THROWN_DAMAGE : [[fallthrough]];
      case THING_EVENT_MELEE_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You are killed by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_WATER_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You die in the undrinkable depths from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You die in the blast from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_ENERGY_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You die in the blinding light from %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_FIRE_DAMAGE : //
        if (thing_is_lava(it)) {
          topcon(UI_IMPORTANT_FMT_STR "You are consumed by lava!" UI_RESET_FMT);
        } else if (thing_is_fire(it)) {
          topcon(UI_IMPORTANT_FMT_STR "You are consumed in flames!" UI_RESET_FMT);
        } else if (thing_is_water_shallow(it) || thing_is_water_deep(it)) {
          topcon(UI_IMPORTANT_FMT_STR "You are boiled to death!" UI_RESET_FMT);
        } else if (thing_is_steam(it)) {
          topcon(UI_IMPORTANT_FMT_STR "You are steamed to death!" UI_RESET_FMT);
        } else {
          topcon(UI_IMPORTANT_FMT_STR "You are burnt by %s." UI_RESET_FMT, by_the_thing.c_str());
        }
        break;
      case THING_EVENT_EATEN : //
        topcon(UI_IMPORTANT_FMT_STR "You are eaten by %s." UI_RESET_FMT, by_the_thing.c_str());
        break;
      case THING_EVENT_CARRIED :          [[fallthrough]];
      case THING_EVENT_CARRIED_MERGED :   break;
      case THING_EVENT_OPEN :             [[fallthrough]];
      case THING_EVENT_USED :             [[fallthrough]];
      case THING_EVENT_LEVITATED :        [[fallthrough]];
      case THING_EVENT_NONE :             [[fallthrough]];
      case THING_EVENT_FALL :             [[fallthrough]];
      case THING_EVENT_GAME_OVER :        [[fallthrough]];
      case THING_EVENT_LIFESPAN_EXPIRED : [[fallthrough]];
      case THING_EVENT_MELT :             [[fallthrough]];
      case THING_EVENT_USER_INITIATED :   [[fallthrough]];
      case THING_EVENT_SPAWNED :          [[fallthrough]];
      case THING_EVENT_ENUM_MAX : //
        ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
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
      case THING_EVENT_THROWN : //
        topcon(UI_IMPORTANT_FMT_STR "You are thrown and die." UI_RESET_FMT);
        break;
      case THING_EVENT_SHOVED : //
        topcon(UI_IMPORTANT_FMT_STR "You are shoved and die." UI_RESET_FMT);
        break;
      case THING_EVENT_CRUSH_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You are crushed and die." UI_RESET_FMT);
        break;
      case THING_EVENT_ENGULF_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You are engulfed and die." UI_RESET_FMT);
        break;
      case THING_EVENT_POISON_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You are poisoned and die." UI_RESET_FMT);
        break;
      case THING_EVENT_THROWN_DAMAGE : [[fallthrough]];
      case THING_EVENT_MELEE_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You are beaten to death." UI_RESET_FMT);
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
      case THING_EVENT_ENERGY_DAMAGE : //
        topcon(UI_IMPORTANT_FMT_STR "You are blasted to death." UI_RESET_FMT);
        break;
      case THING_EVENT_EATEN : //
        topcon(UI_IMPORTANT_FMT_STR "You are eaten." UI_RESET_FMT);
        break;
      case THING_EVENT_GAME_OVER : //
        break;
      case THING_EVENT_OPEN :           [[fallthrough]];
      case THING_EVENT_USED :           [[fallthrough]];
      case THING_EVENT_LEVITATED :      [[fallthrough]];
      case THING_EVENT_CARRIED :        [[fallthrough]];
      case THING_EVENT_CARRIED_MERGED : [[fallthrough]];
      case THING_EVENT_MELT :           [[fallthrough]];
      case THING_EVENT_USER_INITIATED : [[fallthrough]];
      case THING_EVENT_SPAWNED :        [[fallthrough]];
      case THING_EVENT_ENUM_MAX : //
        ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
        break;
    }
  }
}

//
// The player has attacked
//
static void thing_killed_by_player(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  auto *player = e.source;
  if (player == nullptr) [[unlikely]] {
    return;
  }

  //
  // Record the death count
  //
  auto *p = thing_player_struct(g);
  if (p != nullptr) {
    p->defeated[ tp_id_get(thing_tp(me)) ]++;
  }

  if (! thing_is_described_when_killed(me)) {
    return;
  }

  auto the_thing = capitalize_first(thing_name_long_the(g, v, l, me));

  std::string by_player;
  auto       *fired_by = thing_missile_fired_by_get(g, v, l, player);
  if (fired_by != nullptr) {
    if (fired_by == player) {
      by_player = "your " + thing_name_long(g, v, l, player);
    } else {
      by_player = thing_name_apostrophize_the(g, v, l, fired_by) + " " + thing_name_long(g, v, l, player);
    }
  } else {
    by_player = thing_name_long_the(g, v, l, player);
  }

  switch (e.event_type) {
    case THING_EVENT_THROWN : //
      topcon("%s is thrown by %s.", the_thing.c_str(), by_player.c_str());
      break;
    case THING_EVENT_SHOVED : //
      topcon("%s is knocked over by %s.", the_thing.c_str(), by_player.c_str());
      break;
    case THING_EVENT_CRUSH_DAMAGE : //
      topcon("%s is crushed by %s.", the_thing.c_str(), by_player.c_str());
      break;
    case THING_EVENT_ENGULF_DAMAGE : //
      topcon("%s is engulfed by %s.", the_thing.c_str(), by_player.c_str());
      break;
    case THING_EVENT_POISON_DAMAGE : topcon("%s is killed by poison.", the_thing.c_str()); break;
    case THING_EVENT_THROWN_DAMAGE : [[fallthrough]];
    case THING_EVENT_MELEE_DAMAGE : //
      topcon("%s is killed by %s.", the_thing.c_str(), by_player.c_str());
      break;
    case THING_EVENT_WATER_DAMAGE : //
      topcon("%s is killed by water damage from %s.", the_thing.c_str(), by_player.c_str());
      break;
    case THING_EVENT_EXPLOSION_DAMAGE : //
      topcon("%s is killed by blast damage from %s.", the_thing.c_str(), by_player.c_str());
      break;
    case THING_EVENT_FIRE_DAMAGE : //
      if (thing_is_burning(me)) {
        topcon("%s is killed by %s.", the_thing.c_str(), by_player.c_str());
      } else {
        topcon("%s is burnt to death by %s.", the_thing.c_str(), by_player.c_str());
      }
      break;
    case THING_EVENT_ENERGY_DAMAGE : //
      topcon("%s is blasted to death by %s.", the_thing.c_str(), by_player.c_str());
      break;
    case THING_EVENT_EATEN : //
      topcon("%s is eaten by %s.", the_thing.c_str(), by_player.c_str());
      break;
    case THING_EVENT_CARRIED :          [[fallthrough]];
    case THING_EVENT_CARRIED_MERGED :   break;
    case THING_EVENT_OPEN :             [[fallthrough]];
    case THING_EVENT_USED :             [[fallthrough]];
    case THING_EVENT_LEVITATED :        [[fallthrough]];
    case THING_EVENT_NONE :             [[fallthrough]];
    case THING_EVENT_GAME_OVER :        [[fallthrough]];
    case THING_EVENT_FALL :             [[fallthrough]];
    case THING_EVENT_LIFESPAN_EXPIRED : [[fallthrough]];
    case THING_EVENT_MELT :             [[fallthrough]];
    case THING_EVENT_USER_INITIATED :   [[fallthrough]];
    case THING_EVENT_SPAWNED :          [[fallthrough]];
    case THING_EVENT_ENUM_MAX : //
      ERR("unexpected event: %s", ThingEventType_to_string(e.event_type).c_str());
      break;
  }
}

//
// Who really dunnit?
//
static auto thing_get_killer(Gamep g, Levelsp v, Levelp l, ThingEvent &e) -> Thingp
{
  auto *killer = e.source;

  if (killer == nullptr) {
    return nullptr;
  }

  auto *fired_by = thing_missile_fired_by_get(g, v, l, killer);
  if (fired_by != nullptr) {
    return fired_by;
  }

  auto *owner = thing_owner(g, v, l, killer);
  if (owner != nullptr) {
    return owner;
  }

  return killer;
}

//
// Initiate the death process
//
void thing_dead(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  THING_DBG(g, v, l, me, "is dead");
  TRACE_INDENT();

  if (thing_is_corpse(me)) {
    if (! thing_is_able_to_resurrect(me)) {
      return;
    }
    THING_DBG(g, v, l, me, "is a corpse already");
  } else if (thing_is_dead(me)) {
    THING_DBG(g, v, l, me, "is already dead");
    return;
  }

  auto *tp = thing_tp(me);

  //
  // Where did the thing die? Might not be on the current level.
  //
  auto *t_level = game_level_get(g, v, me->level_num);
  if (t_level != nullptr) {
    l = t_level;
  }

  //
  // Log the reason of demise?
  //
  if (thing_is_loggable(me)) {
    THING_DBG(g, v, l, me, "%s: dead", to_string(g, v, l, e).c_str());
  }

  auto *killer = thing_get_killer(g, v, l, e);

  //
  // Call this prior to setting death, else we are told that we killed an already dead thing
  //
  if (thing_is_player(me)) {
    thing_killed_player(g, v, l, me, e);
  } else if ((killer != nullptr) && thing_is_player(killer)) {
    thing_killed_by_player(g, v, l, me, e);
  }

  thing_is_dead_set(g, v, l, me);

  //
  // Update the animation, for example, flattened grass
  //
  if (thing_is_burning(me) && (tp_tiles_size(tp, THING_ANIM_BURNT) != 0)) {
    //
    // If it has burnt anim frames
    //
    me->anim_type = THING_ANIM_BURNT;
    //
    // Restart the animation if we have burnt frames
    //
    thing_anim_init(g, v, l, me, THING_ANIM_BURNT);
  } else if (tp_tiles_size(tp, THING_ANIM_DEAD) != 0) {
    //
    // Restart the animation if we have dead frames
    //
    thing_anim_init(g, v, l, me, THING_ANIM_DEAD);
  }

  //
  // Stop it moving
  //
  thing_move_finish(g, v, l, me);

  //
  // Do adjacent tiles need updating due to the destruction of this tiled thing?
  //
  if (thing_is_dmap(me) || thing_is_tiled(me)) {
    level_update_paths_set(g, v, l, thing_at(g, v, l, me));
  }

  me->tick_dead = v->tick;

  //
  // Leaves a corpse?
  //
  if (thing_is_corpse(me)) {
    //
    // Already a corpse, clean it up
    //
    thing_is_scheduled_for_cleanup_set(g, v, l, me);
  } else if (thing_corpse_allowed(g, v, l, me)) {
    //
    // Keep the thing on the map, but in dead state.
    //
    thing_is_corpse_set(g, v, l, me);
  } else {
    //
    // Schedule for removal from the map and freeing
    //
    thing_is_scheduled_for_cleanup_set(g, v, l, me);
  }

  //
  // Request end of game if this is the player
  //
  if (thing_is_player(me)) {
    //
    // No more following the cursor if dead...
    //
    player_state_change(g, v, l, PLAYER_STATE_DEAD);

    auto death_reason = to_death_reason_string(g, v, l, me, e);

    //
    // Hiscore?
    //
    auto score = thing_score(g, me);
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
  thing_on_death(g, v, l, me, e);

  //
  // If the mob dies, unleash or kill minions
  //
  if (thing_is_mob(me)) {
    if (thing_is_mob_kill_minions_on_death(me)) {
      (void) thing_mob_kill_all_minions(g, v, l, me, e);
    } else {
      (void) thing_mob_detach_all_minions(g, v, l, me);
    }
  }

  //
  // Unleash minions from mobs
  //
  if (thing_is_minion(me)) {
    (void) thing_minion_detach_me_from_mob(g, v, l, me);
  }

  //
  // Not sure if we kill or just detach projectiles
  //
  if (thing_is_able_to_fire_weapons(me)) {
    (void) thing_missile_detach_all_fired(g, v, l, me);
  }

  //
  // Detach weapons from owners
  //
  if (thing_is_projectile(me) || thing_is_beam_weapon(me)) {
    (void) thing_missile_detach_me_from_firer(g, v, l, me);
  }

  //
  // Detach buffs
  //
  if (thing_is_able_to_be_buffed(me)) {
    (void) thing_buff_detach_all(g, v, l, me);
  }

  if (thing_is_buff(me)) {
    (void) thing_buff_detach_me_from_owner(g, v, l, me);
  }

  //
  // Detach items from owners
  //
  if (thing_is_carried(me)) {
    auto *owner = thing_owner(g, v, l, me);
    if (owner != nullptr) {
      if (! thing_drop(g, v, l, owner, me, e)) {
        thing_err(g, v, l, me, "item is carried but could not drop");
      }
    }
  } else {
    //
    // Drop all items
    //
    if (thing_is_able_to_drop_all_items_on_death(me)) {
      (void) thing_drop_all(g, v, l, me, e);
    }
  }

  //
  // Bridges need to leave their group now
  //
  // There is a problem here potentially if the group leader dies, leaving the members
  // detached.
  //
  thing_group_leave(g, v, l, me);

  //
  // Unset various flags so the dead thing is not still described as "sleeping" when dead
  //
  thing_is_sleeping_unset(g, v, l, me);

  //
  // Give score bonus to the player
  //
  if ((killer != nullptr) && (killer != me) && thing_is_player(killer)) {
    auto bonus = tp_score_value_get(tp);
    (void) thing_score_incr(g, v, l, killer, bonus);
  }
}

void thing_is_dead_set(Gamep g, Levelsp v, Levelp l, Thingp t, bool val)
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  if (t->_is_dead == static_cast< int >(val)) {
    return;
  }
  t->_is_dead = val;

  if (val) {
    THING_DBG(g, v, l, t, "is dead set");
  }
}

void thing_is_dead_unset(Gamep g, Levelsp v, Levelp l, Thingp t)
{
  TRACE_DEBUG();

  thing_is_dead_set(g, v, l, t, false);
}

[[nodiscard]] auto thing_is_dead_when_discharged(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_dead_when_discharged) != 0;
}

[[nodiscard]] auto thing_is_hit_when_dead(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_hit_when_dead) != 0;
}

[[nodiscard]] auto thing_is_obs_when_dead(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_obs_when_dead) != 0;
}

[[nodiscard]] auto thing_is_dead_on_collision(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_dead_on_collision) != 0;
}

[[nodiscard]] auto thing_is_undead(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_undead) != 0;
}

[[nodiscard]] auto thing_is_wait_on_dead_anim(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_wait_on_dead_anim) != 0;
}

[[nodiscard]] auto thing_is_mob_kill_minions_on_death(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_mob_kill_minions_on_death) != 0;
}

[[nodiscard]] auto thing_is_extinguished_on_death(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_extinguished_on_death) != 0;
}

[[nodiscard]] auto thing_is_broken_on_death(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_broken_on_death) != 0;
}

[[nodiscard]] auto thing_is_described_when_killed(Thingp t) -> bool
{
  TRACE_DEBUG();

  if (t == nullptr) {
    ERR("no thing pointer");
    return false;
  }
  return tp_flag(thing_tp(t), is_described_when_killed) != 0;
}
