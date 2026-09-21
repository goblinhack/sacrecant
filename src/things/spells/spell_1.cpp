//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_main.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"

static const std::string upgrade_1 = "increase radius";
static const std::string upgrade_2 = "increase damage";

static auto tp_spell_1_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                         //
      UI_INFO1_FMT_STR "Conjure a devastating fireball, targeted at your enemies or radially around yourself.\n" //
      UI_INFO2_FMT_STR "Spell effect radius: "                                                                   //
      + std::to_string(thing_effect_radius(g, v, l, me));
}

static bool tp_spell_1_on_cast_request(Gamep g, Levelsp v, Levelp l, Thingp me, Thingp dropper, ThingEvent &e)
{
  TRACE();

  topcon("todo");

  return false;
}

static auto tp_spell_1_on_upgrade_possible(Gamep g, Levelsp v, Levelp l, Thingp me, TpSpellUpgrade u) -> bool
{
  TRACE();

  if (u.name == upgrade_1) {
    if (thing_effect_radius(g, v, l, me) < 4) {
      return true;
    }
  }

  if (u.name == upgrade_2) {
    return true;
  }

  return false;
}

static auto tp_spell_1_on_upgrade_do(Gamep g, Levelsp v, Levelp l, Thingp me, TpSpellUpgrade u) -> bool
{
  TRACE();

  if (u.name == upgrade_1) {
    (void) thing_effect_radius_incr(g, v, l, me);
    return true;
  }

  if (u.name == upgrade_2) {
    return true;
  }

  return false;
}

[[nodiscard]] auto tp_load_spell_1() -> bool
{
  TRACE();

  auto *tp   = tp_load("spell_1"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_spell_1_get);
  thing_on_upgrade_possible_set(tp, tp_spell_1_on_upgrade_possible);
  thing_on_upgrade_do_set(tp, tp_spell_1_on_upgrade_do);
  thing_on_cast_request_set(tp, tp_spell_1_on_cast_request);
  tp_flag_set(tp, is_spell);
  tp_stat_set(tp, THING_STAT_ARCANA_FIRE, "11");
  tp_flag_set(tp, is_unused_spell);
  tp_effect_radius_set(tp, 3);
  tp_flag_set(tp, is_loggable);
  tp_spell_cost_set(tp, 1);
  tp_name_long_set(tp, "fireball");
  // end sort marker1 }

  tp_spell_upgrade_add(tp,
                       TpSpellUpgrade {
                           .type = "1", //
                           .name = upgrade_1,
                       });
  tp_spell_upgrade_add(tp,
                       TpSpellUpgrade {
                           .type = "2", //
                           .name = upgrade_2,
                       });
  tp_spell_option_add(tp,
                      TpSpellOption {
                          .type = "1",                 //
                          .name = "targeted fireball", //
                      });
  tp_spell_option_add(tp,
                      TpSpellOption {
                          .type = "2",                           //
                          .name = "radial, including your tile", //
                      });
  tp_spell_option_add(tp,
                      TpSpellOption {
                          .type = "3",                           //
                          .name = "radial, excluding your tile", //
                      });

  return true;
}
