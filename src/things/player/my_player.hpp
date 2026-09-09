//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_dice_rolls.hpp"
#include "my_game_popups.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_music.hpp"
#include "my_random.hpp"
#include "my_sound.hpp"
#include "my_thing.hpp"
#include "my_thing_callbacks.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

extern auto tp_player_description_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string;
extern auto tp_player_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string;
extern void tp_player_on_moved(Gamep g, Levelsp v, Levelp l, Thingp me);
extern void tp_player_on_moving_to(Gamep g, Levelsp v, Levelp l, Thingp me, const bpoint &to);
extern bool tp_player_on_damage(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent &e);
extern void tp_player_on_jump_end(Gamep g, Levelsp v, Levelp l, Thingp me);
extern void tp_player_on_fall_begin(Gamep g, Levelsp v, Levelp l, Thingp me);
extern void tp_player_on_fall_end(Gamep g, Levelsp v, Levelp l, Thingp me);
extern void tp_player_level_leave(Gamep g, Levelsp v, Levelp l, Thingp me);
extern void tp_player_level_enter(Gamep g, Levelsp v, Levelp l, Thingp me);
extern void tp_player_level_populated(Gamep g, Levelsp v, Levelp l, Thingp me);
extern void tp_player_tick_begin(Gamep g, Levelsp v, Levelp l, Thingp me);
extern void tp_player_on_teleported(Gamep g, Levelsp v, Levelp l, Thingp me);
extern void tp_player_tick_idle(Gamep g, Levelsp v, Levelp l, Thingp me);
extern void tp_player_tick_end(Gamep g, Levelsp v, Levelp l, Thingp me);
extern void tp_player_on_spawned(Gamep g, Levelsp v, Levelp l, Thingp me, ThingEvent *e_maybe_null);
extern bool tp_player_on_attacking(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e);
extern bool tp_player_on_missing(Gamep g, Levelsp v, Levelp l, Thingp attacker, Thingp target, ThingEvent &e);
extern void tp_load_player_common(Tpp tp);
