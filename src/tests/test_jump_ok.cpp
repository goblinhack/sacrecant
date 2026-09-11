//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_jump_ok(Gamep g, Testp t) -> bool
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
        "x.....x"
        "x..C..x"
        "x.@C..x"
        "x..C..x"
        "x.....x"
        "xxxxxxx";
  std::string const expect1
      = "xxxxxxx"
        "x.....x"
        "x..C..x"
        "x..C@.x"
        "x..C..x"
        "x.....x"
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

  //
  // Jump
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "jump");
    TRACE();

    auto *player = thing_player(g);
    if (player == nullptr) [[unlikely]] {
      TEST_FAILED(t, "no player");
      goto exit;
    }

    if (! (result = player_jump(g, v, l, player, thing_at(g, v, l, player) + bpoint(2, 0)))) {
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

[[nodiscard]] auto test_load_jump_ok() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("jump_ok");

  test_callback_set(test, test_jump_ok);

  return true;
}
