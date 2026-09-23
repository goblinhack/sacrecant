//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
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
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tp.hpp"
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
#include <string>
#include <vector>

static Widp          wid_available_mana;
static Widp          wid_spellbook_window;
static WidPopup     *wid_spellbook_list;
static WidPopup     *wid_over_stats;
static ThingStatType wid_spell_filter = THING_STAT_NONE;

static Widp wid_spell[ THING_SPELLBOOK_MAX ];

static void wid_spellbook_destroy(Gamep g)
{
  TRACE();

  memset(wid_spell, 0, sizeof(wid_spell));

  game_cand_spell_unset(g, nullptr);

  delete wid_spellbook_list;
  wid_spellbook_list = nullptr;

  delete wid_over_stats;
  wid_over_stats = nullptr;

  if (wid_spellbook_window != nullptr) {
    wid_destroy(g, &wid_spellbook_window);
  }

  wid_available_mana = nullptr;

  game_state_reset(g, "close spell window");
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
      spent += thing_spell_mana_cost_for(g, v, l, spell, player);
    }
  }

  return spent;
}

static auto wid_player_avail_points(Gamep g, Levelsp v, Levelp l, Thingp player) -> int
{
  TRACE();

  auto spent = wid_player_spent_points(g, v, l, player);
  auto avail = thing_mana(g, v, l, player) - spent;

  return avail;
}

static void wid_player_update_spending(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  TRACE();

  auto avail = wid_player_avail_points(g, v, l, player);
  auto a     = string_sprintf("Mana available for spell casting");
  auto b     = string_sprintf("%d", avail);
  auto line  = string_sprintf("%-46s%10s", a.c_str(), b.c_str());

  wid_set_text_lhs(wid_available_mana, 1u);
  wid_set_text(wid_available_mana, line);
  wid_update(g, wid_available_mana);
}

static void wid_player_update_spell_selections(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  TRACE();

  auto avail = wid_player_avail_points(g, v, l, player);
  Widp w     = nullptr;

  for (auto &n : wid_spell) {
    w = n;
    if (w == nullptr) {
      continue;
    }

    auto *spell = wid_get_thing_context(g, v, w, 0);
    if (spell == nullptr) {
      continue;
    }

    auto        index           = wid_get_int_context(w);
    auto        spell_mana_cost = thing_spell_mana_cost_for(g, v, l, spell, player);
    auto       *tp              = thing_tp(spell);
    std::string s;

    if (spell_mana_cost <= avail) {
      s += "%%fg=gray90$";
    } else {
      s += "%%fg=gray50$";
    }

    switch (index) {
      case 0 :  s += ::to_string(game_key_spell1_get(g)); break;
      case 1 :  s += ::to_string(game_key_spell2_get(g)); break;
      case 2 :  s += ::to_string(game_key_spell3_get(g)); break;
      case 3 :  s += ::to_string(game_key_spell4_get(g)); break;
      case 4 :  s += ::to_string(game_key_spell5_get(g)); break;
      case 5 :  s += ::to_string(game_key_spell6_get(g)); break;
      case 6 :  s += ::to_string(game_key_spell7_get(g)); break;
      case 7 :  s += ::to_string(game_key_spell8_get(g)); break;
      case 8 :  s += ::to_string(game_key_spell9_get(g)); break;
      case 9 :  s += ::to_string(game_key_spell10_get(g)); break;
      case 10 : s += ::to_string(game_key_spell11_get(g)); break;
      case 11 : s += ::to_string(game_key_spell12_get(g)); break;
      case 12 : s += ::to_string(game_key_spell13_get(g)); break;
      case 13 : s += ::to_string(game_key_spell14_get(g)); break;
      case 14 : s += ::to_string(game_key_spell15_get(g)); break;
      case 15 : s += ::to_string(game_key_spell16_get(g)); break;
      case 16 : s += ::to_string(game_key_spell17_get(g)); break;
      case 17 : s += ::to_string(game_key_spell18_get(g)); break;
      case 18 : s += ::to_string(game_key_spell19_get(g)); break;
      case 19 : s += ::to_string(game_key_spell20_get(g)); break;
    }

    s += ") ";

    s += capitalize_first(tp_name_long(tp));
    s = string_sprintf("%-50s", s.c_str());
    s += "%%fg=reset$";

    switch (thing_spell_arcana(g, v, l, spell)) {
      case THING_STAT_ARCANA_FIRE :  s += "%%fg=orange$Fire%%fg=reset$    "; break;
      case THING_STAT_ARCANA_DEATH : s += "%%fg=gray50$Death%%fg=reset$   "; break;
      case THING_STAT_ARCANA_LIFE :  s += "%%fg=green$Life%%fg=reset$    "; break;
      default :                      s += "-    "; break;
    }

    s += string_sprintf("%2d", spell_mana_cost);

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

  wid_player_update_spending(g, v, l, player);
}

static void wid_spellbook_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
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
  game_request_to_remake_ui_set(g);
}

