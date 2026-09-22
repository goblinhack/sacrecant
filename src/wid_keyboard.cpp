//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_color_defs.hpp"
#include "my_main.hpp" // NOLINT
#include "my_ptrcheck.hpp"
#include "my_sdl_event.hpp"
#include "my_sound.hpp"
#include "my_spoint.hpp"
#include "my_time.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wid_keyboard.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <strings.h>
#include <cstddef>
#include <cstdint>
#include <string>

/*
 * How keys appear on screen
 */
static const char *keys[ WID_KEYBOARD_DOWN ][ WID_KEYBOARD_ACROSS ] = {
    {
        "!",
        "@",
        "#",
        "$",
        "%",
        "^",
        "*",
        "(",
        ")",
        "_",
        "+",
        "DEL",
    },
    {
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "0",
        "-",
        "CLEAR",
    },
    {
        "a",
        "b",
        "c",
        "d",
        "e",
        "f",
        "g",
        "h",
        "i",
        "j",
        ";",
        "CANCL", // codespell:ignore
    },
    {
        "k",
        "l",
        "m",
        "n",
        "o",
        "p",
        "q",
        "r",
        "s",
        "t",
        ":",
        "OK",
    },
    {
        "u",
        "v",
        "w",
        "x",
        "y",
        "z",
        "<",
        ">",
        "k",
        ",",
        ".",
        "SPACE",
    },
};

/*
 * The real key behind the scenes
 */
