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
#include "my_random.hpp"
#include "my_sdl_proto.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_sprintf.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

static Widp      wid_total;
static Widp      wid_spell_learn_window;
static WidPopup *wid_spell_learn_list;
static WidPopup *wid_spell_learn_learn_window;
static WidPopup *wid_over_stats;

static Widp wid_spell[ TP_ID_MAX ];

static void wid_spell_learn_destroy(Gamep g)
{
  TRACE();

  memset(wid_spell, 0, sizeof(wid_spell));

  game_cand_spell_unset(g, nullptr);

  delete wid_spell_learn_list;
  wid_spell_learn_list = nullptr;

  delete wid_spell_learn_learn_window;
  wid_spell_learn_learn_window = nullptr;

  if (wid_spell_learn_window != nullptr) {
    wid_destroy(g, &wid_spell_learn_window);
  }

  game_state_reset(g, "close spell window");
}

static void wid_spell_learn_all_done(Gamep g)
{
  TRACE();

  game_chosen_spell_set(g, game_cand_spell_get(g));

  wid_spell_learn_destroy(g);

  game_spell_clear(g);
}

[[nodiscard]] static auto wid_learn_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  wid_spell_learn_all_done(g);

  return true;
}

static void wid_spell_learn_check_if_done(Gamep g)
{
  TRACE();

  if (game_cand_spell_get(g).empty()) {
    delete wid_spell_learn_learn_window;
    wid_spell_learn_learn_window = nullptr;
    return;
  }

  if (wid_spell_learn_learn_window == nullptr) {
    auto         m = TERM_WIDTH / 2;
    auto         n = TERM_HEIGHT - 5;
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

static auto wid_player_spent_points(Gamep g) -> int
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return 0;
  }

  Widp w = nullptr;
  int  spent {};

  for (auto &n : wid_spell) {
    w = n;
    if (w != nullptr) {
      auto *t = wid_get_thing_context(g, v, w, 0);
      if (game_cand_spell_find(g, t)) {
        spent += thing_spell_cost(t);
      }
    }
  }

  return spent;
}

static int wid_player_avail_points(Gamep g)
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

  auto spent = wid_player_spent_points(g);
  auto avail = thing_sac_points(g, v, l, player) - spent;

  return avail + 10;
}

static void wid_player_update_spending(Gamep g)
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

  auto spent = wid_player_spent_points(g);
  auto avail = wid_player_avail_points(g);

  auto a    = string_sprintf("Sacrificial points (SPs)");
  auto b    = string_sprintf("Spent:%d", spent);
  auto c    = string_sprintf("Avail:%d", avail);
  auto line = string_sprintf("%-30s%10s%10s", a.c_str(), b.c_str(), c.c_str());

  wid_set_text_lhs(wid_total, 1u);
  wid_set_text(wid_total, line);
  wid_update(g, wid_total);
}

static void wid_player_update_selections(Gamep g)
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

  auto avail = wid_player_avail_points(g);
  Widp w     = nullptr;

  wid_unset_focus(g);
  wid_mouse_over_end(g);

  for (auto &n : wid_spell) {
    w = n;
    if (w == nullptr) {
      continue;
    }

    auto *t = wid_get_thing_context(g, v, w, 0);
    if (! t) {
      continue;
    }

    auto        index      = wid_get_int_context(w);
    auto        spell_cost = thing_spell_cost(t);
    auto        tp         = thing_tp(t);
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
    if (thing_stat(g, v, l, t, THING_STAT_ARCANA_FIRE) >= THING_STAT_DEFAULT) {
      s += "%%fg=orange$Fire%%fg=reset$    ";
    } else if (thing_stat(g, v, l, t, THING_STAT_ARCANA_DEATH) >= THING_STAT_DEFAULT) {
      s += "%%fg=gray50$Death%%fg=reset$   ";
    } else if (thing_stat(g, v, l, t, THING_STAT_ARCANA_LIFE) >= THING_STAT_DEFAULT) {
      s += "%%fg=green$Life%%fg=reset$    ";
    } else {
      s += "-    ";
    }

    s += string_sprintf("%2d", spell_cost);

    wid_set_text(w, s);
    wid_apply_bar_button(g, w);

    if (game_cand_spell_find(g, t)) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, RED);
      wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_BAR);
      wid_set_color(w, WID_COLOR_BG, RED);
    }
  }

  wid_player_update_spending(g);
}

