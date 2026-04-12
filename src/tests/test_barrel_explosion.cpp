//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_barrel_explosion(Gamep g, Testp t) -> bool
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
        "xxxbbbx"
        "xxxbxbx"
        "x@.bxbx"
        "xxxxxbx"
        "xbbbbbx"
        "xxxxxxx";
  std::string const expect1
      = "xxxxxxx"
        "xxx...x"
        "xxx.x.x"
        "x@..x.x"
        "xxxxx.x"
        "x.....x"
        "xxxxxxx";
  Levelp  l      = nullptr;
  Levelsp v      = game_test_init(g, &l, level_num, w, h, start.c_str());
  bool    result = true;

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  //
  // Spawn fire. This should be enough to blow up all the barrels
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (thing_spawn(g, v, l, tp_first(is_fire), thing_at(player) + bpoint(1, 0)) == nullptr) {
    TEST_FAILED(t, "spawn failed");
    goto exit;
  }

  //
  // Wait for the fire to ignite a barrel
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 20; tries++) {
    TEST_LOG(t, "try: %d", tries);
    TRACE();
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

  TEST_ASSERT(t, game_tick_get(g, v) == 20, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

auto test_load_barrel_explosion() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("barrel_explosion");

  // begin sort marker1 {
  test_callback_set(test, test_barrel_explosion);
  // end sort marker1 }

  return true;
}
