//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_buff_prot_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                               //
      UI_INFO1_FMT_STR "With this buff your health issues are a thing of the past.\n"; //
}

static bool tp_buff_prot_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e)
{
  TRACE();

  if (! e.damage) {
    return true; // allow the damage to be applied
  }

  auto owner        = thing_buff_owner_get(g, v, l, me);
  auto lifespan     = thing_lifespan(g, v, l, me);
  auto new_lifespan = thing_lifespan_decr(g, v, l, me, e.damage);

  THING_DBG(g, v, l, me, "activated: damage %d lifespan %d => %d", e.damage, lifespan, new_lifespan);

  e.damage = 0;

  if (new_lifespan <= 0) {
    if (owner && thing_is_player(owner)) {
      topcon(UI_GOOD_FMT_STR "Your trial period of protection has ended." UI_RESET_FMT);
      thing_sound_play(g, v, l, owner, "bonus");
    }
  } else {
    if (owner && thing_is_player(owner)) {
      topcon(UI_GOOD_FMT_STR "Your protection shield activates." UI_RESET_FMT);
    }
  }

  return true; // allow the damage to be applied
}

[[nodiscard]] auto tp_load_buff_prot() -> bool
{
  TRACE();

  auto *tp   = tp_load("buff_prot"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_buff_prot_detail_get);
  thing_on_damage_set(tp, tp_buff_prot_on_damage);
  tp_flag_set(tp, is_buff);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_tick_on_use);
  tp_flag_set(tp, is_tickable);
  tp_lifespan_set(tp, "1d100+100");
  tp_name_short_set(tp, "protection");
  tp_flag_set(tp, is_hook);
  // end sort marker1 }

  return true;
}
