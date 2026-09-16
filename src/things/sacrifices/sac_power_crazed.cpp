//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_dice_rolls.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"

static auto tp_sac_power_crazed_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                             //
      UI_INFO1_FMT_STR "The fight is all that matters to you in your power crazed frenzy.\n"                         //
      UI_INFO2_FMT_STR "Not only do physical type attacks cause quad damage to you, you deliver quad damage also.\n" //
      UI_INFO3_FMT_STR "Additionally, you eschew defence for greater chance of attack.\n"                            //
      UI_INFO4_FMT_STR "This is a powerful sacrifice that only the most hardened accept.\n";                         //
}

static bool tp_sac_power_crazed_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  auto owner = thing_hook_owner_get(g, v, l, me);
  if (! owner) {
    return true;
  }

  THING_DBG(g, v, l, me, "on_damage");

  switch (e.event_type) {
    case THING_EVENT_FALL :          [[fallthrough]];
    case THING_EVENT_THROWN :        [[fallthrough]];
    case THING_EVENT_SHOVED :        [[fallthrough]];
    case THING_EVENT_ENGULF_DAMAGE : [[fallthrough]];
    case THING_EVENT_THROWN_DAMAGE : [[fallthrough]];
    case THING_EVENT_POISON_DAMAGE : [[fallthrough]];
    case THING_EVENT_CRUSH_DAMAGE :  [[fallthrough]];
    case THING_EVENT_MELEE_DAMAGE :  [[fallthrough]];
    case THING_EVENT_EXPLOSION_DAMAGE :
      THING_DBG(g, v, l, me, "receive quad damage %d -> %d", e.damage, e.damage * 4);
      e.damage *= 4;
      break;
    case THING_EVENT_WATER_DAMAGE :     [[fallthrough]];
    case THING_EVENT_ENERGY_DAMAGE :    [[fallthrough]];
    case THING_EVENT_FIRE_DAMAGE :      [[fallthrough]];
    case THING_EVENT_EATEN :            [[fallthrough]];
    case THING_EVENT_CARRIED :          [[fallthrough]];
    case THING_EVENT_CARRIED_MERGED :   [[fallthrough]];
    case THING_EVENT_OPEN :             [[fallthrough]];
    case THING_EVENT_USED :             [[fallthrough]];
    case THING_EVENT_LEVITATED :        [[fallthrough]];
    case THING_EVENT_NONE :             [[fallthrough]];
    case THING_EVENT_GAME_OVER :        [[fallthrough]];
    case THING_EVENT_LIFESPAN_EXPIRED : [[fallthrough]];
    case THING_EVENT_MELT :             [[fallthrough]];
    case THING_EVENT_USER_INITIATED :   [[fallthrough]];
    case THING_EVENT_SPAWNED :          [[fallthrough]];
    case THING_EVENT_ENUM_MAX : //
      break;
  }

  return true;
}

static bool tp_sac_power_crazed_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp target, ThingEvent &e)
{
  TRACE();

  auto owner = thing_hook_owner_get(g, v, l, me);
  if (! owner) {
    return true;
  }

  THING_DBG(g, v, l, owner, "attack with quad damage %d -> %d", e.damage, e.damage * 4);

  e.damage *= 4;

  return true;
}

[[nodiscard]] auto tp_load_sac_power_crazed() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_power_crazed"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_power_crazed_detail_get);
  thing_on_damage_set(tp, tp_sac_power_crazed_on_damage);
  thing_on_attacking_set(tp, tp_sac_power_crazed_on_attacking);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_sac_points_set(tp, 100);
  tp_name_long_set(tp, "power crazed");
  tp_stat_set(tp, THING_STAT_ATT, "18");
  tp_stat_set(tp, THING_STAT_DEF, "2");
  // end sort marker1 }

  return true;
}