static void wid_spell_learn_spell_via_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  game_spell_mouse_over_currently_set(g, t);

  level_cursor_describe_clear(g, v);

  if (level_cursor_describe_add(g, v, t)) {
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

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  game_spell_mouse_over_currently_set(g, nullptr);

  if (level_cursor_describe_remove(g, v, t)) {
    game_request_to_remake_ui_set(g);
  }
}

[[nodiscard]] static auto wid_spell_learn_spell_via_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = levels_memory_alloc(g);
  if (v == nullptr) {
    return false;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return false;
  }

  auto cost  = thing_spell_cost(t);
  auto avail = wid_player_avail_points(g);

  if (game_cand_spell_find(g, t)) {
    game_cand_spell_unset(g, t);
    (void) sound_play(g, "select");
  } else if (cost <= avail) {
    game_cand_spell_set(g, t);
    (void) sound_play(g, "select");
  } else {
    topcon("You do not have enough Sacrificial Points to learn that spell.\n");
    (void) sound_play(g, "error");
  }

  wid_spell_learn_check_if_done(g);
  wid_player_update_selections(g);
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
                  wid_spell_learn_all_done(g);
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

static void wid_spell_learn_stats_arcana_fire_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = UI_INVENTORY_WIDTH;
  int const height = 16;

  tlx = UI_LEFTBAR_WIDTH;
  tly -= 12;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Fire Arcana");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "With the Fire Arcana, you specialize in all things flaming hot, fireballs, scorched earth etc...\n",
                      TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "Fire modifiers can impact costs and mana drain.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO1_FMT_STR " - -3 modifier increases Fire cost by 1.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO1_FMT_STR " - +3 modifier reduces Fire cost by 1.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO1_FMT_STR " - +5 modifier reduces Fire casting mana by 50%%%.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "Select this to filter spells to Fire only.\n", TEXT_FORMAT_LHS);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_spell_learn_stats_arcana_life_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = UI_INVENTORY_WIDTH;
  int const height = 17;

  tlx = UI_LEFTBAR_WIDTH;
  tly -= 14;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Life Arcana");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g,
                      UI_INFO1_FMT_STR "With the Life Arcana, you specialize in all things living, plant summoning, healing of allies etc...\n",
                      TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "Modifiers can impact costs and mana drain.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO1_FMT_STR " - -3 modifier increases Life cost by 1.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO1_FMT_STR " - +3 modifier reduces Life cost by 1.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO1_FMT_STR " - +5 modifier reduces Life casting mana by 50%%%.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "Select this to filter spells to Life only.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_IMPORTANT_FMT_STR "Specializing in Death will make Life spells more costly.\n", TEXT_FORMAT_LHS);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_spell_learn_stats_arcana_death_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = UI_INVENTORY_WIDTH;
  int const height = 17;

  tlx = UI_LEFTBAR_WIDTH;
  tly -= 14;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Death Arcana");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "With the Death Arcana, you specialize in all things dead, undead summoning, finger of death etc...\n",
                      TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "Modifiers can impact costs and mana drain.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO1_FMT_STR " - -3 modifier increases Death cost by 1.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO1_FMT_STR " - +3 modifier reduces Death cost by 1.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO1_FMT_STR " - +5 modifier reduces Death casting mana by 50%%%.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "Select this to filter spells to Death only.\n", TEXT_FORMAT_LHS);
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