static const char key_char[ WID_KEYBOARD_DOWN ][ WID_KEYBOARD_ACROSS ] = {
    {'!', '@', '#', '$', '%', '^', '*', '(', ')', '_', '+', ''},
    {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', ''},
    {
        'a',
        'b',
        'c',
        'd',
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        ';',
        '',
    },
    {
        'k',
        'l',
        'm',
        'n',
        'o',
        'p',
        'q',
        'r',
        's',
        't',
        ':',
        '\n',
    },
    {
        'u',
        'v',
        'w',
        'x',
        'y',
        'z',
        '<',
        '>',
        'k',
        ',',
        '.',
        ' ',
    },
};

int wid_keyboard_visible;

static void               wid_keyboard_destroy(Gamep g, Widp w);
static void               wid_keyboard_set_focus(Gamep g, wid_keyboard_ctx *ctx, int focusx, int focusy);
[[nodiscard]] static auto wid_keyboard_text_input_key_event(Gamep g, Widp w, const SDL_Keysym *key) -> bool;

static void wid_keyboard_update_buttons(Gamep g, Widp w)
{
  TRACE();
  auto *ctx = static_cast< wid_keyboard_ctx * >(wid_get_void_context(w));
  VERIFY(MTYPE_MISC, ctx);

  int const width  = 7;
  int const height = 5;

  int x = 0;
  int y = 0;

  ctx->b = nullptr;

  for (x = 0; x < WID_KEYBOARD_ACROSS; x++) {
    for (y = 0; y < WID_KEYBOARD_DOWN; y++) {

      Widp b = ctx->buttons[ y ][ x ];

      spoint tl = {};
      spoint br = {};
      color  c;

      tl.x = x * width;
      tl.y = y * height;

      br.x = tl.x;
      br.y = tl.y;
      br.x += 6;
      br.y += 4;

      if ((x == ctx->focusx) && (y == ctx->focusy)) {
        c      = GREEN;
        ctx->b = b;
        wid_raise(g, b);
      } else {
        wid_lower(g, b);

        c = GRAY70;
      }

      if (ctx->is_new != 0) {
        wid_set_pos(b, tl, br);
      }
    }
  }
  wid_update(g, w);
}

static void wid_keyboard_event(Gamep g, Widp w, int focusx, int focusy, const SDL_Keysym *key_in)
{
  TRACE();
  auto *ctx = static_cast< wid_keyboard_ctx * >(wid_get_void_context(w));
  VERIFY(MTYPE_MISC, ctx);

  const char *add = nullptr;
  if ((focusx == -1) && (focusy == -1)) {
    add = nullptr;
  } else {
    add = keys[ focusy ][ focusx ];
  }

  if (key_in != nullptr) {
    (void) wid_receive_input(g, ctx->input, key_in);
  } else if (strcasecmp(add, "OK") == 0) {
    (ctx->selected)(g, ctx->w, wid_get_text(ctx->input));
  } else if (strcasecmp(add, "CANCL") == 0) { // codespell:ignore
    (ctx->cancelled)(g, ctx->w, wid_get_text(ctx->input));
  } else if (strcasecmp(add, "CLEAR") == 0) {
    for (;;) {
      SDL_Keysym key = {};
      key.sym        = SDLK_BACKSPACE;

      std::string const tmp = wid_get_text(ctx->input);
      if (tmp.empty()) {
        break;
      }
      (void) wid_receive_input(g, ctx->input, &key);
    }

  } else if (strcasecmp(add, "DEL") == 0) {
    SDL_Keysym key = {};
    key.sym        = SDLK_BACKSPACE;
    (void) wid_receive_input(g, ctx->input, &key);
  } else if (strcasecmp(add, "SPACE") == 0) {
    SDL_Keysym key = {};
    key.sym        = ' ';
    (void) wid_receive_input(g, ctx->input, &key);
  } else {
    SDL_Keysym key = {};
    key.sym        = add[ 0 ];
    (void) wid_receive_input(g, ctx->input, &key);
  }

  if ((key_in != nullptr) && (focusx == -1) && (focusy == -1)) {
    int x = 0;
    int y = 0;

    for (x = 0; x < WID_KEYBOARD_ACROSS; x++) {
      for (y = 0; y < WID_KEYBOARD_DOWN; y++) {
        char const c = key_char[ y ][ x ];
        if (c == key_in->sym) {
          focusx = x;
          focusy = y;
          break;
        }
      }

      if ((focusx != -1) && (focusy != -1)) {
        break;
      }
    }
  }

  if ((focusx != -1) && (focusy != -1)) {
    wid_keyboard_set_focus(g, ctx, focusx, focusy);
  }
}

[[nodiscard]] static auto wid_keyboard_mouse_event(Gamep g, Widp w, int focusx, int focusy) -> bool
{
  TRACE();
  wid_keyboard_event(g, w, focusx, focusy, nullptr /* key */);

  return true;
}

[[nodiscard]] static auto wid_keyboard_button_mouse_event(Gamep g, Widp w, int x, int y, uint32_t button) -> bool
{
  TRACE();
  int const focus  = wid_get_int_context(w);
  int const focusx = (focus & 0xff);
  int const focusy = (focus & 0xff00) >> 8;

  return wid_keyboard_mouse_event(g, w, focusx, focusy);
}

static void wid_keyboard_focus_right(Gamep g, wid_keyboard_ctx *ctx)
{
  TRACE();
  ctx->focusx++;
  if (ctx->focusx > WID_KEYBOARD_ACROSS - 1) {
    ctx->focusx = 0;
  }

  wid_keyboard_update_buttons(g, ctx->w);
}

static void wid_keyboard_focus_left(Gamep g, wid_keyboard_ctx *ctx)
{
  TRACE();
  ctx->focusx--;
  if (ctx->focusx < 0) {
    ctx->focusx = WID_KEYBOARD_ACROSS - 1;
  }

  wid_keyboard_update_buttons(g, ctx->w);
}

static void wid_keyboard_focus_down(Gamep g, wid_keyboard_ctx *ctx)
{
  TRACE();
  ctx->focusy++;
  if (ctx->focusy > WID_KEYBOARD_DOWN - 1) {
    ctx->focusy = 0;
  }

  wid_keyboard_update_buttons(g, ctx->w);
}

static void wid_keyboard_focus_up(Gamep g, wid_keyboard_ctx *ctx)
{
  TRACE();
  ctx->focusy--;
  if (ctx->focusy < 0) {
    ctx->focusy = WID_KEYBOARD_DOWN - 1;
  }

  wid_keyboard_update_buttons(g, ctx->w);
}

static void wid_keyboard_last_focus(Gamep g, wid_keyboard_ctx *ctx)
{
  TRACE();
  ctx->focusx = WID_KEYBOARD_ACROSS - 1;
  ctx->focusy = WID_KEYBOARD_DOWN - 1;

  wid_keyboard_update_buttons(g, ctx->w);
}

static void wid_keyboard_first_focus(Gamep g, wid_keyboard_ctx *ctx)
{
  TRACE();
  ctx->focusx = 0;
  ctx->focusy = 0;

  wid_keyboard_update_buttons(g, ctx->w);
}

static void wid_keyboard_set_focus(Gamep g, wid_keyboard_ctx *ctx, int focusx, int focusy)
{
  TRACE();
  ctx->focusx = focusx;
  ctx->focusy = focusy;

  wid_keyboard_update_buttons(g, ctx->w);
}

[[nodiscard]] static auto wid_keyboard_parent_key_down(Gamep g, Widp w, const SDL_Keysym *key) -> bool
{
  TRACE();
  auto *ctx = static_cast< wid_keyboard_ctx * >(wid_get_void_context(w));
  VERIFY(MTYPE_MISC, ctx);

  /*
   * Don't process events too soon. Else the menu might not even have
   * appeared.
   */
  if (game_time_ms() - ctx->created < 100) {
    return false;
  }

  (void) sound_play(g, "keypress");

  switch (key->sym) {
    case '`' :         return false;

    case SDLK_ESCAPE : (ctx->cancelled)(g, ctx->w, wid_get_text(ctx->input)); return true;

    case SDLK_RETURN :
      {
        (ctx->selected)(g, ctx->w, wid_get_text(ctx->input));
        return true;

        case SDLK_LEFT :  wid_keyboard_focus_left(g, ctx); break;

        case SDLK_RIGHT : wid_keyboard_focus_right(g, ctx); break;

        case SDLK_UP :    wid_keyboard_focus_up(g, ctx); break;

        case SDLK_DOWN :  wid_keyboard_focus_down(g, ctx); break;

        case SDLK_HOME :  wid_keyboard_first_focus(g, ctx); break;

        case SDLK_END :   wid_keyboard_last_focus(g, ctx); break;

        default :         wid_keyboard_event(g, ctx->w, -1, -1, key); return true;
      }
  }

  return true;
}

[[nodiscard]] static auto wid_keyboard_parent_joy_button(Gamep g, Widp w, int x, int y) -> bool
{
  TRACE();
  auto *ctx = static_cast< wid_keyboard_ctx * >(wid_get_void_context(w));
  VERIFY(MTYPE_MISC, ctx);
  int ret = 0;

  /*
   * Don't process events too soon. Else the menu might not even have
   * appeared.
   */
  if (game_time_ms() - ctx->created < 100) {
    return false;
  }

  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_A ])) {
    (ctx->selected)(g, ctx->w, wid_get_text(ctx->input));
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_B ])) {
    (ctx->cancelled)(g, ctx->w, wid_get_text(ctx->input));
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_X ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_Y ])) {
    (ctx->selected)(g, ctx->w, wid_get_text(ctx->input));
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_TOP_LEFT ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_TOP_RIGHT ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_LEFT_STICK_DOWN ])) {
    (ctx->selected)(g, ctx->w, wid_get_text(ctx->input));
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_RIGHT_STICK_DOWN ])) {
    (ctx->selected)(g, ctx->w, wid_get_text(ctx->input));
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_START ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_XBOX ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_BACK ])) {
    (ctx->cancelled)(g, ctx->w, wid_get_text(ctx->input));
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_UP ])) {
    wid_keyboard_focus_up(g, ctx);
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_DOWN ])) {
    wid_keyboard_focus_down(g, ctx);
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_LEFT ])) {
    wid_keyboard_focus_left(g, ctx);
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_RIGHT ])) {
    wid_keyboard_focus_right(g, ctx);
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_LEFT_FIRE ])) {
    (ctx->selected)(g, ctx->w, wid_get_text(ctx->input));
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_RIGHT_FIRE ])) {
    (ctx->selected)(g, ctx->w, wid_get_text(ctx->input));
    ret = 1;
  }

  wid_keyboard_update_buttons(g, ctx->w);
  if (ctx->b != nullptr) {
    wid_mouse_warp(g, ctx->b);
  }

  return ret != 0;
}

