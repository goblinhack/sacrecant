//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_weapon_worn(Gamep g, Testp t) -> bool
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
        "x@........................x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
  std::string const expect1
      = "xxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "x.........................x"
        "x.........................x"
        "x@........................x"
        "x.........................x"
        "x.........................x"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxx";

  Overrides  overrides;
  Levelp     l       = nullptr;
  Levelsp    v       = game_test_init(g, &l, level_num, w, h, start.c_str());
  bool       result  = true;
  Thingp     weapon  = nullptr;
  Thingp     worn    = nullptr;
  Tpp        item_tp = nullptr;
  ThingEvent e       = {};

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    TEST_FAILED(t, "no player");
    goto exit;
  }

  item_tp = tp_find_mand("wand_fire");
  if (! item_tp) {
    TEST_FAILED(t, "no weapon found");
    goto exit;
  }

  weapon = thing_spawn(g, v, l, item_tp, thing_at(g, v, l, player));
  if (! weapon) {
    TEST_FAILED(t, "no weapon spawned");
    goto exit;
  }

  e.reason     = "spawned";           //
  e.event_type = THING_EVENT_SPAWNED; //
  e.source     = player;              //

  if (! thing_carry(g, v, l, player, weapon, e)) {
    TEST_FAILED(t, "no weapon carried");
    goto exit;
  }

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  //
  // Wield and strip over and over
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 20; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);

    TEST_LOG(t, "first, wield the weapon");
    TRACE_INDENT();

    if (! thing_wear_item(g, v, l, player, weapon, e)) {
      TEST_FAILED(t, "failed to wield");
      goto exit;
    }

    TEST_LOG(t, "check it is worn");
    TRACE_INDENT();

    if (! thing_is_worn(weapon)) {
      TEST_FAILED(t, "weapon is not worn");
      goto exit;
    }

    TEST_LOG(t, "get the weapon from its slot");
    TRACE_INDENT();

    worn = thing_worn_get(g, v, l, player, WORN_TYPE_WEAPON);
    if (! worn) {
      TEST_FAILED(t, "unexpectedly not worn a weapon");
      goto exit;
    }

    TEST_LOG(t, "now strip it");
    TRACE_INDENT();

    if (! thing_strip_item(g, v, l, player, worn, e)) {
      TEST_FAILED(t, "failed to strip");
      goto exit;
    }

    TEST_LOG(t, "check the weapon is striped");
    TRACE_INDENT();

    if (thing_is_worn(weapon)) {
      TEST_FAILED(t, "weapon is not striped");
      goto exit;
    }

    TEST_LOG(t, "check ths slot is empty");
    TRACE_INDENT();

    worn = thing_worn_get(g, v, l, player, WORN_TYPE_WEAPON);
    if (worn) {
      thing_log(g, v, l, worn, "worn this");
      TEST_FAILED(t, "unexpectedly worn a weapon");
      goto exit;
    }

    TRACE();
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

  TEST_ASSERT(t, game_tick_get(g, v) == 20, "final tick counter value");

  level_dump(g, v, l, w, h);
  TEST_PASSED(t);
exit:
  TRACE();
  game_cleanup(g);

  return result;
}

[[nodiscard]] auto test_load_weapon_worn_then_stripped() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("weapon_worn_then_stripped");

  test_callback_set(test, test_weapon_worn);

  return true;
}
