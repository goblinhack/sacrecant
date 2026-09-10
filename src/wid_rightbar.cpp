//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_game_inlines.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wids.hpp"

#include <format>
#include <utility>

static WidPopup *wid_rightbar;
static auto      minimap_size = 14;

static void wid_rightbar_create_minimap_level(Gamep g)
{
  TRACE();

  {
    auto        *w = wid_new_square_button(g, wid_rightbar->wid_popup_container, "level map text");
    spoint const minimap_tl(1, TERM_HEIGHT - minimap_size);
    spoint const minimap_br(minimap_size + 2, TERM_HEIGHT - minimap_size);
    wid_set_text(w, "Minimap:");
    wid_set_text_lhs(w);
    wid_set_text_top(w);
    wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
    wid_set_pos(w, minimap_tl, minimap_br);
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_color(w, WID_COLOR_BG, WHITE);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_color(w, WID_COLOR_BG, WHITE);
  }

  {
    auto        *w = wid_new_square_button(g, wid_rightbar->wid_popup_container, "level map");
    spoint const minimap_tl(1, TERM_HEIGHT - minimap_size + 1);
    spoint const minimap_br(minimap_size + 2, TERM_HEIGHT - 2);
    wid_set_tilename(TILE_LAYER_BOX_FG, w, "FBO_MINIMAP_LEVEL");
    wid_set_text_lhs(w);
    wid_set_text_top(w);
    wid_set_pos(w, minimap_tl, minimap_br);
    wid_set_mode(w, WID_MODE_OVER);
    wid_set_color(w, WID_COLOR_BG, WHITE);
    wid_set_mode(w, WID_MODE_NORMAL);
    wid_set_color(w, WID_COLOR_BG, WHITE);
  }
}

[[nodiscard]] static auto wid_rightbar_thing_info_count(Gamep g, Levelsp v) -> int
{
  int wid_thing_info_count {};

  for (auto n = 0; std::cmp_less(n, v->describe_count); n++) {
    auto *t = thing_find_optional(g, v, v->describe[ n ]);
    if (t == nullptr) {
      continue;
    }

    if (thing_is_player(t)) {
      continue;
    }

    if (thing_is_dead(t)) {
      continue;
    }

    wid_thing_info_count++;
  }

  for (auto n = 0; std::cmp_less(n, v->describe_count); n++) {
    auto *t = thing_find_optional(g, v, v->describe[ n ]);
    if (t == nullptr) {
      continue;
    }

    if (thing_is_player(t)) {
      continue;
    }

    if (thing_is_dead(t)) {
      wid_thing_info_count++;
    }
  }

  return wid_thing_info_count;
}

static auto wid_rightbar_thing_info_add(Gamep g, Levelsp v, Levelp l) -> void
{
  bool got_one = {};

  for (auto n = 0; std::cmp_less(n, v->describe_count); n++) {
    auto *t = thing_find_optional(g, v, v->describe[ n ]);
    if (t == nullptr) {
      continue;
    }

    if (thing_is_player(t)) {
      continue;
    }

    if (thing_is_dead(t)) {
      continue;
    }

    wid_thing_info(g, v, l, t, wid_rightbar, UI_RIGHTBAR_WIDTH);
    got_one = true;
  }

  for (auto n = 0; std::cmp_less(n, v->describe_count); n++) {
    auto *t = thing_find_optional(g, v, v->describe[ n ]);
    if (t == nullptr) {
      continue;
    }

    if (thing_is_player(t)) {
      continue;
    }

    if (thing_is_dead(t)) {
      wid_thing_info(g, v, l, t, wid_rightbar, UI_RIGHTBAR_WIDTH);
      got_one = true;
    }
  }

  if (! got_one) {
    auto *sac = game_cand_sacrifice_get_last(g);
    if (sac != nullptr) {
      wid_thing_info(g, v, l, sac, wid_rightbar, UI_RIGHTBAR_WIDTH);
      got_one = true;
    }
  }

  if (! got_one) {
    auto *sac = game_mouse_over_sacrifice_get(g);
    if (sac != nullptr) {
      wid_thing_info(g, v, l, sac, wid_rightbar, UI_RIGHTBAR_WIDTH);
      got_one = true;
    }
  }
}

[[nodiscard]] static auto wid_rightbar_create_window(Gamep g) -> bool
{
  TRACE();
  DBG2("Remake rightbar");

  wid_rightbar_fini(g);

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return false;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) [[unlikely]] {
    return false;
  }

  auto         width = UI_RIGHTBAR_WIDTH;
  spoint const tl(TERM_WIDTH - width, 0);
  spoint const br(TERM_WIDTH - 1, TERM_HEIGHT - 1);
  wid_rightbar = new WidPopup(g, "right bar", tl, br, nullptr, "", false, false);

  if (level_is_level_select(g, v, l)) {
    //
    // If in level select mode, a different wid is used
    //

    //
    // Minimaps
    //
    wid_rightbar_create_minimap_level(g);
  } else if (game_state(g) == STATE_PLAYER_SELECT_MENU) {
    //
    // Tp info only
    //
    wid_rightbar_thing_info_add(g, v, l);
  } else if (wid_rightbar_thing_info_count(g, v) != 0) {
    //
    // Thing infos
    //
    wid_rightbar_thing_info_add(g, v, l);
  } else {
    //
    // Show level number etc...
    //
    wid_rightbar->log_empty_line(g);
    auto        bs = BiomeType_to_string(l->biome);
    std::string s;
    auto        num = l->level_num + 1;

    //
    // Boss level?
    //
    if (level_type_is_boss_level(num)) {
      s = std::format("{}:{} Seed:{}", bs, "boss", game_seed_name_get(g));
    } else {
      s = std::format("{}:L{} Seed:{}", bs, num, game_seed_name_get(g));
    }

    wid_rightbar->log(g, s);

    //
    // Add move count
    //
    auto mv = std::format("Move:{}", v->tick);
    wid_rightbar->log(g, mv);

    //
    // Minimaps
    //
    wid_rightbar_create_minimap_level(g);
  }

  wid_update(g, wid_rightbar->wid_popup_container);

  return true;
}

void wid_rightbar_fini(Gamep g)
{
  TRACE();

  delete wid_rightbar;
  wid_rightbar = nullptr;
}

[[nodiscard]] auto wid_rightbar_init(Gamep g) -> bool
{
  TRACE();
  return wid_rightbar_create_window(g);
}