[[nodiscard]] static auto wid_keyboard_button_key_event(Gamep g, Widp w, const SDL_Keysym *key) -> bool
{
  TRACE();
  auto *ctx = static_cast< wid_keyboard_ctx * >(wid_get_void_context(w));
  VERIFY(MTYPE_MISC, ctx);

  /*
   * Don't process events too soon. Else the menu might not even have
   * appeared.
   */
  if (game_time_ms() - ctx->created < 100) {
    return false;
  }

  (void) sound_play(g, "keypress");

  switch (key->sym) {
    case '`' : return false;

    case SDLK_ESCAPE :
      (void) sound_play(g, "keypress");
      (ctx->cancelled)(g, ctx->w, wid_get_text(ctx->input));
      return true;

    case SDLK_RETURN :
      (void) sound_play(g, "keypress");
      (ctx->selected)(g, ctx->w, wid_get_text(ctx->input));
      return true;

    case SDLK_BACKSPACE :
    case SDLK_DELETE :
    case SDLK_LEFT :
    case SDLK_RIGHT :
    case SDLK_UP :
    case SDLK_DOWN :
    case SDLK_HOME :
    case SDLK_END :       break;

    default :             wid_keyboard_event(g, w, -1, -1, key); return true;
  }

  return false;
}

[[nodiscard]] static auto wid_keyboard_button_joy_button_event(Gamep g, Widp w, int x, int y) -> bool
{
  TRACE();
  auto *ctx = static_cast< wid_keyboard_ctx * >(wid_get_void_context(w));
  VERIFY(MTYPE_MISC, ctx);
  int ret = 0;

  /*
   * Don't process events too soon. Else the menu might not even have
   * appeared.
   */
  if (game_time_ms() - ctx->created < 100) {
    return false;
  }

  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_A ])) {
    ret = static_cast< int >(wid_keyboard_mouse_event(g, w, ctx->focusx, ctx->focusy));
  }

  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_B ])) {
    (ctx->selected)(g, ctx->w, wid_get_text(ctx->input));
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_X ])) {
    SDL_Keysym key = {};
    key.sym        = SDLK_BACKSPACE;
    (void) wid_keyboard_text_input_key_event(g, ctx->input, &key);
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_Y ])) {
    (ctx->selected)(g, ctx->w, wid_get_text(ctx->input));
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_TOP_LEFT ])) {
    SDL_Keysym key = {};
    key.sym        = SDLK_LEFT;
    (void) wid_keyboard_text_input_key_event(g, ctx->input, &key);
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_TOP_RIGHT ])) {
    SDL_Keysym key = {};
    key.sym        = SDLK_RIGHT;
    (void) wid_keyboard_text_input_key_event(g, ctx->input, &key);
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_LEFT_STICK_DOWN ])) {
    (void) wid_keyboard_mouse_event(g, w, ctx->focusx, ctx->focusy);
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_RIGHT_STICK_DOWN ])) {
    (void) wid_keyboard_mouse_event(g, w, ctx->focusx, ctx->focusy);
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_START ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_XBOX ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_BACK ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_UP ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_DOWN ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_LEFT ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_RIGHT ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_LEFT_FIRE ])) {
    SDL_Keysym key = {};
    key.sym        = SDLK_BACKSPACE;
    (void) wid_receive_input(g, ctx->input, &key);
    ret = 1;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_RIGHT_FIRE ])) {
    (void) wid_keyboard_mouse_event(g, w, ctx->focusx, ctx->focusy);
    ret = 1;
  }

  wid_keyboard_update_buttons(g, ctx->w);
  if (ctx->b != nullptr) {
    wid_mouse_warp(g, ctx->b);
  }

  return ret != 0;
}

