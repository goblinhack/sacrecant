//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"
#include "../my_wids.hpp"

[[nodiscard]] static auto test_fungus_death_player_lives(Gamep g, Testp t) -> bool
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
        "x.@f..x"
        "x.....x"
        "x.....x"
        "xxxxxxx";
  std::string const expect1
      = "xxxxxxx"
        "x.....x"
        "x.....x"
        "x.@f..x"
        "x.....x"
        "x.....x"
        "xxxxxxx";
  std::string const expect2
      = "xxxxxxx"
        "x&&&&&x"
        "x&&&&&x"
        "x&&&&&x"
        "x&&&&&x"
        "x&&&&&x"
        "xxxxxxx";
  std::string const expect3
      = "xxxxxxx"
        "x&&...x"
        "x&....x"
        "x&.&.@x"
        "x&..&.x"
        "x&....x"
        "xxxxxxx";

  //
  // Create the level and start playing
  //
  Overrides overrides;
  overrides[ 'f' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("fungus_death"); };
  Levelp  l        = nullptr;
  Levelsp v        = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);

  //
  // The guts of the test
  //
  bool   result {};
  bool   up {};
  bool   down {};
  bool   left {};
  bool   right {};
  Thingp fungus_death = nullptr;

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  FOR_ALL_THINGS_AT(g, v, l, it, thing_at(g, v, l, player) + bpoint(1, 0))
  {
    if (thing_is_fungus(it)) {
      fungus_death = it;
    }
  }

  TEST_ASSERT(t, fungus_death, "did not find fungus_death");

  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  //
  // Move right and hit fungus_death
  //
  for (auto tries = 0; tries < 10; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);

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

    if (thing_is_dead(fungus_death)) {
      break;
    }
  }

  //
  // Move right onto the body
  //
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
  }

  if (! (result = level_match_contents(g, v, l, t, w, h, expect2.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, wid_console_find_text(g, "You choke in"), "did not find console text");

  //
  // Wait
  //
  for (auto tries = 0; tries < 20; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);

    TEST_ASSERT(t, game_event_wait(g), "failed to wait");

    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  if (! (result = level_match_contents(g, v, l, t, w, h, expect3.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 31, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_fungus_death_player_lives() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("fungus_death_player_lives");

  test_callback_set(test, test_fungus_death_player_lives);

  return true;
}
