//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"
#include "../my_wids.hpp"

[[nodiscard]] static auto test_gas_life_heal_player(Gamep g, Testp t) -> bool
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
        "x&&&&&x"
        "x&&&&&x"
        "x&&@&&x"
        "x&&&&&x"
        "x&&&&&x"
        "xxxxxxx";
  std::string const expect1
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
  Overrides overrides;
  overrides[ '&' ]   = [](char c, bpoint p) -> Tpp { return tp_find_mand("gas_life"); };
  Levelp  l          = nullptr;
  Levelsp v          = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
  int     health     = 0;
  int     health_max = 0;

  //
  // The guts of the test
  //
  bool result {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  (void) thing_health_set(g, v, l, player, 10);

  //
  // Wait
  //
  for (auto tries = 0; tries < 30; tries++) {
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

  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  health     = thing_health(g, v, l, player);
  health_max = thing_health_max(g, v, l, player);
  TEST_ASSERT(t, health == health_max, "expected full health");

  TEST_ASSERT(t, wid_console_find_text(g, "You breathe in the healing gas"), "did not find console text");

  TEST_ASSERT(t, game_tick_get(g, v) == 30, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_gas_life_heal_player() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("gas_life_heal_player");

  test_callback_set(test, test_gas_life_heal_player);

  return true;
}
