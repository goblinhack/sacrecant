//
// Copyright goblinhack@gmail.com
//

#ifdef USE_LZ4
#define MALLOC_PAD
#else
#define MALLOC_PAD *2
#endif

#ifdef USE_LZ4
#include <lz4.h>
#else
#include "3rdparty/minilzo.hpp"
#endif

#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_sdl_event.hpp"
#include "my_sdl_proto.hpp"
#include "my_serialize.hpp"
#include "my_sound.hpp"
#include "my_ui.hpp"
#include "my_wid_popup.hpp"
#include "my_wids.hpp"

#include <array>
#include <format>
#include <utility>

static WidPopup *wid_load;

static std::string                    game_load_error;
bool                                  game_headers_only;
extern int                            GAME_SAVE_MARKER_EOL;
extern int                            GAME_SAVE_MARKER_CONFIG;
std::array< bool, UI_MAX_SAVE_SLOTS > slot_valid;

#define READ_MAGIC(what, m)                                                                                                                \
  {                                                                                                                                        \
    uint32_t magic;                                                                                                                        \
    in >> bits(magic);                                                                                                                     \
    if (magic != (m)) {                                                                                                                    \
      game_load_error = "bad '" what "' magic expected: " + std::format("0x{:x}", m) + " got " + std::format("0x{:x}", magic);             \
      return in;                                                                                                                           \
    }                                                                                                                                      \
    IF_DEBUG2 { con("Read magic '%s' %s", what, std::format("0x{:x}", magic).c_str()); }                                                   \
  }

auto operator>>(std::istream &in, Bits< SDL_Keysym & > my) -> std::istream &
{
  TRACE();

  in >> bits(my.t.scancode);
  in >> bits(my.t.sym);
  in >> bits(my.t.mod);
  in >> bits(my.t.unused);
  return in;
}

