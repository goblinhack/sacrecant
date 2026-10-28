//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_proj_mob_dormant_should_die(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelArrIdx const level_num = 0;
  auto              w         = 27;
  auto              h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x@.......................Gx"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string const expect1
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x@......m.................."
        "x.........................x"
        "x....................m....x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";

  Overrides overrides;
  overrides[ 'G' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("kobalos_mob"); };
  Levelp  l        = nullptr;
  Levelsp v        = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
  bool    result   = true;
  Thingp  mob      = nullptr;

  auto *tp_proj_fire = tp_find_mand("proj_fire");
  tp_damage_set(tp_proj_fire, THING_EVENT_FIRE_DAMAGE, "100");

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  FOR_ALL_THINGS_ON_LEVEL_NO_BREAK(g, v, l, me)
  {
    if (thing_is_mob(me)) {
      mob = me;
    }
  }

  TEST_ASSERT(t, mob, "expecting a mob");

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 100; tries++) {
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }

    if (thing_dormant(g, v, l, mob)) {
      break;
    }
  }

  TEST_ASSERT(t, thing_dormant(g, v, l, mob), "expecting a dormant mob");

  for (auto tries = 0; tries < 10; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    (void) player_fire(g, v, l, 1, 0, tp_proj_fire);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  for (auto tries = 0; tries < 10; tries++) {
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

  TEST_ASSERT(t, game_tick_get(g, v) == 63, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_proj_mob_dormant_should_die() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("proj_mob_dormant_should_die");

  test_callback_set(test, test_proj_mob_dormant_should_die);

  return true;
}
