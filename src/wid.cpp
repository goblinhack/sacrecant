//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_command.hpp"
#include "my_command_history.hpp"
#include "my_font.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_log.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_sdl_event.hpp"
#include "my_sdl_proto.hpp"
#include "my_size.hpp"
#include "my_sound.hpp"
#include "my_sprintf.hpp"
#include "my_thing_inlines.hpp"
#include "my_wid_class.hpp"
#include "my_wid_tiles.hpp"
#include "my_wids.hpp"

#define ENABLE_DEBUG_GFX_GL_BLEND
#ifdef ENABLE_DEBUG_GFX_GL_BLEND
#include <unistd.h>
#endif

#include <cmath>

#include <algorithm>
#include <print>
#include <ranges>
#include <utility>

//
// Unique key for each tree
//
static uint64_t wid_unique_key;

//
// Display sorted.
//
wid_key_map_location wid_top_level;

//
// Creation sorted and global.
//
static wid_key_map_int wid_global;

//
// Creation sorted and per wid parent.
//
static wid_key_map_int wid_top_level2;

//
// For moving things.
//
static wid_key_map_int wid_top_level3;

//
// For garbage collection.
//
static wid_key_map_int wid_top_level4;

//
// For tick things.
//
static wid_key_map_int wid_tick_top_level;
static wid_key_map_int wid_pre_tick_top_level;

//
// Last time we changed what we were over.
//
ts_t wid_last_over_event;

//
// Scope the focus to children of this widget and do not change it.
// Good for popups.
//
Widp wid_focus_locked {};
Widp wid_focus {};
Widp wid_over {};

static ts_t wid_time;

//
// Widget effects
//
const ts_t wid_destroy_delay_ms = 200;

//
// Prototypes.
//
static void wid_destroy_delay(Gamep g, Widp *wp, int delay);
static void wid_destroy_immediate(Gamep g, Widp w);
static void wid_destroy_immediate_internal(Gamep g, Widp w);
static void wid_update_internal(Gamep g, Widp w);
static void wid_tree2_unsorted_remove(Widp w);
static void wid_tree4_wids_being_destroyed_remove(Widp w);
static void wid_tree4_wids_being_destroyed_insert(Widp w);
static void wid_tree5_tick_wids_remove(Widp w);
static void wid_tree5_tick_wids_insert(Widp w);
static void wid_tree6_pre_tick_wids_remove(Widp w);
static void wid_tree6_pre_tick_wids_insert(Widp w);
static void wid_display(Gamep g, Widp w, uint8_t disable_scissor, uint8_t *updated_scissors, int clip);
static void wid_tree_remove(Widp w);
static void wid_tree_detach(Widp w);
static void wid_tree_insert(Widp w);
static void wid_tree_attach(Widp w);

static void wid_dbg(Widp w, const char *fmt, ...) CHECK_FORMAT_STR(printf, 2, 3);

//
// Child sort priority
//
static int wid_highest_priority = 1;
static int wid_lowest_priority  = -1;

//
// History for all text widgets.
//
#define HISTORY_MAX 16
std::array< std::string, HISTORY_MAX > history;
uint32_t                               g_history_at;
uint32_t                               g_history_walk;

std::array< std::array< Widp, TERM_HEIGHT_MAX >, TERM_WIDTH_MAX > wid_on_screen_at {};

bool wid_init_done;
bool wid_exiting;

Wid::Wid() { NEWPTR(MTYPE_WID, this, "wid"); }

Wid::~Wid() { OLDPTR(MTYPE_WID, this); }

//
// Check if we are on the main thread and not exitting
//
[[nodiscard]] static auto wid_safe() -> bool
{
  return wid_init_done && ! wid_exiting && ! g_dying && ! g_quitting && (g_thread_id == MAIN_THREAD);
}

auto wid_init() -> bool
{
  TRACE();

  wid_init_done = true;

  return true;
}

void wid_fini(Gamep g_maybe_null)
{
  TRACE();

  wid_init_done = false;
  wid_exiting   = true;

  wid_on_screen_at = {};

  wid_gc_all(g_maybe_null);

  for (;;) {
    if (wid_top_level.empty()) {
      break;
    }

    auto  iter  = wid_top_level.begin();
    auto *child = iter->second;
    wid_destroy_immediate(g_maybe_null, child);
  }

  wid_top_level          = {};
  wid_global             = {};
  wid_top_level2         = {};
  wid_top_level3         = {};
  wid_top_level4         = {};
  wid_tick_top_level     = {};
  wid_pre_tick_top_level = {};
  wid_last_over_event    = {};
  wid_focus_locked       = {};
  wid_focus              = {};
  wid_over               = {};
  wid_time               = {};
}

void wid_dump(Widp w, int depth)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  if (w == nullptr) [[unlikely]] {
    return;
  }

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;

  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

#if 0
  std::print("\n          {:>{}} dump: [{}] text [{}] {},{} to @{},{} {} children", "", depth * 2, wid_name(w),
             wid_get_text(w), tlx, tly, brx, bry, wid_count(w, depth));
#endif

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    wid_dump(child, depth + 2);
  }
}

auto wid_count(Widp w, int depth) -> int
{
  TRACE();

  if (! wid_safe()) {
    return 0;
  }

  if (w == nullptr) [[unlikely]] {
    return 0;
  }

  int count = 1;

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    count += wid_count(child, depth + 2);
  }

  return count;
}

auto wid_get_tl_x(Widp w) -> int
{
  TRACE();

  int const cx = (w->key.tl.x + w->key.br.x) / 2;

  return cx - (cx - w->key.tl.x);
}

auto wid_get_tl_y(Widp w) -> int
{
  TRACE();

  int const cy = (w->key.tl.y + w->key.br.y) / 2;

  return cy - (cy - w->key.tl.y);
}

auto wid_get_br_x(Widp w) -> int
{
  TRACE();

  int const cx = (w->key.tl.x + w->key.br.x) / 2;

  return cx + (w->key.br.x - cx);
}

auto wid_get_br_y(Widp w) -> int
{
  TRACE();

  int const cy = (w->key.tl.y + w->key.br.y) / 2;

  return cy + (w->key.br.y - cy);
}

void wid_get_tl_x_tl_y_br_x_br_y(Widp w, int *tl_x, int *tl_y, int *br_x, int *br_y)
{
  TRACE();

  if (w == nullptr) {
    *tl_x = 0;
    *tl_y = 0;
    *br_x = 0;
    *br_y = 0;
    ERR("no wid");
    return;
  }

  const int tlx = w->key.tl.x;
  const int tly = w->key.tl.y;
  const int brx = w->key.br.x;
  const int bry = w->key.br.y;

  const int cx = (tlx + brx) / 2;
  const int cy = (tly + bry) / 2;

  *tl_x = cx - (cx - tlx);
  *tl_y = cy - (cy - tly);
  *br_x = cx + (brx - cx);
  *br_y = cy + (bry - cy);
}

//
// Set the wid new co-ords. Returns true if there is a change.
//
void wid_set_pos(Widp w, spoint tl, spoint br)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  Widp p {};

  wid_tree_detach(w);

  w->key.tl = tl;
  w->key.br = br;

  //
  // Child postion is relative from the parent.
  //
  p = w->parent;
  if (p != nullptr) {
    w->key.tl.x += wid_get_tl_x(p);
    w->key.tl.y += wid_get_tl_y(p);
    w->key.br.x += wid_get_tl_x(p);
    w->key.br.y += wid_get_tl_y(p);

    w->key.tl.x -= p->offset.x;
    w->key.tl.y -= p->offset.y;
    w->key.br.x -= p->offset.x;
    w->key.br.y -= p->offset.y;
  }

  wid_tree_attach(w);
}

//
// Set the wid new co-ords. Returns true if there is a change.
//
static void wid_set_pos_pct(Widp w, fpoint tl, fpoint br)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  Widp p {};

  wid_tree_detach(w);

  if (w->parent == nullptr) {
    tl.x *= TERM_WIDTH;
    tl.y *= TERM_HEIGHT;
    br.x *= TERM_WIDTH;
    br.y *= TERM_HEIGHT;
  } else {
    tl.x *= wid_get_width(w->parent);
    tl.y *= wid_get_height(w->parent);
    br.x *= wid_get_width(w->parent);
    br.y *= wid_get_height(w->parent);
  }

  tl.x = std::max< float >(tl.x, 0);
  tl.y = std::max< float >(tl.y, 0);
  if (br.x >= wid_get_width(w->parent)) {
    br.x = wid_get_width(w->parent) - 1;
  }
  if (br.y >= wid_get_height(w->parent)) {
    br.y = wid_get_height(w->parent) - 1;
  }

  int key_tl_x = static_cast< int >(tl.x);
  int key_tl_y = static_cast< int >(tl.y);
  int key_br_x = static_cast< int >(br.x);
  int key_br_y = static_cast< int >(br.y);

  //
  // Child postion is relative from the parent.
  //
  p = w->parent;
  if (p != nullptr) {
    key_tl_x += wid_get_tl_x(p);
    key_tl_y += wid_get_tl_y(p);
    key_br_x += wid_get_tl_x(p);
    key_br_y += wid_get_tl_y(p);
  }

  w->key.tl.x = static_cast< int >(round(key_tl_x));
  w->key.tl.y = static_cast< int >(round(key_tl_y));
  w->key.br.x = static_cast< int >(round(key_br_x));
  w->key.br.y = static_cast< int >(round(key_br_y));

  wid_tree_attach(w);
}

void wid_set_string_context(Widp w, std::string string_context)
{
  TRACE();

  if (w == nullptr) {
    ERR("no widget pointer");
    return;
  }
  w->string_context = std::move(string_context);
}

auto wid_get_string_context(Widp w) -> std::string
{
  TRACE();

  if (w == nullptr) {
    ERR("no widget pointer");
    return "";
  }
  return w->string_context;
}

void wid_set_int_context(Widp w, int int_context)
{
  TRACE();

  if (w == nullptr) {
    ERR("no widget pointer");
    return;
  }
  w->int_context = int_context;
}

auto wid_get_int_context(Widp w) -> int
{
  TRACE();

  if (w == nullptr) {
    ERR("no widget pointer");
    return 0;
  }
  return w->int_context;
}

void wid_set_void_context(Widp w, void *void_context)
{
  TRACE();

  if (w == nullptr) {
    ERR("no widget pointer");
    return;
  }
  w->void_context = void_context;
}

auto wid_get_void_context(Widp w) -> void *
{
  TRACE();

  if (w == nullptr) {
    ERR("no widget pointer");
    return nullptr;
  }
  return w->void_context;
}

void wid_set_thing_context(Gamep g, Levelsp v, Widp w, Thingp t)
{
  TRACE();

  if (g == nullptr) {
    ERR("no game pointer");
    return;
  }

  if (v == nullptr) {
    ERR("no levels pointer");
    return;
  }

  if (w == nullptr) {
    ERR("no widget pointer");
    return;
  }

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  for (auto i = 0; i < UI_MAX_WID_CONTEXT; i++) {
    auto *cand = thing_find_optional(g, v, w->thing_id_context[ i ]);
    if (cand == t) {
      return;
    }
  }

  for (auto i = 0; i < UI_MAX_WID_CONTEXT; i++) {
    auto cand = w->thing_id_context[ i ];
    if (! static_cast< bool >(cand)) {
      w->thing_id_context[ i ] = t->id;
      return;
    }
  }
}

void wid_unset_thing_context(Gamep g, Levelsp v, Widp w, Thingp t)
{
  TRACE();

  if (g == nullptr) {
    ERR("no game pointer");
    return;
  }

  if (v == nullptr) {
    ERR("no levels pointer");
    return;
  }

  if (w == nullptr) {
    ERR("no widget pointer");
    return;
  }

  if (t == nullptr) {
    ERR("no thing pointer");
    return;
  }

  for (auto i = 0; i < UI_MAX_WID_CONTEXT; i++) {
    auto *cand = thing_find_optional(g, v, w->thing_id_context[ i ]);
    if (cand == t) {
      w->thing_id_context[ i ] = 0;
      return;
    }
  }
}

auto wid_get_thing_context(Gamep g, Levelsp v, Widp w, int which) -> Thingp
{
  TRACE();

  if (g == nullptr) {
    ERR("no game pointer");
    return nullptr;
  }

  if (v == nullptr) {
    ERR("no levels pointer");
    return nullptr;
  }

  if (w == nullptr) {
    ERR("no widget pointer");
    return nullptr;
  }

  if (which >= UI_MAX_WID_CONTEXT) {
    ERR("index overflow for UI_MAX_WID_CONTEXT");
    return nullptr;
  }

  return thing_find_optional(g, v, w->thing_id_context[ which ]);
}

void wid_set_prev(Widp w, Widp prev)
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    CROAK("no widget");
  }

  if (w == prev) {
    CROAK("widget list loop");
  }

  w->prev = prev;

  if (prev != nullptr) {
    prev->next = w;
  }
}

auto wid_get_prev(Widp w) -> Widp
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    CROAK("no widget");
  }

  if (w->prev == w) {
    CROAK("widget list get prev loop");
  }

  return w->prev;
}

auto wid_get_next(Widp w) -> Widp
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    CROAK("no widget");
  }

  if (w->next == w) {
    ERR("widget list get next loop");
  }

  return w->next;
}

auto wid_get_head(Widp w) -> Widp
{
  TRACE();

  Widp prev {};

  while (w != nullptr) {
    prev = wid_get_prev(w);
    if (prev == nullptr) {
      break;
    }

    w = prev;
  }

  return w;
}

auto wid_get_tail(Widp w) -> Widp
{
  TRACE();

  Widp next {};

  while (w != nullptr) {
    next = wid_get_next(w);
    if (next == nullptr) {
      break;
    }

    w = next;
  }

  return w;
}

auto wid_get_top_parent(Widp w) -> Widp
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    return w;
  }

  if (w->parent == nullptr) {
    return w;
  }

  while (w->parent != nullptr) {
    w = w->parent;
  }

  return w;
}

auto wid_get_parent(Widp w) -> Widp
{
  TRACE();

  return w->parent;
}

auto wid_get_scrollbar_vert(Widp w) -> Widp
{
  TRACE();
  return w->scrollbar_vert;
}

auto wid_get_scrollbar_horiz(Widp w) -> Widp
{
  TRACE();
  return w->scrollbar_horiz;
}

void wid_set_ignore_events(Widp w, bool val)
{
  TRACE();
  w->ignore_events = static_cast< uint8_t >(val);
}

void wid_set_ignore_scroll_events(Widp w, bool val)
{
  TRACE();
  w->ignore_scroll_events = static_cast< uint8_t >(val);
}

static void wid_set_scissors(int tlx, int tly, int brx, int bry)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  ascii_set_scissors(spoint(tlx, tly), spoint(brx, bry));
}

//
// Should this widget be ignored for events?
//
auto wid_ignore_events(Widp w) -> bool
{
  TRACE();

  if (! wid_safe()) {
    return true;
  }

  Widp top {};

  if (w == nullptr) [[unlikely]] {
    return true;
  }

  if ((static_cast< bool >(w->ignore_events)) || (static_cast< bool >(w->hidden)) || (static_cast< bool >(w->being_destroyed))) {
    return true;
  }

  if (w->parent != nullptr) {
    top = wid_get_top_parent(w);

    if ((static_cast< bool >(top->hidden)) || (static_cast< bool >(top->being_destroyed))) {
      return true;
    }
  }

  return false;
}

auto wid_ignore_events_only(Widp w) -> bool
{
  TRACE();

  if (! wid_safe()) {
    return true;
  }

  if (w == nullptr) [[unlikely]] {
    return true;
  }

  if (static_cast< bool >(w->ignore_events)) {
    return true;
  }

  return false;
}

auto wid_ignore_scroll_events(Widp w) -> bool
{
  TRACE();

  if (! wid_safe()) {
    return true;
  }

  Widp top {};

  if (w == nullptr) [[unlikely]] {
    return true;
  }

  if ((static_cast< bool >(w->ignore_scroll_events)) || (static_cast< bool >(w->hidden)) || (static_cast< bool >(w->being_destroyed))) {
    return true;
  }

  if (w->parent != nullptr) {
    top = wid_get_top_parent(w);

    if ((static_cast< bool >(top->hidden)) || (static_cast< bool >(top->being_destroyed))) {
      return true;
    }
  }

  return false;
}

auto wid_ignore_for_focus(Widp w) -> bool
{
  TRACE();

  if (! wid_safe()) {
    return true;
  }

  Widp top {};

  if ((static_cast< bool >(w->hidden)) || (static_cast< bool >(w->being_destroyed))) {
    return true;
  }

  if (w->parent != nullptr) {
    top = wid_get_top_parent(w);

    if ((static_cast< bool >(top->hidden)) || (static_cast< bool >(top->being_destroyed))) {
      return true;
    }
  }

  return false;
}

//
// Should this widget be ignored for events?
//
auto wid_ignore_being_destroyed(Widp w) -> bool
{
  TRACE();

  if (! wid_safe()) {
    return true;
  }

  Widp top {};

  if (static_cast< bool >(w->being_destroyed)) {
    return true;
  }

  if (w->parent != nullptr) {
    top = wid_get_top_parent(w);

    if (static_cast< bool >(top->being_destroyed)) {
      return true;
    }
  }

  return false;
}

