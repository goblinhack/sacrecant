//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"

[[nodiscard]] static auto test_spell_pyro_storm(Gamep g, Testp t) -> bool
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
        "x.......@......G..........x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string const expect1
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.......m.................x"
        "x.......@m..m..G..........x"
        "x.......m.................x"
        "x......m..................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string const expect2
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x..........!..............x"
        "x.......m.!!!.............x"
        "x.......@!!!!!.G..........x"
        "x......mm.!!!.............x"
        "x..........!..............x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";

  Overrides overrides;
  overrides[ 'm' ]  = [](char c, bpoint p) -> Tpp { return tp_find_mand("kobalos"); };
  overrides[ 'G' ]  = [](char c, bpoint p) -> Tpp { return tp_find_mand("kobalos_mob"); };
  Levelp     l      = nullptr;
  Levelsp    v      = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
  bool       result = true;
  bpoint     target_at;
  Thingp     spell = nullptr;
  ThingEvent e     = {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  (void) thing_mana_set(g, v, l, player, 100);

  target_at = thing_at(g, v, l, player) + bpoint(3, 0);

  //
  // Wait a bit
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 40; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    (void) player_fire(g, v, l, 1, 0);
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

  spell = thing_spawn(g, v, l, tp_find_mand("spell_pyro_storm"), thing_at(g, v, l, player));
  TEST_ASSERT(t, spell, "failed to spawn spell");

  TEST_ASSERT(t, thing_spellbook_add(g, v, l, spell, player), "failed to add spell");
  e.event_type             = THING_EVENT_SPELL_DAMAGE;
  e.source                 = player;
  e.spell_info.spell       = spell;
  e.spell_info.target_set  = true;
  e.spell_info.target      = target_at;
  e.spell_info.option_name = "targeted pyro storm";

  TEST_ASSERT(t, thing_spell_cast_target(g, v, l, &e), "failed to cast spell");

  TEST_ASSERT(t, game_event_wait(g), "failed to wait");
  if (! game_wait_for_tick_to_finish(g, v, l)) {
    TEST_FAILED(t, "wait loop failed");
    goto exit;
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect2.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, game_tick_get(g, v) == 41, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_spell_pyro_storm() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("spell_pyro_storm");

  test_callback_set(test, test_spell_pyro_storm);

  return true;
}
