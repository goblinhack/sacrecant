//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_pot_levit_player_chasm(Gamep g, Testp t) -> bool
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
        "...@..."
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
        "xxLLLxx"
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

  int use_count = 0;

  static std::initializer_list< std::string > usable_items = {
      "pot_levit", //
  };

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  if (! thing_carry(g, v, l1, player, usable_items)) {
    TEST_FAILED(t, "no item carried");
    goto exit;
  }

  for (;;) {
    bool got_item {};

    FOR_ALL_INVENTORY_ITEMS(g, v, l1, player, an_item)
    {
      got_item = true;

      ThingEvent e {
          .reason     = "user used item",           //
          .event_type = THING_EVENT_USER_INITIATED, //
          .source     = player,                     //
      };

      TEST_ASSERT(t, thing_use(g, v, l1, player, an_item, e), "failed to use");

      TRACE();
      level_dump(g, v, l1, w, h);
      TEST_ASSERT(t, game_event_wait(g), "failed to wait");

      if (! game_wait_for_tick_to_finish(g, v, l1)) {
        TEST_FAILED(t, "wait loop failed");
        goto exit;
      }

      use_count++;
    }

    if (! got_item) {
      break;
    }
  }

  TEST_ASSERT(t, use_count == (int) usable_items.size(), "did not use expected item amount");

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

  {
    TEST_PROGRESS(t);
    for (auto tries = 0; tries < 3; tries++) {
      TEST_LOOP_PROGRESS(t, g, v, l2, tries, w, h);
      TEST_LOG(t, "try: %d", tries);
      TRACE();
      TEST_ASSERT(t, game_event_wait(g), "failed to wait");
      TEST_ASSERT(t, game_wait_for_tick_to_finish(g, v, l2), "failed to wait for tick to finish");
    }
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 5, "final tick counter value");

  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_pot_levit_player_chasm() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("pot_levit_player_chasm");

  test_callback_set(test, test_pot_levit_player_chasm);

  return true;
}