auto wid_get_current_focus() -> Widp
{
  TRACE();

  if (! wid_safe()) {
    return nullptr;
  }

  return wid_focus;
}

//
// Map an SDL key event to the char the user typed
//
auto wid_event_to_char(const struct SDL_Keysym *evt) -> char
{
  TRACE();

  if (((evt->mod & KMOD_LSHIFT) != 0) || ((evt->mod & KMOD_RSHIFT) != 0)) {
    switch (evt->sym) {
      case SDLK_a :            return 'A';
      case SDLK_b :            return 'B';
      case SDLK_c :            return 'C';
      case SDLK_d :            return 'D';
      case SDLK_e :            return 'E';
      case SDLK_f :            return 'F';
      case SDLK_g :            return 'G';
      case SDLK_h :            return 'H';
      case SDLK_i :            return 'I';
      case SDLK_j :            return 'J';
      case SDLK_k :            return 'K';
      case SDLK_l :            return 'L';
      case SDLK_m :            return 'M';
      case SDLK_n :            return 'N';
      case SDLK_o :            return 'O';
      case SDLK_p :            return 'P';
      case SDLK_q :            return 'Q';
      case SDLK_r :            return 'R';
      case SDLK_s :            return 'S';
      case SDLK_t :            return 'T';
      case SDLK_u :            return 'U';
      case SDLK_v :            return 'V';
      case SDLK_w :            return 'W';
      case SDLK_x :            return 'X';
      case SDLK_y :            return 'Y';
      case SDLK_z :            return 'Z';
      case SDLK_QUOTE :        return '"';
      case SDLK_COMMA :        return '<';
      case SDLK_MINUS :        return '_';
      case SDLK_PERIOD :       return '>';
      case SDLK_SLASH :        return '?';
      case SDLK_EQUALS :       return '+';
      case SDLK_0 :            return ')';
      case SDLK_1 :            return '!';
      case SDLK_2 :            return '@';
      case SDLK_3 :            return '#';
      case SDLK_4 :            return '$';
      case SDLK_5 :            return '%';
      case SDLK_6 :            return '^';
      case SDLK_7 :            return '&';
      case SDLK_8 :            return '*';
      case SDLK_9 :            return '(';
      case SDLK_SEMICOLON :    return ':';
      case SDLK_LEFTBRACKET :  return '{';
      case SDLK_BACKSLASH :    return '|';
      case SDLK_RIGHTBRACKET : return '}';
      case SDLK_HASH :         return '~';
    }
  }

  switch (evt->sym) {
    case SDLK_BACKSPACE :    return '';
    case SDLK_TAB :          return '\t';
    case SDLK_RETURN :       return '\n';
    case SDLK_ESCAPE :       return '';
    case SDLK_SPACE :        return ' ';
    case SDLK_EXCLAIM :      return '!';
    case SDLK_QUOTEDBL :     return '"';
    case SDLK_HASH :         return '#';
    case SDLK_DOLLAR :       return '$';
    case SDLK_AMPERSAND :    return '%';
    case SDLK_QUOTE :        return '\'';
    case SDLK_LEFTPAREN :    return '(';
    case SDLK_RIGHTPAREN :   return ')';
    case SDLK_ASTERISK :     return '*';
    case SDLK_PLUS :         return '+';
    case SDLK_COMMA :        return ',';
    case SDLK_MINUS :        return '-';
    case SDLK_PERIOD :       return '.';
    case SDLK_SLASH :        return '/';
    case SDLK_0 :            return '0';
    case SDLK_1 :            return '1';
    case SDLK_2 :            return '2';
    case SDLK_3 :            return '3';
    case SDLK_4 :            return '4';
    case SDLK_5 :            return '5';
    case SDLK_6 :            return '6';
    case SDLK_7 :            return '7';
    case SDLK_8 :            return '8';
    case SDLK_9 :            return '9';
    case SDLK_COLON :        return ':';
    case SDLK_SEMICOLON :    return ';';
    case SDLK_LESS :         return '<';
    case SDLK_EQUALS :       return '=';
    case SDLK_GREATER :      return '>';
    case SDLK_QUESTION :     return '?';
    case SDLK_AT :           return '@';
    case SDLK_LEFTBRACKET :  return '[';
    case SDLK_BACKSLASH :    return '\\';
    case SDLK_RIGHTBRACKET : return ']';
    case SDLK_CARET :        return '^';
    case SDLK_UNDERSCORE :   return '_';
    case SDLK_BACKQUOTE :    return '`';
    case SDLK_a :            return 'a';
    case SDLK_b :            return 'b';
    case SDLK_c :            return 'c';
    case SDLK_d :            return 'd';
    case SDLK_e :            return 'e';
    case SDLK_f :            return 'f';
    case SDLK_g :            return 'g';
    case SDLK_h :            return 'h';
    case SDLK_i :            return 'i';
    case SDLK_j :            return 'j';
    case SDLK_k :            return 'k';
    case SDLK_l :            return 'l';
    case SDLK_m :            return 'm';
    case SDLK_n :            return 'n';
    case SDLK_o :            return 'o';
    case SDLK_p :            return 'p';
    case SDLK_q :            return 'q';
    case SDLK_r :            return 'r';
    case SDLK_s :            return 's';
    case SDLK_t :            return 't';
    case SDLK_u :            return 'u';
    case SDLK_v :            return 'v';
    case SDLK_w :            return 'w';
    case SDLK_x :            return 'x';
    case SDLK_y :            return 'y';
    case SDLK_z :            return 'z';
    case SDLK_DELETE :       return '';
#if SDL_MAJOR_VERSION == 1 // {
    case SDLK_KP0 : return '0';
    case SDLK_KP1 : return '1';
    case SDLK_KP2 : return '2';
    case SDLK_KP3 : return '3';
    case SDLK_KP4 : return '4';
    case SDLK_KP5 : return '5';
    case SDLK_KP6 : return '6';
    case SDLK_KP7 : return '7';
    case SDLK_KP8 : return '8';
    case SDLK_KP9 : return '9';
#else
    case SDLK_KP_0 : return '0';
    case SDLK_KP_1 : return '1';
    case SDLK_KP_2 : return '2';
    case SDLK_KP_3 : return '3';
    case SDLK_KP_4 : return '4';
    case SDLK_KP_5 : return '5';
    case SDLK_KP_6 : return '6';
    case SDLK_KP_7 : return '7';
    case SDLK_KP_8 : return '8';
    case SDLK_KP_9 : return '9';
#endif // }
    case SDLK_KP_PERIOD :   return '.';
    case SDLK_KP_DIVIDE :   return '/';
    case SDLK_KP_MULTIPLY : return '*';
    case SDLK_KP_MINUS :    return '-';
    case SDLK_KP_PLUS :     return '+';
    case SDLK_KP_ENTER :    return '\0';
    case SDLK_KP_EQUALS :   return '=';
    case SDLK_UP :          return '\0';
    case SDLK_DOWN :        return '\0';
    case SDLK_RIGHT :       return '\0';
    case SDLK_LEFT :        return '\0';
    case SDLK_INSERT :      return '\0';
    case SDLK_HOME :        return '\0';
    case SDLK_END :         return '\0';
    case SDLK_PAGEUP :      return '\0';
    case SDLK_PAGEDOWN :    return '\0';
    case SDLK_F1 :          return '\0';
    case SDLK_F2 :          return '\0';
    case SDLK_F3 :          return '\0';
    case SDLK_F4 :          return '\0';
    case SDLK_F5 :          return '\0';
    case SDLK_F6 :          return '\0';
    case SDLK_F7 :          return '\0';
    case SDLK_F8 :          return '\0';
    case SDLK_F9 :          return '\0';
    case SDLK_F10 :         return '\0';
    case SDLK_F11 :         return '\0';
    case SDLK_F12 :         return '\0';
    case SDLK_F13 :         return '\0';
    case SDLK_F14 :         return '\0';
    case SDLK_F15 :         return '\0';
    case SDLK_CAPSLOCK :    return '\0';
    case SDLK_RSHIFT :      return '\0';
    case SDLK_LSHIFT :      return '\0';
    case SDLK_RCTRL :       return '\0';
    case SDLK_LCTRL :       return '\0';
    case SDLK_RALT :        return '\0';
    case SDLK_LALT :        return '\0';
    case SDLK_MODE :        return '\0';
    case SDLK_HELP :        return '!';
    case SDLK_SYSREQ :      return '\0';
    case SDLK_MENU :        return '\0';
    case SDLK_POWER :       return '\0';
    case SDLK_UNDO :        return '\0';
    default :               return '\0';
  }
  return '\0';
}

//
// Widget mode, whether it is active, inactive etc...
//
void wid_set_mode(Widp w, wid_mode mode)
{
  TRACE();

  w->ts_last_mode_change = wid_time;
  w->mode                = mode;
}

//
// Widget mode, whether it is active, inactive etc...
//
auto wid_get_mode(Widp w) -> wid_mode
{
  TRACE();

  //
  // Allow focus to override less important modes.
  //
  if (w == wid_focus) {
    if ((w->mode == WID_MODE_NORMAL) || (w->mode == WID_MODE_OVER)) {
      return WID_MODE_FOCUS;
    }
  }

  return w->mode;
}

auto to_string(Widp w) -> std::string
{
  TRACE();
  return w->to_string;
}

auto wid_name(Widp w) -> std::string
{
  TRACE();
  return w->name;
}

auto wid_get_text(Widp w) -> std::string
{
  TRACE();
  return w->text;
}

auto wid_get_name(Widp w) -> std::string
{
  TRACE();
  return w->name;
}

static auto wid_get_text_with_cursor(Widp w) -> std::string
{
  TRACE();

  if (! static_cast< bool >(w->received_input)) {
    w->cursor = static_cast< uint32_t >(w->text.length());
  }

  std::string const t = w->text;
  std::string o       = t.substr(0, w->cursor) + static_cast< char > FONT_CHAR_CURSOR + t.substr(w->cursor);

  return o;
}

void wid_set_name(Widp w, const std::string &name)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  if (! name.empty()) {
    OLDPTR(MTYPE_WID, w);
    w->name = name;
    NEWPTR(MTYPE_WID, w, "wid");
  } else {
    w->name = name;
  }

#ifdef ENABLE_DEBUG_UI
#ifdef ENABLE_DEBUG_UI2
  if (w->parent) {
    w->to_string = string_sprintf("%s[%p] (parent %s[%p])", name.c_str(), w, w->parent->to_string.c_str(), w->parent);
  } else {
    w->to_string = string_sprintf("%s[%p]", name.c_str(), w);
  }
#else
  w->to_string = string_sprintf("%s[%p]", name.c_str(), w);
#endif
#endif
}

void wid_set_ignore_for_mouse_down(Widp w, bool val)
{
  TRACE();
  w->ignore_for_mouse_down = static_cast< uint8_t >(val);
}

void wid_set_debug(Widp w, bool val)
{
  TRACE();
  w->debug = static_cast< uint8_t >(val);
}

void wid_set_text_max_len(Widp w, size_t max_len)
{
  TRACE();
  w->max_len = max_len;
}

void wid_set_text(Widp w, const std::string &text)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  if (text.empty()) {
    w->text = "";
  } else {
    if (text == w->text) {
      return;
    }

    w->text = text;
  }

  if (static_cast< bool >(w->max_len)) {
    if (w->text.size() > w->max_len) {
      w->text.resize(w->max_len);
    }
  }

  w->cursor = std::min(w->cursor, static_cast< uint16_t >(w->text.size()));
}

void wid_set_text(Widp w, int v)
{
  TRACE();
  wid_set_text(w, std::to_string(v));
}

auto wid_get_received_input(Widp w) -> bool
{
  TRACE();
  return static_cast< bool >(w->received_input);
}

void wid_set_received_input(Widp w, bool val)
{
  TRACE();
  w->received_input = static_cast< uint8_t >(val);
}

void wid_set_cursor(Widp w, uint32_t val)
{
  TRACE();
  w->cursor = val;
}

auto wid_get_width(Widp w) -> int
{
  TRACE();
  return (wid_get_br_x(w) - wid_get_tl_x(w)) + 1;
}

auto wid_get_height(Widp w) -> int
{
  TRACE();
  return (wid_get_br_y(w) - wid_get_tl_y(w)) + 1;
}

auto wid_get_focusable(Widp w) -> bool
{
  TRACE();
  return static_cast< bool >(w->focus_order);
}

void wid_set_focusable(Widp w, bool val)
{
  TRACE();
  w->focus_order = static_cast< uint8_t >(val);
}

auto wid_get_show_cursor(Widp w) -> bool
{
  TRACE();
  return static_cast< bool >(w->show_cursor);
}

void wid_set_show_cursor(Widp w, bool val)
{
  TRACE();
  w->show_cursor = static_cast< uint8_t >(val);
}

auto wid_get_do_not_raise(Widp w) -> bool
{
  TRACE();
  return static_cast< bool >(w->do_not_raise);
}

void wid_set_do_not_raise(Widp w, bool val)
{
  TRACE();
  w->do_not_raise = static_cast< uint8_t >(val);
}

auto wid_get_do_not_lower(Widp w) -> bool
{
  TRACE();
  return static_cast< bool >(w->do_not_lower);
}

void wid_set_do_not_lower(Widp w, bool val)
{
  TRACE();
  w->do_not_lower = static_cast< uint8_t >(val);
}

auto wid_get_moveable(Widp w) -> bool
{
  TRACE();

  if (static_cast< bool >(w->moveable_set)) {
    return static_cast< bool >(w->moveable);
  }

  return false;
}

void wid_set_moveable(Gamep g, Widp w, bool val)
{
  TRACE();

  if (g == nullptr) {
    return;
  }

  if (! wid_safe()) {
    return;
  }

  w->moveable     = static_cast< uint8_t >(val);
  w->moveable_set = 1U;

  wid_set_moveable_horiz(g, w, val);
  wid_set_moveable_vert(g, w, val);
}

auto wid_get_moveable_horiz(Widp w) -> bool
{
  TRACE();

  if (static_cast< bool >(w->moveable_horiz_set)) {
    return static_cast< bool >(w->moveable_horiz);
  }

  return false;
}

void wid_set_moveable_horiz(Gamep g, Widp w, bool val)
{
  TRACE();

  if (g == nullptr) {
    return;
  }

  w->moveable_horiz     = static_cast< uint8_t >(val);
  w->moveable_horiz_set = 1U;
}

auto wid_get_moveable_vert(Widp w) -> bool
{
  TRACE();

  if (static_cast< bool >(w->moveable_vert_set)) {
    return static_cast< bool >(w->moveable_vert);
  }

  return false;
}

void wid_set_moveable_vert(Gamep g, Widp w, bool val)
{
  TRACE();

  if (g == nullptr) {
    return;
  }

  w->moveable_vert     = static_cast< uint8_t >(val);
  w->moveable_vert_set = 1U;
}

auto wid_get_moveable_bounded(Widp w) -> bool
{
  TRACE();

  if (static_cast< bool >(w->moveable_bounded_set)) {
    return static_cast< bool >(w->moveable_bounded);
  }

  return false;
}

void wid_set_moveable_bounded(Gamep g, Widp w, bool val)
{
  TRACE();

  if (g == nullptr) {
    return;
  }

  w->moveable_bounded     = static_cast< uint8_t >(val);
  w->moveable_bounded_set = 1U;
}

auto wid_get_moveable_no_user_scroll(Widp w) -> bool
{
  TRACE();

  if (static_cast< bool >(w->moveable_no_user_scroll_set)) {
    return static_cast< bool >(w->moveable_no_user_scroll);
  }

  return false;
}

void wid_set_moveable_no_user_scroll(Gamep g, Widp w, bool val)
{
  TRACE();

  if (g == nullptr) {
    return;
  }

  w->moveable_no_user_scroll     = static_cast< uint8_t >(val);
  w->moveable_no_user_scroll_set = 1U;
}

auto wid_get_text_lhs(Widp w) -> bool
{
  TRACE();
  return static_cast< bool >(w->text_lhs);
}

void wid_set_text_lhs(Widp w, bool val)
{
  TRACE();
  w->text_lhs = static_cast< uint8_t >(val);
}

auto wid_get_text_rhs(Widp w) -> bool
{
  TRACE();
  return static_cast< bool >(w->text_rhs);
}

void wid_set_text_rhs(Widp w, bool val)
{
  TRACE();
  w->text_rhs = 1U;
}

auto wid_get_text_centerx(Widp w) -> bool
{
  TRACE();
  return static_cast< bool >(w->text_centerx);
}

void wid_set_text_centerx(Widp w, bool val)
{
  TRACE();
  w->text_centerx = static_cast< uint8_t >(val);
}

auto wid_get_text_top(Widp w) -> bool
{
  TRACE();
  return static_cast< bool >(w->text_top);
}

void wid_set_text_top(Widp w, bool val)
{
  TRACE();
  w->text_top = static_cast< uint8_t >(val);
}

auto wid_get_text_bot(Widp w) -> bool
{
  TRACE();
  return static_cast< bool >(w->text_bot);
}

void wid_set_text_bot(Widp w, bool val)
{
  TRACE();
  w->text_bot = static_cast< uint8_t >(val);
}

