//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_sac_weaponless_attack_mob(Gamep g, Testp t) -> bool
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
        "x.@g..x"
        "x.....x"
        "x.....x"
        "xxxxxxx";
  std::string const expect1
      = "xxxxxxx"
        "x.....x"
        "x.....x"
        "x....@x"
        "x.....x"
        "x.....x"
        "xxxxxxx";

  //
  // Create the level and start playing
  //
  Overrides overrides;
  overrides[ 'g' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("ghost_mob"); };
  Levelp  l        = nullptr;
  Levelsp v        = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);

  //
  // The guts of the test
  //
  bool result {};
  bool up {};
  bool down {};
  bool left {};
  bool right {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  TEST_ASSERT(t, thing_buff_add(g, v, l, player, tp_find_mand("sac_weaponless")), "failed to add sacrifice");

  //
  // Attack the mob.
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 20; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);

    TEST_LOG(t, "move right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move failed");
      goto exit;
    }

    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 20, "final tick counter value");

  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_sac_weaponless_attack_mob() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("sac_weaponless_attack_mob");

  // begin sort marker1 {
  test_callback_set(test, test_sac_weaponless_attack_mob);
  // end sort marker1 }

  return true;
}
