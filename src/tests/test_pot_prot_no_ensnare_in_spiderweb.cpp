//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_pot_prot_no_ensnare_in_spiderweb(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelArrIdx const level_num = 0;
  auto              w         = 17;
  auto              h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxxxxxxxxxxxx"
        "x...............x"
        "x...............x"
        "x@wwwwwww.......x"
        "x...............x"
        "x...............x"
        "xxxxxxxxxxxxxxxxx";
  std::string const expect1
      = "xxxxxxxxxxxxxxxxx"
        "x...............x"
        "x...............x"
        "x.wwwwwww..@....x"
        "x...............x"
        "x...............x"
        "xxxxxxxxxxxxxxxxx";

  LevelGenOverrides overrides;
  Levelp            l = nullptr;
  Levelsp           v = game_test_init(g, &l, level_num, w, h, start.c_str());
  //
  // The guts of the test
  //
  bool up {};
  bool down {};
  bool left {};
  bool right {};
  bool result {};
  int  use_count = 0;

  const std::initializer_list< std::string > items = {
      "pot_prot", //
      "pot_prot", //
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

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  for (auto tries = 0; tries < 10; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);

    TEST_LOG(t, "move right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

    (void) player_move_request(g, up, down, left, right, false /* fire */);

    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 12, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_pot_prot_no_ensnare_in_spiderweb() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("pot_prot_no_ensnare_in_spiderweb");

  test_callback_set(test, test_pot_prot_no_ensnare_in_spiderweb);

  return true;
}
