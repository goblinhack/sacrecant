//
// Copyright goblinhack@gmail.com
//

#include "my_ascii.hpp"
#include "my_callstack.hpp"
#include "my_command.hpp"
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
#include "my_wid_tiles.hpp"
#include "my_wids.hpp"

class WidOptionsMenu
{
public:
  std::array< color, WID_COLOR_MAX >   colors    = {WHITE};
  std::array< uint8_t, WID_COLOR_MAX > color_set = {};
  int                                  style     = {};
  uint8_t                              style_set = {};
};

class TreeWidKey
{
public:
  TreeWidKey() = default;

  TreeWidKey(int vpriority, spoint vtl, spoint vbr, uint64_t vkey) : priority(vpriority), tl(vtl), br(vbr), key(vkey) {}

  ~TreeWidKey() = default;

  int priority {};

  //
  // The real position on the screen initially.
  //
  spoint tl {};
  spoint br {};

  //
  // Unique wid ID.
  //
  uint64_t key {};
};

struct TreeWidKeyCmp {
  auto operator()(const TreeWidKey &lhs, const TreeWidKey &rhs) const -> bool
  {
    if (lhs.priority < rhs.priority) {
      return true;
    }
    if (lhs.priority > rhs.priority) {
      return false;
    }

    if (lhs.br.y < rhs.br.y) {
      return true;
    }
    if (lhs.br.y > rhs.br.y) {
      return false;
    }

    if (lhs.key < rhs.key) {
      return true;
    }
    if (lhs.key > rhs.key) {
      return false;
    }

    return false;
  }
};

class WidKeyType
{
public:
  WidKeyType() = default;

  explicit WidKeyType(uint64_t v_val) : val(v_val) {}

  auto operator<(const WidKeyType &rhs) const -> bool { return (val < rhs.val); }

  uint64_t val {};
};

using wid_key_map_location = std::map< TreeWidKey, Widp, TreeWidKeyCmp >;
using wid_key_map_int      = std::map< WidKeyType, Widp >;

class Wid
{
public:
  Wid();
  ~Wid();

  WidKeyType tree_global_key;

  //
  // Sorted for display order.
  //
  TreeWidKey key;
  WidKeyType tree2_key;
  WidKeyType tree4_key;
  WidKeyType tree5_key;
  WidKeyType tree6_key;

  wid_key_map_location *in_tree_root {};
  wid_key_map_int      *in_tree2_unsorted_root {};
  wid_key_map_int      *in_tree4_wids_being_destroyed {};
  wid_key_map_int      *in_tree5_tick_wids {};
  wid_key_map_int      *in_tree6_pre_tick_wids {};
  wid_key_map_int      *in_tree_global_unsorted_root {};

  //
  // Sorted for display onto the screen.
  //
  wid_key_map_location children_display_sorted;

  //
  // No particular sort order.
  //
  wid_key_map_int tree2_children_unsorted;

  //
  // A tree for things being destroyed.
  //
  wid_key_map_int tree4_wids_being_destroyed;

  //
  // A tree for tick things
  //
  wid_key_map_int tree5_tick_wids;
  wid_key_map_int tree6_pre_tick_wids;

  //
  // Tiles widget
  //
  wid_tilesp wid_tiles {};

  uint8_t hidden {};
  uint8_t debug {};
  uint8_t tex_tl_set {};
  uint8_t tex_br_set {};
  uint8_t always_hidden {};
  uint8_t visible {};
  uint8_t received_input {};
  uint8_t moveable {};
  uint8_t moveable_set {};
  uint8_t moveable_horiz {};
  uint8_t moveable_horiz_set {};
  uint8_t moveable_vert {};
  uint8_t moveable_vert_set {};
  uint8_t moveable_bounded {};
  uint8_t moveable_bounded_set {};
  uint8_t moveable_no_user_scroll {};
  uint8_t moveable_no_user_scroll_set {};
  uint8_t ignore_events {};
  uint8_t ignore_scroll_events {};
  uint8_t ignore_for_mouse_down {};
  uint8_t first_update {};
  uint8_t show_cursor {};
  uint8_t text_pos_set {};
  uint8_t text_lhs {};
  uint8_t text_rhs {};
  uint8_t text_centerx {};
  uint8_t text_top {};
  uint8_t text_bot {};
  uint8_t text_centery {};
  uint8_t being_destroyed {};
  uint8_t do_not_raise {};
  uint8_t do_not_lower {};
  uint8_t disable_scissors {};
  uint8_t square {};
  uint8_t is_scrollbar_vert_trough {};
  uint8_t is_scrollbar_vert {};
  uint8_t is_scrollbar_horiz_trough {};
  uint8_t is_scrollbar_horiz {};

  //
  // Optionally set to the previous wid in a list
  //
  Widp prev {};
  Widp next {};
  Widp scrollbar_horiz {};
  Widp scrollbar_vert {};
  Widp scrollbar_owner {};

  //
  // The real position after scrollbar adjustments.
  //
  spoint abs_tl {};
  spoint abs_br {};

  //
  // Lots of layers of tiles for widgets.
  //
  TileLayers tiles = {};

  fsize texuv;
  fsize tex_tl;
  fsize tex_br;

  //
  // WID_MODE_NORMAL ...
  //
  wid_mode mode {};

  //
  // Offset of child widgets in the parent window.
  //
  spoint offset {};

  //
  // Config layers:
  //
  std::array< WidOptionsMenu, WID_MODE_LAST > cfg {};

  //
  // Client context
  //
  std::string string_context;
  int         int_context {-1};
  void       *void_context {};

  std::array< ThingId, UI_MAX_WID_CONTEXT > thing_id_context {};

  //
  // Text placement.
  //
  spoint text_pos {};

  ts_t destroy_when {};
  ts_t ts_created {};
  ts_t ts_last_mode_change {};

  //
  // Text input
  //
  uint16_t cursor {};

  //
  // Order of this wid amongst other focusable widgets.
  //
  uint8_t focus_order {};

  //
  // Who had it last ? Used when raising this wid again.
  //
  uint8_t focus_last {};

  //
  // The wids children
  //
  Widp parent {};

  //
  // Widget internal name.
  //
  std::string name;

  //
  // For debugging.
  //
  std::string to_string;

  //
  // Text that appears on the wid.
  //
  std::string text;
  size_t      max_len {};

  //
  // Action handlers
  //
  on_key_down_t          on_key_down {};
  on_key_up_t            on_key_up {};
  on_joy_button_t        on_joy_button {};
  on_mouse_down_t        on_mouse_down {};
  on_mouse_down_t        on_mouse_held {};
  on_mouse_up_t          on_mouse_up {};
  on_mouse_motion_t      on_mouse_motion {};
  on_mouse_focus_begin_t on_mouse_focus_begin {};
  on_mouse_focus_end_t   on_mouse_focus_end {};
  on_mouse_over_begin_t  on_mouse_over_begin {};
  on_mouse_over_end_t    on_mouse_over_end {};
  on_destroy_t           on_destroy {};
  on_destroy_begin_t     on_destroy_begin {};
  on_tick_t              on_tick {};
  on_pre_tick_t          on_pre_tick {};
  on_display_t           on_display {};
};

extern std::array< std::array< Widp, TERM_HEIGHT_MAX >, TERM_WIDTH_MAX > wid_on_screen_at;

extern wid_key_map_location wid_top_level;
