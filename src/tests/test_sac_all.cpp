//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_sac_all(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelArrIdx const level_num = 0;
  auto              w         = 7;
  auto              h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxx"
        "x.....x"
        "x.....x"
        "x.@x..x"
        "x.....x"
        "x.....x"
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

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_blood_pact")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_clumsy")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_crit_more")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_fumble_more")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_glass_bones")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_hollow_bones")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_noisy")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_poison_blood")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_power_crazed")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_sickly_health")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_soul_feast")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_wall_walker")), "failed to add sacrifice");
  TEST_ASSERT(t, thing_hook_add(g, v, l, player, tp_find_mand("sac_weaponless")), "failed to add sacrifice");

  //
  // Move right
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
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
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 1, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_sac_all() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("sac_all");

  test_callback_set(test, test_sac_all);

  return true;
}
