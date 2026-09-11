//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_monst_move_diagonal_rubble(Gamep g, Testp t) -> bool
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
        "x@.r..x"
        "x.r.r.x"
        "xrmr.rx"
        "x.r.r.x"
        "x..r..x"
        "xxxxxxx";
  std::string const expect1
      = "xxxxxxx"
        "x@.r..x"
        "x.r.r.x"
        "xrmr.rx"
        "x.r.r.x"
        "x..r..x"
        "xxxxxxx";
  std::string const expect2
      = "xxxxxxx"
        "x@.r..x"
        "x.r.r.x"
        "xrmr.rx"
        "x.r.r.x"
        "x..r..x"
        "xxxxxxx";
  std::string const expect3
      = "xxxxxxx"
        "x@.r..x"
        "x.r.r.x"
        "xrmr.rx"
        "x.r.r.x"
        "x..r..x"
        "xxxxxxx";
  std::string const expect4
      = "xxxxxxx"
        "x@.r..x"
        "x.r.r.x"
        "xrmr.rx"
        "x.r.r.x"
        "x..r..x"
        "xxxxxxx";

  //
  // Create the level and start playing
  //
  Levelp    l = nullptr;
  Overrides overrides;
  overrides[ 'm' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("mantisman"); };
  Levelsp v        = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);

  //
  // The guts of the test
  //
  bool result {};
  bool up {};
  bool down {};
  bool left {};
  bool right {};

  //
  // Move top left
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

  TEST_ASSERT(t, game_tick_get(g, v) == 1, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_monst_move_diagonal_rubble() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("monst_move_diagonal_rubble");

  test_callback_set(test, test_monst_move_diagonal_rubble);

  return true;
}
