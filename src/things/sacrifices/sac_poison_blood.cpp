//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_sac_poison_blood_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return UI_INFO1_FMT_STR                                                                                                        //
      "You are so full of vengeance that even your own blood is toxic! Unfortunately it will slowly kill you.\n"                 //
      UI_INFO2_FMT_STR                                                                                                           //
      "In detail, your health will slowly tick down inexorably, depending on your luck, as if you are permanently poisoned...\n" //
      UI_INFO3_FMT_STR                                                                                                           //
      "As there is no cure, you will need to be expedient in finding healing potions or, gods forbid, clown meat.\n";            //
}

static void tp_sac_poison_blood_on_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me)
{
  TRACE();

  if (d100() < 95) {
    return;
  }

  auto owner = thing_buff_owner_get(g, v, l, me);
  if (! owner) {
    return;
  }

  THING_DBG(g, v, l, owner, "poisoned blood tick");
  TRACE_INDENT();

  if (thing_stat_success(g, v, l, owner, THING_STAT_LUCK, TARGET_ROLL_POISONED)) {
    THING_DBG(g, v, l, owner, "poisoned blood tick: good luck");
    return;
  }

  THING_DBG(g, v, l, owner, "poisoned blood tick: bad luck");

  if (thing_health_decr(g, v, l, owner, 1) <= 0) {
    THING_DBG(g, v, l, owner, "dead due to poisoned blood");
    TRACE_INDENT();

    if (thing_is_player(owner)) {
      topcon(UI_IMPORTANT_FMT_STR "Your own poisoned blood has killed you!" UI_RESET_FMT);
    }

    ThingEvent e {
        .reason     = "by own blood",            //
        .event_type = THING_EVENT_POISON_DAMAGE, //
    };

    thing_dead(g, v, l, owner, e);
  }
}

[[nodiscard]] auto tp_load_sac_poison_blood() -> bool
{
  TRACE();

  auto *tp   = tp_load("sac_poison_blood"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_sac_poison_blood_detail_get);
  thing_on_tick_begin_set(tp, tp_sac_poison_blood_on_tick_begin);
  tp_flag_set(tp, is_hook);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_sacrifice);
  tp_sac_points_set(tp, 20);
  tp_name_long_set(tp, "poison blood");
  tp_stat_set(tp, THING_STAT_ATT, "12");
  // end sort marker1 }

  return true;
}