auto wid_get_text_centery(Widp w) -> bool
{
  TRACE();
  return static_cast< bool >(w->text_centery);
}

void wid_set_text_centery(Widp w, bool val)
{
  TRACE();
  w->text_centery = static_cast< uint8_t >(val);
}

auto wid_get_text_pos(Widp w, int *x, int *y) -> bool
{
  TRACE();

  if (static_cast< bool >(w->text_pos_set)) {
    *x = w->text_pos.x;
    *y = w->text_pos.y;

    return true;
  }

  return false;
}

void wid_set_tile(int depth, Widp w, Tilep tile)
{
  TRACE();
  w->tiles[ depth ] = tile;
}

void wid_set_tilename(int depth, Widp w, const std::string &name)
{
  TRACE();

  if (g_opt_tests) {
    return;
  }

  Tilep tile = tile_find(name);
  if (tile == nullptr) [[unlikely]] {
    ERR("failed to find wid tile [%s]", name.c_str());
  }

  if (w == nullptr) [[unlikely]] {
    CROAK("widget does not exist to set tile %s", name.c_str());
  }

  w->tiles[ depth ] = tile;
}

//
// Look at all the wid modes and return the most relevent setting
//
auto wid_get_color(Widp w, wid_color which) -> color
{
  TRACE();

  auto            mode = wid_get_mode(w); // for c++, no enum walk
  WidOptionsMenu *cfg  = &w->cfg[ mode ];

  if (static_cast< bool >(cfg->color_set[ which ])) {
    return cfg->colors[ which ];
  }

  if ((wid_focus == w) && (wid_over == w)) {
    mode = WID_MODE_OVER;
    cfg  = &w->cfg[ mode ];
    if (static_cast< bool >(cfg->color_set[ which ])) {
      return cfg->colors[ which ];
    }
  }

  mode = WID_MODE_NORMAL;
  cfg  = &w->cfg[ mode ];
  if (static_cast< bool >(cfg->color_set[ which ])) {
    return cfg->colors[ which ];
  }

  return WHITE;
}

auto wid_get_style(Widp w) -> int
{
  TRACE();

  auto                  mode = wid_get_mode(w); // for c++, no enum walk
  WidOptionsMenu const *cfg  = &w->cfg[ mode ];

  if (static_cast< bool >(cfg->style_set)) {
    return cfg->style;
  }

  if ((wid_focus == w) && (wid_over == w)) {
    mode = WID_MODE_OVER;
    cfg  = &w->cfg[ mode ];
    if (static_cast< bool >(cfg->style_set)) {
      return cfg->style;
    }
  }

  mode = WID_MODE_NORMAL;
  cfg  = &w->cfg[ mode ];
  if (static_cast< bool >(cfg->style_set)) {
    return cfg->style;
  }

  return UI_WID_STYLE_NORMAL;
}

void wid_set_style(Widp w, int style)
{
  w->cfg[ wid_get_mode(w) ].style     = style;
  w->cfg[ wid_get_mode(w) ].style_set = 1U;
}

//
// Look at all the widset modes and return the most relevent setting
//
void wid_set_color(Widp w, wid_color col, color val)
{
  TRACE();

  w->cfg[ wid_get_mode(w) ].colors[ col ]    = val;
  w->cfg[ wid_get_mode(w) ].color_set[ col ] = 1U;
}

void wid_set_focus(Gamep g, Widp w)
{
  TRACE();

  if (g == nullptr) {
    return;
  }

  //
  // Don't allow focus override if hard focus is set.
  //
  if (w != nullptr) {
    if (wid_focus_locked != nullptr) {
      if (wid_get_top_parent(w) != wid_focus_locked) {
        return;
      }
    }

    if (! static_cast< bool >(w->focus_order)) {
      return;
    }
  }

  wid_mouse_focus_begin(g, w);
}

void wid_unset_focus(Gamep g)
{
  TRACE();
  wid_mouse_focus_end(g);
}

void wid_set_shape_square(Widp w)
{
  TRACE();
  w->square = 1U;
}

void wid_set_shape_none(Widp w)
{
  TRACE();
  w->square = 0U;
}

void wid_set_active(Gamep g, Widp w)
{
  TRACE();

  if (g == nullptr) {
    return;
  }

  wid_set_mode(w, WID_MODE_ACTIVE);
}

void wid_focus_lock(Gamep g, Widp w)
{
  TRACE();

  if (g == nullptr) {
    return;
  }

  if (w != nullptr) {
    wid_dbg(w, "focus locked");
  }

  wid_focus_locked = w;
}

void wid_unset_focus_lock()
{
  TRACE();
  wid_focus_locked = nullptr;
}

void wid_set_on_key_down(Widp w, on_key_down_t fn)
{
  TRACE();
  w->on_key_down = fn;
}

void wid_set_on_key_up(Widp w, on_key_up_t fn)
{
  TRACE();
  w->on_key_up = fn;
}

void wid_set_on_joy_button(Widp w, on_joy_button_t fn)
{
  TRACE();
  w->on_joy_button = fn;
}

void wid_set_on_destroy(Widp w, on_destroy_t fn)
{
  TRACE();
  w->on_destroy = fn;
}

void wid_set_on_destroy_begin(Widp w, on_destroy_t fn)
{
  TRACE();
  w->on_destroy_begin = fn;
}

void wid_set_on_display(Widp w, on_display_t fn)
{
  TRACE();
  w->on_display = fn;
}

void wid_set_on_tick(Widp w, on_tick_t fn)
{
  TRACE();
  w->on_tick = fn;
  wid_tree5_tick_wids_insert(w);
}

void wid_set_on_pre_tick(Widp w, on_pre_tick_t fn)
{
  TRACE();
  w->on_pre_tick = fn;
  wid_tree6_pre_tick_wids_insert(w);
}

//
// Remove this wid from any trees it is in.
//
static void wid_tree_detach(Widp w)
{
  TRACE();
  wid_tree_remove(w);
}

//
// Add back to all trees.
//
static void wid_tree_attach(Widp w)
{
  TRACE();

  wid_key_map_location *root = nullptr;

  if (w->in_tree_root != nullptr) {
    CROAK("widget is already attached");
  }

  if (w->parent == nullptr) {
    root = &wid_top_level;
  } else {
    root = &w->parent->children_display_sorted;
  }

  if (root == nullptr) {
    CROAK("no root");
  }

  auto result = root->insert(std::make_pair(w->key, w));
  if (! result.second) {
    CROAK("widget insert name [%s] failed", wid_get_name(w).c_str());
  }

  w->in_tree_root = root;
}

static void wid_tree_insert(Widp w)
{
  TRACE();

  if (w->in_tree_root != nullptr) {
    CROAK("widget is already inserted");
  }

  wid_key_map_location *root = nullptr;

  //
  // Get a wid sort ID.
  //
  w->key.key = ++wid_unique_key;

  if (w->parent == nullptr) {
    root = &wid_top_level;
  } else {
    root = &w->parent->children_display_sorted;
  }

  if (root == nullptr) {
    CROAK("no root");
  }

  auto result = root->insert(std::make_pair(w->key, w));
  if (! result.second) {
    CROAK("widget insert name [%s] failed", wid_get_name(w).c_str());
  }

  w->in_tree_root = root;
}

static void wid_tree_global_unsorted_insert(Widp w)
{
  TRACE();

  static WidKeyType key;

  if (w->in_tree_global_unsorted_root != nullptr) {
    CROAK("widget is already in the global tree");
  }

  auto *root = &wid_global;

  key.val++;

  w->tree_global_key = key;
  auto result        = root->insert(std::make_pair(w->tree_global_key, w));
  if (! result.second) {
    CROAK("widget insert name [%s] tree_global failed", wid_get_name(w).c_str());
  }

  w->in_tree_global_unsorted_root = root;
}

static void wid_tree2_unsorted_insert(Widp w)
{
  TRACE();

  if (w->in_tree2_unsorted_root != nullptr) {
    CROAK("widget is already in the in_tree2_unsorted_root");
  }

  wid_key_map_int *root = nullptr;

  if (w->parent == nullptr) {
    root = &wid_top_level2;
  } else {
    root = &w->parent->tree2_children_unsorted;
  }

  w->tree2_key.val = ++wid_unique_key;
  auto result      = root->insert(std::make_pair(w->tree2_key, w));
  if (! result.second) {
    CROAK("widget insert name [%s] tree2 failed", wid_get_name(w).c_str());
  }

  w->in_tree2_unsorted_root = root;
}

static void wid_tree4_wids_being_destroyed_insert(Widp w)
{
  TRACE();

  if (w->in_tree4_wids_being_destroyed != nullptr) {
    return;
  }

  if (wid_exiting) {
    return;
  }

  wid_key_map_int *root = nullptr;

  root = &wid_top_level4;

  w->tree4_key.val = ++wid_unique_key;
  auto result      = root->insert(std::make_pair(w->tree4_key, w));
  if (! result.second) {
    CROAK("widget insert name [%s] tree4 failed", wid_get_name(w).c_str());
  }

  w->in_tree4_wids_being_destroyed = root;
}

static void wid_tree5_tick_wids_insert(Widp w)
{
  TRACE();

  if (w->in_tree5_tick_wids != nullptr) {
    return;
  }

  if (wid_exiting) {
    return;
  }

  wid_key_map_int *root = nullptr;

  root = &wid_tick_top_level;

  w->tree5_key.val = ++wid_unique_key;
  auto result      = root->insert(std::make_pair(w->tree5_key, w));
  if (! result.second) {
    CROAK("widget insert name [%s] tree5 failed", wid_get_name(w).c_str());
  }

  w->in_tree5_tick_wids = root;
}

static void wid_tree6_pre_tick_wids_insert(Widp w)
{
  TRACE();

  if (w->in_tree6_pre_tick_wids != nullptr) {
    return;
  }

  if (wid_exiting) {
    return;
  }

  wid_key_map_int *root = nullptr;

  root = &wid_pre_tick_top_level;

  w->tree6_key.val = ++wid_unique_key;
  auto result      = root->insert(std::make_pair(w->tree6_key, w));
  if (! result.second) {
    CROAK("widget insert name [%s] tree6 failed", wid_get_name(w).c_str());
  }

  w->in_tree6_pre_tick_wids = root;
}

static void wid_tree_remove(Widp w)
{
  TRACE();

  wid_key_map_location *root = nullptr;

  root = w->in_tree_root;
  if (root == nullptr) {
    return;
  }

  auto result = root->find(w->key);
  if (result == root->end()) {
    CROAK("widget tree did not find wid %s hence cannot remove it", wid_get_name(w).c_str());
  }

  root->erase(w->key);

  w->in_tree_root = nullptr;
}

static void wid_tree2_unsorted_remove(Widp w)
{
  TRACE();

  auto *root = w->in_tree2_unsorted_root;
  if (root == nullptr) {
    return;
  }

  auto result = root->find(w->tree2_key);
  if (result == root->end()) {
    CROAK("widget tree2 did not find wid");
  }
  root->erase(w->tree2_key);

  w->in_tree2_unsorted_root = nullptr;
}

static void wid_tree_global_unsorted_remove(Widp w)
{
  TRACE();

  auto *root = w->in_tree_global_unsorted_root;
  if (root == nullptr) {
    return;
  }

  auto result = root->find(w->tree_global_key);
  if (result == root->end()) {
    CROAK("widget tree_global did not find wid");
  }
  root->erase(w->tree_global_key);

  w->in_tree_global_unsorted_root = nullptr;
}

static void wid_tree4_wids_being_destroyed_remove(Widp w)
{
  TRACE();

  auto *root = w->in_tree4_wids_being_destroyed;
  if (root == nullptr) {
    return;
  }

  auto result = root->find(w->tree4_key);
  if (result == root->end()) {
    CROAK("widget tree4 did not find wid");
  }
  root->erase(w->tree4_key);

  w->in_tree4_wids_being_destroyed = nullptr;
}

static void wid_tree5_tick_wids_remove(Widp w)
{
  TRACE();

  auto *root = w->in_tree5_tick_wids;
  if (root == nullptr) {
    return;
  }

  auto result = root->find(w->tree5_key);
  if (result == root->end()) {
    CROAK("widget tree5 did not find wid");
  }
  root->erase(w->tree5_key);

  w->in_tree5_tick_wids = nullptr;
  w->on_tick            = nullptr;
}

static void wid_tree6_pre_tick_wids_remove(Widp w)
{
  TRACE();

  auto *root = w->in_tree6_pre_tick_wids;
  if (root == nullptr) {
    return;
  }

  auto result = root->find(w->tree6_key);
  if (result == root->end()) {
    CROAK("widget tree6 did not find wid");
  }
  root->erase(w->tree6_key);

  w->in_tree6_pre_tick_wids = nullptr;
  w->on_pre_tick            = nullptr;
}

//
// Initialize a wid with basic settings
//
static auto wid_new(Widp parent) -> Widp
{
  TRACE();

  auto *w = new Wid();

  w->parent     = parent;
  w->ts_created = wid_time;

  wid_tree_insert(w);
  wid_tree2_unsorted_insert(w);
  wid_tree_global_unsorted_insert(w);

  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_color(w, WID_COLOR_BG, WHITE);
  wid_set_color(w, WID_COLOR_TEXT_FG, GRAY90);

  wid_set_mode(w, WID_MODE_OVER);
  wid_set_color(w, WID_COLOR_BG, WHITE);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);

  wid_set_mode(w, WID_MODE_NORMAL);

  w->visible = 1U;
  wid_set_style(w, UI_WID_STYLE_NORMAL);
  wid_dbg(w, "new");

  return w;
}

static auto wid_new() -> Widp
{
  TRACE();

  auto *w = new Wid();

  w->ts_created = wid_time;

  wid_tree_insert(w);
  wid_tree2_unsorted_insert(w);
  wid_tree_global_unsorted_insert(w);

  w->visible = 1U;
  wid_set_style(w, UI_WID_STYLE_NORMAL);
  wid_dbg(w, "new");

  return w;
}

static void wid_destroy_immediate_internal(Gamep g, Widp w)
{
  TRACE();

  wid_tree4_wids_being_destroyed_remove(w);
  wid_tree5_tick_wids_remove(w);
  wid_tree6_pre_tick_wids_remove(w);

  if (w->on_destroy != nullptr) {
    (w->on_destroy)(g, w);
  }

  if (wid_focus == w) {
    wid_mouse_focus_end(g);
  }

  if (wid_focus_locked == w) {
    wid_focus_locked = nullptr;
  }

  //
  // This can easily lead to recursion. Do we need it?
  //
  // if (wid_over == w) {
  //     wid_mouse_over_end();
  // }

  if (w->scrollbar_vert != nullptr) {
    w->scrollbar_vert->scrollbar_owner = nullptr;
  }

  if (w->scrollbar_horiz != nullptr) {
    w->scrollbar_horiz->scrollbar_owner = nullptr;
  }

  if (w->scrollbar_owner != nullptr) {
    if (w->scrollbar_owner->scrollbar_vert == w) {
      w->scrollbar_owner->scrollbar_vert = nullptr;
    }

    if (w->scrollbar_owner->scrollbar_horiz == w) {
      w->scrollbar_owner->scrollbar_horiz = nullptr;
    }
  }

  {
    auto iter = w->children_display_sorted.begin();

    while (iter != w->children_display_sorted.end()) {
      auto *child = iter->second;
      wid_destroy_immediate(g, child);
      iter = w->children_display_sorted.begin();
    }
  }

  {
    auto iter = w->tree2_children_unsorted.begin();

    while (iter != w->tree2_children_unsorted.end()) {
      auto *child = iter->second;
      wid_destroy_immediate(g, child);
      iter = w->tree2_children_unsorted.begin();
    }
  }
}

static void wid_destroy_immediate(Gamep g_maybe_null, Widp w)
{
  TRACE();

  wid_dbg(w, "destroy immediate");

  //
  // If removing a top level widget, choose a new focus.
  //
  if (w->parent == nullptr) {
    wid_set_top_focus(g_maybe_null);
  }

  wid_tree_detach(w);

  wid_tree2_unsorted_remove(w);
  wid_tree_global_unsorted_remove(w);

  wid_destroy_immediate_internal(g_maybe_null, w);

  if (w == wid_focus_locked) {
    wid_focus_locked = nullptr;
  }

  if (w == wid_focus) {
    wid_mouse_focus_end(g_maybe_null);
  }

  if (w == wid_over) {
    //
    // For actionbar buttons. If they create a popup, we must be called when the actionbar
    // is destroyed, to remove the popup. Else we get a dangling popup.
    //
    wid_mouse_over_end(g_maybe_null);
  }

  for (auto x = 0; x < TERM_WIDTH; x++) {
    for (auto y = 0; y < TERM_HEIGHT; y++) {
      if (wid_on_screen_at[ x ][ y ] == w) {
        wid_on_screen_at[ x ][ y ] = static_cast< Widp >(nullptr);
      }
    }
  }

  delete w;
}

