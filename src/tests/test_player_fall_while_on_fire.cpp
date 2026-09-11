//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_player_fall_while_on_fire(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 7;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const level1 // first level
      = "......."
        "......."
        "...C..."
        "..@C..."
        "...C..."
        "......."
        ".......";
  std::string const expect1 // first level
      = "......."
        "......."
        "...C..."
        "..!;..."
        "...C..."
        "......."
        ".......";
  std::string const level2 // second level
      = "xxxxxxx"
        "xxxxxxx"
        "xx...xx"
        "xx...xx"
        "xx...xx"
        "xxxxxxx"
        "xxxxxxx";
  std::string const expect2 // second level
      = "xxxxxxx"
        "xxxxxxx"
        "xx...xx"
        "xx.@.xx"
        "xx...xx"
        "xxxxxxx"
        "xxxxxxx";

  //
  // Create the level and start playing
  //
  Levelp  l1 = nullptr;
  Levelp  l2 = nullptr;
  Levelsp v  = game_test_init(g, &l1, level_num, w, h, level1.c_str());
  game_test_init_level(g, v, &l2, level_num + 1, w, h, level2.c_str());

  //
  // The guts of the test
  //
  bool result {};
  bool up {};
  bool down {};
  bool left {};
  bool right {};

  Thingp player = nullptr;

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
  // Spawn fire on the player
  //
  TEST_PROGRESS(t);
  {
    if (thing_spawn(g, v, l1, tp_first(is_fire), player) == nullptr) {
      TEST_FAILED(t, "spawn failed");
      goto exit;
    }
    if (thing_spawn(g, v, l1, tp_first(is_fire), player) == nullptr) {
      TEST_FAILED(t, "spawn failed");
      goto exit;
    }
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    TEST_ASSERT(t, game_wait_for_tick_to_finish(g, v, l1), "failed to wait for tick to finish");

    TEST_ASSERT(t, thing_is_burning(player), "player is not burning");
    TEST_ASSERT(t, ! thing_is_dead(player), "player is dead");
  }

  //
  // Move right
  //
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move failed");
      goto exit;
    }

    TEST_ASSERT(t, game_wait_for_tick_to_finish(g, v, l1), "failed to wait for tick to finish");
  }

  //
  // Player should have fallen now
  //
  TEST_PROGRESS(t);
  {
    if (! (result = level_match_contents(g, v, l1, t, w, h, expect1.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  //
  // Player should have landed now
  //
  TEST_PROGRESS(t);
  {
    if (! (result = level_match_contents(g, v, l2, t, w, h, expect2.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  TEST_ASSERT(t, ! thing_is_burning(player), "player is burning still");
  TEST_ASSERT(t, ! thing_is_dead(player), "player is dead");
  TEST_ASSERT(t, game_tick_get(g, v) == 2, "final tick counter value");

  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_player_fall_while_on_fire() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("player_fall_while_on_fire");

  test_callback_set(test, test_player_fall_while_on_fire);

  return true;
}