auto operator>>(std::istream &in, Bits< Config & > my) -> std::istream &
{
  TRACE();

  in >> bits(my.t.version);
  log("read config: version                = [%s]", my.t.version.c_str());
  in >> bits(my.t.serialized_size);
  log("read config: serialized_size        = %d", my.t.serialized_size);

  if (my.t.serialized_size != sizeof(Config)) {
    game_load_error = "bad save file header version";
    return in;
  }

  //
  // Check various structure sizes prior to reading the data
  //
  {
    uint32_t tmp = 0;

    in >> bits(tmp);
    if (tmp != offsetof(Config, seed_name)) {
      game_load_error = "Config structure changed: offsetof(Config, seed_name))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, seed_num)) {
      game_load_error = "Config structure changed: offsetof(Config, seed_num))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, seed_source)) {
      game_load_error = "Config structure changed: offsetof(Config, seed_source))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, player_name)) {
      game_load_error = "Config structure changed: offsetof(Config, player_name))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, aspect_ratio)) {
      game_load_error = "Config structure changed: offsetof(Config, aspect_ratio))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, ui_term_height)) {
      game_load_error = "Config structure changed: offsetof(Config, ui_term_height))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, ui_term_width)) {
      game_load_error = "Config structure changed: offsetof(Config, ui_term_width))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, debug_mode)) {
      game_load_error = "Config structure changed: offsetof(Config, debug_mode))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, fps_counter)) {
      game_load_error = "Config structure changed: offsetof(Config, fps_counter))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, gfx_borderless)) {
      game_load_error = "Config structure changed: offsetof(Config, gfx_borderless))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, gfx_fullscreen)) {
      game_load_error = "Config structure changed: offsetof(Config, gfx_fullscreen))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, gfx_fullscreen_desktop)) {
      game_load_error = "Config structure changed: offsetof(Config, gfx_fullscreen_desktop))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, gfx_vsync_enable)) {
      game_load_error = "Config structure changed: offsetof(Config, gfx_vsync_enable))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, mouse_wheel_lr_negated)) {
      game_load_error = "Config structure changed: offsetof(Config, mouse_wheel_lr_negated))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, mouse_wheel_ud_negated)) {
      game_load_error = "Config structure changed: offsetof(Config, mouse_wheel_ud_negated))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, config_pix_height)) {
      game_load_error = "Config structure changed: offsetof(Config, config_pix_height))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, config_pix_width)) {
      game_load_error = "Config structure changed: offsetof(Config, config_pix_width))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, map_fbo_height)) {
      game_load_error = "Config structure changed: offsetof(Config, map_fbo_height))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, map_fbo_width)) {
      game_load_error = "Config structure changed: offsetof(Config, map_fbo_width))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, window_pix_height)) {
      game_load_error = "Config structure changed: offsetof(Config, window_pix_height))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, window_pix_width)) {
      game_load_error = "Config structure changed: offsetof(Config, window_pix_width))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, tiles_visible_across)) {
      game_load_error = "Config structure changed: offsetof(Config, tiles_visible_across))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, tiles_visible_down)) {
      game_load_error = "Config structure changed: offsetof(Config, tiles_visible_down))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, ascii_pix_height)) {
      game_load_error = "Config structure changed: offsetof(Config, ascii_pix_height))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, ascii_pix_width)) {
      game_load_error = "Config structure changed: offsetof(Config, ascii_pix_width))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, music_volume)) {
      game_load_error = "Config structure changed: offsetof(Config, music_volume))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, sdl_delay)) {
      game_load_error = "Config structure changed: offsetof(Config, sdl_delay))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, sound_volume)) {
      game_load_error = "Config structure changed: offsetof(Config, sound_volume))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_wait)) {
      game_load_error = "Config structure changed: offsetof(Config, key_wait))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_console)) {
      game_load_error = "Config structure changed: offsetof(Config, key_console))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_help)) {
      game_load_error = "Config structure changed: offsetof(Config, key_help))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_load)) {
      game_load_error = "Config structure changed: offsetof(Config, key_load))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_move_down)) {
      game_load_error = "Config structure changed: offsetof(Config, key_move_down))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_move_left)) {
      game_load_error = "Config structure changed: offsetof(Config, key_move_left))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_move_right)) {
      game_load_error = "Config structure changed: offsetof(Config, key_move_right))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_move_up)) {
      game_load_error = "Config structure changed: offsetof(Config, key_move_up))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_quit)) {
      game_load_error = "Config structure changed: offsetof(Config, key_quit))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_save)) {
      game_load_error = "Config structure changed: offsetof(Config, key_save))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_screenshot)) {
      game_load_error = "Config structure changed: offsetof(Config, key_screenshot))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused1)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused1))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused2)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused2))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused3)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused3))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused4)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused4))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused5)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused5))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused6)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused6))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused7)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused7))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused8)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused8))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused9)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused9))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused10)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused10))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused11)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused11))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused12)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused12))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused13)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused13))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_unused14)) {
      game_load_error = "Config structure changed: offsetof(Config, key_unused14))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_fire)) {
      game_load_error = "Config structure changed: offsetof(Config, key_fire))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_inventory)) {
      game_load_error = "Config structure changed: offsetof(Config, key_inventory))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_jump)) {
      game_load_error = "Config structure changed: offsetof(Config, key_jump))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_ascend)) {
      game_load_error = "Config structure changed: offsetof(Config, key_ascend))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_descend)) {
      game_load_error = "Config structure changed: offsetof(Config, key_descend))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, key_zoom)) {
      game_load_error = "Config structure changed: offsetof(Config, key_zoom))";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Config, hiscores)) {
      game_load_error = "Config structure changed: offsetof(Config, hiscores))";
      return in;
    }
  }

  in >> bits(my.t.seed_name);
  in >> bits(my.t.seed_num);

  READ_MAGIC("config part 1", (uint32_t) CONFIG_MAGIC_1);

  in >> bits(my.t.seed_source);
  in >> bits(my.t.player_name);

  READ_MAGIC("config part 2", (uint32_t) CONFIG_MAGIC_2);

  in >> bits(my.t.config_pix_height);
  in >> bits(my.t.config_pix_width);
  in >> bits(my.t.debug_mode);
  in >> bits(my.t.fps_counter);

  READ_MAGIC("config part 3", (uint32_t) CONFIG_MAGIC_3);

  in >> bits(my.t.gfx_borderless);
  in >> bits(my.t.gfx_fullscreen);
  in >> bits(my.t.gfx_fullscreen_desktop);
  in >> bits(my.t.gfx_vsync_enable);

  READ_MAGIC("config part 4", (uint32_t) CONFIG_MAGIC_4);

  in >> bits(my.t.key_wait);
  in >> bits(my.t.key_console);
  in >> bits(my.t.key_help);
  in >> bits(my.t.key_load);
  in >> bits(my.t.key_move_down);
  in >> bits(my.t.key_move_left);
  in >> bits(my.t.key_move_right);
  in >> bits(my.t.key_move_up);
  in >> bits(my.t.key_quit);
  in >> bits(my.t.key_save);
  in >> bits(my.t.key_screenshot);

  READ_MAGIC("config part 5", (uint32_t) CONFIG_MAGIC_5);

  in >> bits(my.t.key_unused1);
  in >> bits(my.t.key_unused2);
  in >> bits(my.t.key_unused3);
  in >> bits(my.t.key_unused4);
  in >> bits(my.t.key_unused5);
  in >> bits(my.t.key_unused6);
  in >> bits(my.t.key_unused7);
  in >> bits(my.t.key_unused8);
  in >> bits(my.t.key_unused9);
  in >> bits(my.t.key_unused10);
  in >> bits(my.t.key_unused11);
  in >> bits(my.t.key_unused12);
  in >> bits(my.t.key_unused13);
  in >> bits(my.t.key_unused14);
  in >> bits(my.t.key_fire);
  in >> bits(my.t.key_inventory);
  in >> bits(my.t.key_jump);
  in >> bits(my.t.key_ascend);
  in >> bits(my.t.key_descend);
  in >> bits(my.t.key_zoom);

  READ_MAGIC("config part 6", (uint32_t) CONFIG_MAGIC_6);

  in >> bits(my.t.mouse_wheel_lr_negated);
  in >> bits(my.t.mouse_wheel_ud_negated);
  in >> bits(my.t.music_volume);
  in >> bits(my.t.sdl_delay);
  in >> bits(my.t.sound_volume);

  log("read config: config_pix_height      = %d", my.t.config_pix_height);
  log("read config: config_pix_width       = %d", my.t.config_pix_width);
  log("read config: debug_mode             = %d", static_cast< int >(my.t.debug_mode));
  log("read config: fps_counter            = %d", static_cast< int >(my.t.fps_counter));
  log("read config: gfx_borderless         = %d", static_cast< int >(my.t.gfx_borderless));
  log("read config: gfx_fullscreen         = %d", static_cast< int >(my.t.gfx_fullscreen));
  log("read config: gfx_fullscreen_desktop = %d", static_cast< int >(my.t.gfx_fullscreen_desktop));
  log("read config: gfx_vsync_enable       = %d", static_cast< int >(my.t.gfx_vsync_enable));
  log("read config: mouse_wheel_lr_negated = %d", static_cast< int >(my.t.mouse_wheel_lr_negated));
  log("read config: mouse_wheel_ud_negated = %d", static_cast< int >(my.t.mouse_wheel_ud_negated));
  log("read config: music_volume           = %d", my.t.music_volume);
  log("read config: sdl_delay              = %d", my.t.sdl_delay);
  log("read config: sound_volume           = %d", my.t.sound_volume);

  if (! g_opt_override_debug_level) {
    if (my.t.debug_mode) {
      g_opt_debug2 = true;
      g_opt_debug1 = true;
    }
  }

  in >> bits(my.t.hiscores);

  int eol = 0;
  in >> bits(eol);
  if (eol != GAME_SAVE_MARKER_CONFIG) {
    game_load_error += "end of config marker not found";
    return in;
  }
  if (my.t.ascii_pix_height < 0) {
    game_load_error += "ascii_pix_height is invalid";
    return in;
  }
  if (my.t.ascii_pix_width < 0) {
    game_load_error += "ascii_pix_width is invalid";
    return in;
  }
  if (my.t.config_pix_height < 0) {
    game_load_error += "config_pix_height is invalid";
    return in;
  }
  if (my.t.config_pix_width < 0) {
    game_load_error += "config_pix_width is invalid";
    return in;
  }
  if (my.t.map_fbo_height < 0) {
    game_load_error += "map_fbo_height is invalid";
    return in;
  }
  if (my.t.map_fbo_width < 0) {
    game_load_error += "map_fbo_width is invalid";
    return in;
  }
  if (my.t.aspect_ratio < 0) {
    game_load_error += "aspect_ratio is invalid";
    return in;
  }
  if (my.t.window_pix_height < 0) {
    game_load_error += "window_pix_height is invalid";
    return in;
  }
  if (my.t.window_pix_width < 0) {
    game_load_error += "window_pix_width is invalid";
    return in;
  }

  return in;
}