static void wid_destroy_delay(Gamep g, Widp *wp, int delay)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;

  if (wp == nullptr) {
    return;
  }

  auto *w = *wp;

  if (w == nullptr) [[unlikely]] {
    return;
  }

  wid_dbg(w, "destroy delay");

  (*wp) = nullptr;

  if (static_cast< bool >(w->being_destroyed)) {
    if (delay != 0) {
      return;
    }
  }

  w->being_destroyed = 1U;
  wid_tree4_wids_being_destroyed_insert(w);

  if (wid_focus == w) {
    wid_mouse_focus_end(g);
  }

  //
  // This can easily lead to recursion. Do we need it?
  //
  // if (wid_over == w) {
  //     wid_mouse_over_end();
  // }
  for (auto &iter : w->tree2_children_unsorted) {
    auto *child = iter.second;
    wid_destroy(g, &child);
  }

  if (w->parent == nullptr) {
    wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);
  }

  if (w->on_destroy_begin != nullptr) {
    (w->on_destroy_begin)(g, w);
  }

  //
  // Make sure it stops tick right now as client pointers this widget
  // might use in the ticker may no longer be valid.
  //
  wid_tree5_tick_wids_remove(w);
  wid_tree6_pre_tick_wids_remove(w);
}

void wid_destroy(Gamep g, Widp *wp)
{
  TRACE();
  wid_destroy_delay(g, wp, wid_destroy_delay_ms);
}

void wid_destroy_nodelay(Gamep g, Widp *wp)
{
  TRACE();
  wid_destroy_delay(g, wp, 0);
}

void wid_destroy_in(Gamep g, Widp w, uint32_t ms)
{
  TRACE();

  w->destroy_when = wid_time + ms;

  wid_tree4_wids_being_destroyed_insert(w);
}

//
// Initialize a top level wid with basic settings
//
auto wid_new_container(Gamep g, Widp parent, const std::string &name) -> Widp
{
  TRACE();

  if (! wid_safe()) {
    return nullptr;
  }

  Widp w = wid_new(parent);

#ifdef ENABLE_DEBUG_UI
#ifdef ENABLE_DEBUG_UI2
  w->to_string = string_sprintf("%s[%p] (parent %s[%p])", name.c_str(), w, parent->to_string.c_str(), parent);
#else
  w->to_string = string_sprintf("%s[%p]", name.c_str(), w);
#endif
#endif

  wid_dbg(w, "%s", __FUNCTION__);

  wid_set_name(w, name);
  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_color(w, WID_COLOR_BG, WHITE);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);
  wid_set_shape_square(w);

  //
  // Raise it so if there were other widgets with the same parent
  // then this will be in front
  //
  wid_raise(g, w);
  wid_dbg(w, "new");

  return w;
}

//
// Initialize a top level wid with basic settings
//
auto wid_new_window(Gamep g, const std::string &name) -> Widp
{
  TRACE();

  if (! wid_safe()) {
    return nullptr;
  }

  Widp w = wid_new();

  w->to_string = string_sprintf("%s[%p]", name.c_str(), (void *) w);

  wid_dbg(w, "%s", __FUNCTION__);

  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_name(w, name);
  wid_set_shape_square(w);
  wid_set_color(w, WID_COLOR_BG, WHITE);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);

  //
  // Raise it so if there were other widgets with the same parent
  // then this will be in front
  //
  wid_raise(g, w);
  wid_dbg(w, "new");

  return w;
}

auto wid_new_square_button(Gamep g, Widp parent, const std::string &name) -> Widp
{
  TRACE();

  if (! wid_safe()) {
    return nullptr;
  }

  if (parent == nullptr) {
    ERR("no parent widget");
  }

  Widp w = wid_new(parent);

#ifdef ENABLE_DEBUG_UI
#ifdef ENABLE_DEBUG_UI2
  w->to_string = string_sprintf("%s[%p] (parent %s[%p])", name.c_str(), w, parent->to_string.c_str(), (void *) parent);
#else
  w->to_string = string_sprintf("%s[%p]", name.c_str(), (void *) w);
#endif
#endif

  wid_dbg(w, "%s", __FUNCTION__);

  wid_set_name(w, name);
  wid_set_shape_square(w);

  wid_set_mode(w, WID_MODE_OVER);
  wid_set_color(w, WID_COLOR_BG, GRAY90);
  wid_set_color(w, WID_COLOR_TEXT_FG, UI_HIGHLIGHT_COLOR);

  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_color(w, WID_COLOR_BG, WHITE);
  wid_set_color(w, WID_COLOR_TEXT_FG, UI_TEXT_COLOR);

  //
  // Raise it so if there were other widgets with the same parent
  // then this will be in front
  //
  wid_raise(g, w);
  wid_dbg(w, "new");

  return w;
}

auto wid_new_plain(Gamep g, Widp parent, const std::string &name) -> Widp
{
  TRACE();

  if (! wid_safe()) {
    return nullptr;
  }

  if (parent == nullptr) {
    ERR("no parent widget");
  }

  Widp w = wid_new(parent);

#ifdef ENABLE_DEBUG_UI
#ifdef ENABLE_DEBUG_UI2
  w->to_string = string_sprintf("%s[%p] (parent %s[%p])", name.c_str(), w, parent->to_string.c_str(), parent);
#else
  w->to_string = string_sprintf("%s[%p]", name.c_str(), w);
#endif
#endif

  wid_dbg(w, "%s", __FUNCTION__);

  wid_set_name(w, name);
  wid_set_shape_square(w);

  wid_set_mode(w, WID_MODE_OVER);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);

  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);

  //
  // Raise it so if there were other widgets with the same parent
  // then this will be in front
  //
  wid_raise(g, w);
  wid_dbg(w, "new");

  return w;
}

//
// Initialize a wid with basic settings
//
static auto wid_new_scroll_trough(Gamep g, Widp parent) -> Widp
{
  TRACE();

  if (! wid_safe()) {
    return nullptr;
  }

  if (parent == nullptr) {
    ERR("no parent widget");
  }

  Widp w = wid_new(parent);

  w->to_string = string_sprintf("[%p] scroll trough (parent %s[%p])", (void *) w, parent->to_string.c_str(), (void *) parent);

  wid_dbg(w, "%s", __FUNCTION__);

  wid_set_mode(w, WID_MODE_NORMAL);
  {
    color const c = GRAY90;
    wid_set_color(w, WID_COLOR_BG, c);
  }

  wid_set_on_mouse_down(w, wid_scroll_trough_mouse_down);
  wid_set_on_mouse_motion(w, wid_scroll_motion);
  wid_set_shape_square(w);

  wid_set_mode(w, WID_MODE_OVER);
  wid_set_color(w, WID_COLOR_BG, GRAY90);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);

  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_color(w, WID_COLOR_BG, WHITE);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);

  wid_set_style(w, UI_WID_STYLE_NORMAL);
  wid_dbg(w, "new");

  return w;
}

//
// Initialize a wid with basic settings
//
static auto wid_new_scroll_bar(Gamep g, Widp parent, const std::string &name, Widp scrollbar_owner, bool vertical) -> Widp
{
  TRACE();

  if (! wid_safe()) {
    return nullptr;
  }

  if (parent == nullptr) {
    ERR("no parent widget");
  }

  Widp w = wid_new(parent);

  if (vertical) {
    w->to_string = string_sprintf("%s, %s[%p]", name.c_str(), "vert scroll bar", (void *) w);
  } else {
    w->to_string = string_sprintf("%s, %s[%p]", name.c_str(), "horiz scroll bar", (void *) w);
  }

  wid_dbg(w, "%s", __FUNCTION__);

  wid_set_name(w, name);

  wid_set_mode(w, WID_MODE_ACTIVE);
  {
    color const c = GREEN;
    wid_set_color(w, WID_COLOR_BG, c);
  }

  wid_set_mode(w, WID_MODE_NORMAL);
  {
    color const c = GRAY50;
    wid_set_color(w, WID_COLOR_BG, c);
  }

  wid_set_moveable(g, w);
  wid_set_moveable_bounded(g, w);

  if (vertical) {
    wid_set_moveable_vert(g, w);
    wid_set_moveable_horiz(g, w, false);
    scrollbar_owner->scrollbar_vert = w;
  } else {
    wid_set_moveable_horiz(g, w);
    wid_set_moveable_vert(g, w, false);
    scrollbar_owner->scrollbar_horiz = w;
  }

  w->scrollbar_owner = scrollbar_owner;

  wid_set_shape_square(w);

  wid_set_mode(w, WID_MODE_OVER);
  wid_set_color(w, WID_COLOR_BG, WHITE);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);

  wid_set_mode(w, WID_MODE_NORMAL);
  wid_set_color(w, WID_COLOR_BG, WHITE);
  wid_set_color(w, WID_COLOR_TEXT_FG, WHITE);

  wid_set_style(w, UI_WID_STYLE_NORMAL);
  wid_dbg(w, "new");

  return w;
}

auto wid_new_vert_scroll_bar(Gamep g, Widp parent, const std::string &name, Widp scrollbar_owner) -> Widp
{
  TRACE();

  if (! wid_safe()) {
    return nullptr;
  }

  if (parent == nullptr) {
    ERR("no parent widget");
  }

  spoint vert_tl = {};
  spoint vert_br = {};

  int tlx  = 0;
  int tly  = 0;
  int brx  = 0;
  int bry  = 0;
  int ptlx = 0;
  int ptly = 0;
  int pbrx = 0;
  int pbry = 0;

  //
  // Make the trough line up with the scrolling window.
  //
  wid_get_abs_coords(parent, &ptlx, &ptly, &pbrx, &pbry);
  wid_get_abs_coords(scrollbar_owner, &tlx, &tly, &brx, &bry);

  vert_tl.x = tlx - ptlx + wid_get_width(scrollbar_owner);
  vert_br.x = vert_tl.x;

  vert_tl.y = tly - ptly;
  vert_br.y = tly - ptly + wid_get_height(scrollbar_owner) - 1;

  Widp trough = wid_new_scroll_trough(g, parent);
  wid_set_pos(trough, vert_tl, vert_br);
  wid_set_shape_square(trough);
  wid_set_style(trough, UI_WID_STYLE_VERT_SCROLL_DARK);

  {
    fpoint const tl(0, 0);
    fpoint const br(1, 1);
    Widp         scrollbar = wid_new_scroll_bar(g, trough, name, scrollbar_owner, 1U);
    wid_set_pos_pct(scrollbar, tl, br);

    wid_update_internal(g, scrollbar);
    wid_visible(g, wid_get_parent(scrollbar));
    wid_visible(g, scrollbar);
    wid_set_mode(scrollbar, WID_MODE_ACTIVE);
    wid_set_style(scrollbar, UI_WID_STYLE_VERT_LIGHT);
    wid_set_mode(scrollbar, WID_MODE_NORMAL);
    wid_set_style(scrollbar, UI_WID_STYLE_VERT_DARK);

    trough->is_scrollbar_vert_trough = 1U;
    scrollbar->is_scrollbar_vert     = 1U;

    return scrollbar;
  }
}

auto wid_new_horiz_scroll_bar(Gamep g, Widp parent, const std::string &name, Widp scrollbar_owner) -> Widp
{
  TRACE();

  if (! wid_safe()) {
    return nullptr;
  }

  if (parent == nullptr) {
    ERR("no parent widget");
  }

  spoint horiz_tl = {};
  spoint horiz_br = {};

  int tlx  = 0;
  int tly  = 0;
  int brx  = 0;
  int bry  = 0;
  int ptlx = 0;
  int ptly = 0;
  int pbrx = 0;
  int pbry = 0;

  //
  // Make the trough line up with the scrolling window.
  //
  wid_get_abs_coords(parent, &ptlx, &ptly, &pbrx, &pbry);
  wid_get_abs_coords(scrollbar_owner, &tlx, &tly, &brx, &bry);

  horiz_tl.x = tlx - ptlx;
  horiz_br.x = tlx - ptlx + wid_get_width(scrollbar_owner) - 1;

  horiz_tl.y = tly - ptly + wid_get_height(scrollbar_owner);
  horiz_br.y = horiz_tl.y;

  Widp trough = wid_new_scroll_trough(g, parent);
  wid_set_pos(trough, horiz_tl, horiz_br);
  wid_set_shape_square(trough);
  wid_set_style(trough, UI_WID_STYLE_HORIZ_SCROLL_DARK);

  {
    fpoint const tl(0, 0);
    fpoint const br(1, 1);
    Widp         scrollbar = wid_new_scroll_bar(g, trough, name, scrollbar_owner, 0U);
    wid_set_pos_pct(scrollbar, tl, br);

    wid_update_internal(g, scrollbar);
    wid_visible(g, wid_get_parent(scrollbar));
    wid_visible(g, scrollbar);
    wid_set_style(scrollbar, UI_WID_STYLE_HORIZ_SCROLL_LIGHT);

    trough->is_scrollbar_horiz_trough = 1U;
    scrollbar->is_scrollbar_horiz     = 1U;

    return scrollbar;
  }
}

static void wid_raise_internal(Widp w)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  if (static_cast< bool >(w->do_not_raise)) {
    return;
  }

  wid_tree_detach(w);
  w->key.priority = ++wid_highest_priority;
  wid_tree_attach(w);
}

static void wid_raise_override(Gamep g, Widp parent)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  //
  // If some widget wants to be on top, let it.
  //
  if (static_cast< bool >(parent->do_not_lower)) {
    wid_raise_internal(parent);
  }

  for (auto &iter : parent->children_display_sorted) {
    auto *w = iter.second;

    if (static_cast< bool >(w->do_not_lower)) {
      wid_raise_internal(w);
      break;
    }

    wid_raise_override(g, w);
  }
}

void wid_raise(Gamep g, Widp w_in)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  if (w_in == nullptr) {
    return;
  }

  wid_raise_internal(w_in);

  //
  // If some widget wants to be on top, let it.
  //
  std::vector< Widp > worklist;
  for (auto &iter : wid_top_level) {
    auto *w = iter.second;
    worklist.push_back(w);
  }

  for (auto &w : worklist) {
    wid_raise_override(g, w);
  }

  wid_set_top_focus(g);

  //
  // If we were hovering over a window and it was replaced, we need to fake
  // a mouse movement so we know we are still over it.
  //
  if (w_in->parent == nullptr) {
    wid_update_mouse(g);
  }
}

static void wid_lower_internal(Widp w)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  if (static_cast< bool >(w->do_not_lower)) {
    return;
  }

  wid_tree_detach(w);
  w->key.priority = --wid_lowest_priority;
  wid_tree_attach(w);
}

void wid_lower(Gamep g, Widp w_in)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  if (w_in == nullptr) {
    return;
  }

  wid_lower_internal(w_in);

  //
  // If some widget wants to be on top, let it.
  //
  for (auto &iter : wid_top_level) {
    auto *w = iter.second;
    if (static_cast< bool >(w->do_not_raise)) {
      wid_lower_internal(w);
      break;
    }
  }

  wid_set_top_focus(g);

  //
  // If we were hovering over a window and it was replaced, we need to fake
  // a mouse movement so we know we are still over it.
  //
  if ((w_in->parent == nullptr) && ! w_in->children_display_sorted.empty()) {
    wid_update_mouse(g);
  }
}

void wid_toggle_hidden(Gamep g, Widp w)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  if (static_cast< bool >(w->hidden)) {
    wid_visible(g, w);
    wid_raise(g, w);
  } else {
    wid_hide(g, w);
  }
}

static void wid_find_first_child_focus(Widp w, Widp *best)
{
  TRACE();

  if (static_cast< bool >(w->focus_order)) {
    if (*best == nullptr) {
      *best = w;
    } else if (w->focus_order < (*best)->focus_order) {
      *best = w;
    }
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    wid_find_first_child_focus(child, best);
  }
}

void wid_set_first_focus(Gamep g)
{
  TRACE();

  Widp best {};

  for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
    auto *w = iter.second;
    if (wid_ignore_for_focus(w)) {
      continue;
    }

    best = nullptr;
    wid_find_first_child_focus(w, &best);
    if (best != nullptr) {
      wid_set_focus(g, best);
      return;
    }
  }
}

static void wid_find_specific_child_focus(Widp w, Widp *best, uint8_t focus_order)
{
  TRACE();

  if (static_cast< bool >(w->focus_order)) {
    if (w->focus_order == focus_order) {
      *best = w;
      return;
    }
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    wid_find_specific_child_focus(child, best, focus_order);
  }
}

static auto wid_find_top_wid_focus(Widp w) -> Widp
{
  TRACE();

  Widp best {};

  if (wid_ignore_for_focus(w)) {
    return best;
  }

  //
  // First time we've looked at this widget, hunt for the first focus.
  //
  if (! static_cast< bool >(w->focus_last)) {
    wid_find_first_child_focus(w, &best);
    if (best != nullptr) {
      return best;
    }
  }

  wid_find_specific_child_focus(w, &best, w->focus_last);
  if (best != nullptr) {
    return best;
  }

  return best;
}

void wid_set_top_focus(Gamep g)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  Widp best {};

  for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
    auto *w = iter.second;
    if (wid_ignore_for_focus(w)) {
      continue;
    }

    best = nullptr;

    //
    // First time we've looked at this widget, hunt for the first focus.
    //
    if (! static_cast< bool >(w->focus_last)) {
      wid_find_first_child_focus(w, &best);
      if (best != nullptr) {
        wid_set_focus(g, best);
        return;
      }
    }

    wid_find_specific_child_focus(w, &best, w->focus_last);
    if (best != nullptr) {
      wid_set_focus(g, best);
      return;
    }
  }

  wid_set_first_focus(g);
}

