//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_console.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_string.hpp"
#include "my_test.hpp"
#include "my_tests.hpp"
#include "my_time.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <SDL_timer.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <initializer_list>
#include <map>
#include <string>
#include <utility>
#include <vector>

/* clang-format off */
static std::initializer_list< std::string > tests = {
    /* begin shell marker1 */
    /* shell for i in $(find . -name "*.cpp" | xargs grep -h "test_load(\"" | sort | awk '{print $4}' | cut -d\" -f2) */
    /* shell do */
    /* shell echo "    \"$i\"", */
    /* shell done */
    "alloc_free_monsts",
    "alloc_free_walls",
    "barrel_explosion",
    "barrel",
    "beam_weapon_barrel_explosion",
    "beam_weapon_border_turns_to_lava",
    "beam_weapon_door_locked",
    "beam_weapon_door_secret",
    "beam_weapon_door_unlocked",
    "beam_weapon_mob",
    "beam_weapon_monst_chain",
    "beam_weapon_over_grass",
    "beam_weapon_over_water",
    "beam_weapon_rock_turns_to_lava",
    "beam_weapon_rubble",
    "beam_weapon_teleport",
    "beam_weapon_wall_turns_to_lava",
    "beam_weapon",
    "boots_fire_player_lava",
    "boots_spider_player_spiderweb",
    "brazier_shove_chasm_twice",
    "brazier_shove_chasm",
    "brazier_shove_fail",
    "brazier_shove_into_mob",
    "brazier_shove_ok",
    "bridge_on_fire_over_lava",
    "bridge_on_fire",
    "chocolate_frog",
    "clown_meat",
    "drop_all_items",
    "drop_cursed_item",
    "drop_items",
    "drop_then_recollect_items",
    "fire_on_water",
    "fire_over_chasm",
    "foliage_on_fire",
    "items_drift_away",
    "jump_ok",
    "jump_truncated",
    "large_fire_water",
    "mob_shove_into_lava",
    "mob_shove_ok",
    "mob_water",
    "monst_amphibian",
    "monst_avoids_chasm",
    "monst_avoids_lava",
    "monst_avoids_player",
    "monst_avoids_water_usually",
    "monst_avoids_water",
    "monst_blasts_player",
    "monst_crosses_chasm",
    "monst_crosses_lava",
    "monst_crosses_rubble",
    "monst_crosses_water",
    "monst_door_locked",
    "monst_door_slam_attack",
    "monst_door_unlocked",
    "monst_fast",
    "monst_fish_jump",
    "monst_fish",
    "monst_jump_land_then_pounce_over_water",
    "monst_jump_over_chasm",
    "monst_jump_over_lava",
    "monst_jumper",
    "monst_massacre",
    "monst_maze",
    "monst_mob_leash",
    "monst_mobbing",
    "monst_move_diagonal_barrel",
    "monst_move_diagonal_border",
    "monst_move_diagonal_brazier",
    "monst_move_diagonal_pillar",
    "monst_move_diagonal_rock",
    "monst_move_diagonal_rubble",
    "monst_move_diagonal_vault",
    "monst_move_diagonal_wall",
    "monst_pass_through_open_door",
    "monst_pass_through_walls",
    "monst_poison",
    "monst_resurrect_but_burns",
    "monst_resurrect_fire",
    "monst_resurrect",
    "monst_secret_door",
    "monst_short_jump_around_chasm",
    "monst_slow_attack_multi",
    "monst_slow_attack_single",
    "monst_slow",
    "monst_vision_direction",
    "monst_voider",
    "no_change",
    "path_astar",
    "path_dmap_reverse",
    "path_dmap",
    "player_engulfed_if_levitating",
    "player_engulfed_max_items",
    "player_engulfed_multiple",
    "player_engulfed_same_items",
    "player_engulfed_then_falls",
    "player_engulfed_then_jumps",
    "player_engulfed_then_teleports",
    "player_engulfed_treasure_chest",
    "player_engulfed",
    "player_fall_chasm_into_lava",
    "player_fall_chasm_twice_into_lava",
    "player_fall_chasm_twice",
    "player_fall_while_on_fire",
    "player_hiding_behind_foliage",
    "player_hiding_in_foliage",
    "player_in_steam",
    "player_lava",
    "player_melee",
    "player_move_diagonal_barrel",
    "player_move_diagonal_border",
    "player_move_diagonal_brazier",
    "player_move_diagonal_pillar",
    "player_move_diagonal_rock",
    "player_move_diagonal_vault",
    "player_move_diagonal_wall",
    "player_move_ok",
    "player_moving_in_foliage",
    "player_on_fire_move_into_water",
    "player_on_fire",
    "player_open_door_locked_fail",
    "player_open_door_locked_success",
    "player_open_door_secret",
    "player_open_door_unlocked",
    "player_trap_did_not_trigger_due_to_clover",
    "player_trap_did_not_trigger_due_to_horseshoe",
    "player_triggered",
    "pot_disloc_bad_luck",
    "pot_disloc_good_luck",
    "pot_healing",
    "pot_invis_player_monst_can_see",
    "pot_invis_player_monst_cannot_see_but_made_sound",
    "pot_invis_player_monst_cannot_see",
    "pot_levit_player_chasm",
    "pot_levit_player_grass",
    "pot_levit_player_in_steam",
    "pot_levit_player_lava",
    "pot_levit_player_move_fail",
    "pot_levit_player_move_fail2",
    "pot_levit_player_on_fire",
    "pot_levit_player_rubble",
    "pot_levit_player_spiderweb",
    "pot_levit_player_water",
    "pot_levit_trampled_grass",
    "pot_prot",
    "pot_stealth",
    "pot_tireless",
    "proj_barrel_explosion",
    "proj_border_turns_to_lava",
    "proj_door_locked",
    "proj_door_secret",
    "proj_door_unlocked",
    "proj_mob",
    "proj_over_grass",
    "proj_over_water",
    "proj_rock_turns_to_lava",
    "proj_spiderweb",
    "proj_teleport",
    "proj_wall_turns_to_lava",
    "projectile",
    "ring_life",
    "ring_war_beam_weapon",
    "ring_war_defence",
    "ring_war_melee",
    "ring_war_projectile",
    "sac_noisy_pot_stealth",
    "sac_noisy",
    "save_load",
    "slots",
    "spiderweb_monst_not_trapped_due_to_ethereal",
    "spiderweb_monst_not_trapped_due_to_levitation",
    "spiderweb_monst_trapped",
    "spiderweb_player_trapped",
    "teleport_chasm_bad_luck",
    "teleport_chasm",
    "teleport_lava",
    "teleport_shove_barrel",
    "teleport_shove_brazier",
    "teleport_shove_mob",
    "teleport",
    "throw_item_at_monst",
    "throw_item_chasm",
    "throw_item_pot_disloc",
    "throw_item_pot_healing_into_lava",
    "throw_item_pot_healing",
    "throw_item_pot_incin_chain_explosion",
    "throw_item_pot_incin_chasm",
    "throw_item_pot_incin_into_water",
    "throw_item_pot_incin",
    "throw_item_pot_invis_monst",
    "throw_item_pot_invis",
    "throw_item_pot_levit_hard_landing",
    "throw_item_pot_levit_monst_can_move",
    "throw_item_pot_levit_monst_cannot_move",
    "throw_item_pot_levit_monst_death",
    "throw_item_pot_levit_soft_landing",
    "throw_item_pot_levit",
    "throw_items",
    "trampled_grass",
    "wall",
    "weapon_discharge",
    "weapon_worn_then_stripped",
  /* end shell marker1 */
};
/* clang-format on */

