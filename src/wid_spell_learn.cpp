//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_game_defs.hpp"
#include "my_game_inlines.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_sprintf.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_text_box.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <vector>

static Widp          wid_total;
static Widp          wid_spell_learn_window;
static WidPopup     *wid_spell_learn_list;
static WidPopup     *wid_spell_learn_learn_window;
static WidPopup     *wid_over_stats;
static ThingStatType wid_spell_filter = THING_STAT_NONE;

static std::map< Thingp, std::set< std::string > > wid_spell_upgrades;

static Widp wid_spell[ TP_ID_MAX ];
static Widp wid_upgrade[ TP_ID_MAX ];

static void wid_spell_learn_destroy(Gamep g)
{
  TRACE();

  memset(wid_spell, 0, sizeof(wid_spell));
  memset(wid_upgrade, 0, sizeof(wid_upgrade));

  game_cand_spell_unset(g, nullptr);

  delete wid_spell_learn_list;
  wid_spell_learn_list = nullptr;

  delete wid_spell_learn_learn_window;
  wid_spell_learn_learn_window = nullptr;

  delete wid_over_stats;
  wid_over_stats = nullptr;

  if (wid_spell_learn_window != nullptr) {
    wid_destroy(g, &wid_spell_learn_window);
  }

  wid_total          = nullptr;
  wid_spell_upgrades = {};

  game_state_reset(g, "close spell window");
}

static auto wid_spell_upgrade_find(Thingp spell, const std::string &upgrade) -> bool
{
  TRACE();

  for (const auto &i : wid_spell_upgrades) {
    if (i.first == spell) {
      return i.second.contains(upgrade);
    }
  }

  return false;
}

static void wid_spell_upgrade_add(Thingp spell, const std::string &upgrade)
{
  TRACE();

  wid_spell_upgrades[ spell ].insert(upgrade);
}

static void wid_spell_upgrade_remove(Thingp spell, const std::string &upgrade)
{
  TRACE();

  wid_spell_upgrades[ spell ].erase(upgrade);
}

static void wid_spell_checkout(Gamep g)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) {
    return;
  }

  game_chosen_spell_set(g, game_cand_spell_get(g));

  //
  // Add the chosen spell(s)
  //
  for (auto *spell_tp : game_chosen_spells_get(g)) {
    auto *spell = thing_spawn(g, v, l, spell_tp, thing_at(g, v, l, player));
    if (spell != nullptr) {
      auto cost = thing_spell_cost_for(g, v, l, spell, player);
      auto name = thing_name_long(g, v, l, spell);

      //
      // Only decrement the sac points if the spell was added successfully.
      //
      if (thing_spellbook_add(g, v, l, spell, player)) {
        //
        // Successfully added
        //
        (void) thing_sac_points_decr(g, v, l, player, cost);

        if (cost == 1) {
          topcon(UI_INFO_FMT_STR "You spent %d SP on spell %s." UI_RESET_FMT, cost, name.c_str());
        } else {
          topcon(UI_INFO_FMT_STR "You spent %d SPs on spell %s." UI_RESET_FMT, cost, name.c_str());
        }
        (void) level_tick_begin_requested(g, v, l, "player learned a spell");
      } else {
        //
        // Too many spells?
        //
        topcon(UI_IMPORTANT_FMT_STR
               "You fail to learn %s and add it to your spellbook. You have not been charged for this transaction!" UI_RESET_FMT,
               name.c_str());
      }
    } else {
      //
      // Odd
      //
      topcon(UI_WARN_FMT_STR "You fail to learn %s." UI_RESET_FMT, tp_name(spell_tp).c_str());
    }
  }

  //
  // Upgrade any spells
  //
  for (const auto &i : wid_spell_upgrades) {
    Thingp spell = i.first;
    auto   cost  = thing_spell_cost_for(g, v, l, spell, player);
    auto   name  = thing_name_long(g, v, l, spell);

    for (const auto &u_name : i.second) {
      FOR_ALL_SPELLBOOK_SPELLS(g, v, l, player, learned_spell)
      {
        if (thing_tp(learned_spell) == thing_tp(spell)) {
          for (const auto &upgrade : tp_spell_upgrades_get(thing_tp(learned_spell))) {
            TpSpellUpgrade const u = upgrade.second;
            if (u.name == u_name) {
              if (thing_on_upgrade_do(g, v, l, learned_spell, u)) {
                topcon(UI_INFO_FMT_STR "You spent %d SP on upgrade '%s' for spell %s." UI_RESET_FMT, cost, u_name.c_str(), name.c_str());
              } else {
                topcon(UI_INFO_FMT_STR "You spent %d SPs on upgrade '%s' for spell %s." UI_RESET_FMT, cost, u_name.c_str(), name.c_str());
              }
            }
          }
        }
      }
    }
  }

  wid_spell_learn_destroy(g);

  game_spell_clear(g);
}