static void wid_find_last_child_focus(Widp w, Widp *best)
{
  TRACE();

  if (static_cast< bool >(w->focus_order)) {
    if (*best == nullptr) {
      *best = w;
    } else if (w->focus_order > (*best)->focus_order) {
      *best = w;
    }
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    wid_find_last_child_focus(child, best);
  }
}

auto wid_get_focus(Widp w) -> Widp
{
  TRACE();

  if (! wid_safe()) {
    return nullptr;
  }

  Widp best {};

  if (wid_focus != nullptr) {
    if (wid_get_top_parent(wid_focus) == wid_get_top_parent(w)) {
      return wid_focus;
    }
  }

  if (! static_cast< bool >(w->focus_last)) {
    best = wid_find_top_wid_focus(wid_get_top_parent(w));
    if (best != nullptr) {
      return best;
    }
  }

  wid_find_specific_child_focus(w, &best, w->focus_last);

  return best;
}

static void wid_set_last_focus(Gamep g)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  Widp best {};

  for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
    auto *w = iter.second;
    if (wid_ignore_for_focus(w)) {
      continue;
    }

    best = nullptr;
    wid_find_last_child_focus(w, &best);
    if (best != nullptr) {
      wid_set_focus(g, best);
      return;
    }
  }
}

static void wid_find_next_child_focus(Widp w, Widp *best)
{
  TRACE();

  if (static_cast< bool >(w->focus_order)) {
    if (*best != nullptr) {
      if ((w->focus_order < (*best)->focus_order) && (w->focus_order > wid_focus->focus_order)) {
        *best = w;
      }
    } else if (w->focus_order > wid_focus->focus_order) {
      *best = w;
    }
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    wid_find_next_child_focus(child, best);
  }
}

static void wid_set_next_focus(Gamep g)
{
  TRACE();

  Widp best {};

  if (wid_focus == nullptr) {
    wid_set_first_focus(g);
    return;
  }

  for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
    auto *w = iter.second;

    if (wid_ignore_for_focus(w)) {
      continue;
    }

    if (! static_cast< bool >(w->focus_last)) {
      continue;
    }

    wid_find_next_child_focus(w, &best);
    if (best != nullptr) {
      wid_set_focus(g, best);
      return;
    }

    wid_set_first_focus(g);
    break;
  }
}

static void wid_find_prev_child_focus(Widp w, Widp *best)
{
  TRACE();

  if (static_cast< bool >(w->focus_order)) {
    if (*best != nullptr) {
      if ((w->focus_order > (*best)->focus_order) && (w->focus_order < wid_focus->focus_order)) {
        *best = w;
      }
    } else if (w->focus_order < wid_focus->focus_order) {
      *best = w;
    }
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    wid_find_prev_child_focus(child, best);
  }
}

static void wid_set_prev_focus(Gamep g)
{
  TRACE();

  Widp best {};

  if (wid_focus == nullptr) {
    wid_set_first_focus(g);
    return;
  }

  for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
    auto *w = iter.second;

    if (wid_ignore_for_focus(w)) {
      continue;
    }

    if (! static_cast< bool >(w->focus_last)) {
      continue;
    }

    wid_find_prev_child_focus(w, &best);
    if (best != nullptr) {
      wid_set_focus(g, best);
      return;
    }

    wid_set_last_focus(g);
    break;
  }
}

auto wid_find(Widp w, const std::string &name) -> Widp
{
  TRACE();

  if (strcasecmp(w->name.c_str(), name.c_str()) == 0) {
    return w;
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    Widp ret {};

    ret = wid_find(child, name);
    if (ret != nullptr) {
      return ret;
    }
  }
  return nullptr;
}

auto wid_find(const std::string &name) -> Widp
{
  TRACE();

  for (auto &iter : wid_top_level) {
    auto *w = iter.second;
    auto *r = wid_find(w, name);
    if (r != nullptr) {
      return r;
    }
  }
  // DBG2("widget [%s] not found", name.c_str());
  return nullptr;
}

void wid_visible(Gamep g, Widp w)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  if (w == nullptr) [[unlikely]] {
    return;
  }

  w->visible = 1U;
  w->hidden  = 0U;

  std::vector< Widp > const worklist;
  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;
    wid_visible(g, child);
  }

  wid_set_top_focus(g);
}

auto wid_is_visible(Widp w) -> bool
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    return false;
  }

  return static_cast< bool >(w->visible);
}

void wid_hide(Gamep g, Widp w)
{
  TRACE();

  if (! wid_safe()) {
    return;
  }

  if (w == nullptr) [[unlikely]] {
    return;
  }

  w->hidden  = 1U;
  w->visible = 0U;

  if (wid_over == w) {
    wid_mouse_over_end(g);
  }

  if (wid_get_top_parent(wid_over) == w) {
    wid_mouse_over_end(g);
  }

  if (w == wid_focus) {
    wid_set_top_focus(g);
  }

  std::vector< Widp > const worklist;
  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;
    wid_hide(g, child);
  }
}

static void wid_adjust_scrollbar(Widp scrollbar, Widp owner)
{
  TRACE();

  float const height           = wid_get_height(owner);
  float const width            = wid_get_width(owner);
  float       child_height     = 0;
  float       child_width      = 0;
  float       scrollbar_width  = 0;
  float       scrollbar_height = 0;
  float       trough_height    = 0;
  float       trough_width     = 0;
  float       miny             = 0;
  float       maxy             = 0;
  float       minx             = 0;
  float       maxx             = 0;
  float       pct              = 0;
  bool        first            = true;

  //
  // Find out the space that the children take up then use this to
  // adjust the scrollbar dimensions.
  //
  {
    for (auto &iter : owner->tree2_children_unsorted) {
      auto *child = iter.second;

      int tl_x = 0;
      int tl_y = 0;
      int br_x = 0;
      int br_y = 0;

      wid_get_tl_x_tl_y_br_x_br_y(child, &tl_x, &tl_y, &br_x, &br_y);

      if (first) {
        minx  = tl_x;
        miny  = tl_y;
        maxx  = br_x;
        maxy  = br_y;
        first = false;
        continue;
      }

      minx = std::min< float >(tl_x, minx);

      miny = std::min< float >(tl_y, miny);

      maxx = std::max< float >(br_x, maxx);

      maxy = std::max< float >(br_y, maxy);
    }
  }

  int ptl_x = 0;
  int ptl_y = 0;
  int pbr_x = 0;
  int pbr_y = 0;
  wid_get_tl_x_tl_y_br_x_br_y(owner, &ptl_x, &ptl_y, &pbr_x, &pbr_y);

  minx -= ptl_x;
  miny -= ptl_y;
  maxx -= ptl_x;
  maxy -= ptl_y;

  child_width  = maxx - minx + 1;
  child_height = maxy - miny + 1;

  if (child_width < width) {
    maxx        = minx + width - 1;
    child_width = maxx - minx + 1;
  }

  if (child_height < height) {
    maxy         = miny + height - 1;
    child_height = maxy - miny + 1;
  }

  if (owner->scrollbar_vert != nullptr) {
    if (wid_get_moveable_vert(scrollbar)) {
      trough_height    = wid_get_height(owner->scrollbar_vert->parent);
      scrollbar_height = static_cast< int >(trough_height * (height / child_height));

      if (trough_height - scrollbar_height == 0.0F) {
        pct = 0.0F;
      } else {
        pct = ((static_cast< float >(wid_get_tl_y(scrollbar))) - (static_cast< float >(wid_get_tl_y(scrollbar->parent))))
            / ((trough_height - scrollbar_height));
      }

      owner->offset.y = static_cast< int >(-miny);
      owner->offset.y -= static_cast< int >((pct * (child_height - height)));

      float const n       = (static_cast< float >(wid_get_tl_y(scrollbar->parent))) + (pct * ((trough_height - scrollbar_height)));
      scrollbar->key.tl.y = static_cast< int >(ceil(n));

      wid_tree_detach(scrollbar);
      scrollbar->key.br.y = wid_get_tl_y(scrollbar) + static_cast< int >(scrollbar_height) - 1;
      wid_tree_attach(scrollbar);

      wid_set_mode(scrollbar, WID_MODE_ACTIVE);
    }
  }

  if (owner->scrollbar_horiz != nullptr) {
    if (wid_get_moveable_horiz(scrollbar)) {
      trough_width    = wid_get_width(owner->scrollbar_horiz->parent);
      scrollbar_width = static_cast< int >(trough_width * (width / child_width));

      if (trough_width - scrollbar_width == 0.0F) {
        pct = 0.0F;
      } else {
        pct = (wid_get_tl_x(scrollbar) - wid_get_tl_x(scrollbar->parent)) / (trough_width - scrollbar_width);
      }

      owner->offset.x = static_cast< int >(-minx);
      owner->offset.x -= static_cast< int >(pct * (child_width - width));

      float const n       = (static_cast< float >(wid_get_tl_x(scrollbar->parent))) + (pct * ((trough_width - scrollbar_width)));
      scrollbar->key.tl.x = static_cast< int >(ceil(n));

      wid_tree_detach(scrollbar);
      scrollbar->key.br.x = wid_get_tl_x(scrollbar) + static_cast< int >(scrollbar_width) - 1;
      wid_tree_attach(scrollbar);

      wid_set_mode(scrollbar, WID_MODE_ACTIVE);
    }
  }
}

void wid_get_children_size(Widp owner, int *w, int *h)
{
  TRACE();

  float const height       = wid_get_height(owner);
  float const width        = wid_get_width(owner);
  float       child_height = 0;
  float       child_width  = 0;
  float       miny         = 0;
  float       maxy         = 0;
  float       minx         = 0;
  float       maxx         = 0;
  bool        first        = 1u;

  //
  // Find out the space that the children take up then use this to
  // adjust the scrollbar dimensions.
  //
  for (auto &iter : owner->children_display_sorted) {

    auto *child = iter.second;

    int const tminx = wid_get_tl_x(child) - wid_get_tl_x(child->parent);
    int const tminy = wid_get_tl_y(child) - wid_get_tl_y(child->parent);
    int const tmaxx = wid_get_br_x(child) - wid_get_tl_x(child->parent);
    int const tmaxy = wid_get_br_y(child) - wid_get_tl_y(child->parent);

    if (first) {
      minx  = tminx;
      miny  = tminy;
      maxx  = tmaxx;
      maxy  = tmaxy;
      first = 0u;
      continue;
    }

    minx = std::min< float >(tminx, minx);

    miny = std::min< float >(tminy, miny);

    maxx = std::max< float >(tmaxx, maxx);

    maxy = std::max< float >(tmaxy, maxy);
  }

  child_width  = maxx - minx;
  child_height = maxy - miny;

  if (child_width < width) {
    maxx        = minx + width;
    child_width = maxx - minx;
  }

  if (child_height < height) {
    maxy         = miny + height;
    child_height = maxy - miny;
  }

  if (w != nullptr) {
    *w = static_cast< int >(child_width);
  }

  if (h != nullptr) {
    *h = static_cast< int >(child_height);
  }
}

static void wid_update_internal(Gamep g, Widp w)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;

  if (w == nullptr) {
    return;
  }

  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  //
  // First time around, initialize the wid.
  //
  if (! static_cast< bool >(w->first_update)) {
    w->first_update = 1U;

    if (w->parent == nullptr) {
      //
      // Find the focus.
      //
      wid_set_top_focus(g);
    }

    //
    // Set back to normal to undo any settings when creating.
    //
    // No, make the clients fix their code.
    //
    //        wid_set_mode( w, WID_MODE_NORMAL);
  }

  //
  // Clip all the children. Avoid this for speed for the main g window.
  //
  std::vector< Widp > worklist;

  {
    for (auto &iter : w->tree2_children_unsorted) {
      auto *c = iter.second;
      worklist.push_back(c);
    }
  }

  for (auto &child : worklist) {
    wid_update_internal(g, child);
  }

  //
  // If the source of the event is the scrollbars themselves...
  //
  if (w->scrollbar_owner != nullptr) {
    wid_adjust_scrollbar(w, w->scrollbar_owner);
    wid_update_internal(g, w->scrollbar_owner);
  } else {
    //
    // If the source of the event is the owner of the scrollbars...
    //
    if (w->scrollbar_vert != nullptr) {
      wid_adjust_scrollbar(w->scrollbar_vert, w);
    }

    if (w->scrollbar_horiz != nullptr) {
      wid_adjust_scrollbar(w->scrollbar_horiz, w);
    }
  }
}

void wid_update(Gamep g, Widp w)
{
  TRACE();

  if (w == nullptr) {
    return;
  }

  wid_update_internal(g, w);

  //
  // If we were hovering over a window and it was replaced, we need to fake
  // a mouse movement so we know we are still over it.
  //
  if ((w->parent == nullptr) && ! w->children_display_sorted.empty()) {
    wid_update_mouse(g);
  }
}

void wid_scroll_text(Widp w)
{
  TRACE();

  std::string s;
  Widp        prev {};
  Widp        tmp {};

  //
  // Get the wid on the top of the list/screen.
  //
  tmp = wid_get_tail(w);

  //
  // Now copy the text up to the parent widgets.
  //
  while (tmp != nullptr) {
    prev = wid_get_prev(tmp);

    if (prev != nullptr) {
      s = wid_get_text(prev);

      wid_set_text(tmp, s);
    }

    tmp = prev;
  }
}

//
// Replace the 2nd last line of text and scroll. The assumption is the last
// line is the input line.
//
void wid_scroll_with_input(Gamep g, Widp w, const std::string &str)
{
  TRACE();

  Widp tmp {};

  wid_scroll_text(w);

  //
  // Get the wid on the bottom of the list/screen.
  //
  tmp = wid_get_head(w);

  //
  // Now get the 2nd last line. The last line is the input. The 2nd last
  // line is where new output goes.
  //
  if (tmp != nullptr) {
    tmp = wid_get_next(tmp);
    if (tmp != nullptr) {
      wid_set_text(tmp, str);
    }
  }
}

auto wid_receive_input(Gamep g, Widp w, const SDL_Keysym *key) -> bool
{
  TRACE();

  std::string       beforecursor;
  std::string       aftercursor;
  std::string const tmp;
  std::string       origtext;
  std::string       updatedtext;
  std::string       newchar;
  uint32_t          origlen = 0;
  uint32_t          cnt     = 0;

  if (sdlk_eq(*key, game_key_console_get(g))) {
    sound_play(g, "keypress");
    return false;
  }

  sound_play(g, "keypress");

  newchar += wid_event_to_char(key);
  origtext = wid_get_text(w);
  origlen  = static_cast< uint32_t >(origtext.length());

  if (! static_cast< bool >(w->received_input)) {
    wid_set_received_input(w);
    w->cursor = static_cast< uint32_t >(origtext.length());
  }

  if (origtext.empty()) {
    aftercursor = beforecursor = "";
  } else {
    beforecursor = origtext.substr(0, w->cursor);
    aftercursor  = origtext.substr(w->cursor);
  }

  switch (key->mod) {
    case KMOD_LCTRL :
    case KMOD_RCTRL :
      switch (wid_event_to_char(key)) {
        case 'p' :
          if (! static_cast< bool >(g_history_walk)) {
            g_history_walk = HISTORY_MAX - 1;
          } else {
            g_history_walk--;
          }

          wid_set_text(w, history[ g_history_walk ]);
          w->cursor = static_cast< uint32_t >(wid_get_text(w).length());
          break;

        case 'n' :
          g_history_walk++;
          if (g_history_walk >= HISTORY_MAX) {
            g_history_walk = 0;
          }

          wid_set_text(w, history[ g_history_walk ]);
          w->cursor = static_cast< uint32_t >(wid_get_text(w).length());
          break;

        case 'a' : w->cursor = 0; break;

        case 'e' : w->cursor = origlen; break;
      }
      break;

    default :

      switch (key->sym) {
        case SDLK_BACKSPACE :
          if (! beforecursor.empty()) {
            w->cursor--;

            beforecursor.erase(beforecursor.end() - 1);
            auto result = beforecursor + aftercursor;
            wid_set_text(w, result);
          }
          break;

        case SDLK_TAB :
          if (w != wid_console_input_line) {
            return true;
          }

          command_handle(g, wid_get_text(w), &updatedtext, 0U /* show ambiguous */, 1U /* show complete */, 0U /* execute command */,
                         nullptr /* context */);

          if (! updatedtext.empty()) {
            wid_set_text(w, updatedtext);
            w->cursor = updatedtext.length();
          }
          return true;

        case SDLK_RETURN :
          if (w != wid_console_input_line) {
            return false;
          }

          if ((static_cast< bool >(origlen)) && (w == wid_console_input_line)) {
            static std::string entered;
            static std::string entered2;

            entered  = wid_get_text(w);
            entered2 = ">>> %%fg=green$" + wid_get_text(w);

            wid_scroll_text(w);
            wid_set_text(w->next, entered2);

            if (command_handle(g, entered, &updatedtext, 1U /* show ambiguous */, 0U /* show complete */, 1U /* execute command */,
                               nullptr /* context */)
                == 0) {
              return true;
            }

            updatedtext = trim(updatedtext);

            if (! updatedtext.empty()) {
              wid_set_text(w, updatedtext);
              w->cursor = updatedtext.length();
            }

            history[ g_history_at ] = updatedtext;

            g_history_at++;
            if (g_history_at >= HISTORY_MAX) {
              g_history_at = 0;
            }
            g_history_walk = g_history_at;

            wid_set_text(w, "");
            w->cursor = 0;
          } else if (w == wid_console_input_line) {
            wid_scroll_text(w);
          }
          return true;

        case SDLK_LEFT :
          if (w->cursor > 0) {
            w->cursor--;
          }
          break;

        case SDLK_RIGHT :
          if (w->cursor < origlen) {
            w->cursor++;
          }
          break;

        case SDLK_UP :
          cnt = 0;
          while (cnt < HISTORY_MAX) {
            cnt++;
            if (! static_cast< bool >(g_history_walk)) {
              g_history_walk = HISTORY_MAX - 1;
            } else {
              g_history_walk--;
            }

            wid_set_text(w, history[ g_history_walk ]);
            if (history[ g_history_walk ].empty()) {
              continue;
            }

            w->cursor = static_cast< uint32_t >(wid_get_text(w).length());
            break;
          }
          break;

        case SDLK_DOWN :
          cnt = 0;
          while (cnt < HISTORY_MAX) {
            cnt++;

            g_history_walk++;
            if (g_history_walk >= HISTORY_MAX) {
              g_history_walk = 0;
            }

            wid_set_text(w, history[ g_history_walk ]);
            if (history[ g_history_walk ].empty()) {
              continue;
            }

            w->cursor = static_cast< uint32_t >(wid_get_text(w).length());
            break;
          }
          break;

        case SDLK_HOME : w->cursor = 0; break;

        case SDLK_END :  w->cursor = origlen; break;

        default :
          {
            auto c = wid_event_to_char(key);

            switch (c) {
              case SDLK_ESCAPE :
              case '?' :
                if (w == wid_console_input_line) {
                  command_handle(g, wid_get_text(w), &updatedtext, 1U /* show ambiguous */, 0U /* show complete */,
                                 0U /* execute command */, nullptr /* context */);

                  if (! updatedtext.empty()) {
                    wid_set_text(w, updatedtext);
                    w->cursor = updatedtext.length();
                  }
                  return true;
                }
            }

            if (c != '\0') {
              updatedtext = beforecursor;
              updatedtext += c;
              updatedtext += aftercursor;

              w->cursor++;

              wid_set_text(w, updatedtext);
            }
          }
      }
  }

  return true;
}

