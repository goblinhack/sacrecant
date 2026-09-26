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

static const std::string upgrade_1_increase_radius             = "increase radius and range";
static const std::string upgrade_2_increase_damage             = "increase damage";
static const std::string option_1_targeted                     = "targeted";
static const std::string option_2_radial_including_player_tile = "radial, including your tile";
static const std::string option_3_radial_excluding_player_tile = "radial, excluding your tile";

static auto tp_spell_hellish_onslaught_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  auto        tp_damage  = tp_find_mand("explosion");
  auto        space      = (UI_RIGHTBAR_WIDTH - 4) / 2;
  auto        damage_str = tp_damage_dice_roll_string(tp_damage, THING_EVENT_EXPLOSION_DAMAGE);
  auto        damage_mod = thing_stat_mod(g, v, l, me, THING_STAT_DMG);
  std::string line;

  if (damage_mod) {
    auto damage_total_str = string_sprintf("%s (+%d)", damage_str.c_str(), damage_mod);
    line                  = string_sprintf("- %-*s%*s\n",            //
                                           space, "Damage per tile", //
                                           space, damage_total_str.c_str());
  } else {
    line = string_sprintf("- %-*s%*s\n",            //
                          space, "Damage per tile", //
                          space, damage_str.c_str());
  }

  return                                                                                                    //
      UI_INFO1_FMT_STR "Rain hell fire from above, targeted at your enemies or radially around yourself.\n" //
      UI_INFO2_FMT_STR "For extra fun, you can even target yourself at no added cost!\n"                    //
      UI_INFO1_FMT_STR "Upgrades are as follows:\n"                                                         //
      UI_INFO1_FMT_STR "- Radius and range:\n"                                                              //
      UI_INFO2_FMT_STR "  One extra tile radius per upgrade.\n"                                             //
      UI_INFO1_FMT_STR "- Damage upgrade:\n"                                                                //
      UI_INFO2_FMT_STR "  One extra health point of damage for each explosion per upgrade.\n"
      + line; //
}

