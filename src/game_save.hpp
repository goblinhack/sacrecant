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
#include "my_ptrcheck.hpp"
#include "my_sdl_proto.hpp"
#include "my_serialize.hpp"
#include "my_wid_popup.hpp"

static WidPopup *wid_save;

extern bool game_headers_only;
bool        game_save_config_only;
int         GAME_SAVE_MARKER_EOL    = 123456;
int         GAME_SAVE_MARKER_CONFIG = 987654;

#define WRITE_MAGIC(m)                                                                                                                     \
  {                                                                                                                                        \
    uint32_t magic = m;                                                                                                                    \
    out << bits(magic);                                                                                                                    \
  }

auto operator<<(std::ostream &out, Bits< const SDL_Keysym & > const my) -> std::ostream &
{
  TRACE();

  out << bits(my.t.scancode);
  out << bits(my.t.sym);
  out << bits(my.t.mod);
  out << bits(my.t.unused);
  return out;
}

auto operator<<(std::ostream &out, Bits< const Config & > const my) -> std::ostream &
{
  TRACE();

  out << bits(my.t.version);
  uint32_t serialized_size = sizeof(Config);
  out << bits(serialized_size);

  //
  // Save various structure offsets so we can check what we load looks sane
  //
  {
    uint32_t tmp = 0;
    tmp          = offsetof(Config, seed_name);
    out << bits(tmp);
    tmp = offsetof(Config, seed_num);
    out << bits(tmp);
    tmp = offsetof(Config, seed_source);
    out << bits(tmp);
    tmp = offsetof(Config, player_name);
    out << bits(tmp);
    tmp = offsetof(Config, aspect_ratio);
    out << bits(tmp);
    tmp = offsetof(Config, ui_term_height);
    out << bits(tmp);
    tmp = offsetof(Config, ui_term_width);
    out << bits(tmp);
    tmp = offsetof(Config, debug_mode);
    out << bits(tmp);
    tmp = offsetof(Config, fps_counter);
    out << bits(tmp);
    tmp = offsetof(Config, gfx_borderless);
    out << bits(tmp);
    tmp = offsetof(Config, gfx_fullscreen);
    out << bits(tmp);
    tmp = offsetof(Config, gfx_fullscreen_desktop);
    out << bits(tmp);
    tmp = offsetof(Config, gfx_vsync_enable);
    out << bits(tmp);
    tmp = offsetof(Config, mouse_wheel_lr_negated);
    out << bits(tmp);
    tmp = offsetof(Config, mouse_wheel_ud_negated);
    out << bits(tmp);
    tmp = offsetof(Config, config_pix_height);
    out << bits(tmp);
    tmp = offsetof(Config, config_pix_width);
    out << bits(tmp);
    tmp = offsetof(Config, map_fbo_height);
    out << bits(tmp);
    tmp = offsetof(Config, map_fbo_width);
    out << bits(tmp);
    tmp = offsetof(Config, window_pix_height);
    out << bits(tmp);
    tmp = offsetof(Config, window_pix_width);
    out << bits(tmp);
    tmp = offsetof(Config, tiles_visible_across);
    out << bits(tmp);
    tmp = offsetof(Config, tiles_visible_down);
    out << bits(tmp);
    tmp = offsetof(Config, ascii_pix_height);
    out << bits(tmp);
    tmp = offsetof(Config, ascii_pix_width);
    out << bits(tmp);
    tmp = offsetof(Config, music_volume);
    out << bits(tmp);
    tmp = offsetof(Config, sdl_delay);
    out << bits(tmp);
    tmp = offsetof(Config, sound_volume);
    out << bits(tmp);
    tmp = offsetof(Config, key_wait);
    out << bits(tmp);
    tmp = offsetof(Config, key_console);
    out << bits(tmp);
    tmp = offsetof(Config, key_help);
    out << bits(tmp);
    tmp = offsetof(Config, key_load);
    out << bits(tmp);
    tmp = offsetof(Config, key_move_down);
    out << bits(tmp);
    tmp = offsetof(Config, key_move_left);
    out << bits(tmp);
    tmp = offsetof(Config, key_move_right);
    out << bits(tmp);
    tmp = offsetof(Config, key_move_up);
    out << bits(tmp);
    tmp = offsetof(Config, key_quit);
    out << bits(tmp);
    tmp = offsetof(Config, key_save);
    out << bits(tmp);
    tmp = offsetof(Config, key_screenshot);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused1);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused2);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused3);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused4);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused5);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused6);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused7);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused8);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused9);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused10);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused11);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused12);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused13);
    out << bits(tmp);
    tmp = offsetof(Config, key_unused14);
    out << bits(tmp);
    tmp = offsetof(Config, key_fire);
    out << bits(tmp);
    tmp = offsetof(Config, key_inventory);
    out << bits(tmp);
    tmp = offsetof(Config, key_jump);
    out << bits(tmp);
    tmp = offsetof(Config, key_ascend);
    out << bits(tmp);
    tmp = offsetof(Config, key_descend);
    out << bits(tmp);
    tmp = offsetof(Config, key_zoom);
    out << bits(tmp);
    tmp = offsetof(Config, hiscores);
    out << bits(tmp);
  }

  if (my.t.seed_name == TEST_SEED) {
    std::string nothing;
    out << bits(nothing);
  } else {
    out << bits(my.t.seed_name);
  }
  out << bits(my.t.seed_num);

  WRITE_MAGIC(CONFIG_MAGIC_1);

  out << bits(my.t.seed_source);
  out << bits(my.t.player_name);

  WRITE_MAGIC(CONFIG_MAGIC_2);

  out << bits(my.t.config_pix_height);
  out << bits(my.t.config_pix_width);
  out << bits(my.t.debug_mode);
  out << bits(my.t.fps_counter);

  WRITE_MAGIC(CONFIG_MAGIC_3);

  out << bits(my.t.gfx_borderless);
  out << bits(my.t.gfx_fullscreen);
  out << bits(my.t.gfx_fullscreen_desktop);
  out << bits(my.t.gfx_vsync_enable);

  WRITE_MAGIC(CONFIG_MAGIC_4);

  out << bits(my.t.key_wait);
  out << bits(my.t.key_console);
  out << bits(my.t.key_help);
  out << bits(my.t.key_load);
  out << bits(my.t.key_move_down);
  out << bits(my.t.key_move_left);
  out << bits(my.t.key_move_right);
  out << bits(my.t.key_move_up);
  out << bits(my.t.key_quit);
  out << bits(my.t.key_save);
  out << bits(my.t.key_screenshot);

  WRITE_MAGIC(CONFIG_MAGIC_5);

  out << bits(my.t.key_unused1);
  out << bits(my.t.key_unused2);
  out << bits(my.t.key_unused3);
  out << bits(my.t.key_unused4);
  out << bits(my.t.key_unused5);
  out << bits(my.t.key_unused6);
  out << bits(my.t.key_unused7);
  out << bits(my.t.key_unused8);
  out << bits(my.t.key_unused9);
  out << bits(my.t.key_unused10);
  out << bits(my.t.key_unused11);
  out << bits(my.t.key_unused12);
  out << bits(my.t.key_unused13);
  out << bits(my.t.key_unused14);
  out << bits(my.t.key_fire);
  out << bits(my.t.key_inventory);
  out << bits(my.t.key_jump);
  out << bits(my.t.key_ascend);
  out << bits(my.t.key_descend);
  out << bits(my.t.key_zoom);

  WRITE_MAGIC(CONFIG_MAGIC_6);

  out << bits(my.t.mouse_wheel_lr_negated);
  out << bits(my.t.mouse_wheel_ud_negated);
  out << bits(my.t.music_volume);
  out << bits(my.t.sdl_delay);
  out << bits(my.t.sound_volume);

  out << bits(my.t.hiscores);

  auto eol = GAME_SAVE_MARKER_CONFIG;
  out << bits(eol);

  return out;
}

