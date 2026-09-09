//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_drop_then_recollect_items(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 27;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x@........................x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string const expect1
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x$$$$$$$$$$$$$$$@.........x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string const expect2
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x@........................x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";

  Levelp  l          = nullptr;
  Levelsp v          = game_test_init(g, &l, level_num, w, h, start.c_str());
  bool    result     = true;
  int     drop_count = 0;
  bool    up {};
  bool    down {};
  bool    left {};
  bool    right {};

  static std::initializer_list< std::string > items = {
      "staff_fire",   //
      "staff_fire",   //
      "staff_energy", //
      "wand_fire",    //
      "wand_energy",  //
      "staff_fire",   //
      "staff_fire",   //
      "staff_energy", //
      "wand_fire",    //
      "wand_energy",  //
      "pot_healing",  //
      "pot_healing",  //
      "pot_healing",  //
      "pot_healing",  //
      "pot_healing",  //
  };

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  if (! thing_carry(g, v, l, player, items)) {
    TEST_FAILED(t, "no item carried");
    goto exit;
  }

  //
  // Drop all items
  //
  for (;;) {
    bool got_item {};

    FOR_ALL_INVENTORY_ITEMS(g, v, l, player, an_item)
    {
      got_item = true;

      ThingEvent e {
          .reason     = "drop item",                //
          .event_type = THING_EVENT_USER_INITIATED, //
          .source     = player,                     //
      };

      TEST_ASSERT(t, thing_drop(g, v, l, player, an_item, e), "failed to drop");

      TRACE();
      level_dump(g, v, l, w, h);
      TEST_ASSERT(t, game_event_wait(g), "failed to wait");

      TEST_LOG(t, "move right");
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

      drop_count++;
    }

    if (! got_item) {
      break;
    }
  }

  TEST_ASSERT(t, drop_count == (int) items.size(), "did not drop expected item amount");

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  //
  // Recollect all items
  //
  for (auto i = 0; i < (int) items.size(); i++) {

    TEST_LOG(t, "move left");
    TRACE();
    up = down = left = right = false;
    left                     = true;

    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move fail");
      goto exit;
    }

    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect2.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 45, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_drop_then_recollect_items() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("drop_then_recollect_items");

  // begin sort marker1 {
  test_callback_set(test, test_drop_then_recollect_items);
  // end sort marker1 }

  return true;
}
