//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"
#include "my_sprintf.hpp"
#include "my_string.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_wids.hpp"

#include <algorithm>
#include <print>

//
// The thing icon
//
[[nodiscard]] static auto wid_thing_info_icon(Gamep g, Tpp tp, WidPopup *parent) -> bool
{
  TRACE();

  auto *text = parent->wid_text_area;
  auto *b    = parent->wid_text_area->wid_text_area;

  Tilep tile = tp_tiles_get(tp, THING_ANIM_IDLE, 0);
  if (tile == nullptr) {
    return false;
  }

  auto        *w = wid_new_square_button(g, b, "Icon");
  spoint const tl(0, text->line_count);
  spoint const br(3, text->line_count + 2);
  wid_set_tile(TILE_LAYER_BG_0, w, tile);
  wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
  wid_set_pos(w, tl, br);

  return true;
}

[[nodiscard]] static auto wid_thing_info_keys(Gamep g, Thingp me, WidPopup *parent) -> bool
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
    wid_set_tile(TILE_LAYER_BG_0, w, tile);
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
[[nodiscard]] static auto wid_thing_info_name(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp tp, WidPopup *parent) -> bool
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
[[nodiscard]] static auto wid_thing_info_detail(Gamep g, Levelsp v, Levelp l, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  if (thing_is_dead(me)) {
    return false;
  }

  parent->log(g, thing_detail_get(g, v, l, me), TEXT_FORMAT_LHS);

  return true;
}

//
// Score
//
[[nodiscard]] static auto wid_thing_info_score(Gamep g, Thingp me, Tpp tp, WidPopup *parent, int width) -> bool
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
      UI_INFO1_FMT_STR " Hiscore "
      // newline
      UI_INFO2_FMT_STR " %06u",
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
[[nodiscard]] static auto wid_thing_info_health_bar(Gamep g, Thingp me, Tpp tp, WidPopup *parent, int width) -> bool
{
  TRACE();

  if (! tp_is_health_visible(tp)) {
    return false;
  }

  char tmp[ MAXSHORTSTR ];

  //
  // "Health            "
  //
  memset(tmp, 0, sizeof(tmp));
  memset(tmp, ' ', sizeof(tmp) - 1);

  if (thing_is_dead(me)) {
    my_strlcpy(tmp + 1, "Dead", sizeof("Dead "));
  } else {
    my_strlcpy(tmp + 1, "Health", sizeof("Health "));
  }

  //
  // "Health         a/b"
  //
  auto health_max = thing_health_max(me);
  auto h          = thing_health(me);
  h               = std::max(h, 0);

  std::string const health_str = std::to_string(h) + "/" + std::to_string(health_max);
  my_strlcpy(tmp + width - health_str.size() - 3, health_str.c_str(), width - health_str.size());
  tmp[ strlen(tmp) ] = ' ';

  //
  // "Health         a/b"
  // "xxxxxxxxxxxxxxxxxx"
  //
  auto *w = parent->log(g, std::string(tmp));
  if (w != nullptr) {
    int health_how_much = static_cast< int >((static_cast< float >(thing_health(me)) / static_cast< float >(health_max))
                                             * (static_cast< float > UI_STAT_BAR_STEPS - 1));
    health_how_much     = std::min(health_how_much, UI_STAT_BAR_STEPS - 1);
    health_how_much     = std::max(health_how_much, 0);
    auto icon           = "stat_bar." + std::to_string(health_how_much + 1);

    wid_set_shape_square(w);
    wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
    wid_set_color(w, WID_COLOR_TEXT_FG, UI_HIGHLIGHT_COLOR);
    wid_set_tilename(TILE_LAYER_BG_0, w, icon);
    wid_set_text_lhs(w, 1u);
  }

  return true;
}