auto operator>>(std::istream &in, Bits< class Game & > my) -> std::istream &
{
  TRACE();

  in >> bits(my.t.version);
  in >> bits(my.t.serialized_size);

  if (std::cmp_not_equal(my.t.serialized_size, sizeof(Game))) {
    if (my.t.version == MYVER) {
      game_load_error = "Incompatible save file for version " + my.t.version;
    } else {
      game_load_error = "Incompatible version. Expected version " MYVER ", found version " + my.t.version;
    }
    return in;
  }

  //
  // Check various structure sizes prior to reading the data
  //
  {
    uint32_t tmp = 0;

    in >> bits(tmp);
    log("read config: sizeof(Thing)          = %u", tmp);
    if (tmp != sizeof(Thing)) {
      game_load_error = "Incompatible Thing structure size";
      return in;
    }
    in >> bits(tmp);
    log("read config: sizeof(Level)          = %u", tmp);
    if (tmp != sizeof(Level)) {
      game_load_error = "Incompatible Level structure size";
      return in;
    }
    in >> bits(tmp);
    log("read config: sizeof(Levels)         = %u", tmp);
    if (tmp != sizeof(Levels)) {
      game_load_error = "Incompatible Levels structure size";
      return in;
    }

    //
    // Check Thing offsets
    //
    in >> bits(tmp);
    if (tmp != offsetof(Thing, tp_id)) {
      game_load_error = "thing structure changed: offsetof(Thing, tp_id)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, dir)) {
      game_load_error = "thing structure changed: offsetof(Thing, dir)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, anim_index)) {
      game_load_error = "thing structure changed: offsetof(Thing, anim_index)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _priority)) {
      game_load_error = "thing structure changed: offsetof(Thing, _priority)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _distance_jump)) {
      game_load_error = "thing structure changed: offsetof(Thing, _distance_jump)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _is_hit)) {
      game_load_error = "thing structure changed: offsetof(Thing, _is_hit)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _is_hidden)) {
      game_load_error = "thing structure changed: offsetof(Thing, _is_hidden)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _is_hot)) {
      game_load_error = "thing structure changed: offsetof(Thing, _is_hot)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, anim_type)) {
      game_load_error = "thing structure changed: offsetof(Thing, anim_type)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, level_num)) {
      game_load_error = "thing structure changed: offsetof(Thing, level_num)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, tile_index)) {
      game_load_error = "thing structure changed: offsetof(Thing, tile_index)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, anim_ms_remaining)) {
      game_load_error = "thing structure changed: offsetof(Thing, anim_ms_remaining)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _speed)) {
      game_load_error = "thing structure changed: offsetof(Thing, _speed)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _temperature)) {
      game_load_error = "thing structure changed: offsetof(Thing, _temperature)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _damage_this_tick)) {
      game_load_error = "thing structure changed: offsetof(Thing, _damage_this_tick)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _keys_carried)) {
      game_load_error = "thing structure changed: offsetof(Thing, _keys_carried)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value1)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value1)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value2)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value2)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value3)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value3)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value4)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value4)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value5)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value5)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value6)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value6)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value7)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value7)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value8)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value8)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value9)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value9)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value10)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value10)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value11)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value11)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value12)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value12)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value13)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value13)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value14)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value14)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value15)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value15)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value16)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value16)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value17)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value17)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value18)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value18)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _value19)) {
      game_load_error = "thing structure changed: offsetof(Thing, _value19)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _score_value)) {
      game_load_error = "thing structure changed: offsetof(Thing, _score_value)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _minion_max)) {
      game_load_error = "thing structure changed: offsetof(Thing, _minion_max)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _distance_minion_from_mob_max)) {
      game_load_error = "thing structure changed: offsetof(Thing, _distance_minion_from_mob_max)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _distance_vision)) {
      game_load_error = "thing structure changed: offsetof(Thing, _distance_vision)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _variant)) {
      game_load_error = "thing structure changed: offsetof(Thing, _variant)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _lifespan)) {
      game_load_error = "thing structure changed: offsetof(Thing, _lifespan)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _age)) {
      game_load_error = "thing structure changed: offsetof(Thing, _age)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _submerged_pct)) {
      game_load_error = "thing structure changed: offsetof(Thing, _submerged_pct)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _health)) {
      game_load_error = "thing structure changed: offsetof(Thing, _health)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _fall_ms)) {
      game_load_error = "thing structure changed: offsetof(Thing, _fall_ms)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _lunge_ms)) {
      game_load_error = "thing structure changed: offsetof(Thing, _lunge_ms)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _weight)) {
      game_load_error = "thing structure changed: offsetof(Thing, _weight)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, tick)) {
      game_load_error = "thing structure changed: offsetof(Thing, tick)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, tick_temperature)) {
      game_load_error = "thing structure changed: offsetof(Thing, tick_temperature)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, tick_temperature_last_change)) {
      game_load_error = "thing structure changed: offsetof(Thing, tick_temperature_last_change)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, iter)) {
      game_load_error = "thing structure changed: offsetof(Thing, iter)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, tick_dead)) {
      game_load_error = "thing structure changed: offsetof(Thing, tick_dead)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, tick_teleport)) {
      game_load_error = "thing structure changed: offsetof(Thing, tick_teleport)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, id)) {
      game_load_error = "thing structure changed: offsetof(Thing, id)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, owner_id)) {
      game_load_error = "thing structure changed: offsetof(Thing, owner_id)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, ext_id)) {
      game_load_error = "thing structure changed: offsetof(Thing, ext_id)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _prev_pix_at)) {
      game_load_error = "thing structure changed: offsetof(Thing, _prev_pix_at)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _curr_pix_at)) {
      game_load_error = "thing structure changed: offsetof(Thing, _curr_pix_at)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _moving_from)) {
      game_load_error = "thing structure changed: offsetof(Thing, _moving_from)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, last_pushed_at)) {
      game_load_error = "thing structure changed: offsetof(Thing, last_pushed_at)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, thing_dt)) {
      game_load_error = "thing structure changed: offsetof(Thing, thing_dt)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, angle)) {
      game_load_error = "thing structure changed: offsetof(Thing, angle)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _old_at)) {
      game_load_error = "thing structure changed: offsetof(Thing, _old_at)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Thing, _at)) {
      game_load_error = "thing structure changed: offsetof(Thing, _at)";
      return in;
    }

    //
    // Check Level offsets
    //
    in >> bits(tmp);
    if (tmp != offsetof(Level, level_num)) {
      game_load_error = "Level structure changed: offsetof(Level, level_num)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, info)) {
      game_load_error = "Level structure changed: offsetof(Level, info)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, level_select_at)) {
      game_load_error = "Level structure changed: offsetof(Level, level_select_at)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, entrance)) {
      game_load_error = "Level structure changed: offsetof(Level, entrance)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, exit)) {
      game_load_error = "Level structure changed: offsetof(Level, exit)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, ts)) {
      game_load_error = "Level structure changed: offsetof(Level, ts)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, last_ts)) {
      game_load_error = "Level structure changed: offsetof(Level, last_ts)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, tick_temperature)) {
      game_load_error = "Level structure changed: offsetof(Level, tick_temperature)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, level_num_next)) {
      game_load_error = "Level structure changed: offsetof(Level, level_num_next)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, is_modified_tile)) {
      game_load_error = "Level structure changed: offsetof(Level, is_modified_tile)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, thing_id)) {
      game_load_error = "Level structure changed: offsetof(Level, thing_id)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, player_has_walked_tile)) {
      game_load_error = "Level structure changed: offsetof(Level, player_has_walked_tile)";
      return in;
    }
    in >> bits(tmp);
    if (tmp != offsetof(Level, debug)) {
      game_load_error = "Level structure changed: offsetof(Level, debug)";
      return in;
    }
  }

  in >> bits(my.t.save_slot);
  in >> bits(my.t.save_meta);
  in >> bits(my.t.save_file);
  if (game_headers_only) {
    return in;
  }
  in >> bits(my.t.appdata);
  in >> bits(my.t.saved_dir);

  Levelsp tmp = static_cast< Levelsp >(MYMALLOC(sizeof(Levels), "loaded level"));
  NEWPTR(MTYPE_LEVELS, tmp, "loaded levels");
  in.read(reinterpret_cast< char * >(tmp), sizeof(Levels));
  my.t.levels = tmp;

  return in;
}

