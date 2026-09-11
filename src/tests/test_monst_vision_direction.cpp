//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_monst_vision_direction(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 26;
  auto           h         = 6;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "XXXXXXXXXXXXXXXXXXXXXXXXXX"
        "X.......................mX"
        "X...................xxxxxX"
        "X.....................xxxX"
        "X....................@xxxX"
        "XXXXXXXXXXXXXXXXXXXXXXXXXX";
  std::string const expect1
      = "XXXXXXXXXXXXXXXXXXXXXXXXXX"
        "X........................X"
        "X...................xxxxxX"
        "X.....................xxxX"
        "X.........m..........@xxxX"
        "XXXXXXXXXXXXXXXXXXXXXXXXXX";
  std::string const expect2
      = "XXXXXXXXXXXXXXXXXXXXXXXXXX"
        "X........................X"
        "X.........m.........xxxxxX"
        "X.....................xxxX"
        "X....................@xxxX"
        "XXXXXXXXXXXXXXXXXXXXXXXXXX";
  std::string const expect3
      = "XXXXXXXXXXXXXXXXXXXXXXXXXX"
        "X........................X"
        "X...................xxxxxX"
        "X.....................xxxX"
        "X..............m.....@xxxX"
        "XXXXXXXXXXXXXXXXXXXXXXXXXX";
  std::string const expect4
      = "XXXXXXXXXXXXXXXXXXXXXXXXXX"
        "X........................X"
        "X...................xxxxxX"
        "X.....................xxxX"
        "X...................m@xxxX"
        "XXXXXXXXXXXXXXXXXXXXXXXXXX";

  //
  // Create the level and start playing
  //
  Overrides overrides;
  overrides[ 'm' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("kobalos"); };
  Levelp  l        = nullptr;
  Levelsp v        = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);

  //
  // The guts of the test
  //
  bool result {};

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 15; tries++) {
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
  for (auto tries = 0; tries < 5; tries++) {
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
  for (auto tries = 0; tries < 5; tries++) {
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
    if (! (result = level_match_contents(g, v, l, t, w, h, expect3.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 10; tries++) {
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
  TEST_ASSERT(t, game_tick_get(g, v) == 35, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_monst_vision_direction() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("monst_vision_direction");

  test_callback_set(test, test_monst_vision_direction);

  return true;
}
