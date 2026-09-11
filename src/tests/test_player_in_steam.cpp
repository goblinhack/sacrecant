//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_player_in_steam(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 7;
  auto           h         = 7;

  std::string const level // first level
      = "......."
        "......."
        "......."
        "...@..."
        "......."
        "......."
        ".......";

  //
  // Create the level and start playing
  //
  Levelp  l = nullptr;
  Levelsp v = game_test_init(g, &l, level_num, w, h, level.c_str());

  //
  // The guts of the test
  //
  bool   result {};
  Thingp player = nullptr;

  //
  // Find the player
  //
  level_dump(g, v, l, w, h);
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
  // Spawn steam on the player
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (thing_spawn(g, v, l, tp_first(is_steam), player) == nullptr) {
    TEST_FAILED(t, "spawn failed");
    goto exit;
  }
  if (thing_spawn(g, v, l, tp_first(is_steam), player) == nullptr) {
    TEST_FAILED(t, "spawn failed");
    goto exit;
  }
  if (thing_spawn(g, v, l, tp_first(is_steam), player) == nullptr) {
    TEST_FAILED(t, "spawn failed");
    goto exit;
  }
  if (thing_spawn(g, v, l, tp_first(is_steam), player) == nullptr) {
    TEST_FAILED(t, "spawn failed");
    goto exit;
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 3; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  TEST_ASSERT(t, ! thing_is_dead(player), "player is dead");

  //
  // Wait until the player is dead
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 100; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }

    if (thing_is_dead(player)) {
      break;
    }
  }

  TEST_ASSERT(t, thing_is_dead(player), "player is expected to be dead");
  TEST_ASSERT(t, game_tick_get(g, v) == 4, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);

  result = true;
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_player_in_steam() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("player_in_steam");

  test_callback_set(test, test_player_in_steam);

  return true;
}