// binary mode is only for switching off newline translation
// ios::ate, open at end
auto read_file(const std::string &filename) -> std::vector< char >
{
  TRACE();

  std::ifstream ifs(filename, std::ios::in | std::ios::binary | std::ios::ate);
  if (ifs.is_open()) {
    ifs.unsetf(std::ios::skipws);

    std::ifstream::pos_type const sz = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector< char > bytes(sz);
    ifs.read(bytes.data(), sz);
    return bytes;
  }
  std::vector< char > bytes;
  return bytes;
}

static auto read_lzo_file(const std::string &filename, long *uncompressed_sz) -> std::vector< char >
{
  TRACE();

  std::ifstream ifs(filename, std::ios::in | std::ios::binary | std::ios::ate);
  if (ifs.is_open()) {
    // tellg is not ideal, look into <filesystem> post mojave
    std::ifstream::pos_type sz = ifs.tellg();
    if (sz < 0) {
      return {};
    }

    ifs.seekg(0, std::ios::beg);
    ifs.unsetf(std::ios::skipws);
    ifs.read(reinterpret_cast< char * >(uncompressed_sz), sizeof(*uncompressed_sz));

    sz -= static_cast< int >(sizeof(*uncompressed_sz));
    std::vector< char > bytes(sz);
    ifs.read(bytes.data(), sz);
    return bytes;
  }
  std::vector< char > bytes;
  return bytes;
}