[[nodiscard]] static auto wid_learn_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  wid_spell_checkout(g);

  return true;
}

static void wid_spell_learn_check_if_done(Gamep g)
{
  TRACE();

  if (game_cand_spell_get(g).empty() && wid_spell_upgrades.empty()) {
    delete wid_spell_learn_learn_window;
    wid_spell_learn_learn_window = nullptr;
    return;
  }

  if (wid_spell_learn_learn_window == nullptr) {
    auto         m = TERM_WIDTH / 2;
    auto         n = TERM_HEIGHT - 4;
    spoint const outer_tl(m - (UI_WID_POPUP_WIDTH_NORMAL / 2), n - 3);
    spoint const outer_br(m + (UI_WID_POPUP_WIDTH_NORMAL / 2), n + 1);
    auto         width = outer_br.x - outer_tl.x;

    wid_spell_learn_learn_window = new WidPopup(g, "learn", outer_tl, outer_br, nullptr, "", false, false);
    auto *p                      = wid_spell_learn_learn_window->wid_text_area->wid_text_area;
    auto *w                      = wid_new_continue_button(g, p, "learn");

    spoint const tl(0, 0);
    spoint const br(width - 2, 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, "Learn?");
    wid_set_on_mouse_down(w, wid_learn_mouse_down);
  }
}

static auto wid_player_spent_points(Gamep g, Levelsp v, Levelp l, Thingp player) -> int
{
  TRACE();

  int spent {};

  for (auto &w : wid_spell) {
    if (w == nullptr) {
      continue;
    }

    auto *spell = wid_get_thing_context(g, v, w, 0);
    if (spell == nullptr) {
      continue;
    }

    if (game_cand_spell_find(g, spell)) {
      spent += thing_spell_cost_for(g, v, l, spell, player);
    }
  }

  for (auto &w : wid_upgrade) {
    if (w == nullptr) {
      continue;
    }

    auto *spell = wid_get_thing_context(g, v, w, 0);
    if (spell == nullptr) {
      continue;
    }

    auto upgrade = wid_get_string_context(w);
    if (wid_spell_upgrade_find(spell, upgrade)) {
      spent += thing_spell_cost_for(g, v, l, spell, player);
    }
  }

  return spent;
}

static auto wid_player_avail_points(Gamep g, Levelsp v, Levelp l, Thingp player) -> int
{
  TRACE();

  auto spent = wid_player_spent_points(g, v, l, player);
  auto avail = thing_sac_points(g, v, l, player) - spent;

  return avail;
}

static void wid_player_update_spending(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  TRACE();

  auto spent = wid_player_spent_points(g, v, l, player);
  auto avail = wid_player_avail_points(g, v, l, player);

  auto a    = string_sprintf("Sacrificial points (SPs)");
  auto b    = string_sprintf("Spent:%d", spent);
  auto c    = string_sprintf("Avail:%d", avail);
  auto line = string_sprintf("%-30s%10s%10s", a.c_str(), b.c_str(), c.c_str());

  wid_set_text_lhs(wid_total, 1u);
  wid_set_text(wid_total, line);
  wid_update(g, wid_total);
}

