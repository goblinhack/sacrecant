//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_beam_weapon_wall_turns_to_lava(Gamep g, Testp t) -> bool
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
        "x..x..x"
        "x..x..x"
        "x@.x..x"
        "x..x..x"
        "x..x..x"
        "xxxxxxx";
  std::string const expect1
      = "xxxxxxx"
        "x..x..x"
        "x..x..x"
        "x@.L..x"
        "x..x..x"
        "x..x..x"
        "xxxxxxx";
  Levelp  l      = nullptr;
  Levelsp v      = game_test_init(g, &l, level_num, w, h, start.c_str());
  bool    result = true;

  auto *tp_beam_of_fire = tp_find_mand("beam_of_fire");
  tp_damage_set(tp_beam_of_fire, THING_EVENT_FIRE_DAMAGE, "1d4");

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  for (auto tries = 0; tries < 20; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    (void) player_fire(g, v, l, 1, 0, tp_beam_of_fire, bpoint(13, 3));
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 1; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
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

  TEST_ASSERT(t, game_tick_get(g, v) == 21, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_beam_weapon_wall_turns_to_lava() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("beam_weapon_wall_turns_to_lava");

  test_callback_set(test, test_beam_weapon_wall_turns_to_lava);

  return true;
}
