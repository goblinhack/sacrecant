//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"
#include "../my_wids.hpp"

[[nodiscard]] static auto test_monst_blasts_player(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 15;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "XXXXXXXXXXXXXXX"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X@CCCCCCCCCCCmX"
        "XXXXXXXXXXXXXXX";
  std::string const expect1
      = "XXXXXXXXXXXXXXX"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X@CCCCCCCCMCC.X"
        "XXXXXXXXXXXXXXX";
  std::string const expect2
      = "XXXXXXXXXXXXXXX"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X@CCCCMCCCCCC.X"
        "XXXXXXXXXXXXXXX";
  std::string const expect3
      = "XXXXXXXXXXXXXXX"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X.CCCCCCCCCCC.X"
        "X---MCCCCCCCC.X"
        "XXXXXXXXXXXXXXX";

  //
  // Create the level and start playing
  //
  Overrides overrides;
  overrides[ 'm' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("argusul"); };
  Levelp  l        = nullptr;
  Levelsp v        = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);

  //
  // The guts of the test
  //
  bool result {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  TEST_PROGRESS(t);
  (void) thing_health_set(g, v, l, player, 10);

  //
  // Wait for the argusul to blast the player
  //
  for (auto tries = 0; tries < 7; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  //
  // Check the level contents
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
    if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 7; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  //
  // Check the level contents
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
    if (! (result = level_match_contents(g, v, l, t, w, h, expect2.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  for (auto tries = 0; tries < 100; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    level_dump(g, v, l, w, h);
    TEST_PROGRESS(t);

    if (thing_is_dead(player)) {
      break;
    }

    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  TEST_ASSERT(t, thing_is_dead(player), "player should be dead");

  //
  // Check the level contents
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  {
    TRACE();
    if (! (result = level_match_contents(g, v, l, t, w, h, expect3.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  TEST_ASSERT(t, wid_console_find_text(g, "beam of energy"), "did not find console text");

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 66, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_monst_blasts_player() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("monst_blasts_player");

  test_callback_set(test, test_monst_blasts_player);

  return true;
}