static bool tp_spell_hellish_onslaught_on_cast_request(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e)
{
  TRACE();

  THING_DBG(g, v, l, user, "cast request");
  TRACE_INDENT();
  THING_DBG(g, v, l, spell, "this");

  if (e.spell_info.option_name.empty() || (e.spell_info.option_name == option_1_targeted)) {
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

  if (e.spell_info.option_name == option_2_radial_including_player_tile) {
    THING_DBG(g, v, l, spell, "ok");
    return true;
  }

  if (e.spell_info.option_name == option_3_radial_excluding_player_tile) {
    THING_DBG(g, v, l, spell, "ok");
    return true;
  }

  thing_err(g, v, l, spell, "unknown spell option: %s", e.spell_info.option_name.c_str());
  return false;
}

static void tp_spell_hellish_onslaught_spawn_explosion(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, bpoint p, ThingEvent &e)
{
  TRACE();

  if (level_is_obs_to_explosion(g, v, l, p) == nullptr) {
    if (! level_is_explosion_bool(g, v, l, p)) {
      auto explosion = thing_spawn(g, v, l, tp_first(is_explosion), p, &e);
      if (explosion) {
        auto spell_stat = thing_stat(g, v, l, spell, THING_STAT_DMG);
        (void) thing_stat_set(g, v, l, explosion, THING_STAT_DMG, spell_stat);
      }
    }
  }
}

static bool tp_spell_hellish_onslaught_on_cast_do(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e)
{
  TRACE();

  THING_DBG(g, v, l, user, "cast do");
  TRACE_INDENT();
  THING_DBG(g, v, l, spell, "this");

  auto user_at      = thing_at(g, v, l, user);
  auto spell_radius = thing_spell_radius(g, v, l, spell);
  auto spell_range  = thing_spell_range(g, v, l, spell);

  if (e.spell_info.option_name.empty() || (e.spell_info.option_name == option_1_targeted)) {
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
        topcon("That tile of range for spell '%s'.", capitalize(thing_name_short(g, v, l, spell)).c_str());
        (void) sound_play(g, "error");
      }
      return false;
    }

    for (auto dx = -spell_radius; dx <= spell_radius; dx++) {
      for (auto dy = -spell_radius; dy <= spell_radius; dy++) {
        bpoint p(target.x + dx, target.y + dy);
        if (! is_oob(p)) {
          if (distance(p, target) <= spell_radius) {
            tp_spell_hellish_onslaught_spawn_explosion(g, v, l, spell, user, p, e);
          }
        }
      }
    }

    thing_sound_play(g, v, l, user, "spell");
    THING_DBG(g, v, l, spell, "done");
    return true;
  }

  if (e.spell_info.option_name == option_2_radial_including_player_tile) {
    auto target = thing_at(g, v, l, user);
    for (auto dx = -spell_radius; dx <= spell_radius; dx++) {
      for (auto dy = -spell_radius; dy <= spell_radius; dy++) {
        bpoint p(target.x + dx, target.y + dy);
        if (! is_oob(p)) {
          if (distance(p, target) <= spell_radius) {
            tp_spell_hellish_onslaught_spawn_explosion(g, v, l, spell, user, p, e);
          }
        }
      }
    }
    thing_sound_play(g, v, l, user, "spell");
    THING_DBG(g, v, l, spell, "done");
    return true;
  }

  if (e.spell_info.option_name == option_3_radial_excluding_player_tile) {
    auto target = thing_at(g, v, l, user);
    for (auto dx = -spell_radius; dx <= spell_radius; dx++) {
      for (auto dy = -spell_radius; dy <= spell_radius; dy++) {
        bpoint p(target.x + dx, target.y + dy);
        if (! is_oob(p)) {
          if (p != target) {
            if (distance(p, target) <= spell_radius) {
              tp_spell_hellish_onslaught_spawn_explosion(g, v, l, spell, user, p, e);
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

static auto tp_spell_hellish_onslaught_on_upgrade_possible(Gamep g, Levelsp v, Levelp l, Thingp me, TpSpellUpgrade u) -> bool
{
  TRACE();

  bool upgradeable = false;

  if (u.name == upgrade_1_increase_radius) {
    upgradeable = (thing_spell_radius(g, v, l, me) < thing_spell_radius_max(g, v, l, me));
    upgradeable |= (thing_spell_range(g, v, l, me) < thing_spell_range_max(g, v, l, me));
  }

  if (u.name == upgrade_2_increase_damage) {
    upgradeable = true;
  }

  return upgradeable;
}

static auto tp_spell_hellish_onslaught_on_upgrade_do(Gamep g, Levelsp v, Levelp l, Thingp me, TpSpellUpgrade u) -> bool
{
  TRACE();

  if (u.name == upgrade_1_increase_radius) {
    THING_DBG(g, v, l, me, "upgrade radius");
    TRACE_INDENT();
    (void) thing_spell_radius_incr(g, v, l, me);
    (void) thing_spell_range_incr(g, v, l, me);
    return true;
  }

  if (u.name == upgrade_2_increase_damage) {
    THING_DBG(g, v, l, me, "upgrade damage");
    TRACE_INDENT();
    auto spell_stat = thing_stat(g, v, l, me, THING_STAT_DMG);
    (void) thing_stat_set(g, v, l, me, THING_STAT_DMG, spell_stat + 1);
    return true;
  }

  return false;
}

[[nodiscard]] auto tp_load_spell_hellish_onslaught() -> bool
{
  TRACE();

  auto *tp   = tp_load("spell_hellish_onslaught"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_spell_hellish_onslaught_get);
  thing_on_cast_do_set(tp, tp_spell_hellish_onslaught_on_cast_do);
  thing_on_cast_request_set(tp, tp_spell_hellish_onslaught_on_cast_request);
  thing_on_upgrade_do_set(tp, tp_spell_hellish_onslaught_on_upgrade_do);
  thing_on_upgrade_possible_set(tp, tp_spell_hellish_onslaught_on_upgrade_possible);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_spell);
  tp_name_long_set(tp, "hellish onslaught");
  tp_spell_cost_set(tp, 2);
  tp_spell_mana_cost_set(tp, 10);
  tp_spell_radius_max_set(tp, 6);
  tp_spell_radius_set(tp, 2);
  tp_spell_range_max_set(tp, 12);
  tp_spell_range_set(tp, 8);
  tp_stat_set(tp, THING_STAT_ARCANA_PYRO, "11");
  // end sort marker1 }

  tp_spell_upgrade_add(tp,
                       TpSpellUpgrade {
                           .type = "1", //
                           .name = upgrade_1_increase_radius,
                       });
  tp_spell_upgrade_add(tp,
                       TpSpellUpgrade {
                           .type = "2", //
                           .name = upgrade_2_increase_damage,
                       });
  tp_spell_option_add(tp,
                      TpSpellOption {
                          .type = "1", //
                          .name = option_1_targeted,
                      });
  tp_spell_option_add(tp,
                      TpSpellOption {
                          .type = "2", //
                          .name = option_2_radial_including_player_tile,
                      });
  tp_spell_option_add(tp,
                      TpSpellOption {
                          .type = "3", //
                          .name = option_3_radial_excluding_player_tile,
                      });

  auto *tile = tile_find_mand("icon_" + name);
  tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);

  return true;
}
