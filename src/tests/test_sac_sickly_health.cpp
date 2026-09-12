//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"
#include "../my_wids.hpp"

[[nodiscard]] static auto test_sac_sickly_health(Gamep g, Testp t) -> bool
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
        "X@....X"
        "X.....X"
        "X.....X"
        "X.....X"
        "X.....X"
        "XXXXXXX";

  //
  // Create the level and start playing
  //
  Levelp  l = nullptr;
  Levelsp v = game_test_init(g, &l, level_num, w, h, start.c_str());

  //
  // The guts of the test
  //
  bool result {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  TEST_ASSERT(t, thing_buff_add(g, v, l, player, tp_find_mand("sac_sickly_health")), "failed to add sacrifice");

  TEST_ASSERT(t, thing_health(g, v, l, player) == 90, "expecting lower health");

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 0, "final tick counter value");

  result = true;

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_sac_sickly_health() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("sac_sickly_health");

  test_callback_set(test, test_sac_sickly_health);

  return true;
}
