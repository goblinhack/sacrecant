//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"
#include "../my_wids.hpp"

[[nodiscard]] static auto test_sac_poison_blood(Gamep g, Testp t) -> bool
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

  (void) thing_health_set(g, v, l, player, 1);

  TEST_ASSERT(t, thing_buff_add(g, v, l, player, tp_find_mand("sac_poison_blood")), "failed to add sacrifice");

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 1000; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }

    if (thing_is_dead(player)) {
      break;
    }
  }

  TEST_ASSERT(t, thing_is_dead(player), "expecting a dead player");

  TEST_ASSERT(t, wid_console_find_text(g, "Your own poisoned blood has killed you"), "did not find console text");

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 60, "final tick counter value");

  result = true;

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_sac_poison_blood() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("sac_poison_blood");

  test_callback_set(test, test_sac_poison_blood);

  return true;
}
