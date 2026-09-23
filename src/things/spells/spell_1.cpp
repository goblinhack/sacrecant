//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_main.hpp"
#include "../../my_sprintf.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"

static const std::string upgrade_1 = "increase radius and range";
static const std::string upgrade_2 = "increase damage";
static const std::string option_1  = "targeted fireball";
static const std::string option_2  = "radial, including your tile";
static const std::string option_3  = "radial, excluding your tile";

static auto tp_spell_1_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                                                         //
      UI_INFO1_FMT_STR "Conjure a devastating fireball, targeted at your enemies or radially around yourself.\n" //
      UI_INFO2_FMT_STR "Can be upgraded in power and distance.\n";                                               //
}

static bool tp_spell_1_on_cast_request(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e)
{
  TRACE();

  THING_DBG(g, v, l, user, "cast request");
  TRACE_INDENT();
  THING_DBG(g, v, l, spell, "this");

  if (e.spell_info.option_name.empty() || (e.spell_info.option_name == option_1)) {
    //
    // If target is not set, get one
    //
    if (! e.spell_info.target_set) {
      THING_DBG(g, v, l, user, "need target");
      if (thing_is_player(user)) {
        topcon("Choose a target for this spell.");
        game_spell_cast_set(g, e);
        game_state_change(g, STATE_CHOOSE_SPELL_TARGET, "choose a target");
      }
      return true;
    }

    THING_DBG(g, v, l, user, "got target");
    e.spell_info.spell_was_cast = true;
    thing_sound_play(g, v, l, user, "spell");
    return true;
  }

  if (e.spell_info.option_name == option_2) {
    topcon("todo option 2");
    e.spell_info.spell_was_cast = true;
    thing_sound_play(g, v, l, user, "spell");
    return true;
  }

  if (e.spell_info.option_name == option_3) {
    topcon("todo option 3");
    e.spell_info.spell_was_cast = true;
    thing_sound_play(g, v, l, user, "spell");
    return true;
  }

  thing_err(g, v, l, spell, "unknown spell option: %s", e.spell_info.option_name.c_str());
  return false;
}

static auto tp_spell_1_on_upgrade_possible(Gamep g, Levelsp v, Levelp l, Thingp me, TpSpellUpgrade u) -> bool
{
  TRACE();

  bool upgradeable = false;

  if (u.name == upgrade_1) {
    upgradeable = (thing_spell_radius(g, v, l, me) < thing_spell_radius_max(g, v, l, me));
    upgradeable |= (thing_spell_range(g, v, l, me) < thing_spell_range_max(g, v, l, me));
  }

  if (u.name == upgrade_2) {
    upgradeable = true;
  }

  return upgradeable;
}

static auto tp_spell_1_on_upgrade_do(Gamep g, Levelsp v, Levelp l, Thingp me, TpSpellUpgrade u) -> bool
{
  TRACE();

  if (u.name == upgrade_1) {
    (void) thing_spell_radius_incr(g, v, l, me);
    (void) thing_spell_range_incr(g, v, l, me);
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
  tp_spell_radius_set(tp, 3);
  tp_spell_radius_max_set(tp, 6);
  tp_spell_range_set(tp, 8);
  tp_spell_range_max_set(tp, 12);
  tp_flag_set(tp, is_loggable);
  tp_spell_cost_set(tp, 1);
  tp_spell_mana_cost_set(tp, 50);
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
                          .type = "1", //
                          .name = option_1,
                      });
  tp_spell_option_add(tp,
                      TpSpellOption {
                          .type = "2", //
                          .name = option_2,
                      });
  tp_spell_option_add(tp,
                      TpSpellOption {
                          .type = "3", //
                          .name = option_3,
                      });

  return true;
}
