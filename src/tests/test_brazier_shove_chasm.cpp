//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_brazier_shove_chasm(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 7;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const level1
      = "......."
        "......."
        "....C.."
        "..@BC.."
        "....C.."
        "......."
        ".......";
  std::string const expect_1a // first shove
      = "......."
        "......."
        "....C.."
        "..@BC.."
        "....C.."
        "......."
        ".......";
  std::string const expect_1b // second shove
      = "......."
        "......."
        "....C.."
        "...@C.."
        "....C.."
        "......."
        ".......";
  std::string const level2
      = "......."
        "......."
        "....x.."
        "....x.."
        "....x.."
        "......."
        ".......";
  std::string const expect2 // second level
      = "......."
        "......."
        "...Bx.."
        "....x.."
        "....x.."
        "......."
        ".......";

  //
  // Create the level and start playing
  //
  Levelp  l1 = nullptr;
  Levelp  l2 = nullptr;
  Levelsp v  = game_test_init(g, &l1, level_num, w, h, level1.c_str());
  game_test_init_level(g, v, &l2, level_num + 1, w, h, level2.c_str());

  //
  // The guts of the test
  //
  bool   result {};
  bool   up {};
  bool   down {};
  bool   left {};
  bool   right {};
  bpoint p;
  bool   found_it {};
  Thingp player = nullptr;
  int    tries  = 0;

  //
  // Bump into a brazier. It should be knocked over.
  //
  TEST_PROGRESS(t);
  TEST_LOG(t, "move right");
  TRACE();
  up = down = left = right = false;
  right                    = true;

  if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
    level_dump(g, v, l1, w, h);
    level_dump(g, v, l2, w, h);
    TEST_FAILED(t, "move failed");
    goto exit;
  }

  TEST_ASSERT(t, game_wait_for_tick_to_finish(g, v, l1), "failed to wait for tick to finish");

  //
  // Wait for smoke to clear
  //
  TEST_PROGRESS(t);
  for (tries = 0; tries < 10; tries++) {
    TEST_LOG(t, "try: %d", tries);
    TRACE();
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    TEST_ASSERT(t, game_wait_for_tick_to_finish(g, v, l1), "failed to wait for tick to finish");
  }

  if (! (result = level_match_contents(g, v, l1, t, w, h, expect_1a.c_str()))) {
    level_dump(g, v, l1, w, h);
    level_dump(g, v, l2, w, h);
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  //
  // Shove it. It should fall in the chasm now.
  //
  TEST_PROGRESS(t);
  TEST_LOG(t, "move right again");
  up = down = left = right = false;
  right                    = true;

  if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
    level_dump(g, v, l1, w, h);
    level_dump(g, v, l2, w, h);
    TEST_FAILED(t, "move failed");
    goto exit;
  }

  TEST_ASSERT(t, game_wait_for_tick_to_finish(g, v, l1), "failed to wait for tick to finish");

  if (! (result = level_match_contents(g, v, l1, t, w, h, expect_1b.c_str()))) {
    level_dump(g, v, l1, w, h);
    level_dump(g, v, l2, w, h);
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    level_dump(g, v, l1, w, h);
    level_dump(g, v, l2, w, h);
    TEST_FAILED(t, "no player");
    goto exit;
  }

  //
  // Check the brazier is dead
  //
  TEST_LOG(t, "check brazier is dead");
  p        = thing_at(g, v, l2, player) + bpoint(0, -1);
  found_it = false;

  TEST_PROGRESS(t);
  FOR_ALL_THINGS_AT(g, v, l2, it, p)
  {
    if (thing_is_brazier(it) && thing_is_dead(it)) {
      found_it = true;
      break;
    }
  }

  if (! found_it) {
    level_dump(g, v, l1, w, h);
    level_dump(g, v, l2, w, h);
    TEST_FAILED(t, "no dead brazier");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 13, "final tick counter value");

  TEST_PROGRESS(t);
  {
    if (! (result = level_match_contents(g, v, l2, t, w, h, expect2.c_str()))) {
      level_dump(g, v, l1, w, h);
      level_dump(g, v, l2, w, h);
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_brazier_shove_chasm() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("brazier_shove_chasm");

  // begin sort marker1 {
  test_callback_set(test, test_brazier_shove_chasm);
  // end sort marker1 }

  return true;
}
