//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color_defs.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_pcg_basic.hpp"
#include "my_sdl_event.hpp"
#include "my_sdl_proto.hpp"
#include "my_ui.hpp"
#include "my_wids.hpp"

//
// Main loop
//
void sdl_loop(Gamep g)
{
  log("SDL: main loop");
  TRACE_INDENT();

  //
  // Keep this lowish to avoid too much lag when processing mouse motion events, that redraw the cursor path.
  //
  // 2 - seems too slow as it adds a noticeable lag when you do a wheel mouse event after a lot of mouse moves.
  // 3 - still some lag
  // 4 - seems ok
  // 10 - too much lag now due to cursor redraws
  //
  SDL_Event events[ 6 ] = {};
  int       found       = 0;
  int       i           = 0;
  int       frames      = 0;

  sdl_mouse_center(g);

  SDL_SetEventFilter(sdl_filter_events, nullptr);

  //
  // Wait for events
  //
  int ui_ts_fast_last      = time_ms();
  int ui_ts_slow_last      = ui_ts_fast_last;
  int ui_ts_very_slow_last = ui_ts_fast_last;

  g_main_loop_running = true;

#ifdef ENABLE_UI_ASCII_MOUSE
  SDL_ShowCursor(0);
#endif

  for (; /*ever*/;) {
    // DBG("SDL: tick");
    frames++;

    //
    // Reset joystick handling before we poll and update.
    //
    if ((sdl.joy_axes != nullptr)) [[unlikely]] {
      sdl_tick(g);
    }

    static int old_g_errored_thread_id;
    if ((AN_ERROR_OCCURRED())) [[unlikely]] {
      if (g_errored_thread_id != old_g_errored_thread_id) {
        if (g_errored_thread_id == MAIN_THREAD) {
          con(UI_IMPORTANT_FMT_STR "An error occurred on the main thread. Check the logs." UI_RESET_FMT);
        } else {
          con(UI_IMPORTANT_FMT_STR "An error occurred on thread %d. Check the logs." UI_RESET_FMT, g_errored_thread_id);
        }
        auto key = ::to_string(game_key_console_get(g));
        con("To continue playing at your own risk, 'clear errored' and then press <%s>", key.c_str());
        con("For more info 'show error'");
        wid_console_raise(g);
      }
    }
    old_g_errored_thread_id = g_errored_thread_id;

    //
    // Various event frequencies
    //
    int const  ts_now           = time_ms();
    bool const update_very_slow = (ts_now - ui_ts_very_slow_last >= UI_EVENT_LOOP_FREQ_VERY_SLOW_MS);
    bool const update_slow      = (ts_now - ui_ts_slow_last >= UI_EVENT_LOOP_FREQ_SLOW_MS);
    bool const update_fast      = (ts_now - ui_ts_fast_last >= UI_EVENT_LOOP_FREQ_FAST_MS);

    //
    // Check for clock reset due to leaving robot mode
    //
    if ((ts_now < ui_ts_fast_last) || (ts_now < ui_ts_slow_last) || (ts_now < ui_ts_very_slow_last)) {
      ui_ts_fast_last      = time_ms();
      ui_ts_slow_last      = ui_ts_fast_last;
      ui_ts_very_slow_last = ui_ts_fast_last;
    }

    //
    // This is for when in pixel art mode and between levels and waiting for level fade in
    //
    if ((update_very_slow)) [[unlikely]] {
      ui_ts_very_slow_last = ts_now;
      wid_display_all(g);
    }

    //
    // Less frequent updates like updating the FPS
    //
    if ((update_slow)) [[unlikely]] {
      ui_ts_slow_last = ts_now;
    }

    //
    // Do faster processing of events, like reading the keyboard and updating widgets.
    //
    if ((update_fast)) {
      ui_ts_fast_last = ts_now;

      //
      // Read events
      //
      SDL_PumpEvents();

      sdl.wheel_x = 0;
      sdl.wheel_y = 0;

      found = SDL_PeepEvents(events, ARRAY_SIZE(events), SDL_GETEVENT, SDL_QUIT, SDL_LASTEVENT);

      //
      // Only process one mouse motion event; and when we do we only look at the latest
      // mouse position, to avoid perception of lag. Mouse motion events can be expensive
      // as we redraw the cursor path.
      //
      if (found != 0) {
        if (found > 1) {
          DBG("SDL: Process %u events", found);
        } else {
          DBG("SDL: Process %u event", found);
        }
      }

      bool processed_mouse_motion_event = false;
      for (i = 0; i < found; ++i) {
        sdl_event(g, &events[ i ], processed_mouse_motion_event);
      }

      //
      // Handle key auto repeat
      //
      sdl_key_repeat_events(g);

      //
      // Mouse held?
      //
      if (found == 0) [[unlikely]] {
        auto mouse_down = sdl_get_mouse();
        if (mouse_down != 0) {
          if (static_cast< bool >(sdl.last_mouse_held_down_when)) {
            if (time_have_x_hundredths_passed_since(50, sdl.last_mouse_held_down_when)) {
              if ((sdl.held_mouse_x != 0) && (sdl.held_mouse_y != 0)) {
                DBG2("SDL: Mouse DOWN: held: Button %d now at %d,%d initially at %d,%d", mouse_down, sdl.mouse_x, sdl.mouse_y,
                     sdl.held_mouse_x, sdl.held_mouse_y);
                wid_mouse_held(g, sdl.mouse_down, sdl.held_mouse_x, sdl.held_mouse_y);
                sdl.held_mouse_x = 0;
                sdl.held_mouse_y = 0;
              } else {
                DBG2("SDL: Mouse DOWN: held: Button %d now at %d,%d", sdl.mouse_down, sdl.mouse_x, sdl.mouse_y);
                wid_mouse_held(g, sdl.mouse_down, sdl.mouse_x, sdl.mouse_y);
              }
            }
          }
        }
      }

      //
      // Screenshot?
      //
      if ((! g_do_screenshot)) [[unlikely]] {
        if ((! g_main_loop_running)) [[unlikely]] {
          DBG("Exit main loop");
          break;
        }
      }

      //
      // If the user has moved the mouse, update the widgets.
      //
      if (processed_mouse_motion_event) {
        game_pcg_lock();
        wid_display_all(g);
        game_pcg_unlock();
      }

      //
      // Per tick state handling
      //
      if (NO_ERROR_OCCURRED()) [[likely]] {
        game_tick(g);
      }
    }

    game_pcg_lock();

    //
    // Display the level
    //
    gl_enter_2d_mode(g, game_map_fbo_width_get(g), game_map_fbo_height_get(g));
    game_display(g);
    gl_enter_2d_mode(g, game_window_pix_width_get(g), game_window_pix_height_get(g));

    //
    // Display the FBOs
    //
    sdl_display(g);

    //
    // Config change?
    //
    if ((! g_need_restart_with_given_arguments.empty())) [[unlikely]] {
      log("restart needed");
      break;
    }

    //
    // Update FPS counter.
    //
    if ((game_fps_counter_get(g))) [[unlikely]] {
      static uint32_t fps_ts_begin;
      static uint32_t fps_ts_now;

      if (fps_ts_begin == 0U) [[unlikely]] {
        fps_ts_begin = time_ms();
      }

      if ((frames >= 100)) [[unlikely]] {
        fps_ts_now          = time_ms();
        uint32_t const diff = fps_ts_now - fps_ts_begin;
        if (diff != 0) {
          float const fps = static_cast< float >(frames * ONESEC) / static_cast< float >(diff);
          con("FPS %f", fps);
          game_fps_value_set(g, static_cast< int >(fps));
        } else {
          con("FPS calculating...");
          game_fps_value_set(g, 0);
        }
        fps_ts_begin = fps_ts_now;
        frames       = 0;
      }
    }

    game_pcg_unlock();
  }

  log("SDL: exited main loop");

  gl_leave_2d_mode(g);

#ifdef ENABLE_UI_ASCII_MOUSE
  SDL_ShowCursor(1);
#endif
}
