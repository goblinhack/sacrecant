//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_boots_spider_player_spiderweb(Gamep g, Testp t) -> bool
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
      = "......x"
        "......x"
        "......x"
        "@wwwwwx"
        "......x"
        "......x"
        "......x";
  std::string const expect1
      = "......x"
        "......x"
        "......x"
        ".wwww@x"
        "......x"
        "......x"
        "......x";

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

  static std::initializer_list< std::string > usable_items = {
      "boots_spider", //
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

  //
  // Move over the spiderweb.
  //
  {
    TEST_LOG(t, "move player into spiderweb right");
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

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 5; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);

    //
    // Move right
    //
    TRACE();
    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move failed");
      goto exit;
    }

    TRACE();
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
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

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 5, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_boots_spider_player_spiderweb() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("boots_spider_player_spiderweb");

  // begin sort marker1 {
  test_callback_set(test, test_boots_spider_player_spiderweb);
  // end sort marker1 }

  return true;
}
