//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_clown_meat(Gamep g, Testp t) -> bool
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
      = "xxxxxxx"
        "x.....x"
        "x.....x"
        "x..@..x"
        "x.....x"
        "x.....x"
        "xxxxxxx";
  std::string const expect1
      = "xxxxxxx"
        "x.....x"
        "x.....x"
        "x..@..x"
        "x.....x"
        "x.....x"
        "xxxxxxx";
  std::string const expect2
      = "xxxxxxx"
        "x.....x"
        "x.....x"
        "x..@..x"
        "x.....x"
        "x.....x"
        "xxxxxxx";

  //
  // Create the level and start playing
  //
  Levelp  l = nullptr;
  Levelsp v = game_test_init(g, &l, level_num, w, h, start.c_str());

  //
  // The guts of the test
  //
  bool result {};
  int  eat_count  = 0;
  int  health     = 0;
  int  health_max = 0;

  static std::initializer_list< std::string > items = {
      "clown_meat", //
      "clown_meat", //
      "clown_meat", //
      "clown_meat", //
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

  (void) thing_health_set(g, v, l, player, 10);

  for (;;) {
    bool got_item {};

    FOR_ALL_INVENTORY_ITEMS(g, v, l, player, an_item)
    {
      got_item = true;

      ThingEvent e {
          .reason     = "eatr ate item",            //
          .event_type = THING_EVENT_USER_INITIATED, //
          .source     = player,                     //
      };

      TEST_ASSERT(t, thing_eat(g, v, l, player, an_item, e), "failed to eat");

      TRACE();
      level_dump(g, v, l, w, h);
      TEST_ASSERT(t, game_event_wait(g), "failed to wait");

      if (! game_wait_for_tick_to_finish(g, v, l)) {
        TEST_FAILED(t, "wait loop failed");
        goto exit;
      }

      eat_count++;
    }

    if (! got_item) {
      break;
    }
  }

  TEST_ASSERT(t, eat_count == (int) items.size(), "did not eat expected item amount");

  health     = thing_health(g, v, l, player);
  health_max = thing_health_max(g, v, l, player);
  TEST_ASSERT(t, health == health_max, "expected full health");

  //
  // Check the level contents
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
    if (! (result = level_match_contents(g, v, l, t, w, h, expect2.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 4, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_clown_meat() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("clown_meat");

  test_callback_set(test, test_clown_meat);

  return true;
}
