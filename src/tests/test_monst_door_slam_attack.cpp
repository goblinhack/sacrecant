//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_monst_door_slam_attack(Gamep g, Testp t) -> bool
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
        "x...x.x"
        "x...x.x"
        "x.@k+.x"
        "x...x.x"
        "x...xmx"
        "xxxxxxx";
  std::string const expect1
      = "xxxxxxx"
        "x...x.x"
        "x...x.x"
        "x..@+.x"
        "x...x.x"
        "x...xmx"
        "xxxxxxx";
  std::string const expect2
      = "xxxxxxx"
        "x...x.x"
        "x...x.x"
        "x...@mx"
        "x...x.x"
        "x...x.x"
        "xxxxxxx";
  std::string const expect3
      = "xxxxxxx"
        "x...x.x"
        "x...x.x"
        "x..@m.x"
        "x...x.x"
        "x...x.x"
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

  bpoint p;
  Thingp door   = nullptr;
  Thingp monst  = nullptr;
  Thingp player = nullptr;

  //
  // Move right, collecting key
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move right, collecting key");
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

    if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  //
  // Move right again, open door
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move right open door");
    TRACE();

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

  up = down = left = right = false;
  left                     = true;

  //
  // Move left back out of room
  //
  level_dump(g, v, l, w, h);
  {
    TEST_PROGRESS(t);

    TEST_LOG(t, "move left back out of room");
    TRACE();

    (void) player_move_request(g, up, down, left, right, false /* fire */);

    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  {
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  if (! (result = level_match_contents(g, v, l, t, w, h, expect3.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  //
  // Find the player
  //
  level_dump(g, v, l, w, h);

  player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  //
  // Find the door
  //
  TEST_LOG(t, "find the door");
  p    = thing_at(g, v, l, player) + bpoint(1, 0);
  door = nullptr;

  FOR_ALL_THINGS_AT(g, v, l, it, p)
  {
    if (thing_is_door_locked(it)) {
      door = it;
    }
  }

  p     = thing_at(g, v, l, player) + bpoint(1, 0);
  monst = nullptr;

  FOR_ALL_THINGS_AT(g, v, l, it, p)
  {
    if (thing_is_monst(it)) {
      monst = it;
    }
  }

  TEST_ASSERT(t, monst != nullptr, "could not find monst");

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  for (auto tries = 0; tries < 10; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);

    TEST_ASSERT(t, thing_open(g, v, l, door, player), "door open failed");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }

    TEST_ASSERT(t, thing_close(g, v, l, door, player), "door close failed");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  TEST_ASSERT(t, thing_is_dead(monst), "monster not killed in slam attack");

  TEST_ASSERT(t, game_tick_get(g, v) == 25, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_monst_door_slam_attack() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("monst_door_slam_attack");

  // begin sort marker1 {
  test_callback_set(test, test_monst_door_slam_attack);
  // end sort marker1 }

  return true;
}
