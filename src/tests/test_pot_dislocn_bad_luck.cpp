//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_pot_disloc_bad_luck(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = MAP_WIDTH;
  auto           h         = MAP_HEIGHT;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"  //
        "x......x....................x..................x"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xxxxxxxxxxxxxxxxxxxxxxxxxLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxxxxxxxxxxxxxxxxxxxxxxLLLLLLLLLLLLLLLLLLx"  //
        "x......................@.......................x"  //
        "x..............................................x"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "x...........................x..................x"  //
        "x...........................x..................x"  //
        "x.............................................Ex"  //
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; //
  std::string const expect1
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"  //
        "x......x....................x..................x"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xxxxxxxxxxxxxxxxxxxxxxxxxLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLxxxxxxxxxxxxxxxxxxxxxxLLLLLLLLLLLLLLLLLLx"  //
        "x..............................................x"  //
        "x..............................................x"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLL@xLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLxLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "xLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLx"  //
        "x...........................x..................x"  //
        "x...........................x..................x"  //
        "x.............................................Ex"  //
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; //

  //
  // Create the level and start playing
  //
  Levelp  l = nullptr;
  Levelsp v = game_test_init(g, &l, level_num, w, h, start.c_str());

  //
  // The guts of the test
  //
  bool result {};
  int  use_count = 0;

  static std::initializer_list< std::string > usable_items = {
      "pot_disloc", //
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

  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 1, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_pot_disloc_bad_luck() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("pot_disloc_bad_luck");

  // begin sort marker1 {
  test_callback_set(test, test_pot_disloc_bad_luck);
  // end sort marker1 }

  return true;
}