static void wid_player_update_spell_selections(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  TRACE();

  auto avail = wid_player_avail_points(g, v, l, player);
  Widp w     = nullptr;

  wid_unset_focus(g);
  wid_mouse_over_end(g);

  for (auto &n : wid_spell) {
    w = n;
    if (w == nullptr) {
      continue;
    }

    auto *spell = wid_get_thing_context(g, v, w, 0);
    if (spell == nullptr) {
      continue;
    }

    auto        index      = wid_get_int_context(w);
    auto        spell_cost = thing_spell_cost_for(g, v, l, spell, player);
    auto       *tp         = thing_tp(spell);
    std::string s;

    if (spell_cost <= avail) {
      s += "%%fg=gray90$";
    } else {
      s += "%%fg=gray50$";
    }

    if (index >= 26) {
      s += static_cast< char >('A' + index - 26);
    } else {
      s += static_cast< char >('a' + index);
    }

    s += ") ";

    s += capitalize_first(tp_name_long(tp));
    s = string_sprintf("%-51s", s.c_str());
    s += "%%fg=reset$";

    switch (thing_spell_arcana(g, v, l, spell)) {
      case THING_STAT_ARCANA_FIRE :  s += "%%fg=orange$Fire%%fg=reset$    "; break;
      case THING_STAT_ARCANA_DEATH : s += "%%fg=gray50$Death%%fg=reset$   "; break;
      case THING_STAT_ARCANA_LIFE :  s += "%%fg=green$Life%%fg=reset$    "; break;
      default :                      s += "-    "; break;
    }

    s += string_sprintf("%2d", spell_cost);

    wid_set_text(w, s);
    wid_apply_bar_button(g, w);

    if (game_cand_spell_find(g, spell)) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, RED);
      wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, RED);
    }
  }

  for (auto &n : wid_upgrade) {
    w = n;
    if (w == nullptr) {
      continue;
    }

    auto *spell = wid_get_thing_context(g, v, w, 0);
    if (spell == nullptr) {
      continue;
    }

    auto        upgrade    = wid_get_string_context(w);
    auto        spell_cost = thing_spell_cost_for(g, v, l, spell, player);
    std::string s;

    if (spell_cost <= avail) {
      s += "%%fg=gray90$";
    } else {
      s += "%%fg=gray50$";
    }

    s += " + Upgrade: ";
    s += capitalize_first(upgrade);

    wid_set_text(w, s);
    wid_apply_bar_button(g, w);

    if (wid_spell_upgrade_find(spell, upgrade)) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, RED);
      wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, RED);
    }
  }

  wid_player_update_spending(g, v, l, player);
}

static void wid_spell_learn_spell_via_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *spell = wid_get_thing_context(g, v, w, 0);
  if (spell == nullptr) {
    return;
  }

  game_spell_mouse_over_currently_set(g, spell);

  level_cursor_describe_clear(g, v);

  if (level_cursor_describe_add(g, v, spell)) {
    game_request_to_remake_ui_set(g);
  }
}

static void wid_spell_learn_spell_via_mouse_over_end(Gamep g, Widp w)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *spell = wid_get_thing_context(g, v, w, 0);
  if (spell == nullptr) {
    return;
  }

  game_spell_mouse_over_currently_set(g, nullptr);

  if (level_cursor_describe_remove(g, v, spell)) {
    game_request_to_remake_ui_set(g);
  }
}

[[nodiscard]] static auto wid_spell_learn_spell_via_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return 0;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return 0;
  }

  auto *player = thing_player(g);
  if (player == nullptr) {
    return 0;
  }

  auto *spell = wid_get_thing_context(g, v, w, 0);
  if (spell == nullptr) {
    return false;
  }

  auto cost  = thing_spell_cost_for(g, v, l, spell, player);
  auto avail = wid_player_avail_points(g, v, l, player);

  if (thing_spellbook_is_learned_spell(g, v, l, spell, player)) {
    topcon("You already know this spell. Choose an upgrade.\n");
    (void) sound_play(g, "error");
  } else if (game_cand_spell_find(g, spell)) {
    game_cand_spell_unset(g, spell);
    (void) sound_play(g, "select");
  } else if (cost <= avail) {
    game_cand_spell_set(g, spell);
    (void) sound_play(g, "select");
  } else {
    topcon("You do not have enough SPs to learn that spell.\n");
    (void) sound_play(g, "error");
  }

  wid_spell_learn_check_if_done(g);
  wid_player_update_spell_selections(g, v, l, player);
  game_request_to_remake_ui_set(g);

  return true;
}