auto Game::load(const std::string &file_to_load, class Game &target) -> bool
{
  log("load: %s", file_to_load.c_str());
  TRACE_INDENT();

  VERIFY(MTYPE_GAME, this);
  game_load_error = "";

  //
  // Read to a vector and then copy to a C buffer for LZO to use
  //
  long dst_size = 0;

  if (! game_headers_only) {
    VERIFY(MTYPE_GAME, this);
    wid_progress_bar(this, "Loading...", 0.0);
  }

  auto vec = read_lzo_file(file_to_load, &dst_size);

  if (vec.empty()) {
    if (! game_headers_only) {
      wid_error(game, "load error, empty file [" + file_to_load + "] ?");
    }
    VERIFY(MTYPE_GAME, this);
    wid_progress_bar_destroy(this);
    return false;
  }

  auto      *data     = vec.data();
  long const src_size = vec.size();

  auto *src = malloc(src_size MALLOC_PAD);
  if (src == nullptr) {
    CROAK("malloc %ld failed", (long) src_size);
  }
  auto *dst = malloc(dst_size MALLOC_PAD);
  if (dst == nullptr) {
    CROAK("malloc %ld failed", (long) dst_size);
  }
  memcpy(src, data, src_size);

  if (! game_headers_only) {
    VERIFY(MTYPE_GAME, this);
    wid_progress_bar(this, "Decompressing...", 0.5);
  }

  auto start = time_ms();

  log("expect: %s, decompress %ld (%ld bytes) -> %ld Mb (%ld bytes)", file_to_load.c_str(),
      src_size / (1024 * 1024), //
      src_size,                 //
      dst_size / (1024 * 1024), //
      dst_size);

#ifdef USE_LZ4
  const auto *which   = "LZ4";
  long        new_len = 0;
  new_len             = LZ4_decompress_safe(static_cast< const char * >(src), static_cast< char * >(dst), src_size, dst_size);
  if (new_len >= 0)
#else
  auto     which   = "LZ0";
  lzo_uint new_len = 0;
  auto     r       = lzo1x_decompress((lzo_bytep) src, src_size, (lzo_bytep) dst, &new_len, nullptr);
  if ((r == LZO_E_OK) && ((long) new_len == (long) dst_size))
#endif

  {
    log("%s decompress %ld Mb (%ld bytes) -> %ld Mb (%ld bytes) took %u ms (%s)",
        which,                    //
        src_size / (1024 * 1024), //
        src_size,                 //
        dst_size / (1024 * 1024), //
        dst_size,                 //
        time_ms() - start,        //
        file_to_load.c_str());
  } else {
    ERR("%s decompress %ld Mb (%ld bytes) -> %ld Mb (%ld error code) took %u ms (%s)",
        which,                           //
        (long) src_size / (1024 * 1024), //
        (long) src_size,                 //
        (long) dst_size / (1024 * 1024), //
        (long) dst_size,                 //
        time_ms() - start,               //
        file_to_load.c_str());
    VERIFY(MTYPE_GAME, this);
    wid_progress_bar_destroy(this);
    return false;
  }

  if (! game_headers_only) {
    VERIFY(MTYPE_GAME, this);
    wid_progress_bar(this, "Reading...", 0.75);
  }

#if 0
  IF_DEBUG2 {
    std::cout << "decompressed as ";
    hexdump((const uint8_t *)dst, dst_size);
  }
#endif

  std::string const  s(static_cast< const char * >(dst), static_cast< size_t >(dst_size));
  std::istringstream in(s);

  game_load_error = "";
  in >> bits(target);
  if (! game_load_error.empty()) {
    if (! game_headers_only) {
      wid_error(game, "load error, " + game_load_error);
    }
    return false;
  }

  free(dst);
  free(src);

  if (! game_headers_only) {
    VERIFY(MTYPE_GAME, this);
    wid_progress_bar(this, "Loaded", 1.0);
  }

  wid_progress_bar_destroy(this);

  return true;
}

