//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_glass_bones(Gamep g, Testp t) -> bool
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
      = "......."
        "......."
        "...C..."
        "...C..."
        "...C..."
        "......."
        ".......";
  std::string const expect2 // second level
      = "......."
        "......."
        "...C..."
        "...C..."
        "...C..."
        "......."
        ".......";
  std::string const level3 // third level
      = "......."
        "......."
        "......."
        "......."
        "......."
        "......."
        ".......";
  std::string const expect3 // third level
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
  Levelp  l  = nullptr;
  Levelp  l1 = nullptr;
  Levelp  l2 = nullptr;
  Levelp  l3 = nullptr;
  Levelsp v  = game_test_init(g, &l1, level_num, w, h, level1.c_str());
  game_test_init_level(g, v, &l2, level_num + 1, w, h, level2.c_str());
  game_test_init_level(g, v, &l3, level_num + 2, w, h, level3.c_str());

  //
  // The guts of the test
  //
  bool result {};
  bool up {};
  bool down {};
  bool left {};
  bool right {};
  int  health     = 0;
  int  health_max = 0;

  Thingp player = nullptr;

  static std::initializer_list< std::string > items = {
      "pot_tireless", //
  };

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

  if (! thing_carry(g, v, l1, player, items)) {
    TEST_FAILED(t, "no item carried");
    goto exit;
  }

  TEST_ASSERT(t, thing_hook_add(g, v, l1, player, tp_find_mand("sac_glass_bones")), "failed to add sacrifice");

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

  if (compiler_unused) {
    TEST_PROGRESS(t);
    for (auto tries = 0; tries < 2; tries++) {
      TEST_LOOP_PROGRESS(t, g, v, l1, tries, w, h);
      TEST_LOG(t, "try: %d", tries);
      TRACE();
      TEST_ASSERT(t, game_event_wait(g), "failed to wait");
      TEST_ASSERT(t, game_wait_for_tick_to_finish(g, v, l1), "failed to wait for tick to finish");
    }
  }

  //
  // Player should have fallen all the way down now
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
  {
    if (! (result = level_match_contents(g, v, l3, t, w, h, expect3.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  l          = thing_level(g, v, player);
  health     = thing_health(g, v, l, player);
  health_max = thing_health_max(g, v, l, player);
  TEST_ASSERT(t, health == health_max / 2, "expected full health");

  TEST_ASSERT(t, game_tick_get(g, v) == 2, "final tick counter value");

  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_sac_glass_bones() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("sac_glass_bones");

  test_callback_set(test, test_glass_bones);

  return true;
}