auto operator<<(std::ostream &out, Bits< const class Game & > const my) -> std::ostream &
{
  TRACE();

  auto serialized_size = static_cast< uint32_t >(sizeof(Game));
  out << bits(my.t.version);
  out << bits(serialized_size);

  //
  // Save various structure sizes
  //
  serialized_size = static_cast< uint32_t >(sizeof(Thing));
  out << bits(serialized_size);
  serialized_size = static_cast< uint32_t >(sizeof(Level));
  out << bits(serialized_size);
  serialized_size = static_cast< uint32_t >(sizeof(Levels));
  out << bits(serialized_size);

  //
  // Save various structure offsets so we can check what we load looks sane
  //
  {
    uint32_t tmp = 0;
    //
    // Save Thing offsets
    //
    tmp = offsetof(Thing, tp_id);                         // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, dir);                           // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, anim_index);                    // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _priority);                     // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _distance_jump);                // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _is_hit);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _is_hidden);                    // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _is_hot);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, anim_type);                     // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, level_num);                     // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, tile_index);                    // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, anim_ms_remaining);             // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _speed);                        // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _temperature);                  // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _damage_this_tick);             // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _keys_carried);                 // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value1);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value2);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value3);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value4);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value5);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value6);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value7);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value8);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value9);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value10);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value11);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value12);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value13);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value14);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value15);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value16);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value17);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value18);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value19);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _value20);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _minion_max);                   // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _distance_minion_from_mob_max); // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _distance_vision);              // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _variant);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _lifespan);                     // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _age);                          // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _submerged_pct);                // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _health);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _fall_ms);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _lunge_ms);                     // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _weight);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, tick);                          // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, tick_temperature);              // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, tick_temperature_last_change);  // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, iter);                          // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, tick_dead);                     // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, tick_teleport);                 // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, id);                            // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, owner_id);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, ext_id);                        // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _prev_pix_at);                  // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _curr_pix_at);                  // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _moving_from);                  // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, last_pushed_at);                // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, thing_dt);                      // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, angle);                         // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _old_at);                       // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later
    tmp = offsetof(Thing, _at);                           // all we're saving here is offsets for validation later
    out << bits(tmp);                                     // all we're saving here is offsets for validation later

    //
    // Save Level offsets
    //
    tmp = offsetof(Level, level_num);
    out << bits(tmp);
    tmp = offsetof(Level, info);
    out << bits(tmp);
    tmp = offsetof(Level, level_select_at);
    out << bits(tmp);
    tmp = offsetof(Level, entrance);
    out << bits(tmp);
    tmp = offsetof(Level, exit);
    out << bits(tmp);
    tmp = offsetof(Level, ts);
    out << bits(tmp);
    tmp = offsetof(Level, last_ts);
    out << bits(tmp);
    tmp = offsetof(Level, tick_temperature);
    out << bits(tmp);
    tmp = offsetof(Level, level_num_next);
    out << bits(tmp);
    tmp = offsetof(Level, is_modified_tile);
    out << bits(tmp);
    tmp = offsetof(Level, thing_id);
    out << bits(tmp);
    tmp = offsetof(Level, player_has_walked_tile);
    out << bits(tmp);
    tmp = offsetof(Level, debug);
    out << bits(tmp);
  }

  out << bits(my.t.save_slot);
  out << bits(my.t.save_meta);
  out << bits(my.t.save_file);
  out << bits(my.t.appdata);
  out << bits(my.t.saved_dir);

  if (! game_headers_only) {
    out.write(reinterpret_cast< const char * >(my.t.levels), sizeof(*my.t.levels));
  }

  return out;
}