auto game_load(Gamep g, const std::string &file_to_load) -> bool
{
  TRACE();

  return g->load(file_to_load, *g);
}

auto Game::load_config() const -> std::string
{
  TRACE();

  game_load_error = "";

  auto          filename = saved_dir + "config";
  std::ifstream in(filename);
  if (in.is_open()) {
    in >> bits(*(&game->config));
  }

  return game_load_error;
}

auto Game::load(int slot) -> bool
{
  log("load slot: %d", slot);
  TRACE_INDENT();

  if (slot < 0) {
    con("no game at that slot; bad slot.");
    sound_play(this, "error");
    return false;
  }

  if (slot >= UI_MAX_SAVE_SLOTS) {
    con("no game at that slot; bad slot.");
    sound_play(this, "error");
    return false;
  }

  if (! slot_valid[ slot ]) {
    con("no game at that slot.");
    sound_play(this, "error");
    return false;
  }

  log("clean up current game");
  game->fini();

  auto this_save_file = saved_dir + "saved-slot-" + std::to_string(slot);

  if (slot == UI_MAX_SAVE_SLOTS - 1) {
    this_save_file = saved_dir + "saved-snapshot";
  }

  log("loading: %s", this_save_file.c_str());
  g_loading = true;
  load(this_save_file, *this);
  g_loading = false;

  state_change(STATE_LOADED, "reset");
  state_change(STATE_PLAYING, "loaded game");

  if (! game_load_error.empty()) {
    sound_play(this, "error");
    topcon("failed to load the game from %s", this_save_file.c_str());
  } else {
    topcon("Loaded the game from %s", this_save_file.c_str());
  }

  return game_load_error.empty();
}

