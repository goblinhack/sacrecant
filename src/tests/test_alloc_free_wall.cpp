//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_globals.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_alloc_free_walls(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum level_num = 0;
  auto     w         = 0;
  auto     h         = 0;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start;

  //
  // Create the level and start playing
  //
  Levelp  l      = nullptr;
  Levelsp v      = game_test_init(g, &l, level_num, w, h, start.c_str());
  bool    result = true;

  TEST_ASSERT(t, levels_thing_count(g, v) == 0, "expected thing count to be 0");
  TEST_ASSERT(t, levels_thing_ext_count(g, v) == 0, "expected thing count to be 0");

  for (auto X = 0; X < LEVEL_ACROSS; X++) {
    for (auto Y = 0; Y < LEVEL_DOWN; Y++) {
      if (level_num++ == 0) {
        //
        // Already inited
        //
      } else {
        game_test_init_level(g, v, &l, level_num, bpoint(X, Y), w, h, start.c_str());
      }

      //
      // No need to test all slots for level select. Additionally, we've filled
      // all the other levels, so not a lot of space to alloc IDs from.
      //
      auto slot_depth_to_test = static_cast< int >(MAP_SLOTS);
      if (level_num == LEVEL_SELECT_ID) {
        //
        // There's just no space in other levels if we fully allocate them!
        //
        slot_depth_to_test = 2;
        continue;
      }

      if (level_num > 2) {
        //
        // Takes too long to test them all
        //
        continue;
      }

      for (auto x = 0; x < MAP_WIDTH; x++) {
        for (auto y = 0; y < MAP_HEIGHT; y++) {
          for (auto slot = 0; slot < slot_depth_to_test; slot++) {
            if (thing_spawn(g, v, l, tp_random(g, v, l, is_wall), bpoint(x, y)) == nullptr) {
              break;
            }

            if (AN_ERROR_OCCURRED()) {
              break;
            }
          }

          if (AN_ERROR_OCCURRED()) {
            break;
          }
        }
        if (AN_ERROR_OCCURRED()) {
          break;
        }
      }
      if (AN_ERROR_OCCURRED()) {
        break;
      }

      level_debug(g, v, l);
    }

    if (AN_ERROR_OCCURRED()) {
      break;
    }
  }

  game_debug_info(g);

  if (AN_ERROR_OCCURRED()) {
    result = false;
    TEST_FAILED(t, "error happened during thing placement");
    goto exit;
  }

  //
  // no player, so 0
  //
  TEST_ASSERT(t, game_tick_get(g, v) == 0, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

auto test_load_alloc_free_walls() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("alloc_free_walls");

  // begin sort marker1 {
  test_callback_set(test, test_alloc_free_walls);
  // end sort marker1 }

  return true;
}
