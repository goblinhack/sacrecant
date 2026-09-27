//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_level_inlines.hpp"
#include "../../my_main.hpp"
#include "../../my_sound.hpp"
#include "../../my_sprintf.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"
#include "my_spell_common.hpp"

static const std::string upgrade_1_increase_radius                 = "increase radius and range";
const std::string        spell_option_targeted                     = "targeted";
const std::string        spell_option_radial                       = "radial";
const std::string        spell_option_radial_excluding_player_tile = "radial, excluding your tile";

bool tp_spell_common_on_cast_request(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e, SpellOnCastRequest fn)
{
  TRACE();

  THING_DBG(g, v, l, user, "cast request");
  TRACE_INDENT();
  THING_DBG(g, v, l, spell, "this");

  if (e.spell_info.option_name.empty() || (e.spell_info.option_name == spell_option_targeted)) {
    //
    // If target is not set, get one
    //
    if (! e.spell_info.target_set) {
      if (thing_is_player(user)) {
        topcon("Choose a target for spell '%s'.", capitalize(thing_name_short(g, v, l, spell)).c_str());
        game_spell_tmp_while_targeting_set(g, e);
        game_state_change(g, STATE_CHOOSE_SPELL_TARGET, "choose a target");
      }
      THING_DBG(g, v, l, spell, "need target");
      return true;
    }

    THING_DBG(g, v, l, spell, "have target");
    return true;
  }

  if (e.spell_info.option_name == spell_option_radial) {
    THING_DBG(g, v, l, spell, "ok");
    return true;
  }

  if (e.spell_info.option_name == spell_option_radial_excluding_player_tile) {
    THING_DBG(g, v, l, spell, "ok");
    return true;
  }

  thing_err(g, v, l, spell, "unknown spell option: %s", e.spell_info.option_name.c_str());
  return false;
}

bool tp_spell_common_on_cast_do(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e, SpellOnCastDo fn)
{
  TRACE();

  THING_DBG(g, v, l, user, "cast do");
  TRACE_INDENT();
  THING_DBG(g, v, l, spell, "this");

  auto user_at      = thing_at(g, v, l, user);
  auto spell_radius = thing_spell_radius(g, v, l, spell);
  auto spell_range  = thing_spell_range(g, v, l, spell);

  if (e.spell_info.option_name.empty() || (e.spell_info.option_name == spell_option_targeted)) {
    if (! e.spell_info.target_set) {
      thing_err(g, v, l, spell, "no target set");
      return false;
    }

    //
    // Check the range
    //
    auto target = e.spell_info.target;
    if (distance(target, user_at) > spell_range) {
      if (thing_is_player(user)) {
        topcon("That tile is out of range for spell '%s'.", capitalize(thing_name_short(g, v, l, spell)).c_str());
        (void) sound_play(g, "error");
      }
      return false;
    }

    //
    // Targeted
    //
    for (auto dx = -spell_radius; dx <= spell_radius; dx++) {
      for (auto dy = -spell_radius; dy <= spell_radius; dy++) {
        bpoint p(target.x + dx, target.y + dy);
        if (! is_oob(p)) {
          if (distance(p, target) <= spell_radius) {
            if (! fn(g, v, l, spell, user, p, e)) {
              return false;
            }
          }
        }
      }
    }

    thing_sound_play(g, v, l, user, "spell");
    THING_DBG(g, v, l, spell, "done");
    return true;
  }

  //
  // Radial, including player tile
  //
  if (e.spell_info.option_name == spell_option_radial) {
    auto target = thing_at(g, v, l, user);
    for (auto dx = -spell_radius; dx <= spell_radius; dx++) {
      for (auto dy = -spell_radius; dy <= spell_radius; dy++) {
        bpoint p(target.x + dx, target.y + dy);
        if (! is_oob(p)) {
          if (distance(p, target) <= spell_radius) {
            if (! fn(g, v, l, spell, user, p, e)) {
              return false;
            }
          }
        }
      }
    }
    thing_sound_play(g, v, l, user, "spell");
    THING_DBG(g, v, l, spell, "done");
    return true;
  }

  //
  // Radial, excluding player tile
  //
  if (e.spell_info.option_name == spell_option_radial_excluding_player_tile) {
    auto target = thing_at(g, v, l, user);
    for (auto dx = -spell_radius; dx <= spell_radius; dx++) {
      for (auto dy = -spell_radius; dy <= spell_radius; dy++) {
        bpoint p(target.x + dx, target.y + dy);
        if (! is_oob(p)) {
          if (p != target) {
            if (distance(p, target) <= spell_radius) {
              if (! fn(g, v, l, spell, user, p, e)) {
                return false;
              }
            }
          }
        }
      }
    }
    thing_sound_play(g, v, l, user, "spell");
    THING_DBG(g, v, l, spell, "done");
    return true;
  }

  thing_err(g, v, l, spell, "unknown spell casting option: %s", e.spell_info.option_name.c_str());
  return false;
}
