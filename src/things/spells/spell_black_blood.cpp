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

static const std::string upgrade_1_increase_radius = "increase radius and range";

static auto tp_spell_black_blood_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return //
      UI_INFO1_FMT_STR
      "Poison the blood of all vulnerable enemies within target range. "  //
      "Each move they will lose health until the inevitable happens...\n" //
      "Does not have an impact on the ethereal or undead...\n"            //
      UI_INFO1_FMT_STR "Upgrades are as follows:\n"                       //
      UI_INFO1_FMT_STR "- Radius and range:\n"                            //
      UI_INFO2_FMT_STR "  One extra tile radius per upgrade.\n";          //
}

static bool tp_spell_black_blood_on_cast_request(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e)
{
  TRACE();

  return tp_spell_common_on_cast_request(g, v, l, spell, user, e);
}

static bool tp_spell_black_blood_on_cast_per_tile(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, const bpoint &p, ThingEvent &e)
{
  TRACE();

  auto The_user_thing = thing_name_long_The(g, v, l, user);

  FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, p)
  {
    if (thing_is_critical_to_dungeon_design(it)) {
      continue;
    }

    if (! thing_is_monst(it) && ! thing_is_player(it)) {
      continue;
    }

    if (thing_is_immune_to(g, v, l, it, THING_EVENT_POISON_DAMAGE)) {
      auto the_thing = thing_name_long_the(g, v, l, it);
      if (thing_is_player(user)) {
        if (user == it) {
          topcon(UI_GOOD_FMT_STR "You fail to poison yourself, as you are immune!" UI_RESET_FMT);
        } else {
          topcon(UI_WARN_FMT_STR "You fail to poison %s, it is immune!" UI_RESET_FMT, the_thing.c_str());
        }
      } else {
        if (user == it) {
          topcon("%s fails to poison itself!", The_user_thing.c_str());
        } else {
          topcon("%s fail to poison %s, it is immune!", The_user_thing.c_str(), the_thing.c_str());
        }
      }
      continue;
    }

    if (thing_hook_add(g, v, l, it, tp_find_mand("debuff_poison"))) {
      auto the_thing = thing_name_long_the(g, v, l, it);
      if (thing_is_player(user)) {
        if (user == it) {
          topcon(UI_GOOD_FMT_STR "You poison yourself!" UI_RESET_FMT);
        } else {
          topcon(UI_WARN_FMT_STR "You poison %s!" UI_RESET_FMT, the_thing.c_str());
        }
      } else {
        if (user == it) {
          topcon("%s poisons itself!", The_user_thing.c_str());
        } else {
          topcon("%s poisons %s!", The_user_thing.c_str(), the_thing.c_str());
        }
      }

      (void) thing_spawn(g, v, l, tp_first(is_effect_white_explosion), p);
    }
  }

  return true;
}

static bool tp_spell_black_blood_on_cast_do(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e)
{
  TRACE();

  return tp_spell_common_on_cast_do(g, v, l, spell, user, e, tp_spell_black_blood_on_cast_per_tile);
}

static auto tp_spell_black_blood_on_upgrade_possible(Gamep g, Levelsp v, Levelp l, Thingp me, TpSpellUpgrade u) -> bool
{
  TRACE();

  bool upgradeable = false;

  if (u.name == upgrade_1_increase_radius) {
    upgradeable = (thing_spell_radius(g, v, l, me) < thing_spell_radius_max(g, v, l, me));
    upgradeable |= (thing_spell_range(g, v, l, me) < thing_spell_range_max(g, v, l, me));
  }

  return upgradeable;
}

static auto tp_spell_black_blood_on_upgrade_do(Gamep g, Levelsp v, Levelp l, Thingp me, TpSpellUpgrade u) -> bool
{
  TRACE();

  if (u.name == upgrade_1_increase_radius) {
    THING_DBG(g, v, l, me, "upgrade radius");
    TRACE_INDENT();
    (void) thing_spell_radius_incr(g, v, l, me);
    (void) thing_spell_range_incr(g, v, l, me);
    return true;
  }

  return false;
}

[[nodiscard]] auto tp_load_spell_black_blood() -> bool
{
  TRACE();

  auto *tp   = tp_load("spell_black_blood"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_spell_black_blood_get);
  thing_on_cast_do_set(tp, tp_spell_black_blood_on_cast_do);
  thing_on_cast_request_set(tp, tp_spell_black_blood_on_cast_request);
  thing_on_upgrade_do_set(tp, tp_spell_black_blood_on_upgrade_do);
  thing_on_upgrade_possible_set(tp, tp_spell_black_blood_on_upgrade_possible);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_memory_ext);
  tp_flag_set(tp, is_spell);
  tp_name_long_set(tp, "black blood");
  tp_spell_cost_set(tp, 1);
  tp_spell_mana_cost_set(tp, 5);
  tp_spell_radius_max_set(tp, 6);
  tp_spell_radius_set(tp, 2);
  tp_spell_range_max_set(tp, 12);
  tp_spell_range_set(tp, 8);
  tp_stat_set(tp, THING_STAT_ARCANA_NECRO, "11");
  // end sort marker1 }

  tp_spell_upgrade_add(tp,
                       TpSpellUpgrade {
                           .type = "1", //
                           .name = upgrade_1_increase_radius,
                       });
  tp_spell_option_add(tp,
                      TpSpellOption {
                          .type = "1", //
                          .name = spell_option_targeted,
                      });
  tp_spell_option_add(tp,
                      TpSpellOption {
                          .type = "2", //
                          .name = spell_option_radial,
                      });
  tp_spell_option_add(tp,
                      TpSpellOption {
                          .type = "3", //
                          .name = spell_option_radial_excluding_player_tile,
                      });

  auto *tile = tile_find_mand("icon_" + name);
  tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);

  return true;
}
