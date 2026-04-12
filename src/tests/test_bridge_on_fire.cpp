//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_bridge_on_fire(Gamep g, Testp t) -> bool
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
      = "......."
        "......."
        "...CCC."
        "..@===."
        "...CCC."
        "......."
        ".......";
  std::string const expect1
      = "......."
        "......."
        "...CCC."
        "..@C;C."
        "...CCC."
        "......."
        ".......";
  std::string const level2
      = "......."
        "......."
        "......."
        "......."
        "......."
        "......."
        ".......";
  std::string const expect2 // second level
      = "......."
        "......."
        "......."
        "....!.."
        "......."
        "......."
        ".......";

  //
  // Create the level and start playing
  //
  Levelp  l1 = nullptr;
  Levelp  l2 = nullptr;
  Levelsp v  = game_test_init(g, &l1, level_num, w, h, start.c_str());
  game_test_init_level(g, v, &l2, level_num + 1, w, h, level2.c_str());

  //
  // The guts of the test
  //
  bool   result = false;
  Thingp player = nullptr;

  //
  // Push the mob into lava
  //
  TEST_LOG(t, "spawn fire over a bridge");
  TRACE();

  //
  // Find the player
  //
  TEST_PROGRESS(t);
  {
    TRACE();
    player = thing_player(g);
    if (player == nullptr) [[unlikely]] {
      TEST_FAILED(t, "no player");
      goto exit;
    }
  }

  //
  // Spawn fire twice. This should be enough to destroy the bridge.
  //
  if (thing_spawn(g, v, l1, tp_first(is_fire), thing_at(player) + bpoint(2, 0)) == nullptr) {
    TEST_FAILED(t, "spawn failed");
    goto exit;
  }

  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 3; tries++) {
    TEST_LOG(t, "try: %d", tries);
    TRACE();
    level_dump(g, v, l1, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l1)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  //
  // Check the level contents
  //
  TEST_PROGRESS(t);
  {
    TRACE();
    if (! (result = level_match_contents(g, v, l1, t, w, h, expect1.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  TEST_PROGRESS(t);
  {
    TRACE();
    if (! (result = level_match_contents(g, v, l2, t, w, h, expect2.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  //
  // Check the tick is as expected
  //
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 3, "final tick counter value");

  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

auto test_load_bridge_on_fire() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("bridge_on_fire");

  // begin sort marker1 {
  test_callback_set(test, test_bridge_on_fire);
  // end sort marker1 }

  return true;
}
