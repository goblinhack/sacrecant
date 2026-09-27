//
// Copyright goblinhack@gmail.com
//

#include "../../my_callstack.hpp"
#include "../../my_level_inlines.hpp"
#include "../../my_main.hpp"
#include "../../my_sound.hpp"
#include "../../my_sprintf.hpp"
#include "../../my_thing_callbacks.hpp"
#include "../../my_thing_inlines.hpp"
#include "../../my_tp.hpp"
#include "../../my_tps.hpp"
#include "../../my_ui.hpp"

#include <functional>

extern const std::string spell_option_targeted;
extern const std::string spell_option_radial;
extern const std::string spell_option_radial_excluding_player_tile;

using SpellOnCastRequest = std::function< bool(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e) >;
using SpellOnCastDo      = std::function< bool(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, const bpoint &p, ThingEvent &e) >;

bool tp_spell_common_on_cast_request(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e, SpellOnCastRequest fn = {});
bool tp_spell_common_on_cast_do(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp user, ThingEvent &e, SpellOnCastDo fn);