static void wid_spellbook_mouse_over_end(Gamep g, Widp w)
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
  game_request_to_remake_ui_set(g);
}

[[nodiscard]] static auto wid_spellbook_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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

  auto cost  = thing_spell_mana_cost_for(g, v, l, spell, player);
  auto avail = wid_player_avail_points(g, v, l, player);

  if (cost > avail) {
    topcon("You do not have enough Mana to cast that spell.\n");
    (void) sound_play(g, "error");
    return true;
  }

  (void) sound_play(g, "select");
  wid_spellbook_destroy(g);

  auto index = wid_get_int_context(w);
  if (! tp_spell_options_exist(thing_tp(spell))) {
    (void) game_event_cast_spell_default(g, index);
    return true;
  }

  wid_spell_options_show(g, v, l, player, spell);

  return true;
}

static void wid_spellbook_key_down_which_spell(Gamep g, Widp w, int index)
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) [[unlikely]] {
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return;
  }

  w = wid_spell[ index ];
  if (w == nullptr) {
    (void) sound_play(g, "error");
    return;
  }

  auto *spell = wid_get_thing_context(g, v, w, 0);
  if (spell == nullptr) {
    (void) sound_play(g, "error");
    return;
  }

  (void) wid_spellbook_mouse_down(g, w, -1, -1, 0);
}

[[nodiscard]] static auto wid_spellbook_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game_key_console_get(g))) {
    (void) sound_play(g, "keypress");
    return false;
  }

  if (sdlk_eq(*key, game_key_spell1_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 0);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell2_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 1);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell3_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 2);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell4_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 3);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell5_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 4);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell6_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 5);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell7_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 6);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell8_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 7);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell9_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 8);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell10_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 9);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell11_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 10);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell12_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 11);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell13_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 12);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell14_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 13);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell15_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 14);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell16_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 15);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell17_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 16);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell18_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 17);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell19_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 18);
    return true;
  }

  if (sdlk_eq(*key, game_key_spell20_get(g))) {
    (void) sound_play(g, "keypress");
    wid_spellbook_key_down_which_spell(g, w, 19);
    return true;
  }

  switch (key->mod) {
    default :
      switch (key->sym) {
        default : break;
        case SDLK_ESCAPE :
          (void) sound_play(g, "keypress");
          wid_spellbook_destroy(g);
          return true;
      }
  }

  //
  // So screenshots can work
  //
  return false;
}

[[nodiscard]] static auto wid_spellbook_stats_arcana_fire_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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
    wid_spellbook(g, v, l, player, THING_STAT_NONE);
  } else {
    wid_spellbook(g, v, l, player, THING_STAT_ARCANA_FIRE);
  }

  return true;
}

[[nodiscard]] static auto wid_spellbook_stats_arcana_life_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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
    wid_spellbook(g, v, l, player, THING_STAT_NONE);
  } else {
    wid_spellbook(g, v, l, player, THING_STAT_ARCANA_LIFE);
  }

  return true;
}

[[nodiscard]] static auto wid_spellbook_stats_arcana_death_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
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
    wid_spellbook(g, v, l, player, THING_STAT_NONE);
  } else {
    wid_spellbook(g, v, l, player, THING_STAT_ARCANA_DEATH);
  }

  return true;
}

static void wid_spellbook_stats_arcana_common_mouse_over_begin(Gamep g)
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

