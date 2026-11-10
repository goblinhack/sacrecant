//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_level.hpp"
#include "my_main.hpp" // NOLINT
#include "my_spoint.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_popup.hpp"
#include "my_wid_text_box.hpp"

#include <cstdint>
#include <cstring>
#include <format>
#include <map>
#include <string>

//
// Show a sorted list of vales
//
static auto level_select_show_sorted_values(Gamep g, WidPopup *parent, std::map< std::string, int > &map_in, const std::string &map_name) -> int
{
  TRACE();

  int count = 0;

  if (map_in.empty()) {
    return count;
  }

  {
    auto s1 = std::format("{}:", map_name);
    parent->log(g, UI_INFO_FMT_STR + std::string(s1) + UI_RESET_FMT, TEXT_FORMAT_LHS);
  }

  while (static_cast< uint32_t >(! map_in.empty()) != 0U) {
    std::string highest;
    for (const auto &m : map_in) {
      auto name = m.first;
      auto val  = m.second;
      if (highest.empty()) {
        highest = name;
      } else if (val > map_in[ highest ]) {
        highest = name;
      }
    }

    {
      TRACE();
      auto             *tp   = tp_find_mand(highest);
      std::string const name = tp_name_long(tp);

      auto s2 = std::format("  {} x %tp={}$ {}", map_in[ highest ], highest, name);
      parent->log(g, s2, TEXT_FORMAT_LHS);
      count++;
    }

    map_in.erase(highest);
  }
  return count;
}

//
// If in level select mode, update what we're hovering over
//
void wid_level_show_contents(Gamep g, Levelsp v, Levelp l, WidPopup *parent)
{
  TRACE();

  //
  // Don't show info for the level select level itself. Choose the next player level.
  //
  if (level_is_level_select(g, v, l)) {
    l = thing_player_level(g);
    if (l == nullptr) [[unlikely]] {
      return;
    }

    if (v->tick != 0U) {
      l = level_select_get_next_level(g, v, l);
      if (l == nullptr) [[unlikely]] {
        return;
      }
    } else {
      //
      //  Stick with the first level
      //
    }
  }

  Levelp player_level = thing_player_level(g);

  std::map< std::string, int > mobs;
  std::map< std::string, int > monsts;
  std::map< std::string, int > treasure;
  std::map< std::string, int > spell_stones;
  std::map< std::string, int > temples;

  FOR_ALL_THINGS_ON_LEVEL_NO_BREAK(g, v, l, t)
  {
    auto name = tp_name(thing_tp(t));

    if (thing_is_mob(t)) {
      mobs[ name ]++;
    }

    if (thing_is_monst(t)) {
      monsts[ name ]++;
    }

    if (thing_is_treasure(t)) {
      treasure[ name ]++;
    }

    if (thing_is_spell_stone(t)) {
      spell_stones[ name ]++;
    }

    if (thing_is_temple(t)) {
      temples[ name ]++;
    }
  }

  if (l->is_treasure_level) {
    parent->log(g, UI_GOOD_FMT_STR "Treasure level" UI_RESET_FMT);
    parent->log_empty_line(g);
  }

  if (level_is_boss_level(g, v, l)) {
    parent->log(g, UI_GOOD_FMT_STR "Boss level" UI_RESET_FMT);
    parent->log_empty_line(g);
  }

  auto tmp = std::format("Preview of level {}", l->level_arr_idx + 1);
  parent->log(g, UI_INFO_FMT_STR + std::string(tmp) + UI_RESET_FMT);
  parent->log_empty_line(g);

  if (v->tick != 0U) {
    if (l->player_can_enter_this_level_next) {
      if (player_level->player_completed_level_via_exit) {
        //
        // Obvious
        //
        if (compiler_unused) {
          parent->log(g, "You can enter this level.", TEXT_FORMAT_LHS);
          parent->log_empty_line(g);
        }
      } else if (player_level->player_fell_out_of_level) {
        parent->log(g, "You can climb back here.", TEXT_FORMAT_LHS);
        parent->log_empty_line(g);
      } else {
        parent->log(g, "You can go back to here.", TEXT_FORMAT_LHS);
        parent->log_empty_line(g);
      }
    } else if (l != player_level) {
      if (l->player_fell_out_of_level) {
        parent->log(g, "You can re-enter this level.", TEXT_FORMAT_LHS);
        parent->log_empty_line(g);
      } else {
        parent->log(g, "You cannot enter here yet.", TEXT_FORMAT_LHS);
        parent->log_empty_line(g);
      }
    } else {
      parent->log(g, "You can re-enter this level.", TEXT_FORMAT_LHS);
      parent->log_empty_line(g);
    }

    if (l->player_completed_level_via_exit) {
      parent->log(g, "You completed this level.", TEXT_FORMAT_LHS);
      parent->log_empty_line(g);
    } else if (l->player_fell_out_of_level) {
      parent->log(g, "You fell out of this level.", TEXT_FORMAT_LHS);
      parent->log_empty_line(g);
    }
  }

  parent->log(g, "Level contents:", TEXT_FORMAT_LHS);

  int count = 0;
  count += level_select_show_sorted_values(g, parent, temples, "Temple");
  count += level_select_show_sorted_values(g, parent, spell_stones, "Spell Stone");
  count += level_select_show_sorted_values(g, parent, treasure, "Loot");
  count += level_select_show_sorted_values(g, parent, mobs, "Mobs");
  count += level_select_show_sorted_values(g, parent, monsts, "Monsters");

  if (count == 0) {
    parent->log_empty_line(g);
    parent->log(g, UI_INFO_FMT_STR "This level appears empty...", TEXT_FORMAT_LHS);
  }

  if (1 || l->is_treasure_level) {
    static auto minimap_size = UI_RIGHTBAR_WIDTH - 8;
    static auto pad          = (UI_RIGHTBAR_WIDTH - minimap_size) / 2;

    level_minimaps_update(g, v, l);

    auto *w = parent->log_empty_line(g);
    if (w != nullptr) {
      auto        *w2 = wid_new_square_button(g, parent->wid_popup_container, "level map");
      spoint const minimap_tl(pad, TERM_HEIGHT - minimap_size);
      spoint const minimap_br(UI_RIGHTBAR_WIDTH - 1 - pad, TERM_HEIGHT - 2);
      wid_set_tilename(TILE_LAYER_BOX_FG, w2, "FBO_MINIMAP_LEVEL");
      wid_set_text_lhs(w2);
      wid_set_text_top(w2);
      wid_set_pos(w2, minimap_tl, minimap_br);
      wid_set_mode(w2, WID_MODE_OVER);
      wid_set_color(w2, WID_COLOR_BG, WHITE);
      wid_set_mode(w2, WID_MODE_NORMAL);
      wid_set_color(w2, WID_COLOR_BG, WHITE);
    }
  }

  wid_update(g, parent->wid_popup_container);
}