auto Game::load_snapshot() -> bool
{
  log("load snapshot");
  TRACE_INDENT();

  game->fini();

  auto this_save_file = saved_dir + "saved-snapshot";

  log("loading: %s", this_save_file.c_str());
  g_loading = true;
  load(this_save_file, *this);
  g_loading = false;

  state_change(STATE_PLAYING, "loaded snapshot");

  topcon("Loaded the game from %s", this_save_file.c_str());

  return game_load_error.empty();
}

void wid_load_destroy(Gamep g)
{
  if (wid_load == nullptr) {
    return;
  }

  log("widget load destroy");
  TRACE_INDENT();

  delete wid_load;
  wid_load = nullptr;
}

static auto wid_load_key_up(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game->config.key_console)) {
    return false;
  }

  switch (key->mod) {
    case KMOD_LCTRL :
    case KMOD_RCTRL :
    default :
      switch (key->sym) {
        default :
          {
            auto c = wid_event_to_char(key);
            switch (c) {
              case '0' :
              case '1' :
              case '2' :
              case '3' :
              case '4' :
              case '5' :
              case '6' :
              case '7' :
              case '8' :
              case '9' :
                {
                  int const slot = c - '0';
                  if (! slot_valid[ slot ]) {
                    sound_play(g, "error");
                    con("no game at that slot.");
                  } else {
                    game->load(slot);
                    wid_load_destroy(game);
                    game_state_reset(game, "load cancel");
                  }
                  return true;
                }
              case 'b' :
              case 'B' :
              case SDLK_ESCAPE :
                {
                  log("load game cancelled");
                  TRACE_INDENT();

                  wid_load_destroy(game);
                  game_state_reset(game, "load cancel");
                  return true;
                }
            }
          }
      }
  }

  return true;
}

static auto wid_load_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game->config.key_console)) {
    sound_play(g, "keypress");
    return false;
  }

  sound_play(g, "keypress");
  return true;
}

static auto wid_load_mouse_up(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("load selected slot");
  TRACE_INDENT();

  auto slot = wid_get_int_context(w);
  game->load(slot);
  wid_load_destroy(game);
  return true;
}

static auto wid_load_saved_snapshot(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("load snapshot");
  TRACE_INDENT();

  game->load_snapshot();
  wid_load_destroy(game);
  return true;
}

static auto wid_load_cancel(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  log("load cancel");
  TRACE_INDENT();

  wid_load_destroy(game);
  game_state_reset(g, "load cancel");
  return true;
}

