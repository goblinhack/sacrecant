//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_level_inlines.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_monst_massacre(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 15;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "XXXXXXXXXXXXXXX"
        "XkkkkkkkkkkkkkX"
        "XkkkkkkkkkkkkkX"
        "XkkkkkkkkkkkkkX"
        "XxxkkkkkkkkkkkX"
        "X@xkkkkkkkkkkmX"
        "XXXXXXXXXXXXXXX";
  std::string const expect1
      = "XXXXXXXXXXXXXXX"
        "Xmmmmmmmmm..mmX"
        "XmmmmmmmmmmmmmX"
        "Xmmm.mmm.mmmmmX"
        "Xxxmmmmmmm.mmmX"
        "X@xmmmmmmmmmmmX"
        "XXXXXXXXXXXXXXX";

  //
  // Create the level and start playing
  //
  Overrides overrides;
  overrides[ 'k' ]           = [](char c, bpoint p) -> Tpp { return tp_find_mand("kobalos"); };
  overrides[ 'm' ]           = [](char c, bpoint p) -> Tpp { return tp_find_mand("spider_giant"); };
  Levelp  l                  = nullptr;
  Levelsp v                  = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
  int     dead_kobalos       = 0;
  int     spider_count       = 0;
  int     spider_giant_count = 0;
  int     spider_baby_count  = 0;

  //
  // The guts of the test
  //
  bool result {};

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 50; tries++) {
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

  {
    FOR_ALL_THINGS_ON_LEVEL(g, v, l, it)
    {
      if (thing_is_dead(it) && thing_is_kobalos(it)) {
        dead_kobalos++;
      }
      if (thing_is_spider(it)) {
        spider_count++;
      }
      if (thing_is_spider_giant(it)) {
        spider_giant_count++;
      }
      if (thing_is_spider_baby(it)) {
        spider_baby_count++;
      }
    }
  }

  TEST_LOG(t, "dead_kobalos: %d", dead_kobalos);
  TEST_LOG(t, "spider_count: %d", spider_count);
  TEST_LOG(t, "spider_giant_count: %d", spider_giant_count);
  TEST_LOG(t, "spider_baby_count: %d", spider_baby_count);

  TEST_ASSERT(t, dead_kobalos == 5, "missing dead kobalos");
  TEST_ASSERT(t, spider_count == 5, "missing spiders");
  TEST_ASSERT(t, spider_giant_count == 1, "missing giant spiders");
  TEST_ASSERT(t, spider_baby_count == 4, "missing baby spiders");

  //
  // Check the tick is as expected
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  TEST_ASSERT(t, game_tick_get(g, v) == 50, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_monst_massacre() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("monst_massacre");

  // begin sort marker1 {
  test_callback_set(test, test_monst_massacre);
  // end sort marker1 }

  return true;
}
