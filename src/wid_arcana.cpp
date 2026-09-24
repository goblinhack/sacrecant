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

static void wid_arcana_common_mouse_over_begin(Gamep g, WidPopup *w)
{
  TRACE();
  w->log_empty_line(g);
  w->log(g, UI_INFO1_FMT_STR "Modifiers can impact costs and mana drain.\n", TEXT_FORMAT_LHS);
  w->log_empty_line(g);
  w->log(g, UI_INFO1_FMT_STR "SP is used when buying a spell.\n", TEXT_FORMAT_LHS);
  w->log_empty_line(g);
  w->log(g, UI_INFO1_FMT_STR "Mana is used when casting a spell.\n", TEXT_FORMAT_LHS);
  w->log_empty_line(g);
  w->log(g, UI_INFO1_FMT_STR "Cost modifier table:\n", TEXT_FORMAT_LHS);
  w->log(g, UI_INFO2_FMT_STR " -9 mod increases SP by 3.\n", TEXT_FORMAT_LHS);
  w->log(g, UI_INFO2_FMT_STR " -6 mod increases SP by 2.\n", TEXT_FORMAT_LHS);
  w->log(g, UI_INFO2_FMT_STR " -5 mod increases mana by 50%%%.\n", TEXT_FORMAT_LHS);
  w->log(g, UI_INFO2_FMT_STR " -3 mod increases SP by 1.\n", TEXT_FORMAT_LHS);
  w->log(g, UI_INFO2_FMT_STR " +3 mod decreases SP by 1.\n", TEXT_FORMAT_LHS);
  w->log(g, UI_INFO2_FMT_STR " +5 mod decreases mana by 50%%%.\n", TEXT_FORMAT_LHS);
  w->log(g, UI_INFO2_FMT_STR " +6 mod decreases SP by 2.\n", TEXT_FORMAT_LHS);
  w->log(g, UI_INFO2_FMT_STR " +9 mod decreases SP by 3.\n", TEXT_FORMAT_LHS);
  w->log_empty_line(g);
  w->log(g, UI_INFO1_FMT_STR "Select this option to filter spells to this arcana only.\n", TEXT_FORMAT_LHS);
  w->log_empty_line(g);
}

WidPopup *wid_arcana_pyro_mouse_over_begin(Gamep g)
{
  TRACE();

  int const tlx = TERM_WIDTH - UI_RIGHTBAR_WIDTH - 2;
  int const brx = tlx + UI_RIGHTBAR_WIDTH;
  int const tly = UI_TOPCON_HEIGHT + 10;
  int const bry = tly + 40;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  auto w = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  w->log(g, UI_HIGHLIGHT_FMT_STR "Pyromancer Arcana");
  w->log_empty_line(g);
  w->log(g, UI_INFO2_FMT_STR "With this Arcana you specialize in all things flaming hot and destructive, fireballs, scorched earth etc...\n",
         TEXT_FORMAT_LHS);
  wid_arcana_common_mouse_over_begin(g, w);
  w->log(g, UI_WARN_FMT_STR "Specializing in Pyromancy will make Geomancy spells more costly.\n", TEXT_FORMAT_LHS);
  w->compress(g);

  level_cursor_path_reset(g);
  return w;
}

WidPopup *wid_arcana_geo_mouse_over_begin(Gamep g)
{
  TRACE();

  int const tlx = TERM_WIDTH - UI_RIGHTBAR_WIDTH - 2;
  int const brx = tlx + UI_RIGHTBAR_WIDTH;
  int const tly = UI_TOPCON_HEIGHT + 10;
  int const bry = tly + 40;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  auto w = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  w->log(g, UI_HIGHLIGHT_FMT_STR "Geomancy Arcana");
  w->log_empty_line(g);
  w->log(g, UI_INFO2_FMT_STR "With this Arcana you specialize modification of the physical environment.\n", TEXT_FORMAT_LHS);
  wid_arcana_common_mouse_over_begin(g, w);
  w->log(g, UI_WARN_FMT_STR "Specializing in Geomancy will make Pyromancy spells more costly.\n", TEXT_FORMAT_LHS);
  w->compress(g);

  level_cursor_path_reset(g);
  return w;
}

WidPopup *wid_arcana_bio_mouse_over_begin(Gamep g)
{
  TRACE();

  int const tlx = TERM_WIDTH - UI_RIGHTBAR_WIDTH - 2;
  int const brx = tlx + UI_RIGHTBAR_WIDTH;
  int const tly = UI_TOPCON_HEIGHT + 10;
  int const bry = tly + 40;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  auto w = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  w->log(g, UI_HIGHLIGHT_FMT_STR "Biomancy Arcana");
  w->log_empty_line(g);
  w->log(g, UI_INFO2_FMT_STR "With this Arcana, you specialize in creating living things, plant summoning, healing of allies etc...\n",
         TEXT_FORMAT_LHS);
  wid_arcana_common_mouse_over_begin(g, w);
  w->log(g, UI_WARN_FMT_STR "Specializing in Necromancy will make Biomancy spells more costly.\n", TEXT_FORMAT_LHS);
  w->compress(g);

  level_cursor_path_reset(g);
  return w;
}

WidPopup *wid_arcana_necro_mouse_over_begin(Gamep g)
{
  TRACE();

  int const tlx = TERM_WIDTH - UI_RIGHTBAR_WIDTH - 2;
  int const brx = tlx + UI_RIGHTBAR_WIDTH;
  int const tly = UI_TOPCON_HEIGHT + 10;
  int const bry = tly + 40;

  spoint const tl(tlx, tly);
  spoint const br(brx, bry);

  auto w = new WidPopup(g, "stats", tl, br, nullptr, "", false, false);
  w->log(g, UI_HIGHLIGHT_FMT_STR "Necromancy Arcana");
  w->log_empty_line(g);
  w->log(g, UI_INFO2_FMT_STR "With this, the darkest of Arcana, you specialize in all things dead, undead summoning, finger of death etc...\n",
         TEXT_FORMAT_LHS);
  wid_arcana_common_mouse_over_begin(g, w);
  w->log(g, UI_WARN_FMT_STR "Specializing in Biomancy will make Necromancy spells more costly.\n", TEXT_FORMAT_LHS);
  w->compress(g);

  level_cursor_path_reset(g);
  return w;
}
