//
// Copyright goblinhack@gmail.com
//

#include "my_console.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_sprintf.hpp"
#include "my_test.hpp"
#include "my_tests.hpp"
#include "my_time.hpp"
#include "my_ui.hpp"

#include <map>
#include <print>

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
    "brazier_shove_chasm_twice",
    "brazier_shove_chasm",
    "brazier_shove_fail",
    "brazier_shove_into_mob",
    "brazier_shove_ok",
    "bridge_on_fire_over_lava",
    "bridge_on_fire",
    "fire_on_water",
    "fire_over_chasm",
    "foliage_on_fire",
    "jump_ok",
    "jump_truncated",
    "large_fire_water",
    "mob_shove_into_lava",
    "mob_shove_ok",
    "mob_water",
    "monst_avoids_chasm",
    "monst_avoids_lava",
    "monst_avoids_water_usually",
    "monst_avoids_water",
    "monst_crosses_chasm",
    "monst_crosses_lava",
    "monst_crosses_water",
    "monst_door_locked",
    "monst_door_unlocked",
    "monst_fast",
    "monst_hiding_behind_bush",
    "monst_hiding_in_bush",
    "monst_jump_over_chasm",
    "monst_maze",
    "monst_mob_leash",
    "monst_mobbing",
    "monst_pass_through_open_door",
    "monst_pass_through_walls",
    "monst_secret_door",
    "monst_short_jump_around_chasm",
    "monst_slow",
    "monst_vision_direction",
    "move_ok",
    "no_change",
    "path_astar",
    "path_dmap_reverse",
    "path_dmap",
    "player_fall_chasm_into_lava",
    "player_fall_chasm_twice_into_lava",
    "player_fall_chasm_twice",
    "player_fall_while_on_fire",
    "player_in_steam",
    "player_lava",
    "player_on_fire_move_into_water",
    "player_on_fire",
    "player_open_door_locked_fail",
    "player_open_door_locked_success",
    "player_open_door_secret",
    "player_open_door_unlocked",
    "projectile_barrel_explosion",
    "projectile_border_turns_to_lava",
    "projectile_door_locked",
    "projectile_door_secret",
    "projectile_door_unlocked",
    "projectile_mob",
    "projectile_over_water",
    "projectile_rock_turns_to_lava",
    "projectile_wall_turns_to_lava",
    "projectile",
    "save_load",
    "slots",
    "teleport_chasm",
    "teleport_shove_barrel",
    "teleport_shove_brazier",
    "teleport_shove_mob",
    "teleport",
    "trampled_grass",
    "wall",
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

  tests_init();
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

auto test_load(const char *name_in) -> Testp
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

auto test_name(Testp test) -> const char *
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
      if (name != g_opt_test_name_filter) {
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
    auto started = time_ms();
    if (! skipped) {
      result = t->callback(g, t);
    }
    auto elapsed  = time_ms() - started;
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

  test_fini();

  g_opt_tests = false;

  if (failed != 0) {
    con("Results: %d passed, %d failed", passed, failed);
    term_log("%%fg=red$Some tests failed%%fg=reset$\n");
    exit(1);
  } else {
    con("Results: %d tests passed", passed);
    term_log("%%fg=green$All tests passed%%fg=reset$\n");
    DIE_CLEAN("done");
  }
}
