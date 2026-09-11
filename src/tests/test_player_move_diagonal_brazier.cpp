//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_player_move_diagonal_brazier(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 7;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxx"
        "x..B..x"
        "x.B.B.x"
        "xB@B.Bx"
        "x.B.B.x"
        "x..B..x"
        "xxxxxxx";
  std::string const expect1
      = "xxxxxxx"
        "x..B..x"
        "x@B.B.x"
        "xB.B.Bx"
        "x.B.B.x"
        "x..B..x"
        "xxxxxxx";
  std::string const expect2
      = "xxxxxxx"
        "x.@B..x"
        "x.B.B.x"
        "xB.B.Bx"
        "x.B.B.x"
        "x..B..x"
        "xxxxxxx";
  std::string const expect3
      = "xxxxxxx"
        "x..B..x"
        "x.B@B.x"
        "xB.B.Bx"
        "x.B.B.x"
        "x..B..x"
        "xxxxxxx";
  std::string const expect4
      = "xxxxxxx"
        "x..B..x"
        "x.B.B.x"
        "xB@B.Bx"
        "x.B.B.x"
        "x..B..x"
        "xxxxxxx";

  //
  // Create the level and start playing
  //
  Levelp  l = nullptr;
  Levelsp v = game_test_init(g, &l, level_num, w, h, start.c_str());

  //
  // The guts of the test
  //
  bool result {};
  bool up {};
  bool down {};
  bool left {};
  bool right {};

  //
  // Top left
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move top left");
    TRACE();
    up = down = left = right = false;
    left                     = true;
    up                       = true;

    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move fail");
      goto exit;
    }

    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }

    if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  //
  // Top right
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move top right");
    TRACE();
    up = down = left = right = false;
    up                       = true;
    right                    = true;

    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move failed");
      goto exit;
    }

    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }

    if (! (result = level_match_contents(g, v, l, t, w, h, expect2.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  //
  // Bottom right
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move botrom right");
    TRACE();
    up = down = left = right = false;
    down                     = true;
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
  // Bottom left
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move bottom left");
    TRACE();
    up = down = left = right = false;
    down                     = true;
    left                     = true;

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

  TEST_ASSERT(t, game_tick_get(g, v) == 4, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_player_move_diagonal_brazier() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("player_move_diagonal_brazier");

  test_callback_set(test, test_player_move_diagonal_brazier);

  return true;
}
