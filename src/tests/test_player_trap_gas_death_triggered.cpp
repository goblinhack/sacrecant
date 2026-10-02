//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"
#include "../my_wids.hpp"

[[nodiscard]] static auto test_player_trap_gas_death_triggered(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 10;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const level1 // first level
      = ".........."
        ".........."
        ".........."
        "@ttttttttt"
        ".........."
        ".........."
        "..........";
  std::string const expect1 // first level
      = ".........."
        "&...&....."
        "&&&&&&&&&&"
        "&&&t&&&&&&"
        "&&&.&&&&&&"
        ".........."
        "..........";

  //
  // Create the level and start playing
  //
  Levelp    l = nullptr;
  Overrides overrides;
  overrides[ 't' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("trap_gas_death"); };
  Levelsp v        = game_test_init(g, &l, level_num, w, h, level1.c_str(), overrides);

  //
  // The guts of the test
  //
  bool result {};
  bool up {};
  bool down {};
  bool left {};
  bool right {};

  Thingp player = nullptr;

  //
  // Find the player
  //
  TEST_PROGRESS(t);
  {
    TRACE();
    player = thing_player(g);
    if (player == nullptr) [[unlikely]] {
      TEST_FAILED(t, "no player");
      goto exit;
    }
  }

  for (auto tries = 0; tries < 8; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);

    //
    // Move right
    //
    TEST_LOG(t, "move right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move failed");
      goto exit;
    }

    TEST_ASSERT(t, game_wait_for_tick_to_finish(g, v, l), "failed to wait for tick to finish");
  }

  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, wid_console_find_text(g, "You choke in"), "did not find console text");

  TEST_ASSERT(t, game_tick_get(g, v) == 8, "final tick counter value");

  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_player_trap_gas_death_triggered() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("player_trap_gas_death_triggered");

  test_callback_set(test, test_player_trap_gas_death_triggered);

  return true;
}