class Test
{
public:
  std::string name;

  //
  // Callbacks
  //
  test_callback_t callback {};

  Test();
  ~Test();
};

using Testidmap = std::vector< class Test * >;

//
// Use std::map so the test names are sorted
//
static std::map< std::string, class Test * > test_name_map;

static bool test_init_done;

Test::Test() { NEWPTR(MTYPE_TP, this, "Test"); }

Test::~Test() { OLDPTR(MTYPE_TP, this); }

static auto test_find(const char *name_in) -> Testp
{
  TRACE();

  std::string const name(name_in);
  auto              result = test_name_map.find(name);

  if ((result != test_name_map.end())) [[unlikely]] {
    return result->second;
  }

  return nullptr;
}

void test_init()
{
  TRACE();

  test_init_done = 1u;

  if (! tests_init()) {
    CROAK("tests_init failed");
  }
}

void test_fini()
{
  TRACE();

  if (! test_init_done) {
    return;
  }
  test_init_done = 0u;

  for (auto &test : test_name_map) {
    delete test.second;
  }

  test_name_map.clear();
}

void test_callback_set(Testp test, test_callback_t callback)
{
  TRACE();
  if (test == nullptr) {
    ERR("no test for %s", __FUNCTION__);
    return;
  }
  test->callback = callback;
}

[[nodiscard]] auto test_load(const char *name_in) -> Testp
{
  TRACE();

  std::string const name(name_in);

  log("load test '%s'", name_in);

  if (test_find(name_in) != nullptr) {
    CROAK("test_load: test name [%s] already loaded", name_in);
  }

  auto *test = new Test();
  test->name = name;

  auto result = test_name_map.insert(std::make_pair(name, test));
  if (! result.second) {
    CROAK("test insert name [%s] failed", name_in);
  }

  return test;
}