[[nodiscard]] static auto wid_keyboard_text_input_key_event(Gamep g, Widp w, const SDL_Keysym *key) -> bool
{
  TRACE();
  auto *ctx = static_cast< wid_keyboard_ctx * >(wid_get_void_context(w));
  VERIFY(MTYPE_MISC, ctx);

  //
  // Don't process events too soon. Else the menu might not even have appeared.
  //
  if (game_time_ms() - ctx->created < 100) {
    return false;
  }

  (void) sound_play(g, "keypress");

  switch (key->sym) {
    case SDLK_ESCAPE :    (ctx->cancelled)(g, ctx->w, wid_get_text(ctx->input)); return true;

    case SDLK_RETURN :    (ctx->selected)(g, ctx->w, wid_get_text(ctx->input)); return true;

    case SDLK_BACKSPACE :
    case SDLK_DELETE :
    case SDLK_LEFT :
    case SDLK_RIGHT :
    case SDLK_UP :
    case SDLK_DOWN :
    case SDLK_HOME :
    case SDLK_END :       (void) wid_receive_input(g, ctx->input, key); return true;

    default :             wid_keyboard_event(g, w, -1, -1, key); return true;
  }

  return false;
}

static void wid_keyboard_destroy(Gamep g, Widp w)
{
  TRACE();
  auto *ctx = static_cast< wid_keyboard_ctx * >(wid_get_void_context(w));
  VERIFY(MTYPE_MISC, ctx);

  wid_set_void_context(w, nullptr);
  OLDPTR(MTYPE_MISC, ctx);

  wid_keyboard_visible = 0;
}

