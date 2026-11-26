//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"
#include "../my_thing_inlines.hpp"
#include "../my_wids.hpp"

[[nodiscard]] static auto test_spell_holding_move_blocked(Gamep g, Testp t) -> bool
{
  TEST_LOG(t, "begin");
  TRACE();

  LevelArrIdx const level_num = 0;
  auto              w         = 27;
  auto              h         = 7;

  //
  // How the dungeon starts out, and how we expect it to change
  //
  std::string const start
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x.......@.................x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string const expect
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x.......@.................x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";

  LevelGenOverrides overrides;
  Levelp            l      = nullptr;
  Levelsp           v      = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
  bool              result = true;
  bpoint            target_at;
  Thingp            spell = nullptr;
  ThingEvent        e     = {};
  bool              up {};
  bool              down {};
  bool              left {};
  bool              right {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  (void) thing_mana_set(g, v, l, player, 100);

  target_at = thing_at(g, v, l, player) + bpoint(0, 0);

  spell = thing_spawn(g, v, l, tp_find_mand("spell_holding"), thing_at(g, v, l, player));
  TEST_ASSERT(t, spell, "failed to spawn spell");

  TEST_ASSERT(t, thing_spellbook_add(g, v, l, spell, player), "failed to add spell");
  e.reason                 = "test_spell_holding_move_blocked";
  e.event_type             = THING_EVENT_SPELL_DAMAGE;
  e.source                 = player;
  e.spell_info.spell       = spell;
  e.spell_info.target_set  = true;
  e.spell_info.target      = target_at;
  e.spell_info.option_name = "targeted";

  TEST_ASSERT(t, thing_spell_cast_target(g, v, l, &e), "failed to cast spell");

  //
  // Move right
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  {
    TEST_LOG(t, "move right");
    TRACE();
    up = down = left = right = false;
    right                    = true;

    (void) player_move_request(g, up, down, left, right, false /* fire */);

    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  if (! (result = level_match_contents(g, v, l, t, w, h, expect.c_str()))) {
    TEST_FAILED(t, "unexpected contents");
    goto exit;
  }

  TEST_ASSERT(t, wid_console_find_text(g, "You are being held and cannot move!"), "did not find console text");

  TEST_ASSERT(t, game_tick_get(g, v) == 1, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_spell_holding_move_blocked() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("spell_holding_move_blocked");

  test_callback_set(test, test_spell_holding_move_blocked);

  return true;
}