[[nodiscard]] auto test_name(Testp test) -> const char *
{
  TRACE();

  return test->name.c_str();
}

void tests_run(Gamep g)
{
  TRACE();

  int passed = 0;
  int failed = 0;

  //
  // For tests that create a widget, we need some defaults
  //
  if (TERM_HEIGHT == 0) {
    TERM_HEIGHT = TERM_HEIGHT_DEF;
  }
  if (TERM_WIDTH == 0) {
    TERM_WIDTH = TERM_WIDTH_DEF;
  }

  term_log("Running tests\n");
  term_log("-------------\n");

  bool found_match       = {};
  bool exact_match_found = {};

  if (! g_opt_test_name_filter.empty()) {
    for (auto &test : test_name_map) {
      auto name = "test_" + test.first;
      if (g_opt_test_name_filter == name) {
        exact_match_found = true;
      }
    }
  }

  if (g_opt_test_repeat == 0) {
    g_opt_test_repeat = 1;
  }

  auto started_tests = SDL_GetTicks();

  tp_fini();
  for (auto repeat = 0; repeat < g_opt_test_repeat; repeat++) {
    for (auto &test : test_name_map) {

      bool result  = false;
      bool skipped = false;

      //
      // Test name
      //
      auto  name = "test_" + test.first;
      auto *t    = test.second;
      auto  pre  = std::format("Running {:<70s}", name);

      //
      // Skip the test if needed
      //
      if (! g_opt_test_name_filter.empty()) {
        if (g_opt_test_name_filter == "all") {
          //
          // All tests or "--tests"
          //
          found_match = true;
        } else if (exact_match_found) {
          //
          // Exact match
          //
          if (g_opt_test_name_filter == name) {
            found_match = true;
          } else {
            continue;
          }
        } else if (name.find(g_opt_test_name_filter) != std::string::npos) {
          //
          // Partial match e.g. "monst" for all monst tests
          //
          found_match = true;
        } else if (name != g_opt_test_name_filter) {
          //
          // Failed to match
          //
          skipped = true;
          continue;
        }
      }

      g_opt_test_current = name;

      //
      // Preamble
      //
      if (! skipped) {
        log("running test: %s", name.c_str());
        log("-------------------------------------------");
      }

      //
      // Run the test
      //
      auto started = SDL_GetTicks();

      //
      // Some tests modify the Tps so we need to init and reset them each test
      //
      (void) tp_init();
      if (! skipped) {
        result = t->callback(g, t);
      }
      tp_fini();

      auto elapsed  = SDL_GetTicks() - started;
      auto how_long = std::format("(took {:.2f} secs, {} ms)", static_cast< float >(elapsed) / 1000.0, elapsed);

      //
      // Print the timestamp
      //
      char buf[ MAXLONGSTR ];
      buf[ 0 ] = '\0';
      get_timestamp(buf, MAXLONGSTR);

#ifdef GITHUB_BUILD
      std::string out(buf);

      //
      // Test preamble. We print this after the test has ran to avoid messing up the output.
      //
      out += pre;

      if (skipped) {
        out += "skipped";
      } else if (result) {
        passed++;
        out += "OK ";
        out += how_long;

        log("passed %s", how_long.c_str());
      } else {
        failed++;
        out += "FAILED";
        log("failed");
      }
      std::println("{}", out);
#else
      term_log(buf);
      //
      // Test preamble. We print this after the test has ran to avoid messing up the output.
      //
      term_log(pre.c_str());

      if (skipped) {
        term_log("%%fg=yellow$skipped%%fg=reset$\n");
      } else if (result) {
        passed++;
        term_log("%%fg=green$OK%%fg=reset$ ");
        term_log(how_long.c_str());
        term_log("\n");
        log("passed %s", how_long.c_str());
      } else {
        failed++;
        term_log("%%fg=red$FAILED%%fg=reset$\n");
        log("failed");
      }
#endif

      if (! skipped) {
        log("-");
      }

      //
      // github output seems to be buffered.
      //
      fflush(stdout);
      fflush(stderr);
    }
  }

  if (! g_opt_test_name_filter.empty()) {
    if (! found_match) {
      CROAK("found no test matching filter: %s", g_opt_test_name_filter.c_str());
    }
  }

  test_fini();

  g_opt_tests = false;

  if (failed != 0) {
    con("Results: %d passed, %d failed", passed, failed);
    term_log("%%fg=red$Some tests failed%%fg=reset$\n");
    exit(1);
  } else {
    auto elapsed = SDL_GetTicks() - started_tests;
    con("Results: %d tests passed, took %.2f secs %u ms", passed, static_cast< float >(elapsed) / 1000.0, elapsed);
    term_log("%%fg=green$All tests passed%%fg=reset$\n");
    DIE_CLEAN("done");
  }
}
