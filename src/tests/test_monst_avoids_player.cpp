//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"
#include "../my_wids.hpp"

[[nodiscard]] static auto test_monst_avoids_player(Gamep g, Testp t) -> bool
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
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X@..CCCCCCCCCmX"
        "XXXXXXXXXXXXXXX";
  std::string const expect1
      = "XXXXXXXXXXXXXXX"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X@..CCCCCMCCC.X"
        "XXXXXXXXXXXXXXX";
  std::string const expect2
      = "XXXXXXXXXXXXXXX"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X@..CMCCCCCCC.X"
        "XXXXXXXXXXXXXXX";
  std::string const expect3
      = "XXXXXXXXXXXXXXX"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X.@.CMCCCCCCC.X"
        "XXXXXXXXXXXXXXX";
  std::string const expect4
      = "XXXXXXXXXXXXXXX"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X...CCCCCCCCC.X"
        "X..@CCMCCCCCC.X"
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
  bool up {};
  bool down {};
  bool left {};
  bool right {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  for (auto tries = 0; tries < 8; tries++) {
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
  for (auto tries = 0; tries < 8; tries++) {
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

  //
  // Move right
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move failed");
      goto exit;
    }

    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }

    if (! (result = level_match_contents(g, v, l, t, w, h, expect3.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  //
  // Move right
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move failed");
      goto exit;
    }

    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }

    if (! (result = level_match_contents(g, v, l, t, w, h, expect4.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 18, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_monst_avoids_player() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("monst_avoids_player");

  // begin sort marker1 {
  test_callback_set(test, test_monst_avoids_player);
  // end sort marker1 }

  return true;
}
