//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_throw_item_pot_levit_soft_landing(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  //
  // How the dungeon starts out, and how we expect it to change
  //
  LevelNum const level_num = 0;
  auto           w         = 8;
  auto           h         = 5;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxxx"  //
        "x......x"  //
        "x..@.M.x"  //
        "x......x"  //
        "xxxxxxxx"; //
  std::string const expect1
      = "xxxxxxxx"  //
        "x......x"  //
        "x..@.$.x"  //
        "x......x"  //
        "xxxxxxxx"; //

  Overrides overrides;
  overrides[ 'M' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("cleaner"); };
  Levelp  l        = nullptr;
  Levelsp v        = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
  bool    result   = true;
  bpoint  throw_to;
  int     threw_count = 0;
  Thingp  monst       = nullptr;

  static std::initializer_list< std::string > items = {
      "pot_levit", //
  };

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  if (! thing_carry(g, v, l, player, items)) {
    TEST_FAILED(t, "no item carried");
    goto exit;
  }

  //
  // Throw all items
  //
  throw_to = thing_at(g, v, l, player) + bpoint(2, 0);

  for (;;) {
    bool got_item {};

    FOR_ALL_INVENTORY_ITEMS(g, v, l, player, an_item)
    {
      got_item = true;
      TEST_ASSERT(t, thing_throw_to(g, v, l, player, an_item, throw_to), "failed to throw");

      if (thing_is_dead(player)) {
        break;
      }

      TRACE();
      level_dump(g, v, l, w, h);
      TEST_ASSERT(t, game_event_wait(g), "failed to wait");

      if (! game_wait_for_tick_to_finish(g, v, l)) {
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

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  //
  // Check the monster is levitating
  //
  FOR_ALL_THINGS_AT(g, v, l, it, throw_to)
  {
    if (thing_is_monst(it)) {
      monst = it;
    }
  }

  TEST_ASSERT(t, monst, "expecting monster");
  TEST_ASSERT(t, ! thing_is_levitating(g, v, l, monst), "expecting non levitating monster");
  TEST_ASSERT(t, game_tick_get(g, v) == 1, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_throw_item_pot_levit_soft_landing() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("throw_item_pot_levit_soft_landing");

  // begin sort marker1 {
  test_callback_set(test, test_throw_item_pot_levit_soft_landing);
  // end sort marker1 }

  return true;
}