auto Game::save(const std::string &file_to_save) -> bool
{
  log("save: %s", file_to_save.c_str());
  TRACE_INDENT();

  //
  // Not sur why I needed this
  //
  bool const need_larger_src_buffer = false;

  if (! game_headers_only) {
    wid_progress_bar(this, "Serializing...", 0.2F);
  }

  std::stringstream s(std::ios::in | std::ios::out | std::ios::binary);

  const class Game &c = *this;
  s << bits(c);

  //
  // Get the pre compress buffer
  //
  if (! game_headers_only) {
    wid_progress_bar(this, "Stringifying...", 0.4F);
  }

  auto data = s.str(); // This is a bit slow, but the buffere may not be contiguous
  s.seekg(0, std::ios::end);
  long src_size = s.tellg();
  s.seekg(0, std::ios::beg);

  void *src = nullptr;
  if (need_larger_src_buffer) {
    if (! game_headers_only) {
      wid_progress_bar(this, "Allocating src buffer...", 0.5F);
    }

    src = malloc(src_size MALLOC_PAD);
    if (src == nullptr) {
      CROAK("malloc %d failed", (int) src_size);
    }
  }

  if (! game_headers_only) {
    wid_progress_bar(this, "Allocating dst buffer...", 0.6F);
  }

  auto *dst = malloc(src_size MALLOC_PAD);
  if (dst == nullptr) {
    CROAK("malloc %d failed", (int) src_size);
  }

  if (need_larger_src_buffer) {
    if (! game_headers_only) {
      wid_progress_bar(this, "Copying data...", 0.7F);
    }

    memcpy(src, data.c_str(), src_size);
  } else {
    src = (void *) data.c_str();
  }

#if 0
  IF_DEBUG2 { //

    std::cout << "before compression ";
    (void) hexdump((const uint8_t*)src, src_size);
  }
#endif

#ifndef USE_LZ4
  if (lzo_init() != LZO_E_OK) {
    err("LZO init fail: Enable '-DLZO_DEBUG' for diagnostics)");
  }
#endif

  if (! game_headers_only) {
    wid_progress_bar(this, "Compressing...", 0.8F);
  }

  auto start    = time_ms();
  long dst_size = 0;

#ifdef USE_LZ4
  const auto *which = "LZ4";
  dst_size          = LZ4_compress_default(static_cast< const char * >(src), static_cast< char * >(dst), src_size, src_size);
  if (dst_size != 0)
#else
  auto which  = "LZ0";
  auto wrkmem = malloc(LZO1X_1_MEM_COMPRESS);
  if (! wrkmem) {
    CROAK("malloc %ld failed", (long) LZO1X_1_MEM_COMPRESS);
  }

  lzo_uint new_len = dst_size;
  long     r       = lzo1x_1_compress((lzo_bytep) src, src_size, (lzo_bytep) dst, &new_len, wrkmem);
  dst_size         = new_len;
  if (r == LZO_E_OK)
#endif

  {
    log("%s compressed %ld Mb (%ld bytes) -> %ld Mb (%ld bytes) took %u ms",
        which,                    //
        src_size / (1024 * 1024), //
        src_size,                 //
        dst_size / (1024 * 1024), //
        dst_size,                 //
        time_ms() - start);
  } else {
    err("%s compressed failed %ld Mb (%ld bytes) -> %ld Mb (%ld error code) took %u ms",
        which,                           //
        (long) src_size / (1024 * 1024), //
        src_size,                        //
        (long) dst_size / (1024 * 1024), //
        dst_size,                        //
        time_ms() - start);
    wid_progress_bar_destroy(this);
    return false;
  }

  //
  // Dump the post compress buffer
  //
#if 0
  IF_DEBUG2 { //

    std::cout << "after compression ";
    (void) hexdump((const uint8_t *)dst, dst_size);
  }
#endif

  if (! game_headers_only) {
    wid_progress_bar(this, "Writing...", 0.9F);
  }

  //
  // Save the post compress buffer
  //
  auto *ofile = fopen(file_to_save.c_str(), "wb");
  if (ofile == nullptr) {
    err("failed to open %s for writing: %s", file_to_save.c_str(), strerror(errno));
    wid_progress_bar_destroy(this);
    return false;
  }

  log("opened: %s for writing", file_to_save.c_str());

  fwrite(reinterpret_cast< char * >(&src_size), sizeof(src_size), 1, ofile);
  fwrite(dst, dst_size, 1, ofile);
  fclose(ofile);

  log("wrote: %s", file_to_save.c_str());

  if (need_larger_src_buffer) {
    free(src);
  }
  free(dst);

#ifndef USE_LZ4
  free(wrkmem);
#endif

  if (! game_headers_only) {
    wid_progress_bar(this, "Saved", 1.0F);
  }

  wid_progress_bar_destroy(this);

  return true;
}