static void wid_keyboard_tick(Gamep g, Widp w)
{
  TRACE();
  auto *ctx = static_cast< wid_keyboard_ctx * >(wid_get_void_context(w));
  VERIFY(MTYPE_MISC, ctx);

  static int       val;
  static int       delta = 1;
  static int const step  = 2;

  val += delta * step;

  if (val > 255) {
    val   = 255;
    delta = -1;
  }

  if (val < 200) {
    val   = 200;
    delta = 1;
  }

  int x = 0;
  int y = 0;

  for (x = 0; x < WID_KEYBOARD_ACROSS; x++) {
    for (y = 0; y < WID_KEYBOARD_DOWN; y++) {

      if ((x != ctx->focusx) || (y != ctx->focusy)) {
        continue;
      }

      Widp b = ctx->buttons[ y ][ x ];

      color c;
      c   = GREEN;
      c.g = val;

      /*
       * Make sure the other widgets look plain in all modes.
       */
      int mode = 0;
      for (mode = WID_MODE_NORMAL; mode < WID_MODE_LAST; mode++) {
        wid_set_mode(b, static_cast< wid_mode >(mode));
        wid_set_color(b, WID_COLOR_TEXT_FG, c);
      }

      wid_set_mode(w, static_cast< wid_mode >(WID_MODE_NORMAL));
    }
  }
}