//
// Handle keys no one grabbed.
//
[[nodiscard]] static auto wid_receive_unhandled_input(Gamep g, const SDL_Keysym *key) -> bool
{
  TRACE();

  Widp w {};

  //
  // Pass to the game first
  //
  if (NO_ERROR_OCCURRED()) {
    //
    // Unless in an errored condition
    //
    if (game_input(g, key)) {
      return true;
    }
  }

  w = wid_get_top_parent(wid_console_input_line);

  if (sdlk_eq(*key, game_key_console_get(g))) {
    log("open console");

    if (AN_ERROR_OCCURRED()) {
      //
      // Console is always present until errors are cleared
      //
      auto console_key = ::to_string(game_key_console_get(g));
      con("To continue, 'clear errored' and then press <%s>", console_key.c_str());

      wid_console_raise(g);
    } else {
      //
      // Toggle console on/off
      //
      wid_toggle_hidden(g, wid_console_window);

      if (static_cast< bool >(wid_console_window->visible)) {
        wid_console_raise(g);
      }
    }

    //
    // Need this so the console gets focus over the menu.
    //
    if ((w != nullptr) && (static_cast< bool >(w->visible))) {
      wid_set_focus(g, w);
      wid_focus_lock(g, w);
    } else {
      wid_unset_focus(g);
      wid_unset_focus_lock();
    }
    return true;
  }

  if (sdlk_eq(*key, game_key_screenshot_get(g))) {
    sdl_screenshot(g);
    return true;
  }

  switch (static_cast< int >(key->sym)) {
    case '?' :
      if (AN_ERROR_OCCURRED()) {
        wid_console_raise(g);
      } else {
        wid_cfg_help_select(g);
      }
      break;

    case SDLK_ESCAPE :
      if ((w != nullptr) && (static_cast< bool >(w->visible))) {
        wid_hide(g, w);
      }

      //
      // Need this so the console gets focus over the menu.
      //
      if ((w != nullptr) && (static_cast< bool >(w->visible))) {
        wid_set_focus(g, w);
        wid_focus_lock(g, w);
      } else {
        wid_unset_focus(g);
        wid_unset_focus_lock();
      }
      break;

    case SDLK_TAB :
    case SDLK_RETURN :
    case SDLK_DOWN :
    case SDLK_RIGHT :  wid_set_next_focus(g); break;

    case SDLK_UP :
    case SDLK_LEFT :   wid_set_prev_focus(g); break;

    default :
      if ((wid_console_window != nullptr) && (static_cast< bool >(wid_console_window->visible))) {
        wid_console_receive_input(g, wid_console_input_line, key);
      }
      break;
  }

  return true;
}

auto wid_find_at(int x, int y) -> Widp
{
  TRACE();

  auto *w = wid_on_screen_at[ x ][ y ];
  if (w == nullptr) [[unlikely]] {
    return nullptr;
  }

  if (wid_ignore_being_destroyed(w)) {
    return nullptr;
  }

  return w;
}

static auto wid_key_down_handler_at(Gamep g, Widp w, int x, int y, uint8_t strict) -> Widp
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    return nullptr;
  }

  if (! static_cast< bool >(w->visible)) {
    return nullptr;
  }

  if (wid_ignore_events(w)) {
    return nullptr;
  }

  if (static_cast< bool >(strict)) {
    if ((x < w->abs_tl.x) || (y < w->abs_tl.y) || (x > w->abs_br.x) || (y > w->abs_br.y)) {
      return nullptr;
    }
  }

  {
    for (auto &iter : w->children_display_sorted) {
      auto *child = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(child) != wid_get_top_parent(wid_focus_locked))) {
        continue;
      }

      Widp closer_match = wid_key_down_handler_at(g, child, x, y, 1U /* strict */);
      if (closer_match != nullptr) {
        return closer_match;
      }
    }
  }

  {
    for (auto &iter : w->children_display_sorted) {
      auto *child = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(child) != wid_get_top_parent(wid_focus_locked))) {
        continue;
      }

      Widp closer_match = wid_key_down_handler_at(g, child, x, y, 0U /* strict */);
      if (closer_match != nullptr) {
        return closer_match;
      }
    }

    if (w->on_key_down != nullptr) {
      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
        return nullptr;
      }

      return w;
    }
  }

  w = wid_get_top_parent(w);
  if (w->on_key_down != nullptr) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  return nullptr;
}

static auto wid_key_up_handler_at(Gamep g, Widp w, int x, int y, uint8_t strict) -> Widp
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    return nullptr;
  }

  if (! static_cast< bool >(w->visible)) {
    return nullptr;
  }

  if (wid_ignore_events(w)) {
    return nullptr;
  }

  if (static_cast< bool >(strict)) {
    if ((x < w->abs_tl.x) || (y < w->abs_tl.y) || (x > w->abs_br.x) || (y > w->abs_br.y)) {
      return nullptr;
    }
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(child) != wid_get_top_parent(wid_focus_locked))) {
      continue;
    }

    Widp closer_match = wid_key_up_handler_at(g, child, x, y, 1U /* strict */);
    if (closer_match != nullptr) {
      return closer_match;
    }
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(child) != wid_get_top_parent(wid_focus_locked))) {
      continue;
    }

    Widp closer_match = wid_key_up_handler_at(g, child, x, y, 0U /* strict */);
    if (closer_match != nullptr) {
      return closer_match;
    }
  }

  if (w->on_key_up != nullptr) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  return nullptr;
}

static auto wid_joy_button_handler_at(Gamep g, Widp w, int x, int y, uint8_t strict) -> Widp
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    return nullptr;
  }

  if (! static_cast< bool >(w->visible)) {
    return nullptr;
  }

  if (wid_ignore_events(w)) {
    return nullptr;
  }

  if (static_cast< bool >(strict)) {
    if ((x < w->abs_tl.x) || (y < w->abs_tl.y) || (x > w->abs_br.x) || (y > w->abs_br.y)) {
      return nullptr;
    }
  }

  for (auto &iter : w->children_display_sorted) {
    auto *child = iter.second;

    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(child) != wid_get_top_parent(wid_focus_locked))) {
      continue;
    }

    Widp closer_match = wid_joy_button_handler_at(g, child, x, y, 1U /* strict */);
    if (closer_match != nullptr) {
      return closer_match;
    }
  }

  if (w->on_joy_button != nullptr) {
    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      return nullptr;
    }

    return w;
  }

  return nullptr;
}

static void wid_children_move_delta_internal(Gamep g, Widp w, int dx, int dy)
{
  TRACE();

  //
  // Make sure you can't move a wid outside the parents box.
  //
  Widp p = w->parent;
  if (p != nullptr) {
    if (wid_get_moveable_bounded(w)) {
      if (wid_get_tl_x(w) + dx < wid_get_tl_x(p)) {
        dx = wid_get_tl_x(p) - wid_get_tl_x(w);
      }

      if (wid_get_tl_y(w) + dy < wid_get_tl_y(p)) {
        dy = wid_get_tl_y(p) - wid_get_tl_y(w);
      }

      if (wid_get_br_x(w) + dx > wid_get_br_x(p)) {
        dx = wid_get_br_x(p) - wid_get_br_x(w);
      }

      if (wid_get_br_y(w) + dy > wid_get_br_y(p)) {
        dy = wid_get_br_y(p) - wid_get_br_y(w);
      }
    }
  }

  w->key.tl.x += dx;
  w->key.tl.y += dy;
  w->key.br.x += dx;
  wid_tree_detach(w);
  w->key.br.y += dy;
  wid_tree_attach(w);

  for (auto &iter : w->tree2_children_unsorted) {
    auto *child = iter.second;

    wid_children_move_delta_internal(g, child, dx, dy);
  }
}

static void wid_move_delta_internal(Gamep g, Widp w, int dx, int dy)
{
  TRACE();

  wid_tree_detach(w);

  //
  // Make sure you can't move a wid outside the parents box.
  //
  Widp p = w->parent;
  if (p != nullptr) {
    if (wid_get_moveable_bounded(w)) {
      if (wid_get_tl_x(w) + dx < wid_get_tl_x(p)) {
        dx = wid_get_tl_x(p) - wid_get_tl_x(w);
      }

      if (wid_get_tl_y(w) + dy < wid_get_tl_y(p)) {
        dy = wid_get_tl_y(p) - wid_get_tl_y(w);
      }

      if (wid_get_br_x(w) + dx > wid_get_br_x(p)) {
        dx = wid_get_br_x(p) - wid_get_br_x(w);
      }

      if (wid_get_br_y(w) + dy > wid_get_br_y(p)) {
        dy = wid_get_br_y(p) - wid_get_br_y(w);
      }
    }
  }

  w->key.tl.x += dx;
  w->key.tl.y += dy;
  w->key.br.x += dx;
  w->key.br.y += dy;
  wid_tree_attach(w);

  std::vector< Widp > worklist;

  {
    for (auto &iter : w->tree2_children_unsorted) {
      auto *c = iter.second;
      worklist.push_back(c);
    }
  }

  for (auto &child : worklist) {
    wid_children_move_delta_internal(g, child, dx, dy);
  }
}

void wid_move_delta(Gamep g, Widp w, int dx, int dy)
{
  TRACE();

  wid_move_delta_internal(g, w, dx, dy);

  wid_update_internal(g, w);
}

void wid_resize(Gamep g, Widp w, int width, int height)
{
  TRACE();

  wid_tree_detach(w);

  if (width != -1) {
    w->key.br.x = w->key.tl.x + width;
  }
  if (height != -1) {
    w->key.br.y = w->key.tl.y + height;
  }

  wid_tree_attach(w);
}

void wid_move_delta_pct(Gamep g, Widp w, float dx, float dy)
{
  TRACE();

  if (w->parent == nullptr) {
    dx *= static_cast< float >(TERM_WIDTH);
    dy *= static_cast< float >(TERM_HEIGHT);
  } else {
    dx *= wid_get_width(w->parent);
    dy *= wid_get_height(w->parent);
  }

  wid_move_delta_internal(g, w, static_cast< int >(dx), static_cast< int >(dy));

  wid_update_internal(g, w);
}

void wid_move_to(Gamep g, Widp w, int x, int y)
{
  TRACE();

  if (w->parent != nullptr) {
    wid_move_delta(g, w, x - wid_get_tl_x(w->parent), y - wid_get_tl_y(w->parent));
  } else {
    wid_move_delta(g, w, x - wid_get_tl_x(w), y - wid_get_tl_y(w));
  }
}

void wid_move_to_bottom(Gamep g, Widp w)
{
  TRACE();

  if (w->parent != nullptr) {
    wid_move_delta(g, w, 0, wid_get_br_y(w->parent) - wid_get_br_y(w));
  } else {
    wid_move_delta(g, w, 0, TERM_HEIGHT - wid_get_br_y(w));
  }
}

void wid_move_to_y_off(Gamep g, Widp w, int off)
{
  TRACE();
  if (g == nullptr) {
    ERR("no game pointer");
    return;
  }
  wid_move_delta(g, w, 0, off);
}

void wid_move_to_left(Gamep g, Widp w)
{
  TRACE();

  if (w->parent != nullptr) {
    wid_move_delta(g, w, wid_get_tl_x(w->parent) - wid_get_tl_x(w), 0);
  } else {
    wid_move_delta(g, w, -wid_get_tl_x(w), 0);
  }
}

void wid_move_to_right(Gamep g, Widp w)
{
  TRACE();

  if (w->parent != nullptr) {
    wid_move_delta(g, w, wid_get_br_x(w->parent) - wid_get_br_x(w), 0);
  } else {
    wid_move_delta(g, w, TERM_WIDTH - wid_get_br_x(w), 0);
  }
}

void wid_move_to_vert_pct(Gamep g, Widp w, float pct)
{
  TRACE();

  float const pheight = wid_get_br_y(w->parent) - wid_get_tl_y(w->parent);
  float const at      = (wid_get_tl_y(w) - wid_get_tl_y(w->parent)) / pheight;
  float const delta   = (pct - at) * pheight;

  wid_move_delta(g, w, 0, static_cast< int >(delta));
}

void wid_move_to_horiz_pct(Gamep g, Widp w, float pct)
{
  TRACE();

  float const pwidth = wid_get_br_x(w->parent) - wid_get_tl_x(w->parent);
  float const at     = (wid_get_tl_x(w) - wid_get_tl_x(w->parent)) / pwidth;
  float const delta  = (pct - at) * pwidth;

  wid_move_delta(g, w, static_cast< int >(delta), 0);
}

void wid_move_to_top(Gamep g, Widp w)
{
  TRACE();

  if (w->parent != nullptr) {
    wid_move_delta(g, w, 0, wid_get_tl_y(w->parent) - wid_get_tl_y(w));
  } else {
    wid_move_delta(g, w, 0, -wid_get_tl_y(w));
  }
}

static auto wid_joy_button_handler(Gamep g, int x, int y) -> Widp
{
  TRACE();

  for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
    auto *w = iter.second;

    if ((wid_focus_locked != nullptr) && (wid_get_top_parent(w) != wid_get_top_parent(wid_focus_locked))) {
      continue;
    }

    w = wid_joy_button_handler_at(g, w, x, y, 0U /* strict */);
    if (w == nullptr) [[unlikely]] {
      continue;
    }

    return w;
  }

  return nullptr;
}

//
// If no handler for this button, fake a mouse event.
//
void wid_fake_joy_button(Gamep g, int x, int y)
{
  TRACE();

  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_A ])) {
    wid_mouse_down(g, SDL_BUTTON_LEFT, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_B ])) {
    wid_mouse_down(g, SDL_BUTTON_RIGHT, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_X ])) {
    wid_mouse_down(g, SDL_BUTTON_RIGHT, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_Y ])) {
    wid_mouse_down(g, 2, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_TOP_LEFT ])) {
    wid_mouse_down(g, SDL_BUTTON_LEFT, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_TOP_RIGHT ])) {
    wid_mouse_down(g, SDL_BUTTON_RIGHT, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_LEFT_STICK_DOWN ])) {
    wid_mouse_down(g, SDL_BUTTON_LEFT, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_RIGHT_STICK_DOWN ])) {
    wid_mouse_down(g, SDL_BUTTON_RIGHT, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_START ])) {
    wid_mouse_down(g, SDL_BUTTON_LEFT, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_XBOX ])) {
    wid_mouse_down(g, SDL_BUTTON_LEFT, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_BACK ])) {
    wid_mouse_down(g, SDL_BUTTON_RIGHT, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_UP ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_DOWN ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_LEFT ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_RIGHT ])) {}
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_LEFT_FIRE ])) {
    wid_mouse_down(g, SDL_BUTTON_LEFT, x, y);
    return;
  }
  if (static_cast< bool >(sdl.joy_buttons[ SDL_JOY_BUTTON_RIGHT_FIRE ])) {
    wid_mouse_down(g, SDL_BUTTON_RIGHT, x, y);
    return;
  }
}