auto game_save(Gamep g, const std::string &file_to_save) -> bool
{
  log("save: %s", file_to_save.c_str());

  return g->save(file_to_save);
}

auto Game::save(int slot) -> bool
{
  log("save slot: %d", slot);
  TRACE_INDENT();

  if (slot < 0) {
    return false;
  }

  if (slot >= UI_MAX_SAVE_SLOTS) {
    return false;
  }

  auto this_save_file = saved_dir + "saved-slot-info-" + std::to_string(slot);

  log("saving: %s", this_save_file.c_str());
  game_headers_only = true;
  save(this_save_file);
  game_headers_only = false;

  this_save_file = saved_dir + "saved-slot-" + std::to_string(slot);

  log("saving: %s", this_save_file.c_str());
  auto ret = save(this_save_file);

  con("Saved the game to %s.", this_save_file.c_str());

  return ret;
}

auto Game::save_snapshot() -> bool
{
  log("save snapshot");
  TRACE_INDENT();

  auto this_save_file = saved_dir + "saved-snapshot-info";

  log("saving: %s", this_save_file.c_str());
  game_headers_only = true;
  save(this_save_file);
  game_headers_only = true;

  this_save_file = saved_dir + "saved-snapshot";

  log("saving: %s", this_save_file.c_str());
  auto ret = save(this_save_file);

  con("%%fg=green$Autosaved.%%fg=reset$");
  return ret;
}

