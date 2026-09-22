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

static auto tp_sac_glass_bones_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                               //
      UI_INFO1_FMT_STR "You have glass bones and all physical trauma causes you double the damage!\n"; //
}

static bool tp_sac_glass_bones_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
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
    case THING_EVENT_SPELL_DAMAGE :  [[fallthrough]];
    case THING_EVENT_EXPLOSION_DAMAGE :
      THING_DBG(g, v, l, owner, "receive double damage %d->%d", e.damage, e.damage * 2);
      e.damage *= 2;
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

[[nodiscard]] auto tp_load_sac_glass_bones() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_glass_bones"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_glass_bones_detail_get);
  thing_on_damage_set(tp, tp_sac_glass_bones_on_damage);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_sac_points_set(tp, 5);
  tp_name_long_set(tp, "glass bones");
  tp_stat_set(tp, THING_STAT_DEF, "8");
  // end sort marker1 }

  return true;
}
