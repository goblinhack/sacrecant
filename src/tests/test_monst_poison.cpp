//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"
#include "../my_wids.hpp"

[[nodiscard]] static auto test_monst_poison(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 15;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "XXXXXXXXXXXXXXX"
        "X....~~~~~~~~.X"
        "X....~~~~~~~~.X"
        "X...@~~~~~m~~.X"
        "X....~~~~~~~~.X"
        "X.............X"
        "XXXXXXXXXXXXXXX";
  std::string const expect1
      = "XXXXXXXXXXXXXXX"
        "X....~~~~~~~~.X"
        "X....~~~~~~~~.X"
        "X..@.m~~~~.~~.X"
        "X....~~~~~~~~.X"
        "X.............X"
        "XXXXXXXXXXXXXXX";

  //
  // Create the level and start playing
  //
  Overrides overrides;
  overrides[ 'm' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("coil_eel"); };
  Levelp  l        = nullptr;
  Levelsp v        = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
  bool    up {};
  bool    down {};
  bool    left {};
  bool    right {};

  //
  // The guts of the test
  //
  bool result {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  (void) thing_health_set(g, v, l, player, 30);

  //
  // Wait until bitten
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 20; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }

    if (wid_console_find_text(g, "The coil eel sinks its fangs into you")) {
      break;
    }
  }

  //
  // Move left
  //
  {
    level_dump(g, v, l, w, h);
    TEST_PROGRESS(t);
    {
      TRACE();
      up = down = left = right = false;
      left                     = true;
      if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
        TEST_FAILED(t, "move failed");
        goto exit;
      }

      if (! game_wait_for_tick_to_finish(g, v, l)) {
        TEST_FAILED(t, "wait loop failed");
        goto exit;
      }
    }
  }

  //
  // Wait on the poison
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 100; tries++) {
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

  //
  // Check the level contents
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
    if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  TEST_ASSERT(t, thing_is_dead(player), "player is not dead");

  TEST_ASSERT(t, wid_console_find_text(g, "You are being poisoned"), "did not find console text");

  TEST_ASSERT(t, wid_console_find_text(g, "You are poisoned and die."), "did not find console text");

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 19, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_monst_poison() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("monst_poison");

  test_callback_set(test, test_monst_poison);

  return true;
}