[[nodiscard]] static auto wid_spell_upgrade_spell_via_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return 0;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return 0;
  }

  auto *player = thing_player(g);
  if (player == nullptr) {
    return 0;
  }

  auto *spell = wid_get_thing_context(g, v, w, 0);
  if (spell == nullptr) {
    return false;
  }

  auto cost  = thing_spell_cost_for(g, v, l, spell, player);
  auto avail = wid_player_avail_points(g, v, l, player);

  auto upgrade = wid_get_string_context(w);
  if (upgrade.empty()) {
    return false;
  }

  if (wid_spell_upgrade_find(spell, upgrade)) {
    wid_spell_upgrade_remove(spell, upgrade);
    (void) sound_play(g, "select");
  } else if (cost <= avail) {
    wid_spell_upgrade_add(spell, upgrade);
    (void) sound_play(g, "select");
  } else {
    topcon("You do not have enough SPs to upgrade that spell.\n");
    (void) sound_play(g, "error");
  }

  wid_spell_learn_check_if_done(g);
  wid_player_update_spell_selections(g, v, l, player);
  game_request_to_remake_ui_set(g);

  return true;
}

[[nodiscard]] static auto wid_spell_learn_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    (void) sound_play(g, "keypress");
    return false;
  }

  switch (key->mod) {
    case KMOD_LCTRL :
    case KMOD_RCTRL :
    default :
      switch (key->sym) {
        default :
          {
            TRACE();
            auto c = wid_event_to_char(key);
            switch (c) {
              case ' ' :
                if (wid_spell_learn_learn_window != nullptr) {
                  //
                  // All done
                  //
                  wid_spell_checkout(g);
                }
                return true;

              case 'a' :
              case 'b' :
              case 'c' :
              case 'd' :
              case 'e' :
              case 'f' :
              case 'g' :
              case 'h' :
              case 'i' :
              case 'j' :
              case 'k' :
              case 'l' :
              case 'm' :
              case 'n' :
              case 'o' :
              case 'p' :
              case 'q' :
              case 'r' :
              case 's' :
              case 't' :
              case 'u' :
              case 'v' :
              case 'w' :
              case 'x' :
              case 'y' :
              case 'z' :
                game_spell_mouse_over_currently_set(g, nullptr);
                w = wid_spell[ c - 'a' ];
                if (w != nullptr) {
                  (void) wid_spell_learn_spell_via_mouse_down(g, w, -1, -1, 0);
                }
                break;
              case 'A' :
              case 'B' :
              case 'C' :
              case 'D' :
              case 'E' :
              case 'F' :
              case 'G' :
              case 'H' :
              case 'I' :
              case 'J' :
              case 'K' :
              case 'L' :
              case 'M' :
              case 'N' :
              case 'O' :
              case 'P' :
              case 'Q' :
              case 'R' :
              case 'S' :
              case 'T' :
              case 'U' :
              case 'V' :
              case 'W' :
              case 'X' :
              case 'Y' :
              case 'Z' :
                game_spell_mouse_over_currently_set(g, nullptr);
                w = wid_spell[ c - 'A' + 26 ];
                if (w != nullptr) {
                  (void) wid_spell_learn_spell_via_mouse_down(g, w, -1, -1, 0);
                }
                break;
              case SDLK_ESCAPE :
                {
                  TRACE();
                  (void) sound_play(g, "keypress");
                  wid_spell_learn_destroy(g);
                  return true;
                }
            }
          }
      }
  }

  //
  // So screenshots can work
  //
  return false;
}

