//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_player_engulfed_treasure_chest(Gamep g, Testp t) -> bool
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
        "X@....$...mX"
        "X..........X"
        "X..........X"
        "XXXXXXXXXXXX";
  std::string const expect1
      = "XXXXXXXXXXXX"
        "X..........X"
        "X..........X"
        "X@..M......X"
        "X..........X"
        "X..........X"
        "XXXXXXXXXXXX";
  std::string const expect2
      = "XXXXXXXXXXXX"
        "X..........X"
        "X..........X"
        "X@..$......X"
        "X..........X"
        "X..........X"
        "XXXXXXXXXXXX";
  std::string const expect3
      = "XXXXXXXXXXXX"
        "X..........X"
        "X..........X"
        "X...@......X"
        "X..........X"
        "X..........X"
        "XXXXXXXXXXXX";

  //
  // Create the level and start playing
  //
  ThingEvent e {};
  Overrides  overrides;
  overrides[ 'm' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("cleaner"); };
  overrides[ '$' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("chest"); };
  Thingp  monst    = nullptr;
  Thingp  chest    = nullptr;
  Levelp  l        = nullptr;
  Levelsp v        = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);

  //
  // The guts of the test
  //
  bool result {};
  bool up {};
  bool down {};
  bool left {};
  bool right {};

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
  for (auto tries = 0; tries < 12; tries++) {
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
  // Find the engulfer
  //
  FOR_ALL_THINGS_AT(g, v, l, it, thing_at(g, v, l, player) + bpoint(3, 0))
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

  //
  // Treasure chest should be back
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 10; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  //
  // Find the treasure
  //
  FOR_ALL_THINGS_AT(g, v, l, it, thing_at(g, v, l, player) + bpoint(3, 0))
  {
    if (thing_is_chest(it)) {
      chest = it;
    }
  }

  TEST_ASSERT(t, chest, "was expecting to find a chest");

  //
  // Check the level contents
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
    if (! (result = level_match_contents(g, v, l, t, w, h, expect2.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  //
  // Move right
  //
  for (auto tries = 0; tries < 3; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);

    TEST_LOG(t, "move right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move fail");
      goto exit;
    }

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
    if (! (result = level_match_contents(g, v, l, t, w, h, expect3.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  TEST_ASSERT(t, thing_inventory_get_item_count(g, v, l, player) == 2, "expected items");

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 26, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_player_engulfed_treasure_chest() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("player_engulfed_treasure_chest");

  // begin sort marker1 {
  test_callback_set(test, test_player_engulfed_treasure_chest);
  // end sort marker1 }

  return true;
}
