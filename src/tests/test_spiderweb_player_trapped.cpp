//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_spiderweb_player_trapped(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 17;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxxxxxxxxxxxx"
        "x...............x"
        "x...............x"
        "x@wwwwwww.......x"
        "x...............x"
        "x...............x"
        "xxxxxxxxxxxxxxxxx";
  std::string const expect1
      = "xxxxxxxxxxxxxxxxx"
        "x...............x"
        "x...............x"
        "x.ww@wwww.......x"
        "x...............x"
        "x...............x"
        "xxxxxxxxxxxxxxxxx";

  Overrides overrides;
  Levelp    l = nullptr;
  Levelsp   v = game_test_init(g, &l, level_num, w, h, start.c_str());
  //
  // The guts of the test
  //
  bool up {};
  bool down {};
  bool left {};
  bool right {};
  bool result {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  for (auto tries = 0; tries < 50; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);

    TEST_LOG(t, "move right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

    (void) player_move_request(g, up, down, left, right, false /* fire */);

    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 50, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_spiderweb_player_trapped() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("spiderweb_player_trapped");

  test_callback_set(test, test_spiderweb_player_trapped);

  return true;
}