[[nodiscard]] static auto wid_spell_learn_stats_arcana_fire_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return 0;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return 0;
  }

  auto *player = thing_player(g);
  if (player == nullptr) {
    return 0;
  }

  if (wid_spell_filter == THING_STAT_ARCANA_FIRE) {
    wid_spell_learn(g, v, l, player, THING_STAT_NONE);
  } else {
    wid_spell_learn(g, v, l, player, THING_STAT_ARCANA_FIRE);
  }

  return true;
}

[[nodiscard]] static auto wid_spell_learn_stats_arcana_life_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return 0;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return 0;
  }

  auto *player = thing_player(g);
  if (player == nullptr) {
    return 0;
  }

  if (wid_spell_filter == THING_STAT_ARCANA_LIFE) {
    wid_spell_learn(g, v, l, player, THING_STAT_NONE);
  } else {
    wid_spell_learn(g, v, l, player, THING_STAT_ARCANA_LIFE);
  }

  return true;
}

[[nodiscard]] static auto wid_spell_learn_stats_arcana_death_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return 0;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) {
    return 0;
  }

  auto *player = thing_player(g);
  if (player == nullptr) {
    return 0;
  }

  if (wid_spell_filter == THING_STAT_ARCANA_DEATH) {
    wid_spell_learn(g, v, l, player, THING_STAT_NONE);
  } else {
    wid_spell_learn(g, v, l, player, THING_STAT_ARCANA_DEATH);
  }

  return true;
}

static void wid_spell_learn_stats_arcana_common_mouse_over_begin(Gamep g)
{
  TRACE();
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "Modifiers can impact costs and mana drain.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "SP is used when buying a spell.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "Mana is used when casting a spell.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "Cost modifier table:\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR " -9 mod increases SP by 3.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR " -6 mod increases SP by 2.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR " -5 mod increases mana by 50%%%.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR " -3 mod increases SP by 1.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR " +3 mod decreases SP by 1.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR " +5 mod decreases mana by 50%%%.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR " +6 mod decreases SP by 2.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR " +9 mod decreases SP by 3.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "Select this option to filter spells to this arcana only.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
}

static void wid_spell_learn_stats_arcana_fire_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int const tlx = TERM_WIDTH - UI_RIGHTBAR_WIDTH - 2;
  int const brx = tlx + UI_RIGHTBAR_WIDTH;
  int const tly = UI_TOPCON_HEIGHT + 10;
  int const bry = tly + 40;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Fire Arcana");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "With the Fire Arcana, you specialize in all things flaming hot, fireballs, scorched earth etc...\n",
                      TEXT_FORMAT_LHS);
  wid_spell_learn_stats_arcana_common_mouse_over_begin(g);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_spell_learn_stats_arcana_life_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int const tlx = TERM_WIDTH - UI_RIGHTBAR_WIDTH - 2;
  int const brx = tlx + UI_RIGHTBAR_WIDTH;
  int const tly = UI_TOPCON_HEIGHT + 10;
  int const bry = tly + 40;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Life Arcana");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g,
                      UI_INFO2_FMT_STR "With the Life Arcana, you specialize in all things living, plant summoning, healing of allies etc...\n",
                      TEXT_FORMAT_LHS);
  wid_spell_learn_stats_arcana_common_mouse_over_begin(g);
  wid_over_stats->log(g, UI_IMPORTANT_FMT_STR "Specializing in Death will make Life spells more costly.\n", TEXT_FORMAT_LHS);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_spell_learn_stats_arcana_death_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int const tlx = TERM_WIDTH - UI_RIGHTBAR_WIDTH - 2;
  int const brx = tlx + UI_RIGHTBAR_WIDTH;
  int const tly = UI_TOPCON_HEIGHT + 10;
  int const bry = tly + 40;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Death Arcana");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "With the Death Arcana, you specialize in all things dead, undead summoning, finger of death etc...\n",
                      TEXT_FORMAT_LHS);
  wid_spell_learn_stats_arcana_common_mouse_over_begin(g);
  wid_over_stats->log(g, UI_IMPORTANT_FMT_STR "Specializing in Life will make Death spells more costly.\n", TEXT_FORMAT_LHS);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_spell_learn_stats_mouse_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_stats;
  wid_over_stats = nullptr;
}

