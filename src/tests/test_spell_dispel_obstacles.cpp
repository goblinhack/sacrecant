//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_spell_dispel_obstacles(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelNum const level_num = 0;
  auto           w         = 27;
  auto           h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const level1
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x......bGB................x"
        "x.....v...t...............x"
        "x.....v.@.T...............x"
        "x.....D...+...............x"
        "x......XLX................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string const level2
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x.........................x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string const level1_expect1
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x......bGB................x"
        "x.....v.m.t...............x"
        "x.....v.@.T...............x"
        "x.....D...+...............x"
        "x......XLX................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string const level1_expect2
      = "xxxxxx!!...xxxxxxxxxxxxxxxx"
        "x.....!!!!................x"
        "x.....!!!.t...............x"
        "x.......@.T...............x"
        "x.........................x"
        "x......XLX................x"
        "xxxxxx.....xxxxxxxxxxxxxxxx";
  std::string const level2_expect2
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x.........................x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";

  Overrides overrides;
  overrides[ 'm' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("kobalos"); };
  overrides[ 'G' ] = [](char c, bpoint p) -> Tpp { return tp_find_mand("kobalos_mob"); };
  Levelp  l1       = nullptr;
  Levelp  l2       = nullptr;
  Levelsp v        = game_test_init(g, &l1, level_num, w, h, level1.c_str(), overrides);
  game_test_init_level(g, v, &l2, level_num + 1, w, h, level2.c_str(), overrides);
  bool       result = true;
  bpoint     target_at;
  Thingp     spell = nullptr;
  ThingEvent e     = {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  (void) thing_mana_set(g, v, l1, player, 100);

  target_at = thing_at(g, v, l1, player);

  //
  // Wait a bit
  //
  level_dump(g, v, l1, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 10; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l1, tries, w, h);
    (void) player_fire(g, v, l1, 1, 0);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l1)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  level_dump(g, v, l1, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l1, t, w, h, level1_expect1.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  spell = thing_spawn(g, v, l1, tp_find_mand("spell_dispel_obstacles"), thing_at(g, v, l1, player));

  TEST_ASSERT(t, spell, "failed to spawn spell");

  (void) thing_spell_radius_incr(g, v, l1, spell, 2);

  TEST_ASSERT(t, thing_spellbook_add(g, v, l1, spell, player), "failed to add spell");
  e.reason                 = "test_spell_dispel_obstacles";
  e.event_type             = THING_EVENT_SPELL_DAMAGE;
  e.source                 = player;
  e.spell_info.spell       = spell;
  e.spell_info.target_set  = true;
  e.spell_info.target      = target_at;
  e.spell_info.option_name = "radial";

  TEST_ASSERT(t, thing_spell_cast_target(g, v, l1, &e), "failed to cast spell");

  TEST_ASSERT(t, game_event_wait(g), "failed to wait");
  if (! game_wait_for_tick_to_finish(g, v, l1)) {
    TEST_FAILED(t, "wait loop failed");
    goto exit;
  }

  level_dump(g, v, l1, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l1, t, w, h, level1_expect2.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  level_dump(g, v, l2, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l2, t, w, h, level2_expect2.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 11, "final tick counter value");

  level_dump(g, v, l1, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_spell_dispel_obstacles() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("spell_dispel_obstacles");

  test_callback_set(test, test_spell_dispel_obstacles);

  return true;
}