void Game::load_select()
{
  log("load menu");
  TRACE_INDENT();

  if (wid_load != nullptr) {
    return;
  }

  game_load_error = "";

  int const    menu_height = UI_MAX_SAVE_SLOTS + 8;
  int const    menu_width  = UI_WID_POPUP_WIDTH_WIDE;
  spoint const outer_tl((TERM_WIDTH / 2) - (menu_width / 2), (TERM_HEIGHT / 2) - (menu_height / 2));
  spoint const outer_br((TERM_WIDTH / 2) + (menu_width / 2), (TERM_HEIGHT / 2) + (menu_height / 2));
  wid_load = new WidPopup(game, "game load", outer_tl, outer_br, nullptr, "", false, false);

  wid_set_on_key_up(wid_load->wid_popup_container, wid_load_key_up);
  wid_set_on_key_down(wid_load->wid_popup_container, wid_load_key_down);

  {
    auto *p = wid_load->wid_text_area->wid_text_area;
    auto *w = wid_new_back_button(game, p, "back");

    spoint const tl((menu_width / 2) - 4, menu_height - 4);
    spoint const br((menu_width / 2) + 3, menu_height - 2);

    wid_set_style(w, UI_WID_STYLE_NORMAL);
    wid_set_on_mouse_up(w, wid_load_cancel);

    wid_set_pos(w, tl, br);
  }

  game_headers_only = true;

  wid_load->log(game, "Choose a load slot.");

  int y_at = 3;
  for (auto slot = 0; slot < UI_MAX_SAVE_SLOTS; slot++) {
    Game tmp;
    auto tmp_file = saved_dir + "saved-slot-info-" + std::to_string(slot);

    if (slot == UI_MAX_SAVE_SLOTS - 1) {
      tmp_file = saved_dir + "saved-snapshot-info";
    }

    auto        *p = wid_load->wid_text_area->wid_text_area;
    auto        *w = wid_new_button(game, p, "load slot");
    spoint const tl(0, y_at);
    spoint const br(menu_width - 2, y_at);

    std::string s = std::to_string(slot) + ": ";
    if (! load(tmp_file, tmp)) {
      if (! game_load_error.empty()) {
        s += game_load_error;
        con("GAME LOADING ERROR: %s", game_load_error.c_str());
      } else {
        if (slot == UI_MAX_SAVE_SLOTS - 1) {
          s += "<no-snapshot>";
        } else {
          s += "<empty>";
        }
      }
      slot_valid[ slot ] = false;
    } else {
      if (slot == UI_MAX_SAVE_SLOTS - 1) {
        s += "snapshot: " + tmp.save_meta;
      } else {
        s += tmp.save_meta;
      }
      if (slot == UI_MAX_SAVE_SLOTS - 1) {
        wid_set_on_mouse_up(w, wid_load_saved_snapshot);
      } else {
        wid_set_on_mouse_up(w, wid_load_mouse_up);
      }
      slot_valid[ slot ] = true;
    }
    wid_set_int_context(w, slot);

    wid_set_pos(w, tl, br);
    wid_set_text(w, s);
    y_at++;
  }
  game_headers_only = false;
  wid_update(game, wid_load->wid_text_area->wid_text_area);

  game->state_change(STATE_LOAD_MENU, "load select");
}

void wid_load_select(Gamep g) { g->load_select(); }

auto game_load_last_config(const char *appdata) -> bool
{
  log("load config");
  TRACE_INDENT();

  game = new Game(std::string(appdata));
  NEWPTR(MTYPE_GAME, game, "game");

  auto config_error = game->load_config();

  std::string const version = "" MYVER "";

  if (game->config.version != version) {
    if (sdl.window != nullptr) {
      warn("Config version change. Will need to reset config. Found version [%s]. Expected version [%s].", game->config.version.c_str(),
           version.c_str());
    } else {
      sdl_msg_box("Config version change. Will need to reset config. Found version [%s]. Expected version [%s].",
                  game->config.version.c_str(), version.c_str());
    }
    OLDPTR(MTYPE_GAME, game);
    delete game;
    game = new Game(std::string(appdata));
    NEWPTR(MTYPE_GAME, game, "game (1)");
    reset_globals();
    game_save_config(game);
    g_errored_thread_id = -1;
  } else if (! config_error.empty()) {
    if (sdl.window != nullptr) {
      warn("Config error: %s. Will need to reset config.", config_error.c_str());
    } else {
      sdl_msg_box("Config error: %s. Will need to reset config.", config_error.c_str());
    }
    OLDPTR(MTYPE_GAME, game);
    delete game;
    game = nullptr;
    game = new Game(std::string(appdata));
    NEWPTR(MTYPE_GAME, game, "game (2)");
    reset_globals();
    game_save_config(game);
    g_errored_thread_id = -1;
  }

  return game_load_error.empty();
}
