//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_monst_voider(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 12;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "XXXXXXXXXXXX"
        "X..........X"
        "X..........X"
        "X.@......m.X"
        "X..........X"
        "X..........X"
        "XXXXXXXXXXXX";
  std::string const expect1
      = "XXXXXXXXXXXX"
        "X..........X"
        "X.......CCCX"
        "X.@.....CCCX"
        "X.......CCCX"
        "X..........X"
        "XXXXXXXXXXXX";

  //
  // Create the level and start playing
  //
  ThingEvent e {};
  Overrides  overrides;
  overrides[ 'm' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("voider"); };
  Thingp  monst    = nullptr;
  Levelp  l        = nullptr;
  Levelsp v        = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);

  //
  // The guts of the test
  //
  bool result {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  //
  // Find the monster
  //
  FOR_ALL_THINGS_AT(g, v, l, it, thing_at(g, v, l, player) + bpoint(7, 0))
  {
    if (thing_is_monst(it)) {
      monst = it;
    }
  }

  TEST_ASSERT(t, monst, "was expecting to find a monster");

  //
  // Kill the monster. Dumping the chest again.
  //
  thing_dead(g, v, l, monst, e);

  for (auto tries = 0; tries < 1; tries++) {
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

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 1, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_monst_voider() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("monst_voider");

  test_callback_set(test, test_monst_voider);

  return true;
}
