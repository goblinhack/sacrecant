//
// Copyright goblinhack@gmail.com
//

#ifndef MY_UI_HPP
#define MY_UI_HPP

#include "my_ascii.hpp"

//
// Console size
//
#define UI_CONSOLE_HEIGHT      (TERM_HEIGHT * 2)
#define UI_CONSOLE_INPUT_COLOR ORANGE
#define UI_CONSOLE_TEXT_COLOR  WHITE
#define UI_CONSOLE_WIDTH       (TERM_WIDTH - 3)

//
// If we use a character for the mouse:
//
#define UI_CURSOR_COLOR       PURPLE
#define UI_CURSOR_OTHER_COLOR WHITE

//
// Default text colors
//
#define UI_TEXT_COLOR       GRAY // Default text color
#define UI_TEXT_COLOR_STR   "gray"
#define UI_HIGHLIGHT_COLOR  WHITE // Default highlight color
#define UI_POPUP_TEXT_COLOR UI_TEXT_COLOR

//
// Default UI style
//
enum { UI_TYPE_PIXELART = 0, UI_TYPES_MAX = 1 };

//
// Format strings
//
#define UI_FMT_STR           "%%fg=" UI_TEXT_COLOR_STR "$"
#define UI_INFO1_FMT_STR     "%%fg=gray70$"
#define UI_INFO2_FMT_STR     "%%fg=gray90$"
#define UI_INFO3_FMT_STR     "%%fg=gray70$"
#define UI_INFO4_FMT_STR     "%%fg=gray90$"
#define UI_INFO5_FMT_STR     "%%fg=gray70$"
#define UI_INFO6_FMT_STR     "%%fg=gray90$"
#define UI_GOOD_FMT_STR      "%%fg=gold$"
#define UI_GREEN_FMT_STR     "%%fg=green$"
#define UI_HIGHLIGHT_FMT_STR "%%fg=white$"
#define UI_IMPORTANT_FMT_STR "%%fg=red$"
#define UI_INFO_FMT_STR      "%%fg=green$"
#define UI_ORANGE_FMT_STR    "%%fg=orange$"
#define UI_RED_FMT_STR       "%%fg=red$"
#define UI_RESET_FMT         "%%fg=reset$"
#define UI_SHORTCUT_FMT_STR  "%%fg=orange$"
#define UI_WARNING_FMT_STR   "%%fg=yellow$"
#define UI_WHITE_FMT_STR     "%%fg=white$"
#define UI_YELLOW_FMT_STR    "%%fg=yellow$"

//
// Character sizes for boxes in the UI
//
enum { UI_TILE_HEIGHT = 8, UI_TILE_WIDTH = 8 };

//
// Character sizes for text in the UI
//
enum { UI_FONT_HEIGHT = 8, UI_FONT_WIDTH = 6 };

//
// Name limits
//
enum {
  UI_MAX_PLAYER_NAME_LEN = 12,
  UI_MAX_SEED_NAME_LEN   = 10,
  UI_MAX_WID_CONTEXT     = 2,
  UI_MAX_SAVE_SLOTS      = 10 // How many save game slots
};

//
// Popups
//
enum { UI_WID_POPUP_WIDTH_NORMAL = 18, UI_WID_POPUP_WIDTH_WIDE = 60 };

//
// Left and right bar sizes
//
#define UI_STAT_BAR_STEPS (UI_LEFTBAR_WIDTH - 2)
enum {
  UI_LEFTBAR_WIDTH  = 34, // In characters
  UI_RIGHTBAR_WIDTH = 34  // In characters
};

//
// Mouse scrolling
//
#define UI_GAME_MOUSE_WHEEL_SCALE 2.0 // How much the wheel mouse moves when in game
enum { UI_GAME_MOUSE_WHEEL_SCALE_MAX = 50 };
#define UI_WID_MOUSE_WHEEL_SCALE 1.0 // How much the wheel mouse moves when over a wid
enum {
  UI_WID_MOUSE_WHEEL_SCALE_MAX = 10,
  UI_MOUSE_DOUBLE_CLICK        = 500 // Double click time in ms
};

//
// Joystick scrolling
//
#define UI_SCROLL_JOY_SCALE     1.0
#define UI_SCROLL_JOY_SCALE_MAX 1.5

//
// This is for areas we've visited before. The higher the number, the darker the background is.
//
enum { UI_LIGHT_BACKGROUND = 3 };

//
// This is to allow the player light to fade rapidly, which allows other lighting like fire
// to appear stronger.
//
#define UI_LIGHT_PLAYER_FADE 1.3f

#define UI_CENTER_WIDTH (TERM_WIDTH - UI_RIGHTBAR_WIDTH - UI_LEFTBAR_WIDTH)

#define UI_TOPCON_TEXT_COLOR UI_TEXT_COLOR
enum { UI_TOPCON_HEIGHT = 4 };
#define UI_TOPCON_WIDTH UI_CENTER_WIDTH

#define UI_BOTCON_WIDTH UI_CENTER_WIDTH
enum {
  UI_BOTCON_HEIGHT = 1 // Scrollbar limit
};
#define UI_BOTCON_TEXT_COLOR UI_TEXT_COLOR

#define UI_INVENTORY_WIDTH  (UI_CENTER_WIDTH - 2)
#define UI_INVENTORY_HEIGHT (THING_INVENTORY_MAX + 9)

//
// Do faster processing of events, like reading the keyboard and updating widgets.
//
enum { UI_EVENT_LOOP_FREQ_FAST_MS = 5 };

//
// Less frequent updates like updating the FPS.
//
enum { UI_EVENT_LOOP_FREQ_SLOW_MS = 10 };

//
// This is for waiting for level fade in.
//
enum { UI_EVENT_LOOP_FREQ_VERY_SLOW_MS = 20 };

enum {
  UI_WID_STYLE_SPARSE_NONE = -1, // Used for widgets with their own backgrounds
  UI_WID_STYLE_SOLID_GRAY,
  UI_WID_STYLE_GREEN,
  UI_WID_STYLE_RED,
  UI_WID_STYLE_NORMAL,
  UI_WID_STYLE_CONSOLE,
  UI_WID_STYLE_UNUSED1,
  UI_WID_STYLE_UNUSED2,
  UI_WID_STYLE_UNUSED3,
  UI_WID_STYLE_VERT_DARK,
  UI_WID_STYLE_VERT_LIGHT,
  UI_WID_STYLE_HORIZ_DARK,
  UI_WID_STYLE_HORIZ_LIGHT,
  UI_WID_STYLE_VERT_SCROLL_DARK,
  UI_WID_STYLE_VERT_SCROLL_LIGHT,
  UI_WID_STYLE_HORIZ_SCROLL_DARK,
  UI_WID_STYLE_HORIZ_SCROLL_LIGHT,
  UI_WID_STYLE_MAX,
};

#endif
