//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_spoint.hpp"
#include "my_sprintf.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wid_text_box.hpp"

#include <cstring>
#include <string>

//
// The tp icon
//
[[nodiscard]] auto wid_tp_info_icon(Gamep g, Tpp me, WidPopup *parent) -> bool
{
  TRACE();

  auto *text = parent->wid_text_area;
  auto *b    = parent->wid_text_area->wid_text_area;

  if (tp_tiles_size(me, THING_ANIM_IDLE) == 0) {
    return false;
  }

  Tilep tile = tp_tiles_get(me, THING_ANIM_IDLE, 0);
  if (tile == nullptr) {
    return false;
  }

  auto        *w = wid_new_square_button(g, b, "Icon");
  spoint const tl(0, text->line_count);
  spoint const br(3, text->line_count + 2);
  wid_set_tile(TILE_LAYER_BOX_BG, w, tile);
  wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
  wid_set_pos(w, tl, br);

  return true;
}

//
// The tp name
//
[[nodiscard]] static auto wid_tp_info_name(Gamep g, Tpp me, WidPopup *parent) -> bool
{
  TRACE();

  std::string name_str;
  name_str = tp_name_long(me);
  name_str = capitalize_first(name_str);

  parent->log(g, UI_INFO_FMT_STR + name_str + UI_RESET_FMT);

  return true;
}

//
// Add damage types
//
[[nodiscard]] auto wid_tp_info_damage(Gamep g, Levelsp v, Levelp l, Tpp me, WidPopup *parent, int width, bool title_allowed) -> bool
{
  TRACE();

  bool printed_something = false;

  FOR_ALL_THING_EVENT(e)
  {
    auto damage_str = tp_damage_dice_roll_string(me, e);
    if (damage_str.empty()) {
      continue;
    }

    bool show_string = false;

    switch (e) {
      case THING_EVENT_SHOVED :           [[fallthrough]];
      case THING_EVENT_CRUSH_DAMAGE :     [[fallthrough]];
      case THING_EVENT_ENERGY_DAMAGE :    [[fallthrough]];
      case THING_EVENT_THROWN_DAMAGE :    [[fallthrough]];
      case THING_EVENT_MELEE_DAMAGE :     [[fallthrough]];
      case THING_EVENT_POISON_DAMAGE :    [[fallthrough]];
      case THING_EVENT_ENGULF_DAMAGE :    [[fallthrough]];
      case THING_EVENT_SPELL_DAMAGE :     [[fallthrough]];
      case THING_EVENT_EXPLOSION_DAMAGE : [[fallthrough]];
      case THING_EVENT_FIRE_DAMAGE :      [[fallthrough]];
      case THING_EVENT_WATER_DAMAGE : //
        show_string = true;
        break;
      case THING_EVENT_NONE :             [[fallthrough]];
      case THING_EVENT_GAME_OVER :        [[fallthrough]];
      case THING_EVENT_MELT :             [[fallthrough]];
      case THING_EVENT_OPEN :             [[fallthrough]];
      case THING_EVENT_LIFESPAN_EXPIRED : [[fallthrough]];
      case THING_EVENT_FALL :             [[fallthrough]];
      case THING_EVENT_CARRIED :          [[fallthrough]];
      case THING_EVENT_CARRIED_MERGED :   [[fallthrough]];
      case THING_EVENT_USER_INITIATED :   [[fallthrough]];
      case THING_EVENT_SPAWNED :          [[fallthrough]];
      case THING_EVENT_THROWN :           [[fallthrough]];
      case THING_EVENT_USED :             [[fallthrough]];
      case THING_EVENT_EATEN :            [[fallthrough]];
      case THING_EVENT_LEVITATED :        [[fallthrough]];
      case THING_EVENT_ENUM_MAX : //
        show_string = false;
        break;
    }

    if (! show_string) {
      continue;
    }

    if (title_allowed) {
      if (! printed_something) {
        parent->log(g, UI_INFO_FMT_STR "Attacks:", TEXT_FORMAT_LHS);
      }
    }

    auto space = (width - 4) / 2;

    if (title_allowed) {
      auto line = string_sprintf("- %-*s%*s",                                                  //
                                 space, capitalize_first(ThingEventType_to_string(e)).c_str(), //
                                 space, damage_str.c_str());
      parent->log(g, line, TEXT_FORMAT_RHS);
    } else {
      auto line = string_sprintf("%-*s%*s",                                                    //
                                 space, capitalize_first(ThingEventType_to_string(e)).c_str(), //
                                 space, damage_str.c_str());
      parent->log(g, line, TEXT_FORMAT_RHS);
    }
    printed_something = true;
  }

  return printed_something;
}

