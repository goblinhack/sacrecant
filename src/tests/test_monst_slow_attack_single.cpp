//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_monst_slow_attack_single(Gamep g, Testp t) -> bool
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
        "X.....X"
        "X.....X"
        "X@...mX"
        "X.....X"
        "X.....X"
        "XXXXXXX";
  std::string const expect1
      = "XXXXXXX"
        "X.....X"
        "X.....X"
        "X@..M.X"
        "X.....X"
        "X.....X"
        "XXXXXXX";
  std::string const expect2
      = "XXXXXXX"
        "X.....X"
        "X.....X"
        "X@.M..X"
        "X.....X"
        "X.....X"
        "XXXXXXX";
  std::string const expect3
      = "XXXXXXX"
        "X.....X"
        "X.....X"
        "X@M...X"
        "X.....X"
        "X.....X"
        "XXXXXXX";

  //
  // Create the level and start playing
  //
  Overrides overrides;
  overrides[ 'm' ]   = [](char c, bpoint p) -> Tpp { return tp_find_mand("glorp"); };
  Levelp       l     = nullptr;
  Levelsp      v     = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
  ThingPlayer *p     = nullptr;
  Thingp       monst = nullptr;

  //
  // The guts of the test
  //
  bool result {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 2; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
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

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 2; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  //
  // Check the level contents
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
    if (! (result = level_match_contents(g, v, l, t, w, h, expect2.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 2; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  //
  // Check the level contents
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
    if (! (result = level_match_contents(g, v, l, t, w, h, expect3.c_str()))) {
      TEST_FAILED(t, "unexpected contents");
      goto exit;
    }
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 50; tries++) {
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

  TEST_ASSERT(t, thing_is_dead(player), "player is dead");

  //
  // Check the monster attacked enough
  //
  FOR_ALL_THINGS_AT(g, v, l, it, thing_at(g, v, l, player) + bpoint(1, 0))
  {
    if (thing_is_monst(it)) {
      monst = it;
    }
  }

  p = thing_player_struct(g);
  if (p != nullptr) {
    auto attacked_count = p->attacked_by[ tp_id_get(thing_tp(monst)) ];

    log("attacked_count %d", attacked_count);

    TEST_ASSERT(t, attacked_count == 13, "attacked by count not as expected");
  }

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 36, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_monst_slow_attack_single() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("monst_slow_attack_single");

  test_callback_set(test, test_monst_slow_attack_single);

  return true;
}
