//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_kraken_teleport(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelArrIdx const level_num = 0;
  auto           w         = 14;
  auto           h         = 18;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "XXXXXXXXXXXXXX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWW@WWWWWWX"
        "XWWWWWWWWWWWWX"
        "XXXXXXXXXXXXXX";
  std::string const expect
      = "XXXXXXXXXXXXXX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWTTTTTTTTTTWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWWWWWWWWWWX"
        "XWWWmmmmWWWWWX"
        "XWWWWW@WWWWWWX"
        "XWWWWWWWWWWWWX"
        "XXXXXXXXXXXXXX";

  //
  // Create the level and start playing
  //
  Overrides overrides;
  Levelp    l = nullptr;
  Levelsp   v = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);

  //
  // The guts of the test
  //
  bool result {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  for (auto x = -4; x <= 5; x++) {
    if (thing_spawn(g, v, l, tp_first(is_teleport), thing_at(g, v, l, player) + bpoint(x, -6)) == nullptr) {
      TEST_FAILED(t, "spawn failed");
      goto exit;
    }
  }

  if (thing_spawn(g, v, l, tp_first(is_boss2), thing_at(g, v, l, player) + bpoint(0, -9)) == nullptr) {
    TEST_FAILED(t, "spawn failed");
    goto exit;
  }

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
    if (! (result = level_match_contents(g, v, l, t, w, h, expect.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 15, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_kraken_teleport() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("kraken_teleport");

  test_callback_set(test, test_kraken_teleport);

  return true;
}
