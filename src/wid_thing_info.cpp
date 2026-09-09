//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp" // NOLINT
#include "my_main.hpp"
#include "my_spoint.hpp"
#include "my_sprintf.hpp"
#include "my_string.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp" // NOLINT
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tp_inlines.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wid_text_box.hpp"
#include "my_wids.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <format>
#include <string>

static WidPopup *wid_over_stats;

static void wid_thing_info_stats_att_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 17;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Attack");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "This is your overall attack roll out of a max of 20.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "If you roll more than your target's defence roll, you hit. Otherwise you miss.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO3_FMT_STR "A roll of 20 always hits and does double damage. A roll of 1 always misses.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO4_FMT_STR "Attack rolls are used for all types of attacks, melee, staffs, wands etc...\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO5_FMT_STR "Don't worry, the computer does the rolls for you! And you can trust the computer, right?\n",
                      TEXT_FORMAT_LHS);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_def_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 13;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Defence");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "This is your overall defence roll out of a max of 20.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "If your attacker rolls more than this, you are hit.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO3_FMT_STR "A roll of 20 always hits and does double damage. A roll of 1 always misses.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO4_FMT_STR "Don't worry, the computer does the rolls for you! And you can trust the computer, right?\n",
                      TEXT_FORMAT_LHS);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_str_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 12;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Strength");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "Strength TODO.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_dex_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 12;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Dexterity");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "This is your overall dexterity roll out of a max of 20.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO4_FMT_STR "Use this to break out of tangly spiderwebs and avoid traps.\n", TEXT_FORMAT_LHS);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_dmg_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 10;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Damage");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "This is your damage boost, if any, for all attacks, magical or melee.\n", TEXT_FORMAT_LHS);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_con_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 9;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Constitution");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "Constitution aids you in fending off poison attacks and reducing stamina loss.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_psi_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 9;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Psi");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "Psi rolls aid you in learning spells and fending off attacks of domination.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "Increased Psi reduces spell expense e.g. a +2 Psi cuts costs by 20%%\n", TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_lck_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 12;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Luck");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "Luck can impact your existance in many quiet ways.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "Will that treasure chest explode when you hit it?\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO3_FMT_STR "Will that loose floor tile reveal a hidden chasm?\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO4_FMT_STR "Will you land in lava when jumping into a chasm?\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO5_FMT_STR "Look for horseshoes or clover to increase your luck.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO6_FMT_STR "If you have Lck of 20, then you have a +10 bonus to luck rolls.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO7_FMT_STR "If you have Lck of 5, then you have a -5 penalty to luck rolls.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_health_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 9;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Health");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "Your health score. If it hits zero, bad things happen.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "You can replenish health through discoveries of delicacies such as clown meat and chocolate frogs.\n",
                      TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_mana_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 9;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Mana");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "Mana is used for the learning of and the casting of spells.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "Replenishing Mana is rare and is mainly achieved through additional sacrifices at an alter.\n",
                      TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_stealth_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 10;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Stealth");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "This is how noisy you are being in the dungeon.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g,
                      UI_INFO2_FMT_STR
                      "Clomping through bushes, crushing grass, firing weapons are just some of the ways you can draw attention to yourself in "
                      "non positive ways.\n",
                      TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_stamina_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;
  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  int const width  = 32;
  int const height = 10;

  tlx = UI_LEFTBAR_WIDTH;
  brx = tlx + width;
  bry = tly + height;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  wid_over_stats = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  wid_over_stats->log(g, UI_HIGHLIGHT_FMT_STR "Stamina");
  wid_over_stats->log_empty_line(g);
  wid_over_stats->log(g, UI_INFO1_FMT_STR "Stamina is important for jumping over chasms and throwing items.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO2_FMT_STR "Stamina can be replenished by resting.\n", TEXT_FORMAT_LHS);
  wid_over_stats->log(g, UI_INFO3_FMT_STR "Jumping a chasm with low stamina is not advised...\n", TEXT_FORMAT_LHS);
  wid_over_stats->log_empty_line(g);
  wid_over_stats->compress(g);

  level_cursor_path_reset(g);
}

static void wid_thing_info_stats_mouse_over_end(Gamep g, Widp w)
{
  TRACE();

  delete wid_over_stats;
  wid_over_stats = nullptr;
}

static void wid_thing_info_thing_mouse_over_begin(Gamep g, Widp w, int /*relx*/, int /*rely*/, int /*wheelx*/, int /*wheely*/)
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  level_cursor_describe_clear(g, v);
  (void) level_cursor_describe_add(g, v, t);
  (void) wid_rightbar_init(g);
}

static void wid_thing_info_thing_mouse_over_end(Gamep g, Widp w)
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return;
  }

  (void) level_cursor_describe_remove(g, v, t);
  (void) wid_rightbar_init(g);
}

[[nodiscard]] static auto wid_thing_info_thing_mouse_down(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return false;
  }

  auto *t = wid_get_thing_context(g, v, w, 0);
  if (t == nullptr) {
    return false;
  }

  if (game_state(g) != STATE_PLAYING) {
    return true;
  }

  wid_item_menu_select(g, v, t, false /* not from inventory */);

  return true;
}

[[nodiscard]] auto wid_thing_info_keys(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  auto *text = parent->wid_text_area;
  auto *b    = parent->wid_text_area->wid_text_area;

  auto key_count = thing_keys_carried(me);
  if (key_count == 0) {
    return false;
  }

  {
    auto        *tile = tile_find_mand("key.0");
    auto        *w    = wid_new_square_button(g, b, "Keys");
    spoint const tl(UI_LEFTBAR_WIDTH - 8, text->line_count);
    spoint const br(UI_LEFTBAR_WIDTH - 5, text->line_count + 2);
    wid_set_tile(TILE_LAYER_BOX_BG, w, tile);
    wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
    wid_set_pos(w, tl, br);
  }

  {
    auto        *w = wid_new_square_button(g, b, "Key count");
    spoint const tl(UI_LEFTBAR_WIDTH - 4, text->line_count);
    spoint const br(UI_LEFTBAR_WIDTH - 2, text->line_count + 2);
    std::string  how_many_keys = "x" + std::to_string(key_count);

    if (key_count > 9) {
      how_many_keys = std::to_string(key_count);
    }

    wid_set_text(w, how_many_keys);
    wid_set_shape_none(w);
    wid_set_pos(w, tl, br);
  }

  return true;
}