void wid_spell_learn(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  con("Player select menu: create");
  TRACE_INDENT();

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
  const int menu_height = TERM_HEIGHT - UI_TOPCON_HEIGHT * 2 - 6;

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

  {
    spoint inner_tl(1, 5);
    spoint inner_br(menu_width - 2, menu_height - 8);

    wid_spell_learn_list
        = new WidPopup(g, wid_spell_learn_window, "spell list", inner_tl, inner_br, nullptr, "", false, true, wid_spell_tps.size());
  }

  int wid_spell_index = 0;

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
    auto   at             = bpoint(4 + wid_spell_index / MAP_WIDTH, wid_spell_index % MAP_HEIGHT);
    Thingp existing_thing = nullptr;
    FOR_ALL_THINGS_AT(g, v, level_select, t, at)
    {
      if (t != nullptr) {
        existing_thing = t;
        break;
      }
    }

    if (existing_thing == nullptr) {
      existing_thing = thing_spawn(g, v, level_select, tp, at);
      if (existing_thing == nullptr) {
        continue;
      }
    }

    wid_spell_things.push_back(existing_thing);

    wid_spell_index++;
  }

  //
  // Sort by spell_cost
  //
  std::ranges::sort(wid_spell_things, [](const Thingp &a, const Thingp &b) -> bool { return thing_spell_cost(a) < thing_spell_cost(b); });

  memset(wid_spell, 0, sizeof(wid_spell));

  wid_spell_index = 0;

  for (auto &t : wid_spell_things) {
    TRACE();

    //
    // Spell shortcut and name
    //
    if (wid_spell_index > ('z' - 'a') * 2 + 1) {
      break;
    }

    auto w = wid_spell_learn_list->log(g, "-", TEXT_FORMAT_LHS);

    wid_set_thing_context(g, v, w, t);
    wid_set_int_context(w, wid_spell_index);
    wid_set_on_mouse_down(w, wid_spell_learn_spell_via_mouse_down);
    wid_set_on_mouse_over_begin(w, wid_spell_learn_spell_via_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_spell_learn_spell_via_mouse_over_end);
    wid_apply_bar_button(g, w);

    wid_spell[ wid_spell_index ] = w;
    y_at += button_step;
    wid_spell_index++;
  }

  y_at = menu_height - 6;

  //
  // Filters:
  //
  {
    TRACE();
    auto w = wid_new_bar_button(g, wid_spell_learn_window, "available SP");

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
    auto        *w   = wid_new_bright_button(g, wid_spell_learn_window, stat_to_name(THING_STAT_ARCANA_FIRE).c_str());
    spoint const tl(10, y_at);
    spoint const br(18, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_spell_learn_stats_arcana_fire_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_spell_learn_stats_mouse_over_end);
  }
  {
    auto         out = thing_stat_mod_string(g, v, l, player, THING_STAT_ARCANA_LIFE);
    auto        *w   = wid_new_bright_button(g, wid_spell_learn_window, stat_to_name(THING_STAT_ARCANA_LIFE).c_str());
    spoint const tl(19, y_at);
    spoint const br(27, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_spell_learn_stats_arcana_life_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_spell_learn_stats_mouse_over_end);
  }
  {
    auto         out = thing_stat_mod_string(g, v, l, player, THING_STAT_ARCANA_DEATH);
    auto        *w   = wid_new_bright_button(g, wid_spell_learn_window, stat_to_name(THING_STAT_ARCANA_DEATH).c_str());
    spoint const tl(28, y_at);
    spoint const br(36, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_spell_learn_stats_arcana_death_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_spell_learn_stats_mouse_over_end);
  }

  y_at = menu_height - 2;

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
    wid_player_update_selections(g);
    wid_player_update_spending(g);
  }

  wid_update(g, wid_spell_learn_window);

  game_state_change(g, STATE_SPELL_LEARN_MENU, "spell_learn");

  botcon_newline();
}