//
// Stamina bar
//
[[nodiscard]] static auto wid_thing_info_stamina_bar(Gamep g, Thingp me, Tpp tp, WidPopup *parent, int width) -> bool
{
  TRACE();

  if (thing_is_dead(me)) {
    return false;
  }

  if (! tp_is_stamina_visible(tp)) {
    return false;
  }

  char tmp[ MAXSHORTSTR ];

  //
  // "Stamina           "
  //
  memset(tmp, 0, sizeof(tmp));
  memset(tmp, ' ', sizeof(tmp) - 1);

  if (thing_distance_jump(me) != thing_distance_jump_max(me)) {
    my_strlcpy(tmp + 1, "Jumping impacted", sizeof("Jumping impacted "));
  } else {
    my_strlcpy(tmp + 1, "Stamina", sizeof("Stamina "));
  }

  //
  // "Stamina        a/b"
  //
  auto stamina_max = thing_stamina_max(me);
  auto h           = thing_stamina(me);
  h                = std::max(h, 0);

  std::string const stamina_str = std::to_string(h) + "/" + std::to_string(stamina_max);
  my_strlcpy(tmp + width - stamina_str.size() - 3, stamina_str.c_str(), width - stamina_str.size());
  tmp[ strlen(tmp) ] = ' ';

  //
  // "Stamina        a/b"
  // "xxxxxxxxxxxxxxxxxx"
  //
  auto *w = parent->log(g, std::string(tmp));
  if (w != nullptr) {
    int stamina_how_much = static_cast< int >((static_cast< float >(thing_stamina(me)) / static_cast< float >(stamina_max))
                                              * (static_cast< float > UI_STAT_BAR_STEPS - 1));
    stamina_how_much     = std::min(stamina_how_much, UI_STAT_BAR_STEPS - 1);
    stamina_how_much     = std::max(stamina_how_much, 0);
    auto icon            = "stat_bar." + std::to_string(stamina_how_much + 1);

    wid_set_shape_square(w);
    wid_set_style(w, UI_WID_STYLE_SPARSE_NONE);
    wid_set_color(w, WID_COLOR_TEXT_FG, UI_HIGHLIGHT_COLOR);
    wid_set_tilename(TILE_LAYER_BG_0, w, icon);
    wid_set_text_lhs(w, 1u);
  }

  return true;
}

//
// Add immunities
//
[[nodiscard]] static auto wid_thing_info_immunities(Gamep g, Thingp me, WidPopup *parent, int width) -> bool
{
  TRACE();

  bool printed_something = false;

  FOR_ALL_THING_EVENT(e)
  {
    if (! thing_is_immune_to(me, e)) {
      continue;
    }

    bool skip_showing_immunity_string = true;

    switch (e) {
      case THING_EVENT_SHOVED : //
        skip_showing_immunity_string = true;
        break;
      case THING_EVENT_CRUSH : //
        skip_showing_immunity_string = true;
        break;
      case THING_EVENT_MELEE_DAMAGE : //
        break;
      case THING_EVENT_HEAT_DAMAGE : //
        break;
      case THING_EVENT_EXPLOSION_DAMAGE : //
        break;
      case THING_EVENT_FIRE_DAMAGE : //
        break;
      case THING_EVENT_WATER_DAMAGE :     //
      case THING_EVENT_NONE :             //
      case THING_EVENT_THE_END :          //
      case THING_EVENT_MELT :             //
      case THING_EVENT_OPEN :             //
      case THING_EVENT_LIFESPAN_EXPIRED : //
      case THING_EVENT_FALL :             //
      case THING_EVENT_CARRIED :          //
      case THING_EVENT_CARRIED_MERGED :   //
      case THING_EVENT_ENUM_MAX :         //
        skip_showing_immunity_string = true;
        break;
    }

    if (skip_showing_immunity_string) {
      continue;
    }

    auto immune_str = string_sprintf("Immune to: %*s", width - 13, capitalize(ThingEventType_to_string(e)).c_str());
    parent->log(g, immune_str, TEXT_FORMAT_LHS);
    printed_something = true;
  }

  return printed_something;
}

//
// Add special damage
//
[[nodiscard]] static auto wid_thing_info_special_damage(Gamep g, Thingp me, WidPopup *parent) -> bool
{
  TRACE();

  bool printed_something = false;

  if (! thing_is_immune_to(me, THING_EVENT_WATER_DAMAGE)) {
    parent->log(g, "Takes damage from water.", TEXT_FORMAT_LHS);
    printed_something = true;
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

  if (wid_thing_info_icon(g, tp, parent)) {
    parent->log_empty_line(g);
  }

  (void) wid_thing_info_keys(g, me, parent);

  if (wid_thing_info_name(g, v, l, me, tp, parent)) {
    parent->log_empty_line(g);
  }

  if (wid_thing_info_detail(g, v, l, me, parent)) {
    parent->log_empty_line(g);
  }

  if (wid_thing_info_score(g, me, tp, parent, width)) {
    parent->log_empty_line(g);
  }

  if (wid_thing_info_health_bar(g, me, tp, parent, width)) {
    parent->log_empty_line(g);
  }

  if (wid_thing_info_stamina_bar(g, me, tp, parent, width)) {
    parent->log_empty_line(g);
  }

  if (wid_thing_info_immunities(g, me, parent, width)) {
    parent->log_empty_line(g);
  }

  if (wid_thing_info_special_damage(g, me, parent)) {
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
    auto at = thing_at(me);
    FOR_ALL_THINGS_AT_UNSAFE(g, v, l, it, at)
    {
      auto s = std::format("- {}", thing_name_short(g, v, l, it));
      parent->log(g, s, TEXT_FORMAT_LHS);
    }
  }
}
