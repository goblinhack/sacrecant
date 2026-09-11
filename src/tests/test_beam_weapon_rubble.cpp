//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_beam_weapon_rubble(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 27;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x@.rrrrrr.................x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string const expect1
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x@-----------.............x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  Levelp  l      = nullptr;
  Levelsp v      = game_test_init(g, &l, level_num, w, h, start.c_str());
  bool    result = true;

  auto *tp_beam_of_fire = tp_find_mand("beam_of_fire");
  tp_damage_set(tp_beam_of_fire, THING_EVENT_FIRE_DAMAGE, "100");

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
  (void) player_fire(g, v, l, 1, 0, tp_beam_of_fire, bpoint(13, 3));

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 0, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_beam_weapon_rubble() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("beam_weapon_rubble");

  test_callback_set(test, test_beam_weapon_rubble);

  return true;
}