static void wid_spellbook_stats_arcana_fire_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
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
  wid_spellbook_stats_arcana_common_mouse_over_begin(g);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_spellbook_stats_arcana_life_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
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
  wid_spellbook_stats_arcana_common_mouse_over_begin(g);
  wid_over_stats->log(g, UI_IMPORTANT_FMT_STR "Specializing in Death will make Life spells more costly.\n", TEXT_FORMAT_LHS);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_spellbook_stats_arcana_death_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
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
  wid_spellbook_stats_arcana_common_mouse_over_begin(g);
  wid_over_stats->log(g, UI_IMPORTANT_FMT_STR "Specializing in Life will make Death spells more costly.\n", TEXT_FORMAT_LHS);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_spellbook_stats_mouse_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_stats;
  wid_over_stats = nullptr;
}

[[nodiscard]] static auto wid_spellbook_back(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  wid_spellbook_destroy(g);
  (void) sound_play(g, "select");
  return true;
}

[[nodiscard]] static auto wid_spellbook_how_many_items(Gamep g, Levelsp v, Levelp l, Thingp player)
{
  TRACE();

  return thing_spellbook_get_spell_count(g, v, l, player);
}

void wid_spellbook(Gamep g, Levelsp v, Levelp l, Thingp player, ThingStatType filter)
{
  con("Player select menu: create");
  TRACE_INDENT();

  wid_spell_filter = filter;

  auto *level_select = game_level_get(g, v, LEVEL_ARR_IDX_LEVEL_SELECT);
  if (level_select == nullptr) {
    return;
  }

  auto needed_height = wid_spellbook_how_many_items(g, v, l, player);

  if (wid_spellbook_window != nullptr) {
    wid_spellbook_destroy(g);
  }

  level_cursor_describe_clear(g, v);

  const int menu_width  = UI_INVENTORY_WIDTH;
  const int menu_height = needed_height + 18;

  const auto button_width  = menu_width - 2;
  const auto button_height = 0;
  const auto button_step   = 1;

  auto y_at = 1;

  const int left_half  = menu_width / 2;
  const int right_half = menu_width - left_half;
  const int top_half   = menu_height / 2;

  {
    TRACE();
    spoint const tl((TERM_WIDTH / 2) - left_half, (TERM_HEIGHT / 2) - top_half + 1);
    spoint const br((TERM_WIDTH / 2) + right_half - 1, tl.y + menu_height - 1);

    wid_spellbook_window = wid_new_window(g, "widget spellbook");
    wid_set_pos(wid_spellbook_window, tl, br);
    wid_set_style(wid_spellbook_window, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_on_key_down(wid_spellbook_window, wid_spellbook_key_down);
    wid_set_text_top(wid_spellbook_window, 1u);
    wid_raise(g, wid_spellbook_window);
  }

  {
    TRACE();
    auto        *w = wid_new_square_button(g, wid_spellbook_window, "text");
    spoint const tl(0, y_at);
    spoint const br(menu_width, y_at);
    wid_set_pos(w, tl, br);
    if (thing_spellbook_get_spell_count(g, v, l, player) != 0) {
      wid_set_text(w, UI_INFO_FMT_STR "Choose a spell to cast.");
    } else {
      wid_set_text(w, UI_INFO_FMT_STR "You need to learn a spell first");
      (void) sound_play(g, "error");
    }
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
    auto *w = wid_new_bar_button(g, wid_spellbook_window, "Spells");

    spoint const tl(1, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_text_lhs(w, 1u);
    wid_set_pos(w, tl, br);
    wid_set_text(w, UI_INFO_FMT_STR "Spell name                            Arcana  Mana");
    y_at++;
  }

  int wid_spell_index = 0;

  std::vector< Thingp > wid_spell_things;

  FOR_ALL_SPELLBOOK_SPELLS(g, v, l, player, existing_spell)
  {
    //
    // Check for overflow
    //
    if (wid_spell_index >= ARRAY_SIZE(wid_spell)) {
      break;
    }

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

    wid_spell_index++;
  }

  //
  // Create the scrolling spell list
  //
  {
    spoint const inner_tl(1, 5);
    spoint const inner_br(menu_width - 2, menu_height - 12);

    wid_spellbook_list = new WidPopup(g, wid_spellbook_window, "spell list", inner_tl, inner_br, nullptr, "", false, false, needed_height);
  }

  //
  // Sort by spell_mana_cost
  //
  std::ranges::sort(wid_spell_things, [ g, v, l, player ](const Thingp &a, const Thingp &b) -> bool {
    TRACE();
    return thing_spell_mana_cost_for(g, v, l, a, player) < thing_spell_mana_cost_for(g, v, l, b, player);
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

    auto *w = wid_spellbook_list->log(g, "-", TEXT_FORMAT_LHS);

    wid_set_thing_context(g, v, w, spell);
    wid_set_int_context(w, wid_spell_index);
    wid_set_on_mouse_over_begin(w, wid_spellbook_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_spellbook_mouse_over_end);
    wid_set_on_mouse_down(w, wid_spellbook_mouse_down);
    wid_apply_bar_button(g, w);
    y_at += button_step;

    wid_spell[ wid_spell_index ] = w;
    wid_spell_index++;
  }

  y_at = menu_height - 10;

  //
  // Filters:
  //
  {
    TRACE();
    auto *w = wid_new_bar_button(g, wid_spellbook_window, "filters");

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
    auto        *w   = wid_new_bright_button(g, wid_spellbook_window, stat_to_name(THING_STAT_ARCANA_FIRE));
    spoint const tl(10, y_at);
    spoint const br(18, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_down(w, wid_spellbook_stats_arcana_fire_mouse_down);
    if (filter == THING_STAT_ARCANA_FIRE) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
    } else {
      wid_set_on_mouse_over_begin(w, wid_spellbook_stats_arcana_fire_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_spellbook_stats_mouse_over_end);
    }
  }
  {
    auto         out = thing_stat_mod_string(g, v, l, player, THING_STAT_ARCANA_LIFE);
    auto        *w   = wid_new_bright_button(g, wid_spellbook_window, stat_to_name(THING_STAT_ARCANA_LIFE));
    spoint const tl(19, y_at);
    spoint const br(27, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_down(w, wid_spellbook_stats_arcana_life_mouse_down);
    if (filter == THING_STAT_ARCANA_LIFE) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
    } else {
      wid_set_on_mouse_over_begin(w, wid_spellbook_stats_arcana_life_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_spellbook_stats_mouse_over_end);
    }
  }
  {
    auto         out = thing_stat_mod_string(g, v, l, player, THING_STAT_ARCANA_DEATH);
    auto        *w   = wid_new_bright_button(g, wid_spellbook_window, stat_to_name(THING_STAT_ARCANA_DEATH));
    spoint const tl(28, y_at);
    spoint const br(36, y_at + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_down(w, wid_spellbook_stats_arcana_death_mouse_down);
    if (filter == THING_STAT_ARCANA_DEATH) {
      wid_set_mode(w, WID_MODE_OVER);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
      wid_set_mode(w, WID_MODE_NORMAL);
      wid_set_style(w, UI_WID_STYLE_BUTTON_ROUNDED_SOLID);
    } else {
      wid_set_on_mouse_over_begin(w, wid_spellbook_stats_arcana_death_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_spellbook_stats_mouse_over_end);
    }
  }

  y_at = menu_height - 6;

  //
  // Total sac points:
  //
  {
    TRACE();
    wid_available_mana = wid_new_bar_button(g, wid_spellbook_window, "available mana");

    spoint const tl(1, y_at);
    spoint const br(button_width, y_at + button_height);
    wid_set_pos(wid_available_mana, tl, br);

    wid_set_text_lhs(wid_available_mana, 1u);
    wid_player_update_spell_selections(g, v, l, player);
    wid_player_update_spending(g, v, l, player);
  }

  {
    TRACE();
    auto *w = wid_new_back_button(g, wid_spellbook_window, "BACK");

    spoint const tl((menu_width / 2) - 4, menu_height - 4);
    spoint const br((menu_width / 2) + 3, menu_height - 2);
    wid_set_on_mouse_down(w, wid_spellbook_back);
    wid_set_pos(w, tl, br);
  }

  wid_update(g, wid_spellbook_window);

  game_state_change(g, STATE_SPELLBOOK_MENU, "spellbook");

  botcon_newline();
}