[[nodiscard]] static auto wid_spell_learn_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_spell_learn_destroy(g);
  return true;
}

[[nodiscard]] static auto wid_spell_learn_how_many_items(Gamep g, Levelsp v, Levelp l, Thingp player,
                                                         const std::vector< Thingp > &wid_spell_things) -> int
{
  TRACE();

  int y_at = 0;

  for (auto &spell : wid_spell_things) {
    y_at++;

    //
    // Add upgrades?
    //
    FOR_ALL_SPELLBOOK_SPELLS(g, v, l, player, learned_spell)
    {
      if (thing_tp(spell) == thing_tp(learned_spell)) {
        for (const auto &i : tp_spell_upgrades_get(thing_tp(spell))) {
          auto u = i.second;
          if (thing_is_upgradable(g, v, l, learned_spell, u)) {
            y_at++;
          }
        }
        break;
      }
    }
  }

  return y_at;
}

void wid_spell_learn(Gamep g, Levelsp v, Levelp l, Thingp player, ThingStatType filter)
{
  con("Player select menu: create");
  TRACE_INDENT();

  wid_spell_filter = filter;

  auto *level_select = game_level_get(g, v, LEVEL_ARR_IDX_LEVEL_SELECT);
  if (level_select == nullptr) {
    return;
  }

  if (wid_spell_learn_window != nullptr) {
    wid_spell_learn_destroy(g);
  }

  std::vector< Tpp > wid_spell_tps;

  for (auto &tp : tp_vec) {
    if (! tp_is_spell(tp)) {
      continue;
    }
    wid_spell_tps.push_back(tp);
  }

  const int menu_width  = UI_INVENTORY_WIDTH;
  const int menu_height = TERM_HEIGHT - (UI_TOPCON_HEIGHT * 2);

  const auto button_width  = menu_width - 2;
  const auto button_height = 0;
  const auto button_step   = 1;

  auto y_at = 1;

  const int left_half  = menu_width / 2;
  const int right_half = menu_width - left_half;

  {
    TRACE();
    spoint const tl((TERM_WIDTH / 2) - left_half, UI_TOPCON_HEIGHT + 1);
    spoint const br((TERM_WIDTH / 2) + right_half - 1, tl.y + menu_height - 1);

    wid_spell_learn_window = wid_new_window(g, "widget spell_learn");
    wid_set_pos(wid_spell_learn_window, tl, br);
    wid_set_style(wid_spell_learn_window, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_on_key_down(wid_spell_learn_window, wid_spell_learn_key_down);
    wid_set_text_top(wid_spell_learn_window, 1u);
    wid_raise(g, wid_spell_learn_window);
  }

  {
    TRACE();
    auto        *w = wid_new_square_button(g, wid_spell_learn_window, "text");
    spoint const tl(0, y_at);
    spoint const br(menu_width, y_at);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_INFO_FMT_STR "Choose spell(s) to learn.");
    wid_set_style(w, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_shape_none(w);
    wid_set_text_centerx(w, 1u);
    y_at += 2;
  }

  //
  // Spells
  //
  {
    TRACE();
    auto *w = wid_new_bar_button(g, wid_spell_learn_window, "Spells");

    spoint const tl(1, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_text_lhs(w, 1u);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_INFO_FMT_STR "Spell name                              Arcana  SP");
    y_at++;
  }

  int wid_spell_index   = 0;
  int wid_upgrade_index = 0;

  std::vector< Thingp > wid_spell_things;

  for (auto &tp : wid_spell_tps) {
    //
    // Check for overflow
    //
    if (wid_spell_index >= ARRAY_SIZE(wid_spell)) {
      break;
    }

    //
    // Massive hack. Use hidden space on level select level for the things.
    //
    // This is one column to the right of player selection things
    //
    // Create a temporary thing on the level select map
    //
    auto   at             = bpoint(4 + (wid_spell_index / MAP_WIDTH), wid_spell_index % MAP_HEIGHT);
    Thingp existing_spell = nullptr;
    FOR_ALL_THINGS_AT(g, v, level_select, spell, at)
    {
      if (spell != nullptr) {
        existing_spell = spell;
        break;
      }
    }

    if (existing_spell == nullptr) {
      existing_spell = thing_spawn(g, v, level_select, tp, at);
      if (existing_spell == nullptr) {
        continue;
      }
    }

    //
    // Already learned?
    //
    Thingp already_learned = {};
    FOR_ALL_SPELLBOOK_SPELLS(g, v, l, player, learned_spell)
    {
      if (thing_tp(learned_spell) == thing_tp(existing_spell)) {
        already_learned = learned_spell;
        break;
      }
    }

    //
    // Skip already learned
    //
    if (already_learned != nullptr) {
      //
      // Upgrades?
      //
      if (thing_is_upgradable(g, v, l, already_learned)) {
        if (filter == 0U) {
          //
          // All spells
          //
          wid_spell_things.push_back(existing_spell);
        } else {
          //
          // Match filter only
          //
          if (thing_spell_arcana(g, v, l, existing_spell) == filter) {
            wid_spell_things.push_back(existing_spell);
          }
        }
      }
    } else {
      //
      // Filter?
      //
      if (filter == 0U) {
        //
        // All spells
        //
        wid_spell_things.push_back(existing_spell);
      } else {
        //
        // Match filter only
        //
        if (thing_spell_arcana(g, v, l, existing_spell) == filter) {
          wid_spell_things.push_back(existing_spell);
        }
      }
    }

    wid_spell_index++;
  }

  //
  // Create the scrolling spell list
  //
  {
    auto         needed_height = wid_spell_learn_how_many_items(g, v, l, player, wid_spell_things);
    spoint const inner_tl(1, 5);
    spoint const inner_br(menu_width - 2, menu_height - 12);

    wid_spell_learn_list = new WidPopup(g, wid_spell_learn_window, "spell list", inner_tl, inner_br, nullptr, "", false, true, needed_height);
  }

  //
  // Sort by spell_cost
  //
  std::ranges::sort(wid_spell_things, [ g, v, l, player ](const Thingp &a, const Thingp &b) -> bool {
    TRACE();
    return thing_spell_cost_for(g, v, l, a, player) < thing_spell_cost_for(g, v, l, b, player);
  });

  memset(wid_spell, 0, sizeof(wid_spell));

  wid_spell_index = 0;

  for (auto &spell : wid_spell_things) {
    TRACE();

    //
    // Spell shortcut and name
    //
    if (wid_spell_index > (('z' - 'a') * 2) + 1) {
      break;
    }

    auto *w = wid_spell_learn_list->log(g, "-", TEXT_FORMAT_LHS);

    wid_set_thing_context(g, v, w, spell);
    wid_set_int_context(w, wid_spell_index);
    wid_set_on_mouse_over_begin(w, wid_spell_learn_spell_via_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_spell_learn_spell_via_mouse_over_end);
    wid_set_on_mouse_down(w, wid_spell_learn_spell_via_mouse_down);
    wid_apply_bar_button(g, w);
    y_at += button_step;

    //
    // Add upgrades?
    //
    FOR_ALL_SPELLBOOK_SPELLS(g, v, l, player, learned_spell)
    {
      if (thing_tp(spell) == thing_tp(learned_spell)) {
        for (const auto &i : tp_spell_upgrades_get(thing_tp(spell))) {
          auto u = i.second;
          if (thing_is_upgradable(g, v, l, learned_spell, u)) {
            auto *w_upgrade = wid_spell_learn_list->log(g, "-", TEXT_FORMAT_LHS);

            wid_set_thing_context(g, v, w_upgrade, spell);
            wid_set_int_context(w_upgrade, wid_spell_index);
            wid_set_string_context(w_upgrade, u.name);
            wid_set_on_mouse_down(w_upgrade, wid_spell_upgrade_spell_via_mouse_down);
            wid_set_on_mouse_over_begin(w_upgrade, wid_spell_learn_spell_via_mouse_over_begin);
            wid_set_on_mouse_over_end(w_upgrade, wid_spell_learn_spell_via_mouse_over_end);
            wid_apply_bar_button(g, w_upgrade);
            y_at += button_step;
            wid_upgrade[ wid_upgrade_index++ ] = w_upgrade;
          }
        }
        break;
      }
    }

    wid_spell[ wid_spell_index ] = w;
    wid_spell_index++;
  }

  y_at = menu_height - 10;

  //
  // Filters:
  //
  {
    TRACE();
    auto *w = wid_new_bar_button(g, wid_spell_learn_window, "available SP");

    spoint const tl(1, y_at + 1);
    spoint const br(button_width, y_at + button_height + 1);
    wid_set_pos(w, tl, br);

    wid_set_text_lhs(w, 1u);
    wid_set_text(w, "Filters:");
  }

  //
  // Arcana stats:
  //
  {
    auto         out = thing_stat_mod_string(g, v, l, player, THING_STAT_ARCANA_FIRE);
    auto        *w   = wid_new_bright_button(g, wid_spell_learn_window, stat_to_name(THING_STAT_ARCANA_FIRE));
    spoint const tl(10, y_at);
    spoint const br(18, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_down(w, wid_spell_learn_stats_arcana_fire_mouse_down);
    if (filter == THING_STAT_ARCANA_FIRE) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
    } else {
      wid_set_on_mouse_over_begin(w, wid_spell_learn_stats_arcana_fire_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_spell_learn_stats_mouse_over_end);
    }
  }
  {
    auto         out = thing_stat_mod_string(g, v, l, player, THING_STAT_ARCANA_LIFE);
    auto        *w   = wid_new_bright_button(g, wid_spell_learn_window, stat_to_name(THING_STAT_ARCANA_LIFE));
    spoint const tl(19, y_at);
    spoint const br(27, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_down(w, wid_spell_learn_stats_arcana_life_mouse_down);
    if (filter == THING_STAT_ARCANA_LIFE) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
    } else {
      wid_set_on_mouse_over_begin(w, wid_spell_learn_stats_arcana_life_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_spell_learn_stats_mouse_over_end);
    }
  }
  {
    auto         out = thing_stat_mod_string(g, v, l, player, THING_STAT_ARCANA_DEATH);
    auto        *w   = wid_new_bright_button(g, wid_spell_learn_window, stat_to_name(THING_STAT_ARCANA_DEATH));
    spoint const tl(28, y_at);
    spoint const br(36, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_down(w, wid_spell_learn_stats_arcana_death_mouse_down);
    if (filter == THING_STAT_ARCANA_DEATH) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
    } else {
      wid_set_on_mouse_over_begin(w, wid_spell_learn_stats_arcana_death_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_spell_learn_stats_mouse_over_end);
    }
  }

  y_at = menu_height - 6;

  //
  // Total sac points:
  //
  {
    TRACE();
    wid_total = wid_new_bar_button(g, wid_spell_learn_window, "available SP");

    spoint const tl(1, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_pos(wid_total, tl, br);

    wid_set_text_lhs(wid_total, 1u);
    wid_player_update_spell_selections(g, v, l, player);
    wid_player_update_spending(g, v, l, player);
  }

  {
    TRACE();
    auto *w = wid_new_back_button(g, wid_spell_learn_window, "BACK");

    spoint const tl((menu_width / 2) - 4, menu_height - 4);
    spoint const br((menu_width / 2) + 3, menu_height - 2);
    wid_set_on_mouse_down(w, wid_spell_learn_back);
    wid_set_pos(w, tl, br);
  }

  wid_update(g, wid_spell_learn_window);

  game_state_change(g, STATE_SPELL_LEARN_MENU, "spell_learn");

  botcon_newline();
}
