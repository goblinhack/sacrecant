//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_teleport_chasm_bad_luck(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 8;
  auto           h         = 8;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxxx"
        "x......x"
        "x......x"
        "x...C.Cx"
        "x@T.CTCx"
        "x...CCCx"
        "x......x"
        "xxxxxxxx";
  std::string const expect1
      = "xxxxxxxx"
        "x......x"
        "x......x"
        "x...C@Cx"
        "x.T.CTCx"
        "x...CCCx"
        "x......x"
        "xxxxxxxx";
  std::string const level2
      = "xxxxxxxx"
        "x......x"
        "x......x"
        "x......x"
        "x......x"
        "x....C.x"
        "x......x"
        "xxxxxxxx";

  //
  // Create the level and start playing
  //
  Levelp  l1 = nullptr;
  Levelp  l2 = nullptr;
  Levelsp v  = game_test_init(g, &l1, level_num, w, h, start.c_str());
  game_test_init_level(g, v, &l2, level_num + 1, w, h, level2.c_str());

  //
  // The guts of the test
  //
  bool result {};
  bool up {};
  bool down {};
  bool left {};
  bool right {};

  //
  // Move right and teleport
  //
  level_dump(g, v, l1, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move fail");
      goto exit;
    }

    if (! game_wait_for_tick_to_finish(g, v, l1)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }

    if (! (result = level_match_contents(g, v, l1, t, w, h, expect1.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 1, "final tick counter value");
  level_dump(g, v, l1, w, h);

  if (! (result = level_match_contents(g, v, l2, t, w, h, level2.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  level_dump(g, v, l2, w, h);

  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_teleport_chasm_bad_luck() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("teleport_chasm_bad_luck");

  test_callback_set(test, test_teleport_chasm_bad_luck);

  return true;
}