auto wid_keyboard(Gamep g, const std::string &text, const std::string &title, wid_keyboard_event_t selected, wid_keyboard_event_t cancelled,
                  size_t max_len) -> Widp
{
  TRACE();
  wid_keyboard_visible = 1;

  /*
   * Create a context to hold button info so we can update it when the focus
   * changes
   */
  wid_keyboard_ctx *ctx = static_cast< wid_keyboard_ctx * >(MYZALLOC(SIZEOF(*ctx), "widget keyboard"));
  ctx->focusx           = -1;
  ctx->focusy           = -1;
  ctx->cancelled        = cancelled;
  ctx->selected         = selected;

  Widp window  = wid_new_window(g, "widget keyboard");
  ctx->w       = window;
  ctx->is_new  = 1;
  ctx->max_len = max_len;

  /*
   * Main window
   */
  const auto width  = (WID_KEYBOARD_ACROSS * 7) + 1;
  const auto height = (WID_KEYBOARD_DOWN * 5) + 8;

  int const left_half  = width / 2;
  int const right_half = width - left_half;
  int const top_half   = height / 2;
  int const bot_half   = height - top_half;

  {
    spoint const tl((TERM_WIDTH / 2) - left_half, (TERM_HEIGHT / 2) - top_half);
    spoint const br((TERM_WIDTH / 2) + right_half, (TERM_HEIGHT / 2) + bot_half);

    wid_set_pos(window, tl, br);
    wid_set_style(window, UI_WID_STYLE_BUTTON_OUTLINE);
    wid_set_on_key_down(window, wid_keyboard_parent_key_down);
    wid_set_on_joy_button(window, wid_keyboard_parent_joy_button);
    wid_set_on_destroy(window, wid_keyboard_destroy);
    wid_set_void_context(window, ctx);
  }

  /*
   * Create the title
   */
  {
    Widp w = wid_new_square_button(g, window, "widget keyboard title");

    spoint const tl = {5, 1};
    spoint const br = {width - 5, 1};

    wid_set_pos(w, tl, br);
    wid_set_text(w, title);
    wid_set_shape_none(w);
    wid_set_color(w, WID_COLOR_TEXT_FG, YELLOW);
  }

  /*
   * Create the text input container
   */
  {
    Widp w = wid_new_red_button(g, window, "widget keyboard input");

    ctx->input = w;

    spoint const tl = {10, 3};
    spoint const br = {width - 10, 5};

    wid_set_pos(w, tl, br);
    wid_set_text(w, text);
    wid_set_show_cursor(w);
    wid_set_on_key_down(w, wid_keyboard_text_input_key_event);
    wid_set_void_context(w, ctx);

    if (static_cast< bool >(max_len)) {
      wid_set_text_max_len(w, max_len);
    }

    wid_set_color(w, WID_COLOR_BG, GRAY20);
    wid_set_color(w, WID_COLOR_TEXT_FG, GREEN);

    wid_set_mode(w, WID_MODE_OVER);

    wid_set_color(w, WID_COLOR_BG, GRAY20);
    wid_set_color(w, WID_COLOR_TEXT_FG, GREEN);

    wid_set_mode(w, WID_MODE_NORMAL);
  }

  /*
   * Create the button container
   */
  {
    Widp button_container = wid_new_square_button(g, window, "widget keyboard buttons");
    wid_set_shape_none(button_container);
    wid_set_on_tick(button_container, wid_keyboard_tick);

    spoint const tl = {1, 7};
    spoint const br = {width - 1, height - 2};

    wid_set_pos(button_container, tl, br);
    wid_set_void_context(button_container, ctx);

    /*
     * Create the buttons
     */
    int x = 0;
    int y = 0;

    for (x = 0; x < WID_KEYBOARD_ACROSS; x++) {
      for (y = 0; y < WID_KEYBOARD_DOWN; y++) {
        Widp b = nullptr;

        if (strcasecmp(keys[ y ][ x ], "CLEAR") == 0) {
          b = wid_new_cancel_button(g, button_container, "widget keyboard button");
        } else if (strcasecmp(keys[ y ][ x ], "CANCL") == 0) { // codespell:ignore
          b = wid_new_cancel_button(g, button_container, "widget keyboard button");
        } else if (strcasecmp(keys[ y ][ x ], "OK") == 0) {
          b = wid_new_green_button(g, button_container, "widget keyboard button");
        } else {
          b = wid_new_menu_button(g, button_container, "widget keyboard button");
        }
        ctx->buttons[ y ][ x ] = b;

        wid_set_text(b, keys[ y ][ x ]);

        wid_set_on_key_down(b, wid_keyboard_button_key_event);
        wid_set_on_joy_button(b, wid_keyboard_button_joy_button_event);
        wid_set_on_mouse_down(b, wid_keyboard_button_mouse_event);

        wid_set_void_context(b, ctx);
        int const focus = (y << 8) | x;
        wid_set_int_context(b, focus);
      }
    }
  }

  wid_keyboard_update_buttons(g, window);
  wid_set_do_not_lower(window, 1);
  wid_update(g, window);
  wid_raise(g, window);
  wid_set_focus(g, window);

  ctx->created = game_time_ms();

  return window;
}
