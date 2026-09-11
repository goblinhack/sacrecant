//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_fumble_more(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 17;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const level // first level
      = "................."
        "................."
        "................."
        "@................"
        "................."
        "................."
        ".................";

  //
  // Create the level and start playing
  //
  Levelp    l = nullptr;
  Overrides overrides;
  Levelsp   v = game_test_init(g, &l, level_num, w, h, level.c_str(), overrides);

  //
  // The guts of the test
  //
  bool   result {};
  Thingp player = nullptr;

  //
  // Find the player
  //
  TEST_PROGRESS(t);
  {
    TRACE();
    player = thing_player(g);
    if (player == nullptr) [[unlikely]] {
      TEST_FAILED(t, "no player");
      goto exit;
    }
  }

  TEST_ASSERT(t, thing_buff_add(g, v, l, player, tp_find_mand("sac_fumble_more")), "failed to add sacrifice");

  TEST_ASSERT(t, 2 <= thing_fumble_roll(g, v, l, player), "expected fumble");

  TEST_ASSERT(t, 3 > thing_fumble_roll(g, v, l, player), "expected not fumble");

  TEST_ASSERT(t, game_tick_get(g, v) == 0, "final tick counter value");

  result = true;
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_fumble_more() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("fumble_more");

  test_callback_set(test, test_fumble_more);

  return true;
}
