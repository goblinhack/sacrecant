//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_pot_tireless(Gamep g, Testp t) -> bool
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
        "x..C..x"
        "x.@C..x"
        "x..C..x"
        "x.....x"
        "xxxxxxx";
  std::string const expect1
      = "xxxxxxx"
        "x.....x"
        "x..C..x"
        "x..C@.x"
        "x..C..x"
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
  int  use_count   = 0;
  int  stamina     = 0;
  int  stamina_max = 0;

  static std::initializer_list< std::string > items = {
      "pot_tireless", //
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

  TEST_ASSERT(t, use_count == (int) items.size(), "did not use expected item amount");

  //
  // Jump
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  TEST_LOG(t, "jump");

  if (! (result = player_jump(g, v, l, player, thing_at(g, v, l, player) + bpoint(2, 0)))) {
    TEST_FAILED(t, "move fail");
    goto exit;
  }

  if (! game_wait_for_tick_to_finish(g, v, l)) {
    TEST_FAILED(t, "wait loop failed");
    goto exit;
  }

  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  stamina     = thing_stamina(g, v, l, player);
  stamina_max = thing_stamina_max(g, v, l, player);
  TEST_ASSERT(t, stamina == stamina_max, "expected full stamina");

  TEST_ASSERT(t, game_tick_get(g, v) == 2, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_pot_tireless() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("pot_tireless");

  test_callback_set(test, test_pot_tireless);

  return true;
}
