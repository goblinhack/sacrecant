//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_cpp_template.hpp"
#include "../../my_level_inlines.hpp"
#include "../../my_main.hpp"
#include "../../my_random.hpp"
#include "../../my_sound.hpp"
#include "../../my_sprintf.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"
#include "my_spell_common.hpp"

static const std::string upgrade_1_increase_radius = "increase radius and range";

static auto tp_spell_level_warp_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return //
      UI_INFO1_FMT_STR "This spell will transport you instantly to another random level within the same biome.";
}

static bool tp_spell_level_warp_on_cast_request(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e)
{
  TRACE();

  return true;
}

static bool tp_spell_level_warp_on_cast_do(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e)
{
  TRACE();

  THING_DBG(g, v, l, user, "level warp");
  TRACE_INDENT();

  auto biome = level_to_biome(g, v, l);

  std::vector< Levelp > cands;

  FOR_ALL_LEVELS(g, v, l_it)
  {
    if (l_it == l) {
      continue;
    }
    if (level_is_boss_level(g, v, l_it)) {
      continue;
    }
    if (level_to_biome(g, v, l_it) == biome) {
      cands.push_back(l_it);
      level_log(g, v, l, "cand level");
    }
  }

  auto l_new = pcg_rand_one_of(cands);
  if (! l_new) {
    if (thing_is_player(user)) {
      topcon(UI_WARN_FMT_STR "That spell seemed to have no effect." UI_RESET_FMT);
      thing_sound_play(g, v, l, user, "error");
    }
    return false;
  }

  level_log(g, v, l, "chosen level");

  if (! thing_teleport_random(g, v, l_new, user)) {
    if (thing_is_player(user)) {
      topcon(UI_WARN_FMT_STR "That spell seemed to have no effect." UI_RESET_FMT);
      thing_sound_play(g, v, l, user, "error");
    }
    return false;
  }

  if (thing_is_player(user)) {
    topcon(UI_GOOD_FMT_STR "You feel moved." UI_RESET_FMT);
  }

  return true;
}

[[nodiscard]] auto tp_load_spell_level_warp() -> bool
{
  TRACE();

  auto *tp   = tp_load("spell_level_warp"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_spell_level_warp_get);
  thing_on_cast_do_set(tp, tp_spell_level_warp_on_cast_do);
  thing_on_cast_request_set(tp, tp_spell_level_warp_on_cast_request);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_memory_ext);
  tp_flag_set(tp, is_spell);
  tp_name_long_set(tp, "level warp");
  tp_spell_cost_set(tp, 1);
  tp_spell_mana_cost_set(tp, 10);
  tp_spell_radius_max_set(tp, 6);
  tp_spell_radius_set(tp, 2);
  tp_spell_range_max_set(tp, 12);
  tp_spell_range_set(tp, 8);
  tp_stat_set(tp, THING_STAT_ARCANA_GEO, "11");
  // end sort marker1 }

  auto *tile = tile_find_mand("icon_" + name);
  tile_size_set(tile, OUTLINE_TILE_WIDTH, OUTLINE_TILE_HEIGHT);
  tp_tiles_push_back(tp, THING_ANIM_IDLE, tile);

  return true;
}