auto Game::save_config() -> bool
{
  TRACE();

  //
  // Stops clang tidy adding const for this function
  //
  this->compiler_non_const_hack = true;

  auto          filename = saved_dir + "config";
  std::ofstream out(filename, std::ios::binary);
  if (! out) {
    err("failed to open %s for writing: %s", filename.c_str(), strerror(errno));
    return false;
  }
  log("opened [%s] for writing", filename.c_str());
  const Config &c = game->config;
  out << bits(c);
  return true;
}

void wid_save_destroy(Gamep g)
{
  if (wid_save == nullptr) {
    return;
  }

  log("widget save destroy");
  TRACE_INDENT();

  delete wid_save;
  wid_save = nullptr;
  g->state_reset("widget save destroy");
}

static auto wid_save_key_up(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
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
                  game->save(slot);
                  wid_save_destroy(game);
                  return true;
                }
              case 'b' :
              case 'B' :
              case SDLK_ESCAPE :
                {
                  log("save game cancelled");
                  TRACE_INDENT();
                  wid_save_destroy(game);
                  return true;
                }
            }
          }
      }
  }

  return true;
}

static auto wid_save_key_down(Gamep g, Widp w, const struct SDL_Keysym *key) -> bool
{
  TRACE();

  if (sdlk_eq(*key, game->config.key_console)) {
    sound_play(g, "keypress");
    return false;
  }

  sound_play(g, "keypress");
  return true;
}

static auto wid_save_mouse_up(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  auto slot = wid_get_int_context(w);
  game->save(slot);
  wid_save_destroy(game);
  return true;
}

static auto wid_save_cancel(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();

  wid_save_destroy(game);
  return true;
}

auto Game::save_select() -> bool
{
  log("save menu");
  TRACE_INDENT();

  if (wid_save != nullptr) {
    return false;
  }

  int const    menu_height = UI_MAX_SAVE_SLOTS + 8;
  int const    menu_width  = UI_WID_POPUP_WIDTH_WIDE;
  spoint const outer_tl((TERM_WIDTH / 2) - (menu_width / 2), (TERM_HEIGHT / 2) - (menu_height / 2));
  spoint const outer_br((TERM_WIDTH / 2) + (menu_width / 2), (TERM_HEIGHT / 2) + (menu_height / 2));
  wid_save = new WidPopup(game, "game save", outer_tl, outer_br, nullptr, "", false, false);

  wid_set_on_key_up(wid_save->wid_popup_container, wid_save_key_up);
  wid_set_on_key_down(wid_save->wid_popup_container, wid_save_key_down);

  {
    auto *p = wid_save->wid_text_area->wid_text_area;
    auto *w = wid_new_back_button(game, p, "back");

    spoint const tl((menu_width / 2) - 4, menu_height - 4);
    spoint const br((menu_width / 2) + 3, menu_height - 2);

    wid_set_on_mouse_up(w, wid_save_cancel);
    wid_set_pos(w, tl, br);
  }

  game_headers_only = true;

  wid_save->log_empty_line(game);
  wid_save->log(game, "Choose a save slot.");

  int y_at = 3;
  for (auto slot = 0; slot < UI_MAX_SAVE_SLOTS; slot++) {
    Game tmp;
    auto tmp_file = saved_dir + "saved-slot-info-" + std::to_string(slot);

    if (slot == UI_MAX_SAVE_SLOTS - 1) {
      tmp_file = saved_dir + "saved-snapshot";
    }

    auto        *p = wid_save->wid_text_area->wid_text_area;
    auto        *w = wid_new_button(game, p, "save slot");
    spoint const tl(0, y_at);
    spoint const br(menu_width - 2, y_at);

    std::string s = std::to_string(slot) + ": ";
    if (! load(tmp_file, tmp)) {
      if (slot == UI_MAX_SAVE_SLOTS - 1) {
        s += "<no snapshot>";
      } else {
        s += "<empty>";
      }
    } else {
      if (slot == UI_MAX_SAVE_SLOTS - 1) {
        s += "snapshot: " + tmp.save_meta;
      } else {
        s += tmp.save_meta;
      }
    }

    if (slot == UI_MAX_SAVE_SLOTS - 1) {
      //
      // Cannot save over
      //
    } else {
      wid_set_on_mouse_up(w, wid_save_mouse_up);
      wid_set_int_context(w, slot);
    }

    wid_set_pos(w, tl, br);
    wid_set_text(w, s);
    y_at++;
  }
  game_headers_only = false;
  wid_update(game, wid_save->wid_text_area->wid_text_area);

  state_change(STATE_SAVE_MENU, "save select");

  return true;
}

void wid_save_select(Gamep g) { g->save_select(); }