void wid_joy_button(Gamep g, int x, int y)
{
  TRACE();

  pixel_to_ascii(g, &x, &y);
  if (ascii_ok(x, y) == 0) {
    return;
  }

  ascii_mouse_x = x;
  ascii_mouse_y = y;

  //
  // Only if there is a change in status, send an event.
  //
  static std::array< ts_t, SDL_MAX_BUTTONS > ts;
  int                                        changed = 0;
  int                                        b       = 0;

  for (b = 0; b < SDL_MAX_BUTTONS; b++) {
    if (static_cast< bool >(sdl.joy_buttons[ b ])) {
      if (time_have_x_tenths_passed_since(2, ts[ b ])) {
        changed = 1;
        ts[ b ] = time_ms_cached();
      }
    }
  }

  if (changed == 0) {
    return;
  }

  Widp w {};

  w = wid_joy_button_handler(g, x, y);
  if (w == nullptr) [[unlikely]] {
    wid_fake_joy_button(g, x, y);
    return;
  }

  //
  // Raise on mouse.
  //
  if (w->on_joy_button != nullptr) {
    //
    // If the button doesn't eat the event, try the parent.
    //
    while (! (w->on_joy_button)(g, w, x, y)) {
      w = w->parent;

      while ((w != nullptr) && (w->on_joy_button == nullptr)) {
        w = w->parent;
      }

      if (w == nullptr) [[unlikely]] {
        wid_fake_joy_button(g, x, y);
        return;
      }
    }

    wid_set_focus(g, w);
    wid_set_mode(w, WID_MODE_ACTIVE);
    wid_raise(g, w);

    //
    // Move on mouse.
    //
    if (wid_get_moveable(w)) {
      wid_mouse_motion_begin(g, w, x, y);
      return;
    }

    return;
  }
  wid_fake_joy_button(g, x, y);

  if (wid_get_moveable(w)) {
    wid_set_mode(w, WID_MODE_ACTIVE);
    wid_raise(g, w);
    wid_mouse_motion_begin(g, w, x, y);
    return;
  }
}

static auto wid_key_down_handler(Gamep g, int x, int y) -> Widp
{
  TRACE();

  Widp w {};

  // con("key down.");
  w = wid_key_down_handler_at(g, wid_focus, x, y, 1U /* strict */);
  if (w != nullptr) {
    // con("%s %d.",to_cstring(w).c_str(),__LINE__);
    return w;
  }

  w = wid_key_down_handler_at(g, wid_get_top_parent(wid_focus), x, y, 0U /* strict */);
  if (w != nullptr) {
    // con("%s %d.",to_string(w).c_str(),__LINE__);
    return w;
  }

  w = wid_key_down_handler_at(g, wid_over, x, y, 1U /* strict */);
  if (w != nullptr) {
    // con("%s %d.",to_string(w).c_str(),__LINE__);
    return w;
  }

  w = wid_key_down_handler_at(g, wid_get_top_parent(wid_over), x, y, 0U /* strict */);
  if (w != nullptr) {
    // con("%s %d.",to_string(w).c_str(),__LINE__);
    return w;
  }

  {
    for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
      auto *c = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(c) != wid_get_top_parent(wid_focus_locked))) {
        // con("  focus is locked.");
        continue;
      }

      c = wid_key_down_handler_at(g, c, x, y, 1U /* strict */);
      if (c == nullptr) [[unlikely]] {
        continue;
      }
      // con("     got top level strict handler%s.",to_string(c).c_str());

      return c;
    }
  }

  {
    for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
      auto *c = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(c) != wid_get_top_parent(wid_focus_locked))) {
        // con("  focus is locked.");
        continue;
      }

      c = wid_key_down_handler_at(g, c, x, y, 0U /* strict */);
      if (c == nullptr) [[unlikely]] {
        continue;
      }

      // con("     got top level loose handler%s.",to_string(c));
      return c;
    }
  }

  return nullptr;
}

static auto wid_key_up_handler(Gamep g, int x, int y) -> Widp
{
  TRACE();

  Widp w {};

  w = wid_key_up_handler_at(g, wid_focus, x, y, 1U /* strict */);
  if (w != nullptr) {
    return w;
  }

  w = wid_key_up_handler_at(g, wid_get_top_parent(wid_focus), x, y, 0U /* strict */);
  if (w != nullptr) {
    return w;
  }

  w = wid_key_up_handler_at(g, wid_over, x, y, 1U /* strict */);
  if (w != nullptr) {
    return w;
  }

  w = wid_key_up_handler_at(g, wid_get_top_parent(wid_over), x, y, 0U /* strict */);
  if (w != nullptr) {
    return w;
  }

  {
    for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
      auto *c = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(c) != wid_get_top_parent(wid_focus_locked))) {
        continue;
      }

      c = wid_key_up_handler_at(g, c, x, y, 1U /* strict */);
      if (c == nullptr) [[unlikely]] {
        continue;
      }

      return c;
    }
  }

  {
    for (auto &iter : std::ranges::reverse_view(wid_top_level)) {
      auto *c = iter.second;

      if ((wid_focus_locked != nullptr) && (wid_get_top_parent(c) != wid_get_top_parent(wid_focus_locked))) {
        continue;
      }

      c = wid_key_up_handler_at(g, c, x, y, 0U /* strict */);
      if (c == nullptr) [[unlikely]] {
        continue;
      }

      return c;
    }
  }

  return nullptr;
}

#ifdef ENABLE_DEBUG_GFX_GL_BLEND
int vals[] = {
    GL_ZERO,
    GL_ONE,
    GL_CONSTANT_ALPHA,
    GL_CONSTANT_COLOR,
    GL_DST_ALPHA,
    GL_DST_COLOR,
    GL_ONE_MINUS_CONSTANT_ALPHA,
    GL_ONE_MINUS_CONSTANT_ALPHA,
    GL_ONE_MINUS_CONSTANT_COLOR,
    GL_ONE_MINUS_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_ONE_MINUS_DST_COLOR,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_ONE_MINUS_SRC_COLOR,
    GL_SRC_ALPHA,
    GL_SRC_ALPHA_SATURATE,
    GL_SRC_COLOR,
};

std::string vals_str[] = {
    "GL_ZERO",
    "GL_ONE",
    "GL_CONSTANT_ALPHA",
    "GL_CONSTANT_COLOR",
    "GL_DST_ALPHA",
    "GL_DST_COLOR",
    "GL_ONE_MINUS_CONSTANT_ALPHA",
    "GL_ONE_MINUS_CONSTANT_ALPHA",
    "GL_ONE_MINUS_CONSTANT_COLOR",
    "GL_ONE_MINUS_DST_ALPHA",
    "GL_ONE_MINUS_DST_ALPHA",
    "GL_ONE_MINUS_DST_COLOR",
    "GL_ONE_MINUS_SRC_ALPHA",
    "GL_ONE_MINUS_SRC_COLOR",
    "GL_SRC_ALPHA",
    "GL_SRC_ALPHA_SATURATE",
    "GL_SRC_COLOR",
};

int g_blend_a;
int g_blend_b;
#endif

#if 0
  //
  // Paste this code prior to the blend in question
  //
  extern int vals[];
  extern std::string vals_str[];
  extern int g_blend_a;
  extern int g_blend_b;
  con("glBlendFunc(%s, %s)", vals_str[g_blend_a].c_str(), vals_str[g_blend_b].c_str());
  glBlendFunc(vals[g_blend_a], vals[g_blend_b]);
#endif

void wid_key_down(Gamep g, const struct SDL_Keysym *key, int x, int y)
{
  TRACE();

  Widp w {};

  pixel_to_ascii(g, &x, &y);
  if (ascii_ok(x, y) == 0) {
    DBG("SDL: Keyboard: bad coord %d,%d", x, y);
    return;
  }
  ascii_mouse_x = x;
  ascii_mouse_y = y;

#ifdef ENABLE_DEBUG_GFX_GL_BLEND
  if (wid_event_to_char(key) == '+') {
    con("ENABLE_DEBUG_GFX_GL_BLEND +");
    usleep(50);
    g_blend_a++;
    if (g_blend_a >= (int) ARRAY_SIZE(vals)) {
      g_blend_a = 0;
      g_blend_b++;
      if (g_blend_b >= (int) ARRAY_SIZE(vals)) {
        g_blend_b = 0;
        ERR("wrapped");
      }
    }
    return;
  }

  if (wid_event_to_char(key) == '-') {
    con("ENABLE_DEBUG_GFX_GL_BLEND -");
    usleep(50);
    g_blend_a--;
    if (g_blend_a < 0) {
      g_blend_a = (int) ARRAY_SIZE(vals) - 1;
      g_blend_b--;
      if (g_blend_b < 0) {
        g_blend_b = (int) ARRAY_SIZE(vals) - 1;
      }
    }
    return;
  }
#endif
  if ((wid_focus != nullptr) && ! wid_is_hidden(wid_focus) && ((wid_focus->on_key_down) != nullptr)) {
    if ((wid_focus->on_key_down)(g, wid_focus, key)) {
      if (wid_focus != nullptr) {
        DBG("WID: key grabbed by focused wid: %s at (%d,%d)", wid_focus->name.c_str(), ascii_mouse_x, ascii_mouse_y);
      }
      //
      // Do not raise, gets in the way of popups the callback creates.
      //
      return;
    }

    w = wid_focus;

    goto try_parent;
  }

  w = wid_key_down_handler(g, x, y);
  if (w == nullptr) [[unlikely]] {
    //
    // If no-one handles it, feed it to the default handler, the console.
    //
    DBG("SDL: Keyboard: no one handled the event");
    (void) wid_receive_unhandled_input(g, key);
    return;
  }

  {
    DBG("WID: Key over by wid: %s for (%d,%d)", w->name.c_str(), ascii_mouse_x, ascii_mouse_y);
    if ((w->on_key_down)(g, w, key)) {
      DBG("WID: Key grabbed by wid: %s for (%d,%d)", w->name.c_str(), ascii_mouse_x, ascii_mouse_y);
      //
      // Do not raise, gets in the way of popups the callback creates.
      //
      // con("widget did not handle");
      return;
    }
  }

try_parent:
  if (w != nullptr) {
    w = w->parent;

    //
    // Ripple the key event to the parent so global things like pressing
    // escape can do things.
    //
    while (w != nullptr) {
      if (w->on_key_down != nullptr) {
        if ((w->on_key_down)(g, w, key)) {
          DBG("WID: key grabbed by wid: %s for (%d,%d)", w->name.c_str(), ascii_mouse_x, ascii_mouse_y);
          //
          // Do not raise, gets in the way of popups the callback
          // creates.
          //
          return;
        }
      }

      w = w->parent;
    }
  }

  //
  // If no-one handles it, feed it to the default handler, the console.
  //
  {
    DBG("WID: key is unhandled");
    (void) wid_receive_unhandled_input(g, key);
  }
}

void wid_key_up(Gamep g, const struct SDL_Keysym *key, int x, int y)
{
  TRACE();

  Widp w {};

  pixel_to_ascii(g, &x, &y);
  if (ascii_ok(x, y) == 0) {
    return;
  }
  ascii_mouse_x = x;
  ascii_mouse_y = y;

  if ((wid_focus != nullptr) && ! wid_is_hidden(wid_focus) && ((wid_focus->on_key_up) != nullptr)) {

    if ((wid_focus->on_key_up)(g, wid_focus, key)) {
      if (wid_focus != nullptr) {
        wid_set_mode(wid_focus, WID_MODE_ACTIVE);
      }

      //
      // Do not raise, gets in the way of popups the callback creates.
      //
      return;
    }

    w = wid_focus;

    goto try_parent;
  }

  w = wid_key_up_handler(g, x, y);
  if (w == nullptr) [[unlikely]] {
    //
    // If no-one handles it, drop it. We only hand key down to the
    // console.
    //
    return;
  }

  if ((w->on_key_up)(g, w, key)) {
    wid_set_mode(w, WID_MODE_ACTIVE);

    //
    // Do not raise, gets in the way of popups the callback creates.
    //
    return;
  }

try_parent:
  if (w != nullptr) {
    w = w->parent;

    //
    // Ripple the key event to the parent so global things like pressing
    // escape can do things.
    //
    while (w != nullptr) {
      if (w->on_key_up != nullptr) {
        if ((w->on_key_up)(g, w, key)) {
          wid_set_mode(w, WID_MODE_ACTIVE);

          //
          // Do not raise, gets in the way of popups the callback
          // creates.
          //
          return;
        }
      }

      w = w->parent;
    }
  }
}

//
// Get the onscreen co-ords of the widget, clipped to the parent.
//
void wid_get_abs_coords(Widp w, int *tlx, int *tly, int *brx, int *bry)
{
  TRACE();

  Widp p {};

  *tlx = wid_get_tl_x(w);
  *tly = wid_get_tl_y(w);
  *brx = wid_get_br_x(w);
  *bry = wid_get_br_y(w);

  p = w->parent;
  while (p != nullptr) {
    *tlx += p->offset.x;
    *tly += p->offset.y;
    *brx += p->offset.x;
    *bry += p->offset.y;
    p = p->parent;
  }

#if 0
  int ptlx;
  int ptly;
  int pbrx;
  int pbry;

  if (w->parent) {
    wid_get_abs_coords(w->parent, &ptlx, &ptly, &pbrx, &pbry);

    if (ptlx > *tlx) {
      *tlx = ptlx;
    }

    if (ptly > *tly) {
      *tly = ptly;
    }

    if (pbrx < *brx) {
      *brx = pbrx;
    }

    if (pbry < *bry) {
      *bry = pbry;
    }
  }
#endif

  w->abs_tl.x = *tlx;
  w->abs_tl.y = *tly;
  w->abs_br.x = *brx;
  w->abs_br.y = *bry;
}

//
// Get the onscreen co-ords of the widget, clipped to the parent.
//
void wid_get_abs(Widp w, int *x, int *y)
{
  TRACE();

  int tlx = 0;
  int tly = 0;
  int brx = 0;
  int bry = 0;

  wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);

  *x = (tlx + brx) / 2;
  *y = (tly + bry) / 2;
}

void wid_get_pct(Widp w, float *px, float *py)
{
  TRACE();

  int x = 0;
  int y = 0;

  wid_get_abs(w, &x, &y);

  *px = static_cast< float >(x) / static_cast< float >(TERM_WIDTH);
  *py = static_cast< float >(y) / static_cast< float >(TERM_HEIGHT);
}