//
// The thing name
//
[[nodiscard]] auto wid_thing_info_name(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  std::string name_str;
  if (thing_is_player(me)) {
    name_str = game_player_name_get(g);
  } else {
    name_str = thing_name_long(g, v, l, me);
  }
  name_str = capitalize(name_str);

  parent->log(g, UI_INFO_FMT_STR + name_str + UI_RESET_FMT);

  return true;
}

//
// The thing description
//
[[nodiscard]] auto wid_thing_info_detail(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  if (thing_is_dead(me)) {
    return false;
  }

  parent->log(g, thing_detail_get(g, v, l, me), TEXT_FORMAT_LHS);

  return true;
}

//
// How tough the game ie
//
[[nodiscard]] auto wid_thing_info_difficulty(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  switch (tp_difficulty_get(thing_tp(me))) {
    case 0 : //
      parent->log(g, "Difficulty: as easy as it gets...");
      break;
    case 1 : //
      parent->log(g, "Difficulty: hard");
      parent->log(g, UI_INFO2_FMT_STR "Expect more monsters and faster mob spawn chances" UI_RESET_FMT);
      break;
    case 2 : //
      parent->log(g, "Difficulty: harder");
      parent->log(g, UI_YELLOW_FMT_STR "Expect more monsters and faster mob spawn chances" UI_RESET_FMT);
      break;
    case 3 : //
      parent->log(g, "Difficulty: nightmare");
      parent->log(g, UI_ORANGE_FMT_STR "Expect more monsters and faster mob spawn chances" UI_RESET_FMT);
      break;
    case 4 : //
      parent->log(g, "Difficulty: psycho");
      parent->log(g, UI_RED_FMT_STR "Expect even more monsters and faster mob spawn chances" UI_RESET_FMT);
      break;
  }

  return true;
}

//
// Score
//
[[nodiscard]] auto wid_thing_info_score(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  if (! thing_is_player(me)) {
    return false;
  }

  auto *player_struct = thing_player_struct(g);
  if (player_struct == nullptr) {
    return false;
  }

  auto score    = player_struct->score;
  auto hiscore  = game_hiscore_get(g);
  auto maxscore = std::max(score, hiscore);

  auto score_str = string_sprintf(
      // newline
      UI_INFO1_FMT_STR "Score "
      // newline
      UI_INFO2_FMT_STR " %06u "
      // newline
      UI_INFO3_FMT_STR " Hiscore "
      // newline
      UI_INFO4_FMT_STR " %06u",
      // newline
      score,
      // newline
      maxscore);

  parent->log(g, score_str);

  return true;
}

