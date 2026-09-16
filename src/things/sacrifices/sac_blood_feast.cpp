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

static auto tp_sac_blood_feast_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                         //
      UI_INFO1_FMT_STR "Drain energy from your living enemies as you attack them, increasing your own health.\n" //
      UI_INFO2_FMT_STR "The downside. This only works for living enemies. Undead cause you quad damage.\n";
}

static bool tp_sac_blood_feast_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  auto owner = thing_hook_owner_get(g, v, l, me);
  if (! owner) {
    return true;
  }

  auto attacker = thing_get_attacker(g, v, l, e);
  if (! attacker) {
    return true;
  }

  if (! thing_is_undead(attacker)) {
    return true;
  }

  THING_DBG(g, v, l, me, "on_damage");
  THING_DBG(g, v, l, owner, "receive quad damage %d->%d", e.damage, e.damage * 4);
  e.damage *= 4;

  return true;
}

static bool tp_sac_blood_feast_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp target, ThingEvent &e)
{
  TRACE();

  auto owner = thing_hook_owner_get(g, v, l, me);
  if (! owner) {
    return true;
  }

  if (! thing_is_monst(target)) {
    return true;
  }

  if (thing_is_undead(target)) {
    return true;
  }

  auto old_health = thing_health(g, v, l, owner);
  auto new_health = thing_health_incr(g, v, l, owner, e.damage);

  THING_DBG(g, v, l, me, "on_attacking");
  THING_DBG(g, v, l, target, "target");
  THING_DBG(g, v, l, owner, "drain health damage %d->%d", old_health, new_health);

  return true;
}

[[nodiscard]] auto tp_load_sac_blood_feast() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_blood_feast"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_blood_feast_detail_get);
  thing_on_damage_set(tp, tp_sac_blood_feast_on_damage);
  thing_on_attacking_set(tp, tp_sac_blood_feast_on_attacking);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_sac_points_set(tp, 20);
  tp_name_long_set(tp, "blood feast");
  // end sort marker1 }

  return true;
}
