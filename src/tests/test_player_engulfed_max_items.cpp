//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_player_engulfed_max_items(Gamep g, Testp t) -> bool
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
        "X@..123456mX"
        "X..........X"
        "X..........X"
        "XXXXXXXXXXXX";
  std::string const expect1
      = "XXXXXXXXXXXX"
        "X..........X"
        "X..........X"
        "X@..$$$....X"
        "X..........X"
        "X..........X"
        "XXXXXXXXXXXX";

  //
  // Create the level and start playing
  //
  Overrides overrides;
  overrides[ '1' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("pot_healing"); };
  overrides[ '2' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("pot_incin"); };
  overrides[ '3' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("pot_stealth"); };
  overrides[ '4' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("pot_tireless"); };
  overrides[ '5' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("horseshoe"); };
  overrides[ '6' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("chest"); };
  overrides[ 'm' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("cleaner"); };
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
  // Wait for the monster to consume all the items
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 20; tries++) {
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

  TEST_ASSERT(t, thing_is_engulfed(player), "was expecting player to be engulfed");

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 20, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_player_engulfed_max_items() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("player_engulfed_max_items");

  test_callback_set(test, test_player_engulfed_max_items);

  return true;
}
