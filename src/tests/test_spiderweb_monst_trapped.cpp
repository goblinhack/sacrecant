//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_spiderweb_monst_trapped(Gamep g, Testp t) -> bool
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
        "x@wwwwwwm.......x"
        "x...............x"
        "x...............x"
        "xxxxxxxxxxxxxxxxx";
  std::string const expect1
      = "xxxxxxxxxxxxxxxxx"
        "x...............x"
        "x...............x"
        "x@wwwwww........x"
        "x...............x"
        "x...............x"
        "xxxxxxxxxxxxxxxxx";
  std::string const expect2
      = "xxxxxxxxxxxxxxxxx"
        "x...............x"
        "x...............x"
        "x@wwwmww........x"
        "x...............x"
        "x...............x"
        "xxxxxxxxxxxxxxxxx";

  Overrides overrides;
  overrides[ 'm' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("kobalos"); };
  Levelp  l        = nullptr;
  Levelsp v        = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
  bool    result   = true;

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  //
  // Wait to see how far the monst gets
  //
  for (auto tries = 0; tries < 50; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect2.c_str()))) {
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

[[nodiscard]] auto test_load_spiderweb_monst_trapped() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("spiderweb_monst_trapped");

  test_callback_set(test, test_spiderweb_monst_trapped);

  return true;
}
