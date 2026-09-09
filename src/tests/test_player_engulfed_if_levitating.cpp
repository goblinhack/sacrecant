//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_player_engulfed_if_levitating(Gamep g, Testp t) -> bool
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
      = "XXXXXXX"
        "X.....X"
        "X.....X"
        "X@...MX"
        "X.....X"
        "X.....X"
        "XXXXXXX";
  std::string const expect1
      = "XXXXXXX"
        "X.....X"
        "X.....X"
        "X@M...X"
        "X.....X"
        "X.....X"
        "XXXXXXX";
  std::string const expect2
      = "XXXXXXX"
        "X.....X"
        "X.....X"
        "X@M...X"
        "X.....X"
        "X.....X"
        "XXXXXXX";

  //
  // Create the level and start playing
  //
  Overrides overrides;
  overrides[ 'M' ]  = [](char c, bpoint p) -> Tpp { return tp_find_mand("cleaner"); };
  Levelp  l         = nullptr;
  Levelsp v         = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
  int     use_count = 0;

  //
  // The guts of the test
  //
  bool result {};

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
  // Wait to be engulfed
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

  TEST_ASSERT(t, ! thing_is_engulfed(player), "was not expecting player to be engulfed");

  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 11, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_player_engulfed_if_levitating() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("player_engulfed_if_levitating");

  // begin sort marker1 {
  test_callback_set(test, test_player_engulfed_if_levitating);
  // end sort marker1 }

  return true;
}