//
// Add special attacks
//
[[nodiscard]] auto wid_tp_info_special_attacks(Gamep g, Levelsp v, Levelp l, Tpp me, WidPopup *parent, int width, bool title_allowed) -> bool
{
  TRACE();

  bool printed_something = false;

  if (title_allowed) {
    if (! me->special_attacks.empty()) {
      parent->log(g, UI_INFO_FMT_STR "Special attacks:", TEXT_FORMAT_LHS);
    }
  }

  //
  // Check for things matching the dice roll first.
  //
  for (const auto &d : me->special_attacks) {
    auto val = d.second;

    auto damage_str = val.roll;

    auto space = (width - 4) / 2;
    auto line  = string_sprintf("- %-*s%*s",                               //
                                space, capitalize_first(val.name).c_str(), //
                                space, damage_str.c_str());
    parent->log(g, line, TEXT_FORMAT_LHS);

    //
    // If there is a weapon, get that damage
    //
    if (! val.what.empty()) {
      auto *weapon = tp_find_mand(val.what);
      if (weapon != nullptr) {
        FOR_ALL_THING_EVENT(e)
        {
          auto s = tp_damage_dice_roll_string(weapon, e);
          if (! s.empty()) {
            damage_str = s;
            (void) wid_tp_info_damage(g, v, l, weapon, parent, width - 2, false);
          }
        }
      }
    }

    printed_something = true;
  }

  return printed_something;
}

//
// Add spell options
//
[[nodiscard]] auto wid_tp_info_spell_options(Gamep g, Levelsp v, Levelp l, Tpp me, WidPopup *parent, int width, bool title_allowed) -> bool
{
  TRACE();

  bool printed_something = false;

  if (title_allowed) {
    if (! me->spell_options.empty()) {
      parent->log(g, UI_INFO_FMT_STR "Spell options:", TEXT_FORMAT_LHS);
    }
  }

  //
  // Check for things matching the dice roll first.
  //
  for (const auto &d : me->spell_options) {
    auto val = d.second;

    auto space = (width - 4) / 2;
    auto line  = string_sprintf("- %-*s", space, capitalize_first(val.name).c_str());
    parent->log(g, UI_INFO2_FMT_STR + line, TEXT_FORMAT_LHS);

    printed_something = true;
  }

  return printed_something;
}

//
// Add spell upgrades
//
[[nodiscard]] auto wid_tp_info_spell_upgrades(Gamep g, Levelsp v, Levelp l, Tpp me, WidPopup *parent, int width, bool title_allowed) -> bool
{
  TRACE();

  bool printed_something = false;

  if (title_allowed) {
    if (! me->spell_upgrades.empty()) {
      parent->log(g, UI_INFO_FMT_STR "Spell upgrades:", TEXT_FORMAT_LHS);
    }
  }

  //
  // Check for things matching the dice roll first.
  //
  for (const auto &d : me->spell_upgrades) {
    auto val = d.second;

    auto space = (width - 4) / 2;
    auto line  = string_sprintf("- %-*s", space, capitalize_first(val.name).c_str());
    parent->log(g, UI_INFO2_FMT_STR + line, TEXT_FORMAT_LHS);

    printed_something = true;
  }

  return printed_something;
}

