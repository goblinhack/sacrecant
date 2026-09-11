//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_ring_war_beam_weapon(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 17;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxxxxxxxxxxxx"
        "x...............x"
        "x...............x"
        "x@......m.......x"
        "x...............x"
        "x...............x"
        "xxxxxxxxxxxxxxxxx";
  std::string const expect1
      = "xxxxxxxxxxxxxxxxx"
        "x...............x"
        "x...............x"
        "x@-------.......x"
        "x...............x"
        "x...............x"
        "xxxxxxxxxxxxxxxxx";
  std::string const expect2
      = "xxxxxxxxxxxxxxxxx"
        "x...............x"
        "x...............x"
        "x@......M.......x"
        "x...............x"
        "x...............x"
        "xxxxxxxxxxxxxxxxx";

  Overrides overrides;
  overrides[ 'm' ]          = [](char c, bpoint p) -> Tpp { return tp_find_mand("ogrik"); };
  Levelp  l                 = nullptr;
  Thingp  player            = nullptr;
  Tpp     tp_beam_of_energy = nullptr;
  Thingp  monst             = nullptr;
  Levelsp v                 = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
  bool    result            = true;

  static std::initializer_list< std::string > items = {
      "ring_war", //
      "ring_war", //
  };

  tp_beam_of_energy = tp_find_mand("beam_of_energy");
  if (tp_beam_of_energy == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no weapon");
    goto exit;
  }

  player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  //
  // Find the monster
  //
  FOR_ALL_THINGS_AT(g, v, l, it, thing_at(g, v, l, player) + bpoint(7, 0))
  {
    if (thing_is_monst(it)) {
      monst = it;
    }
  }

  if (! thing_carry(g, v, l, player, items)) {
    TEST_FAILED(t, "no item carried");
    goto exit;
  }

  //
  // Spawn fire. This should be enough to blow up all the barrels
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  //
  // Wait for the weapon to kill the monster
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 5; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    (void) player_fire(g, v, l, 1, 0, tp_beam_of_energy, bpoint(13, 3));

    if (tries == 0) {
      TEST_PROGRESS(t);
      if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
        TEST_FAILED(t, "unexpected contents");
        goto exit;
      }
    }

    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  //
  // Wait for the weapon to fade away
  //
  for (auto tries = 0; tries < 2; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  TEST_ASSERT(t, thing_is_dead(monst), "expected dead monst");

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect2.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 7, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_ring_war_beam_weapon() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("ring_war_beam_weapon");

  test_callback_set(test, test_ring_war_beam_weapon);

  return true;
}
