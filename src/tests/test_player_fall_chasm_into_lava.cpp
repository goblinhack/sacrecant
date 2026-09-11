//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_player_fall_chasm_into_lava(Gamep g, Testp t) -> bool
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
        "...C..."
        "...C..."
        "......."
        ".......";
  std::string const level2 // second level
      = "xxxxxxx"
        "xxxxxxx"
        "xxLLLxx"
        "xxLLLxx"
        "xxLLLxx"
        "xxxxxxx"
        "xxxxxxx";
  std::string const expect2 // second level
      = "xxxxxxx"
        "xxxxxxx"
        "xxLLLxx"
        "xx@LLxx"
        "xxLLLxx"
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

  bool   found_corpse {};
  Thingp player = nullptr;
  bpoint p;

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
  if (! (result = level_match_contents(g, v, l1, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_PROGRESS(t);
  {
    if (! (result = level_match_contents(g, v, l2, t, w, h, expect2.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 3; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l2, tries, w, h);
    TEST_LOG(t, "try: %d", tries);
    TRACE();
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    TEST_ASSERT(t, game_wait_for_tick_to_finish(g, v, l2), "failed to wait for tick to finish");
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 5, "final tick counter value");

  //
  // Check player is dead when shoved into lava. It should be popped off the level.
  //
  TEST_PROGRESS(t);
  {
    TRACE();
    TEST_LOG(t, "check player is dead when in lava");
    p            = thing_at(g, v, l2, player);
    found_corpse = false;

    FOR_ALL_THINGS_AT(g, v, l2, it, p)
    {
      if (thing_is_player(it) && thing_is_corpse(it)) {
        found_corpse = true;
        break;
      }
    }

    if (! found_corpse) {
      TEST_FAILED(t, "did not find player as a corpse");
      goto exit;
    }
  }

  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_player_fall_chasm_into_lava() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("player_fall_chasm_into_lava");

  test_callback_set(test, test_player_fall_chasm_into_lava);

  return true;
}