//
// Add spell cost
//
[[nodiscard]] auto wid_tp_info_spell_cost(Gamep g, Levelsp v, Levelp l, Tpp me, WidPopup *parent, int /*width*/, bool /*title_allowed*/) -> bool
{
  TRACE();

  auto cost_str = string_sprintf(UI_INFO2_FMT_STR "Mana cost " UI_INFO1_FMT_STR "%u", tp_spell_cost_get(me));

  parent->log(g, cost_str, TEXT_FORMAT_LHS);

  return true;
}

//
// Add immunities
//
[[nodiscard]] static auto wid_tp_info_immunity(Gamep g, Tpp me, WidPopup *parent) -> bool
{
  TRACE();

  std::string out;

  FOR_ALL_THING_EVENT(e)
  {
    if (! tp_is_immune_to(me, e)) {
      continue;
    }

    bool show_string = false;

    switch (e) {
      case THING_EVENT_SHOVED :           [[fallthrough]];
      case THING_EVENT_CRUSH_DAMAGE :     [[fallthrough]];
      case THING_EVENT_ENERGY_DAMAGE :    [[fallthrough]];
      case THING_EVENT_THROWN_DAMAGE :    [[fallthrough]];
      case THING_EVENT_MELEE_DAMAGE :     [[fallthrough]];
      case THING_EVENT_POISON_DAMAGE :    [[fallthrough]];
      case THING_EVENT_ENGULF_DAMAGE :    [[fallthrough]];
      case THING_EVENT_EXPLOSION_DAMAGE : [[fallthrough]];
      case THING_EVENT_SPELL_DAMAGE :     [[fallthrough]];
      case THING_EVENT_FIRE_DAMAGE :      [[fallthrough]];
      case THING_EVENT_WATER_DAMAGE : //
        show_string = true;
        break;
      case THING_EVENT_NONE :             [[fallthrough]];
      case THING_EVENT_GAME_OVER :        [[fallthrough]];
      case THING_EVENT_MELT :             [[fallthrough]];
      case THING_EVENT_OPEN :             [[fallthrough]];
      case THING_EVENT_LIFESPAN_EXPIRED : [[fallthrough]];
      case THING_EVENT_FALL :             [[fallthrough]];
      case THING_EVENT_CARRIED :          [[fallthrough]];
      case THING_EVENT_CARRIED_MERGED :   [[fallthrough]];
      case THING_EVENT_USER_INITIATED :   [[fallthrough]];
      case THING_EVENT_SPAWNED :          [[fallthrough]];
      case THING_EVENT_THROWN :           [[fallthrough]];
      case THING_EVENT_USED :             [[fallthrough]];
      case THING_EVENT_EATEN :            [[fallthrough]];
      case THING_EVENT_LEVITATED :        [[fallthrough]];
      case THING_EVENT_ENUM_MAX : //
        show_string = false;
        break;
    }

    if (! show_string) {
      continue;
    }

    out = string_append_with_comma(out, capitalize_first(ThingEventType_to_string(e)));
  }

  if (out.empty()) {
    return false;
  }

  parent->log(g, UI_INFO_FMT_STR "Immunity:", TEXT_FORMAT_LHS);
  parent->log(g, "- " + out, TEXT_FORMAT_LHS);

  return true;
}

