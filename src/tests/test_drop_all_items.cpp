//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_drop_all_items(Gamep g, Testp t) -> bool
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
        "x$........................x"
        "x@........................x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";

  Levelp  l      = nullptr;
  Levelsp v      = game_test_init(g, &l, level_num, w, h, start.c_str());
  bool    result = true;

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
  // Drop all items. Testing the internal api
  //
  {
    ThingEvent e {
        .reason     = "drop item",                //
        .event_type = THING_EVENT_USER_INITIATED, //
        .source     = player,                     //
    };

    TEST_ASSERT(t, thing_drop_all(g, v, l, player, e), "failed to drop");
  }

  TEST_ASSERT(t, thing_inventory_get_item_count(g, v, l, player) == 0, "expected no items");

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 0, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_drop_all_items() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("drop_all_items");

  test_callback_set(test, test_drop_all_items);

  return true;
}
