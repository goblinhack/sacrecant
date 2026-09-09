//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing_callbacks.hpp"
#include "my_tp.hpp"
#include "my_tps.hpp"
#include "my_ui.hpp"

static auto tp_buff_immune_fire_detail_get(Gamep g, Levelsp v, Levelp l, Thingp me) -> std::string
{
  TRACE();

  return                                                                        //
      UI_INFO1_FMT_STR                                                          //
      "With this buff you are temporarily immune to fire damage of any kind.\n" //
      UI_INFO2_FMT_STR                                                          //
      "This includes lava, steam and other unpleasantly hot things.\n";
}

[[nodiscard]] auto tp_load_buff_immune_fire() -> bool
{
  TRACE();

  auto *tp   = tp_load("buff_immune_fire"); // keep as string for scripts
  auto  name = tp_name(tp);

  // begin sort marker1 {
  thing_detail_set(tp, tp_buff_immune_fire_detail_get);
  tp_flag_set(tp, is_buff);
  tp_flag_set(tp, is_loggable);
  tp_flag_set(tp, is_tick_on_use);
  tp_flag_set(tp, is_tickable);
  tp_is_immune_to_add(tp, THING_EVENT_FIRE_DAMAGE);
  tp_lifespan_set(tp, "1d100+1000");
  tp_name_short_set(tp, "fire immune");
  tp_flag_set(tp, is_hook);
  // end sort marker1 }

  return true;
}