//
// Health bar
//
[[nodiscard]] auto wid_thing_info_health_bar(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp tp, WidPopup *parent, int width) -> bool
{
  TRACE();

  if (! tp_is_shown_health(tp)) {
    return false;
  }

  char line_bar[ MAXSHORTSTR ];

  //
  // "Health            "
  //
  memset(line_bar, 0, sizeof(line_bar));
  memset(line_bar, ' ', sizeof(line_bar) - 1);

  if (thing_is_dead(me)) {
    (void) my_strlcpy(line_bar + 1, "Dead", sizeof("Dead "));
  } else {
    (void) my_strlcpy(line_bar + 1, "Health", sizeof("Health "));
  }

  //
  // "Health         a/b"
  //
  auto health_max = thing_health_max(g, v, l, me);
  auto h          = thing_health(g, v, l, me);
  h               = std::max(h, 0);

  std::string const health_str = std::to_string(h) + "/" + std::to_string(health_max);
  (void) my_strlcpy(line_bar + width - health_str.size() - 3, health_str.c_str(), width - health_str.size());
  line_bar[ strlen(line_bar) ] = ' ';

  //
  // "Health         a/b"
  // "xxxxxxxxxxxxxxxxxx"
  //
  auto *w = parent->log(g, std::string(line_bar));
  if (w != nullptr) {
    int health_how_much = static_cast< int >((static_cast< float >(thing_health(g, v, l, me)) / static_cast< float >(health_max))
                                             * (static_cast< float > UI_STAT_BAR_STEPS - 1));
    health_how_much     = std::min(health_how_much, UI_STAT_BAR_STEPS - 1);
    health_how_much     = std::max(health_how_much, 0);
    auto icon           = "stat_bar." + std::to_string(health_how_much + 1);

    wid_set_shape_square(w);
    wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
    wid_set_color(w, WID_COLOR_TEXT_FG, UI_HIGHLIGHT_COLOR);
    wid_set_tilename(TILE_LAYER_BOX_BG, w, icon);
    wid_set_text_lhs(w, 1u);

    if (thing_is_player(me)) {
      wid_set_on_mouse_over_begin(w, wid_thing_info_stats_health_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
    }
  }

  return true;
}

//
// Mana bar
//
[[nodiscard]] auto wid_thing_info_mana_bar(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp tp, WidPopup *parent, int width) -> bool
{
  TRACE();

  char line_bar[ MAXSHORTSTR ];

  //
  // "Mana            "
  //
  memset(line_bar, 0, sizeof(line_bar));
  memset(line_bar, ' ', sizeof(line_bar) - 1);

  (void) my_strlcpy(line_bar + 1, "Mana", sizeof("Mana"));

  //
  // "Mana         a/b"
  //
  auto mana_max = thing_mana_max(g, v, l, me);
  auto h        = thing_mana(g, v, l, me);
  h             = std::max(h, 0);

  std::string const mana_str = std::to_string(h) + "/" + std::to_string(mana_max);
  (void) my_strlcpy(line_bar + width - mana_str.size() - 3, mana_str.c_str(), width - mana_str.size());
  line_bar[ strlen(line_bar) ] = ' ';

  //
  // "Mana         a/b"
  // "xxxxxxxxxxxxxxxxxx"
  //
  auto *w = parent->log(g, std::string(line_bar));
  if (w != nullptr) {
    int mana_how_much = static_cast< int >((static_cast< float >(thing_mana(g, v, l, me)) / static_cast< float >(mana_max))
                                           * (static_cast< float > UI_STAT_BAR_STEPS - 1));
    mana_how_much     = std::min(mana_how_much, UI_STAT_BAR_STEPS - 1);
    mana_how_much     = std::max(mana_how_much, 0);
    auto icon         = "stat_bar." + std::to_string(mana_how_much + 1);

    wid_set_shape_square(w);
    wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
    wid_set_color(w, WID_COLOR_TEXT_FG, UI_HIGHLIGHT_COLOR);
    wid_set_tilename(TILE_LAYER_BOX_BG, w, icon);
    wid_set_text_lhs(w, 1u);

    if (thing_is_player(me)) {
      wid_set_on_mouse_over_begin(w, wid_thing_info_stats_mana_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
    }
  }

  return true;
}

//
// Stamina bar
//
[[nodiscard]] auto wid_thing_info_stamina_bar(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp tp, WidPopup *parent, int width) -> bool
{
  TRACE();

  if (thing_is_dead(me)) {
    return false;
  }

  if (! tp_is_shown_stamina(tp)) {
    return false;
  }

  char line_bar[ MAXSHORTSTR ];

  //
  // "Stamina           "
  //
  memset(line_bar, 0, sizeof(line_bar));
  memset(line_bar, ' ', sizeof(line_bar) - 1);

  if (thing_distance_jump(g, v, l, me) != thing_distance_jump_max(g, v, l, me)) {
    (void) my_strlcpy(line_bar + 1, "Jumping impacted", sizeof("Jumping impacted "));
  } else {
    (void) my_strlcpy(line_bar + 1, "Stamina", sizeof("Stamina "));
  }

  //
  // "Stamina        a/b"
  //
  auto stamina_max = thing_stamina_max(g, v, l, me);
  auto stamina     = thing_stamina(g, v, l, me);
  stamina          = std::max(stamina, 0);

  std::string const stamina_str = std::to_string(stamina) + "/" + std::to_string(stamina_max);
  (void) my_strlcpy(line_bar + width - stamina_str.size() - 3, stamina_str.c_str(), width - stamina_str.size());
  line_bar[ strlen(line_bar) ] = ' ';

  //
  // "Stamina        a/b"
  // "xxxxxxxxxxxxxxxxxx"
  //
  auto *w = parent->log(g, std::string(line_bar));
  if (w != nullptr) {
    int stamina_how_much
        = static_cast< int >((static_cast< float >(stamina) / static_cast< float >(stamina_max)) * (static_cast< float > UI_STAT_BAR_STEPS - 1));
    stamina_how_much = std::min(stamina_how_much, UI_STAT_BAR_STEPS - 1);
    stamina_how_much = std::max(stamina_how_much, 0);
    auto icon        = "stat_bar." + std::to_string(stamina_how_much + 1);

    wid_set_shape_square(w);
    wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
    wid_set_color(w, WID_COLOR_TEXT_FG, UI_HIGHLIGHT_COLOR);
    wid_set_tilename(TILE_LAYER_BOX_BG, w, icon);
    wid_set_text_lhs(w, 1u);

    if (thing_is_player(me)) {
      wid_set_on_mouse_over_begin(w, wid_thing_info_stats_stamina_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
    }
  }

  return true;
}

//
// Stealth bar
//
[[nodiscard]] static auto wid_thing_info_stealth_bar(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp tp, WidPopup *parent, int width) -> bool
{
  TRACE();

  if (thing_is_dead(me)) {
    return false;
  }

  if (! tp_is_shown_noise(tp)) {
    return false;
  }

  char line_bar[ MAXSHORTSTR ];

  //
  // "Stealth           "
  //
  memset(line_bar, 0, sizeof(line_bar));
  memset(line_bar, ' ', sizeof(line_bar) - 1);
  (void) my_strlcpy(line_bar + 1, "Stealth", sizeof("Stealth "));

  //
  // "Stealth        a/b"
  //
  auto stealth_max = THING_NOISE_MAX;
  auto stealth     = THING_NOISE_MAX - thing_noise(g, v, l, me);
  stealth          = std::max(stealth, 0);

  std::string const stealth_str = std::to_string(stealth) + "/" + std::to_string(stealth_max);
  (void) my_strlcpy(line_bar + width - stealth_str.size() - 3, stealth_str.c_str(), width - stealth_str.size());
  line_bar[ strlen(line_bar) ] = ' ';

  //
  // "Stealth        a/b"
  // "xxxxxxxxxxxxxxxxxx"
  //
  auto *w = parent->log(g, std::string(line_bar));
  if (w != nullptr) {
    int stealth_how_much
        = static_cast< int >((static_cast< float >(stealth) / static_cast< float >(stealth_max)) * (static_cast< float > UI_STAT_BAR_STEPS - 1));
    stealth_how_much = std::min(stealth_how_much, UI_STAT_BAR_STEPS - 1);
    stealth_how_much = std::max(stealth_how_much, 0);
    auto icon        = "stat_bar." + std::to_string(stealth_how_much + 1);

    wid_set_shape_square(w);
    wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
    wid_set_color(w, WID_COLOR_TEXT_FG, UI_HIGHLIGHT_COLOR);
    wid_set_tilename(TILE_LAYER_BOX_BG, w, icon);
    wid_set_text_lhs(w, 1u);

    if (thing_is_player(me)) {
      wid_set_on_mouse_over_begin(w, wid_thing_info_stats_stealth_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
    }
  }

  return true;
}

//
// Add immunities
//
[[nodiscard]] auto wid_thing_info_immunity(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent, int /*width*/) -> bool
{
  TRACE();

  std::string out;

  FOR_ALL_THING_EVENT(e)
  {
    if (! thing_is_immune_to(g, v, l, me, e)) {
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

    out = string_append_with_comma(out, capitalize(ThingEventType_to_string(e)));
  }

  if (out.empty()) {
    return false;
  }

  parent->log(g, UI_INFO_FMT_STR "Immunity:", TEXT_FORMAT_LHS);
  parent->log(g, "- " + out, TEXT_FORMAT_LHS);

  return true;
}

//
// Add stats
//
[[nodiscard]] static auto wid_thing_info_player_stats(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent, int /*width*/) -> bool
{
  TRACE();

  auto *text = parent->wid_text_area;
  auto *b    = parent->wid_text_area->wid_text_area;

  {
    auto         out = thing_stat_string(g, v, l, me, THING_STAT_ATT);
    auto        *w   = wid_new_bright_button(g, b, "Att");
    spoint const tl(6, text->line_count);
    spoint const br(15, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_att_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  {
    auto         out = thing_stat_string(g, v, l, me, THING_STAT_DEF);
    auto        *w   = wid_new_bright_button(g, b, "Def");
    spoint const tl(16, text->line_count);
    spoint const br(25, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_def_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  parent->log_empty_line(g);
  parent->log_empty_line(g);
  parent->log_empty_line(g);

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_STR);
    auto        *w   = wid_new_bright_button(g, b, "Str");
    spoint const tl(1, text->line_count);
    spoint const br(10, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_str_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_CON);
    auto        *w   = wid_new_bright_button(g, b, "Con");
    spoint const tl(11, text->line_count);
    spoint const br(20, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_con_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_DEX);
    auto        *w   = wid_new_bright_button(g, b, "Dex");
    spoint const tl(21, text->line_count);
    spoint const br(30, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_dex_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  parent->log_empty_line(g);
  parent->log_empty_line(g);
  parent->log_empty_line(g);

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_DMG);
    auto        *w   = wid_new_bright_button(g, b, "Dmg");
    spoint const tl(1, text->line_count);
    spoint const br(10, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_dmg_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_LUCK);
    auto        *w   = wid_new_bright_button(g, b, "Lck");
    spoint const tl(11, text->line_count);
    spoint const br(20, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_lck_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_PSI);
    auto        *w   = wid_new_bright_button(g, b, "Psi");
    spoint const tl(21, text->line_count);
    spoint const br(30, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_psi_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  parent->log_empty_line(g);
  parent->log_empty_line(g);
  parent->log_empty_line(g);
  parent->log_empty_line(g);

  return true;
}

//
// Add stats
//
[[nodiscard]] static auto wid_thing_info_monst_stats(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent, int /*width*/) -> bool
{
  TRACE();

  auto *text = parent->wid_text_area;
  auto *b    = parent->wid_text_area->wid_text_area;

  {
    auto         out = thing_stat_string(g, v, l, me, THING_STAT_ATT);
    auto        *w   = wid_new_square_button(g, b, "Att");
    spoint const tl(6, text->line_count);
    spoint const br(15, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
  }

  {
    auto         out = thing_stat_string(g, v, l, me, THING_STAT_DEF);
    auto        *w   = wid_new_square_button(g, b, "Def");
    spoint const tl(16, text->line_count);
    spoint const br(25, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
  }

  parent->log_empty_line(g);
  parent->log_empty_line(g);
  parent->log_empty_line(g);
  parent->log_empty_line(g);

  return true;
}

//
// Add stats
//
[[nodiscard]] static auto wid_thing_info_item_stats(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent, int /*width*/) -> bool
{
  TRACE();

  auto *text = parent->wid_text_area;
  auto *b    = parent->wid_text_area->wid_text_area;

  bool any_set = false;

  FOR_ALL_THING_STAT(stat)
  {
    if (thing_stat_mod(g, v, l, me, stat) != 0) {
      any_set = true;
    }
  }

  if (! any_set) {
    return false;
  }

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_ATT);
    auto        *w   = wid_new_square_button(g, b, "Att");
    spoint const tl(6, text->line_count);
    spoint const br(15, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
  }

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_DEF);
    auto        *w   = wid_new_square_button(g, b, "Def");
    spoint const tl(16, text->line_count);
    spoint const br(25, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
  }

  parent->log_empty_line(g);
  parent->log_empty_line(g);
  parent->log_empty_line(g);

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_STR);
    auto        *w   = wid_new_bright_button(g, b, "Str");
    spoint const tl(1, text->line_count);
    spoint const br(10, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_str_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_CON);
    auto        *w   = wid_new_bright_button(g, b, "Con");
    spoint const tl(11, text->line_count);
    spoint const br(20, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_con_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_DEX);
    auto        *w   = wid_new_bright_button(g, b, "Dex");
    spoint const tl(21, text->line_count);
    spoint const br(30, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_dex_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  parent->log_empty_line(g);
  parent->log_empty_line(g);
  parent->log_empty_line(g);

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_DMG);
    auto        *w   = wid_new_bright_button(g, b, "Dmg");
    spoint const tl(1, text->line_count);
    spoint const br(10, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_dmg_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_LUCK);
    auto        *w   = wid_new_bright_button(g, b, "Lck");
    spoint const tl(11, text->line_count);
    spoint const br(20, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_lck_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  {
    auto         out = thing_stat_mod_string(g, v, l, me, THING_STAT_PSI);
    auto        *w   = wid_new_bright_button(g, b, "Psi");
    spoint const tl(21, text->line_count);
    spoint const br(30, text->line_count + 2);
    wid_set_pos(w, tl, br);
    wid_set_text(w, out);
    wid_set_on_mouse_over_begin(w, wid_thing_info_stats_psi_mouse_over_begin);
    wid_set_on_mouse_over_end(w, wid_thing_info_stats_mouse_over_end);
  }

  parent->log_empty_line(g);
  parent->log_empty_line(g);
  parent->log_empty_line(g);
  parent->log_empty_line(g);

  return true;
}

//
// Add resistances
//
[[nodiscard]] auto wid_thing_info_resistance(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent, int /*width*/) -> bool
{
  TRACE();

  std::string out;

  FOR_ALL_THING_EVENT(e)
  {
    if (! thing_is_resistant_to(g, v, l, me, e)) {
      continue;
    }

    bool show_string = false;

    switch (e) {
      case THING_EVENT_SHOVED :           [[fallthrough]];
      case THING_EVENT_CRUSH_DAMAGE :     [[fallthrough]];
      case THING_EVENT_ENERGY_DAMAGE :    [[fallthrough]];
      case THING_EVENT_MELEE_DAMAGE :     [[fallthrough]];
      case THING_EVENT_POISON_DAMAGE :    [[fallthrough]];
      case THING_EVENT_THROWN_DAMAGE :    [[fallthrough]];
      case THING_EVENT_ENGULF_DAMAGE :    [[fallthrough]];
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

    out = string_append_with_comma(out, capitalize(ThingEventType_to_string(e)));
  }

  if (out.empty()) {
    return false;
  }

  parent->log(g, UI_INFO_FMT_STR "Resistances (half damage):", TEXT_FORMAT_LHS);
  parent->log(g, "- " + out, TEXT_FORMAT_LHS);

  return true;
}

//
// Add abilities
//
[[nodiscard]] static auto wid_thing_info_abilities(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  std::string out;

  auto *tp = thing_tp(me);

  if (thing_is_venomous(me)) {
    out = string_append_with_comma(out, "Venomous");
  }

  if (thing_is_ethereal(g, v, l, me)) {
    //
    // Filter other things that are obvious for being ethereal
    //
    out = string_append_with_comma(out, "Ethereal");
  } else {
    //
    // Non ethereal
    //
    if (thing_is_able_to_move_through_walls(me)) {
      out = string_append_with_comma(out, "Wall-walker");
    }
    if (thing_is_levitating(g, v, l, me)) {
      out = string_append_with_comma(out, "Levitating");
    }
  }

  if (thing_is_able_to_see_invisible(me)) {
    out = string_append_with_comma(out, "See-invis");
  }
  if (thing_is_able_to_resurrect(me)) {
    out = string_append_with_comma(out, "Resurrection");
  }
  if (thing_is_able_to_see_through_walls(me)) {
    out = string_append_with_comma(out, "Xray-vision");
  }
  if (thing_is_gaseous(me)) {
    out = string_append_with_comma(out, "Gaseous");
  }
  if (thing_is_slime(me)) {
    out = string_append_with_comma(out, "Slimey");
  }
  if (thing_is_able_to_collect_items(me)) {
    if (! thing_is_player(me)) {
      out = string_append_with_comma(out, "Collector");
    }
  }
  if (thing_is_able_to_engulf(me)) {
    if (! thing_is_player(me)) {
      out = string_append_with_comma(out, "Engulfer");
    }
  }
  if (thing_is_able_to_ensnare(me)) {
    if (! thing_is_player(me)) {
      out = string_append_with_comma(out, "Ensnarer");
    }
  }
  if (thing_is_able_to_jump(g, v, l, me)) {
    if (! thing_is_player(me)) {
      out = string_append_with_comma(out, "Jumper");
    }
  }
  if (thing_is_able_to_throw(me)) {
    if (! thing_is_player(me)) {
      out = string_append_with_comma(out, "Thrower");
    }
  }
  if (thing_is_able_to_wear_items(me)) {
    if (! thing_is_player(me)) {
      out = string_append_with_comma(out, "Wielder");
    }
  }
  if (thing_is_burning(me)) {
    if (! thing_is_player(me)) {
      out = string_append_with_comma(out, "Burnable");
    }
  }
  if (thing_is_flammable(me)) {
    if (! thing_is_player(me)) {
      out = string_append_with_comma(out, "Flammable");
    }
  }
  if (thing_is_combustible(me)) {
    if (! thing_is_player(me)) {
      out = string_append_with_comma(out, "Combustible");
    }
  }
  if (! thing_is_able_to_be_ensnared(g, v, l, me)) {
    if (thing_is_player(me) || thing_is_monst(me)) {
      out = string_append_with_comma(out, "Snare-proof");
    }
  }
  if (! thing_is_able_to_be_engulfed(g, v, l, me)) {
    if (thing_is_player(me) || thing_is_monst(me)) {
      out = string_append_with_comma(out, "Engulf-proof");
    }
  }
  if (thing_is_able_to_jump_land_then_pounce(g, v, l, me)) {
    out = string_append_with_comma(out, "Jump-pounce");
  }
  if (tp_attack_count_max_per_tick_get(tp) > 1) {
    out = string_sprintf_append_with_comma(out, "Multi-attack(%u)", tp_attack_count_max_per_tick_get(tp));
  }

  {
    auto *player = thing_player(g);
    if (player != nullptr) {
      const float player_speed = thing_speed(g, v, l, player);

      auto pct = static_cast< int >((thing_speed(g, v, l, me) / player_speed) * 100.0);

      if (thing_speed(g, v, l, me) > player_speed) {
        out = string_sprintf_append_with_comma(out, "Faster(%u%%%%%%)", pct);
      } else if (thing_speed(g, v, l, me) < player_speed) {
        out = string_sprintf_append_with_comma(out, "Slower(%u%%%%%%)", pct);
      }
    }
  }

  if (out.empty()) {
    return false;
  }

  parent->log(g, UI_INFO_FMT_STR "Abilities:", TEXT_FORMAT_LHS);
  parent->log(g, "- " + out, TEXT_FORMAT_LHS);

  return true;
}

//
// Add what the thing hates
//
[[nodiscard]] static auto wid_thing_info_hates(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  std::string out;

  for (auto i = 1; i < TP_ID_MAX; i++) {

    auto *it = tp_find(i);
    if (it == nullptr) {
      continue;
    }

    switch (thing_assess_tp(g, v, l, it, me)) {
      case THING_ENVIRON_HATES :    out = string_append_with_comma(out, capitalize_first(tp_name_long(it))); break;
      case THING_ENVIRON_DISLIKES : break;
      case THING_ENVIRON_NEUTRAL :  break;
      case THING_ENVIRON_LIKES :    break;
      case THING_ENVIRON_ENUM_MAX : break;
    }
  }

  if (out.empty()) {
    return false;
  }

  replace(out, "Deep water, Shallow water", "All water");

  parent->log(g, UI_INFO_FMT_STR "Hates:", TEXT_FORMAT_LHS);
  parent->log(g, "- " + out, TEXT_FORMAT_LHS);

  return true;
}

//
// Add what the thing dislikes
//
[[nodiscard]] static auto wid_thing_info_dislikes(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  std::string out;

  for (auto i = 1; i < TP_ID_MAX; i++) {

    auto *it = tp_find(i);
    if (it == nullptr) {
      continue;
    }

    switch (thing_assess_tp(g, v, l, it, me)) {
      case THING_ENVIRON_HATES :    break;
      case THING_ENVIRON_DISLIKES : out = string_append_with_comma(out, capitalize_first(tp_name_long(it))); break;
      case THING_ENVIRON_NEUTRAL :  break;
      case THING_ENVIRON_LIKES :    break;
      case THING_ENVIRON_ENUM_MAX : break;
    }
  }

  if (out.empty()) {
    return false;
  }

  replace(out, "Deep water, Shallow water", "All water");

  parent->log(g, UI_INFO_FMT_STR "Dislikes:", TEXT_FORMAT_LHS);
  parent->log(g, "- " + out, TEXT_FORMAT_LHS);

  return true;
}

//
// Add worn weapon
//
[[nodiscard]] static auto wid_thing_info_worn(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent, int width) -> bool
{
  TRACE();

  bool worn {};

  FOR_ALL_WORN_TYPES(w)
  {
    auto *item = thing_worn_get(g, v, l, me, w);
    if (item == nullptr) {
      continue;
    }

    std::string line;

    switch (w) {
      case WORN_TYPE_WEAPON :
        {
          line = string_sprintf("Wielded(%s)", thing_name_long(g, v, l, item).c_str());
          worn = true;
          break;
        }
      case WORN_TYPE_BOOTS : [[fallthrough]];
      case WORN_TYPE_RING1 : [[fallthrough]];
      case WORN_TYPE_RING2 :
        {
          line = string_sprintf("Worn(%s)", thing_name_long(g, v, l, item).c_str());
          worn = true;
          break;
        }

      case WORN_TYPE_NONE :
      case WORN_TYPE_ENUM_MAX : break;
    }

    if (line.empty()) {
      continue;
    }

    Widp wid = parent->log(g, UI_INFO_FMT_STR + line, TEXT_FORMAT_LHS);

    wid_set_thing_context(g, v, wid, item);
    wid_set_on_mouse_down(wid, wid_thing_info_thing_mouse_down);
    wid_set_on_mouse_over_begin(wid, wid_thing_info_thing_mouse_over_begin);
    wid_set_on_mouse_over_end(wid, wid_thing_info_thing_mouse_over_end);

    (void) wid_tp_info_damage(g, v, l, thing_tp(item), parent, width, false /* title allowed */);
    (void) wid_tp_info_special_attacks(g, v, l, thing_tp(item), parent, width, false /* title allowed */);
  }

  return worn;
}

//
// Add danger level
//
[[nodiscard]] static auto wid_thing_info_danger(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return false;
  }

  //
  // Check for things mathing the dice roll first.
  //
  auto max_damage = thing_damage_max(g, v, l, me);
  if (max_damage == 0) {
    return false;
  }

  auto monst_defeat_count = thing_health(g, v, l, player) / max_damage;

  //
  // Oh dear. You my friend are toast.
  //
  if (monst_defeat_count == 0) {
    monst_defeat_count = 1;
  }

  parent->log(g, UI_INFO_FMT_STR "Danger level:", TEXT_FORMAT_LHS);

  if (monst_defeat_count == 1) {
    parent->log(g, UI_IMPORTANT_FMT_STR "- Could defeat you in " + std::to_string(monst_defeat_count) + " hit!", TEXT_FORMAT_LHS);
  } else if (monst_defeat_count <= 2) {
    parent->log(g, UI_IMPORTANT_FMT_STR "- Could defeat you in " + std::to_string(monst_defeat_count) + " hits", TEXT_FORMAT_LHS);
  } else if (monst_defeat_count <= 5) {
    parent->log(g, UI_WARN_FMT_STR "- Could defeat you in " + std::to_string(monst_defeat_count) + " hits", TEXT_FORMAT_LHS);
  } else if (monst_defeat_count <= 10) {
    parent->log(g, "- Could defeat you in " + std::to_string(monst_defeat_count) + " hits", TEXT_FORMAT_LHS);
  } else {
    parent->log(g, "- Could defeat you eventually.", TEXT_FORMAT_LHS);
  }

  auto player_max_damage = thing_damage_max(g, v, l, player);
  if (player_max_damage != 0) {
    auto player_defeat_count = thing_health(g, v, l, me) / player_max_damage;

    //
    // Oh dear. The monst is toast.
    //
    if (player_defeat_count == 0) {
      player_defeat_count = 1;
    }

    if (player_defeat_count == 1) {
      parent->log(g, "- You could beat it in " + std::to_string(player_defeat_count) + " hit.", TEXT_FORMAT_LHS);
      parent->log(g, "- More likely, " + std::to_string(player_defeat_count * 2) + " hits", TEXT_FORMAT_LHS);
    } else if (player_defeat_count <= 2) {
      parent->log(g, "- You could beat it in " + std::to_string(player_defeat_count) + " hits.", TEXT_FORMAT_LHS);
      parent->log(g, "- More likely, " + std::to_string(player_defeat_count * 2) + " hits.", TEXT_FORMAT_LHS);
    } else if (player_defeat_count <= 10) {
      parent->log(g, "- You could beat it in " + std::to_string(player_defeat_count) + " hits.", TEXT_FORMAT_LHS);
      parent->log(g, "- More likely, " + std::to_string(player_defeat_count * 2) + " hits.", TEXT_FORMAT_LHS);
    } else {
      parent->log(g, "- Will take many hits to beat.");
    }
  }

  return true;
}

//
// Add warnings if invulnerable
//
[[nodiscard]] static auto wid_thing_info_invulnerable(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return false;
  }

  auto damage_types = thing_damage_types(g, v, l, player);
  bool got_one      = false;

  FOR_ALL_THING_EVENT(e)
  {
    if (! thing_is_immune_to(g, v, l, me, e)) {
      for (auto d : damage_types) {
        if (d == THING_EVENT_MELEE_DAMAGE) {
          continue;
        }
        if (d == e) {
          got_one = true;
        }
      }
    }
  }

  if (! got_one) {
    parent->log(g, UI_IMPORTANT_FMT_STR "- May be invulnerable to attack", TEXT_FORMAT_LHS);
    return true;
  }

  return false;
}

//
// Add warnings if invulnerable
//
[[nodiscard]] static auto wid_thing_info_resistant(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    return false;
  }

  auto damage_types = thing_damage_types(g, v, l, player);
  bool got_one      = false;

  FOR_ALL_THING_EVENT(e)
  {
    if (! thing_is_resistant_to(g, v, l, me, e)) {
      for (auto d : damage_types) {
        if (d == THING_EVENT_MELEE_DAMAGE) {
          continue;
        }
        if (d == e) {
          got_one = true;
        }
      }
    }
  }

  if (! got_one) {
    parent->log(g, UI_WARN_FMT_STR "- May be resistant to attack", TEXT_FORMAT_LHS);
    return true;
  }

  return false;
}

[[nodiscard]] static auto wid_thing_info_items(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  bool printed_something = false;

  if (! thing_is_able_to_collect_items(me)) {
    return printed_something;
  }

  bool first = true;

  FOR_ALL_INVENTORY_SLOTS(g, v, l, me, slot, item)
  {
    auto *item_tp = (item != nullptr) ? thing_tp(item) : nullptr;
    if (item_tp == nullptr) {
      continue;
    }

    if (first) {
      first = false;
      (void) parent->log(g, UI_INFO_FMT_STR "Carrying:", TEXT_FORMAT_LHS);
    }

    printed_something = true;

    std::string line = "- ";

    line += "%%tp=";
    line += tp_name(item_tp);
    line += "$";
    line += " ";
    line += thing_name_long(g, v, l, item);

    if (slot->count > 1) {
      line += " x";
      line += std::to_string(slot->count);
    }

    line += " ";

    auto charge_count = thing_charge_count(item);
    if (charge_count > 0) {
      line += std::to_string(charge_count);
      line += "%%tile=icon_lightning$";
    }

    if (thing_is_worn(item)) {
      if (thing_is_boots(item)) {
        line += "%%tile=icon_boots$";
      } else if (thing_is_ring(item)) {
        line += "%%tile=icon_ring$";
      } else {
        line += "%%tile=icon_hand$";
      }
    }

    Widp wid = parent->log(g, line, TEXT_FORMAT_LHS);

    wid_set_thing_context(g, v, wid, item);
    wid_set_on_mouse_down(wid, wid_thing_info_thing_mouse_down);
    wid_set_on_mouse_over_begin(wid, wid_thing_info_thing_mouse_over_begin);
    wid_set_on_mouse_over_end(wid, wid_thing_info_thing_mouse_over_end);
  }

  return printed_something;
}

[[nodiscard]] static auto wid_thing_sacrifices(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  bool printed_something = false;
  bool first             = true;

  FOR_ALL_SACRIFICES(g, v, l, me, sacrifice)
  {
    if (first) {
      first = false;
      (void) parent->log(g, UI_INFO_FMT_STR "Sacrifices:", TEXT_FORMAT_LHS);
    }

    printed_something = true;

    std::string line = "- ";

    line += thing_name_long(g, v, l, sacrifice);
    line += " ";

    Widp wid = parent->log(g, line, TEXT_FORMAT_LHS);

    wid_set_thing_context(g, v, wid, sacrifice);
    wid_set_on_mouse_over_begin(wid, wid_thing_info_thing_mouse_over_begin);
    wid_set_on_mouse_over_end(wid, wid_thing_info_thing_mouse_over_end);
  }

  return printed_something;
}

//
// Buffs
//
[[nodiscard]] static auto wid_thing_info_buffs(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  bool printed_something = false;

  if (! thing_is_player(me)) {
    return printed_something;
  }

  bool first = true;

  FOR_ALL_BUFFS(g, v, l, me, buff)
  {
    if (first) {
      first = false;
      (void) parent->log(g, "Buffs:", TEXT_FORMAT_LHS);
    }

    printed_something = true;

    std::string line = "- ";

    line += thing_name_short(g, v, l, buff);

    auto lifespan = thing_lifespan(g, v, l, buff);
    if (lifespan > 0) {
      line += ", ";
      line += std::to_string(lifespan);
      if (lifespan == 1) {
        line += " move (last move)";
      } else {
        line += " moves";
      }
    }

    char line_bar[ MAXSHORTSTR ];

    memset(line_bar, 0, sizeof(line_bar));
    memset(line_bar, ' ', sizeof(line_bar) - 1);

    auto lifespan_max = thing_lifespan_initial(buff);
    auto h            = thing_lifespan(g, v, l, buff);
    h                 = std::max(h, 0);

    (void) my_strlcpy(line_bar, line.c_str(), line.size() + 1);
    auto *w = parent->log(g, std::string(line_bar));
    if (w != nullptr) {
      int lifespan_how_much = static_cast< int >((static_cast< float >(thing_lifespan(g, v, l, buff)) / static_cast< float >(lifespan_max))
                                                 * (static_cast< float > UI_STAT_BAR_STEPS - 1));
      lifespan_how_much     = std::min(lifespan_how_much, UI_STAT_BAR_STEPS - 1);
      lifespan_how_much     = std::max(lifespan_how_much, 0);
      auto icon             = "stat_bar." + std::to_string(lifespan_how_much + 1);

      wid_set_shape_square(w);
      wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
      wid_set_color(w, WID_COLOR_TEXT_FG, UI_HIGHLIGHT_COLOR);
      wid_set_tilename(TILE_LAYER_BOX_BG, w, icon);
      wid_set_text_lhs(w, 1u);
      wid_set_thing_context(g, v, w, buff);
      wid_set_on_mouse_over_begin(w, wid_thing_info_thing_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_thing_info_thing_mouse_over_end);
    }
  }

  return printed_something;
}

[[nodiscard]] static auto wid_thing_info_debuffs(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  bool printed_something = false;

  if (! thing_is_player(me)) {
    return printed_something;
  }

  bool first = true;

  FOR_ALL_DEBUFFS(g, v, l, me, buff)
  {
    if (! thing_is_debuff(buff)) {
      continue;
    }

    if (first) {
      first = false;
      (void) parent->log(g, "Debuffs:", TEXT_FORMAT_LHS);
    }

    printed_something = true;

    std::string line = "- ";

    line += thing_name_short(g, v, l, buff);

    auto lifespan = thing_lifespan(g, v, l, buff);
    if (lifespan > 0) {
      line += ", ";
      line += std::to_string(lifespan);
      if (lifespan == 1) {
        line += " move (last move)";
      } else {
        line += " moves";
      }
    }

    char line_bar[ MAXSHORTSTR ];

    memset(line_bar, 0, sizeof(line_bar));
    memset(line_bar, ' ', sizeof(line_bar) - 1);

    auto lifespan_max = thing_lifespan_initial(buff);
    auto h            = thing_lifespan(g, v, l, buff);
    h                 = std::max(h, 0);

    (void) my_strlcpy(line_bar, line.c_str(), line.size() + 1);
    auto *w = parent->log(g, std::string(line_bar));
    if (w != nullptr) {
      int lifespan_how_much = static_cast< int >((static_cast< float >(thing_lifespan(g, v, l, buff)) / static_cast< float >(lifespan_max))
                                                 * (static_cast< float > UI_STAT_BAR_STEPS - 1));
      lifespan_how_much     = std::min(lifespan_how_much, UI_STAT_BAR_STEPS - 1);
      lifespan_how_much     = std::max(lifespan_how_much, 0);
      auto icon             = "debuff_bar." + std::to_string(lifespan_how_much + 1);

      wid_set_shape_square(w);
      wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
      wid_set_color(w, WID_COLOR_TEXT_FG, UI_HIGHLIGHT_COLOR);
      wid_set_tilename(TILE_LAYER_BOX_BG, w, icon);
      wid_set_text_lhs(w, 1u);
      wid_set_thing_context(g, v, w, buff);
      wid_set_on_mouse_over_begin(w, wid_thing_info_thing_mouse_over_begin);
      wid_set_on_mouse_over_end(w, wid_thing_info_thing_mouse_over_end);
    }
  }

  return printed_something;
}

//
// Display detailed thing information
//
void wid_thing_info(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent, int width)
{
  ////////////////////////////////////////////////////////////////////////////////
  // Careful here. If we invoked the random number generator in here it can throw
  // off tests.
  ////////////////////////////////////////////////////////////////////////////////

  TRACE();

  if (me == nullptr) {
    return;
  }

  auto *tp = thing_tp(me);
  if (tp == nullptr) [[unlikely]] {
    return;
  }

  if (wid_tp_info_icon(g, tp, parent)) {
    parent->log_empty_line(g);
  }

  (void) wid_thing_info_keys(g, v, l, me, parent);

  if (wid_thing_info_name(g, v, l, me, parent)) {
    parent->log_empty_line(g);
  }

  if (thing_is_player(me)) {
    if (level_is_level_select(g, v, l)) {
      if (wid_thing_info_detail(g, v, l, me, parent)) {
        parent->log_empty_line(g);
      }

      if (wid_thing_info_difficulty(g, v, l, me, parent)) {
        parent->log_empty_line(g);
      }
    } else {
      if (wid_thing_info_score(g, v, l, me, parent)) {
        parent->log_empty_line(g);
      }
    }
  } else {
    if (wid_thing_info_detail(g, v, l, me, parent)) {
      parent->log_empty_line(g);
    }
  }

  if (wid_thing_info_health_bar(g, v, l, me, tp, parent, width)) {}

  if (wid_thing_info_stamina_bar(g, v, l, me, tp, parent, width)) {}

  if (thing_is_player(me)) {
    if (wid_thing_info_mana_bar(g, v, l, me, tp, parent, width)) {}
  }

  if (wid_thing_info_stealth_bar(g, v, l, me, tp, parent, width)) {}

  parent->log_empty_line(g);

  if (wid_thing_info_buffs(g, v, l, me, parent)) {
    parent->log_empty_line(g);
  }

  if (wid_thing_info_debuffs(g, v, l, me, parent)) {
    parent->log_empty_line(g);
  }

  if (thing_is_player(me)) {
    if (wid_thing_info_player_stats(g, v, l, me, parent, width)) {}

    if (wid_thing_info_worn(g, v, l, me, parent, width)) {
      parent->log_empty_line(g);
    }
    if (wid_tp_info_damage(g, v, l, tp, parent, width, true /* title allowed */)) {
      parent->log_empty_line(g);
    }

    if (wid_tp_info_special_attacks(g, v, l, tp, parent, width, true /* title allowed */)) {
      parent->log_empty_line(g);
    }

    if (wid_thing_info_immunity(g, v, l, me, parent, width)) {
      parent->log_empty_line(g);
    }

    if (wid_thing_info_resistance(g, v, l, me, parent, width)) {
      parent->log_empty_line(g);
    }

    if (wid_thing_info_abilities(g, v, l, me, parent)) {
      parent->log_empty_line(g);
    }
  } else if (thing_is_monst(me)) {
    if (! thing_is_dead(me)) {
      if (wid_thing_info_monst_stats(g, v, l, me, parent, width)) {}

      if (wid_thing_info_worn(g, v, l, me, parent, width)) {
        parent->log_empty_line(g);
      }

      if (wid_tp_info_damage(g, v, l, tp, parent, width, true /* title allowed */)) {
        parent->log_empty_line(g);
      }

      if (wid_tp_info_special_attacks(g, v, l, tp, parent, width, true /* title allowed */)) {
        parent->log_empty_line(g);
      }

      if (wid_thing_info_immunity(g, v, l, me, parent, width)) {
        parent->log_empty_line(g);
      }

      if (wid_thing_info_resistance(g, v, l, me, parent, width)) {
        parent->log_empty_line(g);
      }

      if (wid_thing_info_abilities(g, v, l, me, parent)) {
        parent->log_empty_line(g);
      }

      //
      // Just shows too much information for things like eels that hate most things
      //
      if (compiler_unused) {
        if (wid_thing_info_hates(g, v, l, me, parent)) {
          parent->log_empty_line(g);
        }

        if (wid_thing_info_dislikes(g, v, l, me, parent)) {
          parent->log_empty_line(g);
        }
      }

      if (wid_thing_info_danger(g, v, l, me, parent)) {
        if (! wid_thing_info_invulnerable(g, v, l, me, parent)) {
          (void) wid_thing_info_resistant(g, v, l, me, parent);
        }
        parent->log_empty_line(g);
      }
    }
  } else if (thing_is_potion(me)) {
    //
    // Keep it terse
    //
  } else {
    if (wid_thing_info_item_stats(g, v, l, me, parent, width)) {}

    if (wid_thing_info_immunity(g, v, l, me, parent, width)) {
      parent->log_empty_line(g);
    }

    if (wid_thing_info_resistance(g, v, l, me, parent, width)) {
      parent->log_empty_line(g);
    }

    if (wid_thing_info_abilities(g, v, l, me, parent)) {
      parent->log_empty_line(g);
    }

    if (wid_tp_info_damage(g, v, l, tp, parent, width, true /* title allowed */)) {
      parent->log_empty_line(g);
    }

    if (wid_tp_info_special_attacks(g, v, l, tp, parent, width, true /* title allowed */)) {
      parent->log_empty_line(g);
    }

    auto *fire_what_tp = thing_on_use_weapon_request(g, v, l, me, nullptr /* intentional to avoid use */);
    if (fire_what_tp != nullptr) {
      auto charge_count = thing_charge_count(me);
      if (charge_count > 0) {
        auto tmp = string_sprintf(" (x%d charges):", charge_count);
        parent->log(g, UI_INFO_FMT_STR "Fires" + tmp, TEXT_FORMAT_LHS);
      } else {
        parent->log(g, UI_INFO_FMT_STR "Fires:", TEXT_FORMAT_LHS);
      }
      wid_tp_info(g, v, l, fire_what_tp, parent, width);
    }
  }

  if (wid_thing_info_items(g, v, l, me, parent)) {
    parent->log_empty_line(g);
  }

  if (wid_thing_sacrifices(g, v, l, me, parent)) {
    parent->log_empty_line(g);
  }

  IF_DEBUG
  {
    parent->log(g, "Thing:");
    parent->log(g, to_string(g, v, l, me), TEXT_FORMAT_LHS);
    parent->log_empty_line(g);
    parent->log(g, "Things:");

    //
    // Dump the contents at this tile
    //
    auto at = thing_at(g, v, l, me);
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, at)
    {
      auto s = std::format("- {}", thing_name_short(g, v, l, it));
      parent->log(g, s, TEXT_FORMAT_LHS);
    }
  }
}
