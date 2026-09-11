//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_mob_shove_into_lava(Gamep g, Testp t) -> bool
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
        "..@gL.."
        "......."
        "......."
        ".......";
  std::string const expect1
      = "......."
        "......."
        "......."
        "...@!.."
        "......."
        "......."
        ".......";

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

  bpoint  p;
  bool    found_it {};
  ThingId mob_id = 0;

  static std::initializer_list< std::string > items = {
      "wand_fire", //
  };

  auto player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  if (! thing_carry(g, v, l, player, items)) {
    TEST_FAILED(t, "no item carried");
    goto exit;
  }

  //
  // Push the mob into lava
  //
  {
    TEST_LOG(t, "push mob into lava");
    TRACE();
    up = down = left = right = false;
    right                    = true;
  }

  //
  // Find the mob before shoving it via the player
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
    p        = thing_at(g, v, l, player) + bpoint(1, 0);
    found_it = false;
    FOR_ALL_THINGS_AT(g, v, l, it, p)
    {
      if (thing_is_mob(it)) {
        found_it = true;
        mob_id   = it->id;
        break;
      }
    }
  }

  //
  // Move right, shoving the mob
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
    if (! (result = player_move_request(g, up, down, left, right, false /* fire */))) {
      TEST_FAILED(t, "move failed");
      goto exit;
    }
  }

  //
  // Wait for the end of tick
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
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

  //
  // Check mob is dead when shoved into lava. It should be popped off the level.
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
    TEST_LOG(t, "check mob is dead when shoved into lava");
    p        = thing_at(g, v, l, player) + bpoint(1, 0);
    found_it = false;

    FOR_ALL_THINGS_AT(g, v, l, it, p)
    {
      if (thing_is_mob(it)) {
        found_it = true;
        break;
      }
    }

    if (found_it) {
      TEST_FAILED(t, "found mob, but it should have been popped");
      goto exit;
    }
  }

  //
  // Check the mob has been cleaned up
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TRACE();
    if (thing_find_optional(g, v, mob_id) != nullptr) {
      TEST_FAILED(t, "found mob, but it should have been freed");
      goto exit;
    }
  }

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 2, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_mob_shove_into_lava() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("mob_shove_into_lava");

  test_callback_set(test, test_mob_shove_into_lava);

  return true;
}