//
// Display one wid and its children
//
static int  wid_total_count;
static void wid_display(Gamep g, Widp w, uint8_t disable_scissor, uint8_t *updated_scissors, int clip)
{
  TRACE();

  wid_total_count++;
  int     clip_height   = 0;
  int     clip_width    = 0;
  uint8_t hidden        = 0;
  uint8_t always_hidden = 0;
  int     owidth        = 0;
  int     oheight       = 0;
  int     otlx          = 0;
  int     otly          = 0;
  int     obrx          = 0;
  int     obry          = 0;
  int     tlx           = 0;
  int     tly           = 0;
  int     brx           = 0;
  int     bry           = 0;
#if 0
  Widp p {};
#endif

#if 0
  if (!w->parent) {
    wid_dump(w, 0);
  }
#endif

  //
  // Bounding box for drawing the wid. Co-ords are negative as we
  // flipped the screen
  //
  tlx = w->abs_tl.x;
  tly = w->abs_tl.y;
  brx = w->abs_br.x;
  bry = w->abs_br.y;

  //
  // If we're clipped out of existence! then nothing to draw. This can
  // be outside the bounds of a widget or if at the top level, off screeen.
  //
  if (clip != 0) {
    clip_width = brx - tlx;
    if (clip_width < 0) {
      return;
    }

    clip_height = bry - tly;
    if (clip_height < 0) {
      return;
    }
  }

  hidden        = static_cast< uint8_t >(wid_is_hidden(w));
  always_hidden = static_cast< uint8_t >(wid_is_always_hidden(w));

  if (static_cast< bool >(always_hidden)) {
    //
    // Always render. Not hidden yet.
    //
    return;
  }
  if (static_cast< bool >(hidden)) {
    //
    // Hidden or parent is hidden.
    //
    return;
  }

  //
  // Record the original pre clip sizes for text centering.
  //
  otlx = wid_get_tl_x(w);
  otly = wid_get_tl_y(w);
  obrx = wid_get_br_x(w);
  obry = wid_get_br_y(w);

  wid_get_abs_coords(w, &otlx, &otly, &obrx, &obry);
#if 0
  p = w->parent;
  while (p) {
    otlx += p->offset.x;
    otly += p->offset.y;
    obrx += p->offset.x;
    obry += p->offset.y;
    p = p->parent;
  }
#endif

  //
  // Inclusive width
  //
  owidth  = obrx - otlx + 1;
  oheight = obry - otly + 1;

  //
  // If this widget was active and the time has elapsed, make it normal.
  //
  auto mode = wid_get_mode(w);
  if (mode == WID_MODE_ACTIVE) {
    if ((wid_time - w->ts_last_mode_change) > 250) {
      wid_set_mode(w, WID_MODE_NORMAL);
    }
  }

  //
  // If inputting text, show a cursor.
  //
  std::string text;

  if (wid_get_show_cursor(w)) {
    text = wid_get_text_with_cursor(w);
  } else {
    text = wid_get_text(w);
  }

  if (static_cast< bool >(w->disable_scissors)) {
    disable_scissor = 1U;
  }

  //
  // Should be no need for scissors if you do not have any children
  // or are not the top level wid.
  //
  if (! static_cast< bool >(disable_scissor)) {
#if 0
  //
  // Why would we not always want scissors on?
  //
  if (!w->children_display_sorted.empty() || !w->parent || w->show_cursor) {
#endif
    //
    // Tell the parent we are doing scissors so they can re-do
    // their own scissors.
    //
    if (updated_scissors != nullptr) {
      *updated_scissors = 1U;
    }
#if 0

  if (w->parent) {

    if (ptlx > *tlx) {
      *tlx = ptlx;
    }

    if (ptly > *tly) {
      *tly = ptly;
    }

    if (pbrx < *brx) {
      *brx = pbrx;
    }

    if (pbry < *bry) {
      *bry = pbry;
    }
  }
#endif

    auto sciss_tlx = tlx;
    auto sciss_tly = tly;
    auto sciss_brx = brx;
    auto sciss_bry = bry;

    auto *p = w->parent;
    while (p != nullptr) {
      int ptlx = 0;
      int ptly = 0;
      int pbrx = 0;
      int pbry = 0;
      wid_get_abs_coords(p, &ptlx, &ptly, &pbrx, &pbry);

      sciss_tlx = std::max(ptlx, sciss_tlx);
      sciss_brx = std::min(pbrx, sciss_brx);
      sciss_tly = std::max(ptly, sciss_tly);
      sciss_bry = std::min(pbry, sciss_bry);

      p = p->parent;
    }

    wid_set_scissors(sciss_tlx, sciss_tly, sciss_brx, sciss_bry);
#if 0
    }
#endif
  }

  auto wid_width  = wid_get_width(w);
  auto wid_height = wid_get_height(w);

  spoint tl = {};
  spoint br = {};

  tl.x = otlx;
  tl.y = otly;
  br.x = otlx + wid_width;
  br.y = otly + wid_height;

  box_args w_box_args = {};
  w_box_args.x        = tl.x;
  w_box_args.y        = tl.y;
  w_box_args.width    = (br.x - tl.x);
  w_box_args.height   = (br.y - tl.y);

  if (w == wid_over) {
    w_box_args.over = 1;

    if (static_cast< bool >(w->cfg[ WID_MODE_OVER ].color_set[ WID_COLOR_TEXT_FG ])) {
      w_box_args.col_text = w->cfg[ WID_MODE_OVER ].colors[ WID_COLOR_TEXT_FG ];
    } else {
      w_box_args.col_text = w->cfg[ WID_MODE_NORMAL ].colors[ WID_COLOR_TEXT_FG ];
    }

    if (static_cast< bool >(w->cfg[ WID_MODE_OVER ].color_set[ WID_COLOR_BG ])) {
      w_box_args.col_bg = w->cfg[ WID_MODE_OVER ].colors[ WID_COLOR_BG ];
    } else {
      w_box_args.col_bg = w->cfg[ WID_MODE_NORMAL ].colors[ WID_COLOR_BG ];
    }
  } else {
    w_box_args.col_text = w->cfg[ WID_MODE_NORMAL ].colors[ WID_COLOR_TEXT_FG ];
    w_box_args.col_bg   = w->cfg[ WID_MODE_NORMAL ].colors[ WID_COLOR_BG ];
  }

  if (static_cast< bool >(w->square)) {
    ascii_put_box(w_box_args, wid_get_style(w), w->tiles, "");
  } else {
    // shape none
  }

  {
    //
    // Set the tiles this widget is over, taking into account the scissors
    // if the parent is resized.
    //
    // con("%d,%d to @%d,%d %s %p", tl.x, tl.y, br.x, br.y, w->name.c_str(), w);
    for (auto x = tl.x; x < br.x; x++) {
      if ((ascii_x_ok(x) == 0)) [[unlikely]] {
        continue;
      }
      for (auto y = tl.y; y < br.y; y++) {
        if ((ascii_y_ok(y) == 0)) [[unlikely]] {
          continue;
        }

        if (ascii_ok_for_scissors(x, y) != 0) {
          wid_on_screen_at[ x ][ y ] = w;
        }
      }
    }
  }

  if (w->on_display != nullptr) {
    (w->on_display)(g, w, tl, br);
  }

  if (! text.empty()) {
    int x      = 0;
    int y      = 0;
    int xpc    = 0;
    int ypc    = 0;
    int width  = 0;
    int height = 0;

    //
    // Manually specified text position.
    //
    width  = ascii_strlen(text);
    height = 0;

    if (wid_get_text_pos(w, &xpc, &ypc)) {
      x = (owidth * xpc) - (width / 2) + otlx;
      y = (oheight * ypc) - (height / 2) + otly;
    } else {
      //
      // Position the text
      //
      if ((width > owidth) && (static_cast< bool >(w->show_cursor))) {
        //
        // If the text is too big, center it on the cursor.
        //
        x = ((owidth - width) / 2) + otlx;

        uint32_t const c_width = static_cast< int >(width / static_cast< float >(text.length()));

        x -= (w->cursor - (text.length() / 2)) * c_width;
      } else if (wid_get_text_lhs(w)) {
        x = otlx;
      } else if (wid_get_text_centerx(w)) {
        x = ((owidth - width) / 2) + otlx;
      } else if (wid_get_text_rhs(w)) {
        x = obrx - width + 1;
      } else {
        x = ((owidth - width) / 2) + otlx;
      }

      if (wid_get_text_top(w)) {
        y = otly;
      } else if (wid_get_text_centery(w)) {
        y = ((oheight - height) / 2) + otly;
      } else if (wid_get_text_bot(w)) {
        y = obry - height + 1;
      } else {
        y = ((oheight - height) / 2) + otly;
      }
    }

    if ((static_cast< bool >(w->cfg[ mode ].color_set[ WID_COLOR_TEXT_FG ]))
        && (static_cast< bool >(w->cfg[ mode ].color_set[ WID_COLOR_TEXT_BG ]))) {
      ascii_putf_internal2(x, y, w_box_args.col_text, w_box_args.col_bg, text);
    } else if (static_cast< bool >(w->cfg[ mode ].color_set[ WID_COLOR_TEXT_FG ])) {
      ascii_putf_internal2(x, y, w_box_args.col_text, WHITE, text);
    } else if (static_cast< bool >(w->cfg[ mode ].color_set[ WID_COLOR_TEXT_BG ])) {
      ascii_putf_internal2(x, y, WHITE, w_box_args.col_bg, text);
    } else {
      //
      // Do not use COLOR_NONE - it leads to other textures leaking in the background.
      // Not sure why.
      //
      ascii_putf_internal2(x, y, w_box_args.col_text, WHITE, text);
    }
  }

  for (auto &iter : w->children_display_sorted) {

    auto *child = iter.second;

    uint8_t child_updated_scissors = 0U;

    wid_display(g, child, disable_scissor, &child_updated_scissors, clip);

    //
    // Need to re-enforce the parent's scissors if the child did
    // their own bit of scissoring?
    //
    if ((! static_cast< bool >(disable_scissor)) && (static_cast< bool >(child_updated_scissors))) {
      wid_set_scissors(tlx, tly, brx, bry);
    }
  }
}

//
// Delayed destroy?
//
static void wid_gc(Gamep g, Widp w)
{
  TRACE();

  wid_dbg(w, "gc");

  if (static_cast< bool >(w->being_destroyed)) {
    wid_destroy_immediate(g, w);
    return;
  }

  if ((w->destroy_when != 0) && (wid_time >= w->destroy_when)) {
    wid_destroy(g, &w);
  }
}

//
// Check widgets are not getting out of hand
//
void wid_sanity_check(Gamep g)
{
  TRACE();

  //
  // Sanity check widgets lists are not getting large
  //
  IF_DEBUG
  {
    if (static_cast< int >(wid_top_level.size()) > 1000) {
      CROAK("widget size getting large for: wid_top_level %d", (int) wid_top_level.size());
    }
    if (static_cast< int >(wid_global.size()) > 1000) {
      CROAK("widget size getting large for: wid_global %d", (int) wid_global.size());
    }
    if (static_cast< int >(wid_top_level2.size()) > 1000) {
      CROAK("widget size getting large for: wid_top_level2 %d", (int) wid_top_level2.size());
    }
    if (static_cast< int >(wid_top_level3.size()) > 1000) {
      CROAK("widget size getting large for: wid_top_level3 %d", (int) wid_top_level3.size());
    }
    if (static_cast< int >(wid_top_level4.size()) > 1000) {
      CROAK("widget size getting large for: wid_top_level4 %d", (int) wid_top_level4.size());
    }
    if (static_cast< int >(wid_tick_top_level.size()) > 1000) {
      CROAK("widget size getting large for: wid_tick_top_level %d", (int) wid_tick_top_level.size());
    }
    if (static_cast< int >(wid_pre_tick_top_level.size()) > 1000) {
      CROAK("widget size getting large for: wid_pre_tick_top_level %d", (int) wid_pre_tick_top_level.size());
    }
  }
}

//
// Do stuff for all widgets.
//
void wid_gc_all(Gamep g)
{
  TRACE();

  for (;;) {
    if (wid_top_level4.empty()) {
      return;
    }
    auto  i = wid_top_level4.begin();
    auto *w = i->second;

    wid_gc(g, w);
  }
}

//
// Do stuff for all widgets.
//
static void wid_tick_all(Gamep g)
{
  TRACE();

  wid_time = time_ms_cached();

  std::vector< Widp > work;
  for (auto &iter : wid_tick_top_level) {
    auto *w = iter.second;
    work.push_back(w);
  }

  for (auto &w : work) {
    if (w->on_tick == nullptr) {
      ERR("widget on ticker tree, but no callback set");
    }

    (w->on_tick)(g, w);
  }
}

//
// Do stuff for all widgets.
//
static void wid_pre_tick_all(Gamep g)
{
  TRACE();

  wid_time = time_ms_cached();

  std::vector< Widp > work;
  for (auto &iter : wid_pre_tick_top_level) {
    auto *w = iter.second;
    work.push_back(w);
  }

  for (auto &w : work) {
    if (w->on_pre_tick == nullptr) {
      ERR("widget on pre_ticker tree, but no callback set");
    }

    (w->on_pre_tick)(g, w);
  }
}

//
// Display all widgets
//
void wid_display_all(Gamep g)
{
  TRACE();

  wid_pre_tick_all(g);

  gl_enter_2d_mode(g, game_window_pix_width_get(g), game_window_pix_height_get(g));
  blit_fbo_bind_locked(FBO_WID);
  gl_clear();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  ascii_clear_display();

  wid_tick_all(g);

  // con("---------------------------------");

  wid_on_screen_at = {};

  wid_total_count = 0;

  auto wid_top_level_copy = wid_top_level;
  for (auto &iter : wid_top_level_copy) {
    auto *w = iter.second;

    if (w->parent != nullptr) {}

    if (wid_is_hidden(w)) {
      continue;
    }

#if 0
auto last = wid_total_count;
#endif
    wid_display(g, w, 0U /* disable_scissors */, nullptr /* updated_scissors */, 1);
#if 0
printf("%s %d\n", wid_name(w).c_str(), wid_total_count - last);
#endif
  }

#if 0
printf("========================================= %d\n", wid_total_count);
#endif

  if (wid_total_count > 5000) {
    topcon("too many widgets %d", wid_total_count);
  }

#if 0
  for (auto y = 0; y < TERM_HEIGHT; y++) {
    for (auto x = 0; x < TERM_WIDTH; x++) {
      if (wid_on_screen_at[x][y)] {
        putchar('W');
      } else {
        putchar(' ');
      }
    }
    putchar('\n');
  }
#endif

  ascii_clear_scissors();

#ifdef ENABLE_DEBUG_UI_FOCUS
  if (wid_focus) {
    ascii_putf(0, TERM_HEIGHT - 4, WHITE, GRAY, "focus %s", to_string(wid_focus).c_str());
  }

  if (wid_over) {
    ascii_putf(0, TERM_HEIGHT - 3, WHITE, GRAY, "over  %s", to_string(wid_over).c_str());
  }
#endif

  //
  // FPS counter.
  //
  if (game_fps_counter_get(g)) {
    ascii_putf(TERM_WIDTH - 7, TERM_HEIGHT - 1, GREEN, BLACK, "%u FPS", game_fps_value_get(g));
  }

  ascii_display(g);

  //
  // Need this to reset wid_over after display
  //
  wid_update_mouse(g);

  blit_fbo_unbind_locked();
  gl_enter_2d_mode(g, game_window_pix_width_get(g), game_window_pix_height_get(g));
}

auto wid_is_hidden(Widp w) -> bool
{
  TRACE();

  if (w == nullptr) [[unlikely]] {
    return false;
  }

  if (static_cast< bool >(w->hidden)) {
    return true;
  }

  while (w->parent != nullptr) {
    w = w->parent;

    if (static_cast< bool >(w->hidden)) {
      return true;
    }
  }

  return false;
}

auto wid_is_always_hidden(const Widp w) -> bool
{
  TRACE();

  return w->always_hidden != 0;
}

void wid_move_to_pct(Gamep g, Widp w, float x, float y)
{
  TRACE();

  if (w->parent == nullptr) {
    x *= static_cast< float >(TERM_WIDTH);
    y *= static_cast< float >(TERM_HEIGHT);
  } else {
    x *= wid_get_width(w->parent);
    y *= wid_get_height(w->parent);
  }

  float const dx = x - wid_get_tl_x(w);
  float const dy = y - wid_get_tl_y(w);

  wid_move_delta(g, w, static_cast< int >(dx), static_cast< int >(dy));
}

void wid_move_to_abs(Gamep g, Widp w, int x, int y)
{
  TRACE();

  int const dx = x - wid_get_tl_x(w);
  int const dy = y - wid_get_tl_y(w);

  wid_move_delta(g, w, dx, dy);
}

void wid_move_to_pct_centered(Gamep g, Widp w, float ox, float oy)
{
  TRACE();

  float const x = ox;
  float const y = oy;

  int tlx  = 0;
  int tly  = 0;
  int brx  = 0;
  int bry  = 0;
  int ptlx = 0;
  int ptly = 0;
  int pbrx = 0;
  int pbry = 0;

  if (w->parent == nullptr) {
    ptlx = 0;
    ptly = 0;
    pbrx = TERM_WIDTH - 1;
    pbry = TERM_HEIGHT - 1;
    wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);
  } else {
    wid_get_abs_coords(w->parent, &ptlx, &ptly, &pbrx, &pbry);
    wid_get_abs_coords(w, &tlx, &tly, &brx, &bry);
  }

  auto pw     = (pbrx - ptlx) + 1;
  auto width  = (brx - tlx) + 1;
  auto ph     = (pbry - ptly) + 1;
  auto height = (bry - tly) + 1;

  auto nx = ptlx + static_cast< int >(floor(static_cast< float >(pw - width) * x));
  auto ny = ptly + static_cast< int >(floor(static_cast< float >(ph - height) * y));

  wid_move_to(g, w, nx, ny);
}

void wid_move_to_abs_centered(Gamep g, Widp w, int x, int y)
{
  TRACE();

  int dx = x - wid_get_tl_x(w);
  int dy = y - wid_get_tl_y(w);

  dx -= static_cast< int >(ceil((wid_get_br_x(w) - wid_get_tl_x(w)) / 2));
  dy -= static_cast< int >(ceil((wid_get_br_y(w) - wid_get_tl_y(w)) / 2));

  wid_move_delta(g, w, dx, dy);
}

auto wid_some_recent_event_occurred() -> bool
{
  TRACE();

  //
  // If a widget has just closed for example or been recreated, don't
  // immediately allow a double click on a chasm that was behind the
  // widget for example.
  //
  if (! time_have_x_tenths_passed_since(2, wid_last_over_event)) {
    // DBG("wid_some_recent_event_occurred: Too soon since last wid over event");
    return true;
  }

  return false;
}

static void wid_log_(Widp w, const char *fmt, va_list args)
{
  char buf[ MAXLONGSTR ];
  int  len = 0;

  VERIFY(MTYPE_WID, w);
  buf[ 0 ] = '\0';
  get_timestamp(buf, MAXLONGSTR);
  len = static_cast< int >(strlen(buf));
  snprintf(buf + len, MAXLONGSTR - len, "WID [%p/%s]: ", (void *) w, to_string(w).c_str());
  len = static_cast< int >(strlen(buf));
  vsnprintf(buf + len, MAXLONGSTR - len, fmt, args);

  putf(MY_STDOUT, buf);
}

void wid_dbg(Widp w, const char *fmt, ...)
{
  va_list args = {};

  if (! wid_safe()) {
    return;
  }

  VERIFY(MTYPE_WID, w);

  IF_NODEBUG2 { return; }

#ifndef ENABLE_DEBUG_UI
  return;
#endif

  va_start(args, fmt);
  wid_log_(w, fmt, args);
  va_end(args);
}
