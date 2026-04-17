//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_slots(Gamep g, Testp t) -> bool
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
      = "......."
        "......."
        "......."
        "...@..."
        "......."
        "......."
        ".......";
  std::string const expect1
      = "......."
        "......."
        "......."
        "...@..."
        "......."
        "......."
        ".......";

  //
  // Create the level and start playing
  //
  Levelp  l = nullptr;
  Levelsp v = game_test_init(g, &l, level_num, w, h, start.c_str());

  //
  // The guts of the test
  //
  bool result = false;

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  for (auto slot = 0; slot < MAP_SLOTS * 10; slot++) {
    if (thing_spawn(g, v, l, tp_random(g, v, l, is_smoke), thing_at(player)) == nullptr) {
      TEST_FAILED(t, "failed to spawn thing");
      break;
    }

    if (thing_spawn(g, v, l, tp_random(g, v, l, is_fire), thing_at(player)) == nullptr) {
      TEST_FAILED(t, "failed to spawn thing");
      break;
    }

    if (thing_spawn(g, v, l, tp_random(g, v, l, is_steam), thing_at(player)) == nullptr) {
      TEST_FAILED(t, "failed to spawn thing");
      break;
    }

    auto *m = thing_spawn(g, v, l, tp_random(g, v, l, is_monst), thing_at(player));
    if (m == nullptr) {
      TEST_FAILED(t, "failed to spawn thing");
      break;
    }

    THING_DBG(m, "dead due to spawning");
    TRACE_INDENT();

    ThingEvent e;
    thing_dead(g, v, l, m, e);
  }

  TEST_ASSERT(t, game_event_wait(g), "failed to wait");

  if (! game_wait_for_tick_to_finish(g, v, l)) {
    TEST_FAILED(t, "wait loop failed");
    goto exit;
  }

  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 1, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

auto test_load_slots() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("slots");

  // begin sort marker1 {
  test_callback_set(test, test_slots);
  // end sort marker1 }

  return true;
}
