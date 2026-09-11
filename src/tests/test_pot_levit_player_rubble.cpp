//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_pot_levit_player_rubble(Gamep g, Testp t) -> bool
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
        "......."
        "..@r..."
        "......."
        "......."
        ".......";
  std::string const expect1
      = "......."
        "......."
        "......."
        "...r@.."
        "......."
        "......."
        ".......";

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
  bool   found_corpse {};

  int use_count = 0;

  static std::initializer_list< std::string > usable_items = {
      "pot_levit", //
  };

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  if (! thing_carry(g, v, l, player, usable_items)) {
    TEST_FAILED(t, "no item carried");
    goto exit;
  }

  for (;;) {
    bool got_item {};

    FOR_ALL_INVENTORY_ITEMS(g, v, l, player, an_item)
    {
      got_item = true;

      ThingEvent e {
          .reason     = "user used item",           //
          .event_type = THING_EVENT_USER_INITIATED, //
          .source     = player,                     //
      };

      TEST_ASSERT(t, thing_use(g, v, l, player, an_item, e), "failed to use");

      TRACE();
      level_dump(g, v, l, w, h);
      TEST_ASSERT(t, game_event_wait(g), "failed to wait");

      if (! game_wait_for_tick_to_finish(g, v, l)) {
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
  // Move over the rubble.
  //
  {
    TEST_LOG(t, "move player into rubble right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

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
  }

  //
  // Move right
  //
  {
    level_dump(g, v, l, w, h);
    TEST_PROGRESS(t);
    {
      TRACE();
      if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
        TEST_FAILED(t, "move failed");
        goto exit;
      }
    }
  }

  //
  // Wait for the end of tick
  //
  {
    level_dump(g, v, l, w, h);
    TEST_PROGRESS(t);
    {
      TRACE();
      if (! game_wait_for_tick_to_finish(g, v, l)) {
        TEST_FAILED(t, "wait loop failed");
        goto exit;
      }
    }
  }

  //
  // Move right
  //
  {
    level_dump(g, v, l, w, h);
    TEST_PROGRESS(t);
    {
      TRACE();
      if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
        TEST_FAILED(t, "move failed");
        goto exit;
      }
    }
  }

  //
  // Wait for the end of tick
  //
  {
    level_dump(g, v, l, w, h);
    TEST_PROGRESS(t);
    {
      TRACE();
      if (! game_wait_for_tick_to_finish(g, v, l)) {
        TEST_FAILED(t, "wait loop failed");
        goto exit;
      }
    }
  }

  //
  // Check the level contents
  //
  {
    level_dump(g, v, l, w, h);
    TEST_PROGRESS(t);
    {
      TRACE();
      if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
        TEST_FAILED(t, "unexpected contents");
        goto exit;
      }
    }
  }

  {
    level_dump(g, v, l, w, h);
    TEST_PROGRESS(t);
    for (auto tries = 0; tries < 10; tries++) {
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
  }

  //
  // Check player is not dead when over rubble. It should be popped off the level.
  //
  {
    level_dump(g, v, l, w, h);
    TEST_PROGRESS(t);
    {
      TRACE();
      TEST_LOG(t, "check player is not dead when in rubble");
      p            = thing_at(g, v, l, player);
      found_corpse = false;

      FOR_ALL_THINGS_AT(g, v, l, it, p)
      {
        if (thing_is_player(it) && thing_is_corpse(it)) {
          found_corpse = true;
          break;
        }
      }

      if (found_corpse) {
        TEST_FAILED(t, "found the player as a corpse");
        goto exit;
      }
    }
  }

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 13, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_pot_levit_player_rubble() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("pot_levit_player_rubble");

  test_callback_set(test, test_pot_levit_player_rubble);

  return true;
}