//
// Add resistances
//
[[nodiscard]] static auto wid_tp_info_resistance(Gamep g, Tpp me, WidPopup *parent) -> bool
{
  TRACE();

  std::string out;

  FOR_ALL_THING_EVENT(e)
  {
    if (! tp_is_resistant_to(me, e)) {
      continue;
    }

    bool show_string = false;

    switch (e) {
      case THING_EVENT_SHOVED :           [[fallthrough]];
      case THING_EVENT_CRUSH_DAMAGE :     [[fallthrough]];
      case THING_EVENT_ENERGY_DAMAGE :    [[fallthrough]];
      case THING_EVENT_MELEE_DAMAGE :     [[fallthrough]];
      case THING_EVENT_POISON_DAMAGE :    [[fallthrough]];
      case THING_EVENT_ENGULF_DAMAGE :    [[fallthrough]];
      case THING_EVENT_THROWN_DAMAGE :    [[fallthrough]];
      case THING_EVENT_SPELL_DAMAGE :     [[fallthrough]];
      case THING_EVENT_EXPLOSION_DAMAGE : [[fallthrough]];
      case THING_EVENT_FIRE_DAMAGE :      [[fallthrough]];
      case THING_EVENT_WATER_DAMAGE : //
        show_string = true;
        break;
      case THING_EVENT_NONE :             [[fallthrough]];
      case THING_EVENT_GAME_OVER :        [[fallthrough]];
      case THING_EVENT_MELT :             [[fallthrough]];
      case THING_EVENT_OPEN :             [[fallthrough]];
      case THING_EVENT_LIFESPAN_EXPIRED : [[fallthrough]];
      case THING_EVENT_FALL :             [[fallthrough]];
      case THING_EVENT_CARRIED :          [[fallthrough]];
      case THING_EVENT_CARRIED_MERGED :   [[fallthrough]];
      case THING_EVENT_USER_INITIATED :   [[fallthrough]];
      case THING_EVENT_SPAWNED :          [[fallthrough]];
      case THING_EVENT_THROWN :           [[fallthrough]];
      case THING_EVENT_USED :             [[fallthrough]];
      case THING_EVENT_EATEN :            [[fallthrough]];
      case THING_EVENT_LEVITATED :        [[fallthrough]];
      case THING_EVENT_ENUM_MAX : //
        show_string = false;
        break;
    }

    if (! show_string) {
      continue;
    }

    out = string_append_with_comma(out, capitalize_first(ThingEventType_to_string(e)));
  }

  if (out.empty()) {
    return false;
  }

  parent->log(g, UI_INFO_FMT_STR "Resistances (half damage):", TEXT_FORMAT_LHS);
  parent->log(g, "- " + out, TEXT_FORMAT_LHS);

  return true;
}

//
// Display detailed tp information
//
void wid_tp_info(Gamep g, Levelsp v, Levelp l, Tpp me, WidPopup *parent, int width)
{
  ////////////////////////////////////////////////////////////////////////////////
  // Careful here. If we invoked the random number generator in here it can throw
  // off tests.
  ////////////////////////////////////////////////////////////////////////////////

  TRACE();

  if (me == nullptr) {
    return;
  }

  if (wid_tp_info_icon(g, me, parent)) {
    parent->log_empty_line(g);
  }

  if (wid_tp_info_name(g, me, parent)) {
    parent->log_empty_line(g);
  }

  if (wid_tp_info_damage(g, v, l, me, parent, width, true /* title allowed */)) {
    parent->log_empty_line(g);
  }

  if (wid_tp_info_special_attacks(g, v, l, me, parent, width, true /* title allowed */)) {
    parent->log_empty_line(g);
  }

  if (wid_tp_info_spell_options(g, v, l, me, parent, width, true /* title allowed */)) {
    parent->log_empty_line(g);
  }

  if (wid_tp_info_spell_upgrades(g, v, l, me, parent, width, true /* title allowed */)) {
    parent->log_empty_line(g);
  }

  if (wid_tp_info_spell_cost(g, v, l, me, parent, width, true /* title allowed */)) {
    parent->log_empty_line(g);
  }

  if (wid_tp_info_immunity(g, me, parent)) {
    parent->log_empty_line(g);
  }

  if (wid_tp_info_resistance(g, me, parent)) {
    parent->log_empty_line(g);
  }
}
