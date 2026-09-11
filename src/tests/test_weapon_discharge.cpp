//
// Copyright goblinhack@gmail.com
//

#include "../my_game.hpp"
#include "../my_level.hpp"
#include "../my_main.hpp"
#include "../my_test.hpp"

[[nodiscard]] static auto test_weapon_discharge(Gamep g, Testp t) -> bool
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
        "x@.......................Gx"
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

  Overrides overrides;
  overrides[ 'G' ]   = [](char c, bpoint p) -> Tpp { return tp_find_mand("kobalos_mob"); };
  Levelp     l       = nullptr;
  Levelsp    v       = game_test_init(g, &l, level_num, w, h, start.c_str(), overrides);
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

  if (! thing_wear_item(g, v, l, player, weapon, e)) {
    TEST_FAILED(t, "failed to wield");
    goto exit;
  }

  (void) thing_charge_count_set(g, v, l, weapon, 10);

  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);

  //
  // Drain the weapon
  //
  level_dump(g, v, l, w, h);
  TEST_PROGRESS(t);
  for (auto tries = 0; tries < 20; tries++) {
    TEST_LOOP_PROGRESS(t, g, v, l, tries, w, h);
    (void) player_fire(g, v, l, 1, 0);
    TEST_ASSERT(t, game_event_wait(g), "failed to wait");
    if (! game_wait_for_tick_to_finish(g, v, l)) {
      TEST_FAILED(t, "wait loop failed");
      goto exit;
    }
  }

  worn = thing_worn_get(g, v, l, player, WORN_TYPE_WEAPON);
  if (worn) {
    thing_log(g, v, l, worn, "worn this");
    TEST_FAILED(t, "unexpectedly worn a weapon still");
    goto exit;
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

[[nodiscard]] auto test_load_weapon_discharge() -> bool // NOLINT
{
  TRACE();

  Testp test = test_load("weapon_discharge");

  test_callback_set(test, test_weapon_discharge);

  return true;
}
