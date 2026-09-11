//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_throw_item_pot_incin_chasm(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 7;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const level1
      = "......."
        "......."
        "....C.."
        "..@.C.."
        "....C.."
        "......."
        ".......";
  std::string const level2
      = "......."
        "......."
        "....B.."
        "....B.."
        "....B.."
        "......."
        ".......";
  std::string const expect2 // second level
      = "......."
        "......."
        "...$!.."
        "....!.."
        "....B.."
        "......."
        ".......";

  //
  // Create the level and start playing
  //
  Levelp  l1 = nullptr;
  Levelp  l2 = nullptr;
  Levelsp v  = game_test_init(g, &l1, level_num, w, h, level1.c_str());
  game_test_init_level(g, v, &l2, level_num + 1, w, h, level2.c_str());

  bool   result {};
  bpoint throw_to;
  int    threw_count = 0;

  static std::initializer_list< std::string > items = {
      "pot_incin", //
      "pot_incin", //
      "pot_incin", //
  };

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  if (! thing_carry(g, v, l1, player, items)) {
    TEST_FAILED(t, "no item carried");
    goto exit;
  }

  //
  // Throw all items
  //
  throw_to = thing_at(g, v, l1, player) + bpoint(2, 0);

  for (;;) {
    bool got_item {};

    FOR_ALL_INVENTORY_ITEMS(g, v, l1, player, an_item)
    {
      got_item = true;
      TEST_ASSERT(t, thing_throw_to(g, v, l1, player, an_item, throw_to), "failed to throw");

      TRACE();
      level_dump(g, v, l1, w, h);
      TEST_ASSERT(t, game_event_wait(g), "failed to wait");

      if (! game_wait_for_tick_to_finish(g, v, l1)) {
        TEST_FAILED(t, "wait loop failed");
        goto exit;
      }

      threw_count++;
    }

    if (! got_item) {
      break;
    }
  }

  TEST_ASSERT(t, threw_count == (int) items.size(), "did not throw expected item amount");

  level_dump(g, v, l2, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l2, t, w, h, expect2.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 3, "final tick counter value");

  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_throw_item_pot_incin_chasm() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("throw_item_pot_incin_chasm");

  test_callback_set(test, test_throw_item_pot_incin_chasm);

  return true;
}
