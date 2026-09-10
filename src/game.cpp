//
// Copyright goblinhack@gmail.com
//

#include "cfg.hpp"
#include "my_ascii.hpp"
#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_game_defs.hpp"
#include "my_game_inlines.hpp"
#include "my_game_popups.hpp"
#include "my_gl.hpp"
#include "my_globals.hpp"
#include "my_hiscore.hpp"
#include "my_level.hpp"
#include "my_level_ext.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_random.hpp"
#include "my_random_name.hpp"
#include "my_sdl_proto.hpp"
#include "my_spoint.hpp"
#include "my_test.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tile.hpp"
#include "my_time.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"
#include "my_wid.hpp"
#include "my_wids.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_mixer.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

static SDL_Keysym no_key;

using Config = class Config
{
public:
  std::string version = "" MYVER "";

  //
  // Used to check for changes in the size of this struct.
  //
  int serialized_size = {};

  //
  // Human readable version of the above seed.
  //
  std::string seed_name;

  //
  // All randomness jumps off of this.
  //
  uint32_t seed_num {};

  //
  // The source of the seed
  //
  SeedSource seed_source {SEED_SOURCE_RANDOM};

  //
  // Player's name
  //
  std::string player_name;

  //
  // Keep flags int size so the header size will change on a new flag.
  // It does not always for new bools.
  //
  float aspect_ratio = {};

  int ui_term_height = {};
  int ui_term_width  = {};

  bool debug_mode             = {};
  bool fps_counter            = {};
  bool gfx_borderless         = {};
  bool gfx_fullscreen         = {};
  bool gfx_fullscreen_desktop = {};
  bool gfx_vsync_enable       = {};

  bool mouse_wheel_lr_negated = {};
  bool mouse_wheel_ud_negated = {};

  int config_font        = {};
  int config_font_height = {};
  int config_font_width  = {};

  int config_pix_height = {};
  int config_pix_width  = {};

  //
  // The visible map FBO
  //
  int map_fbo_height = {};
  int map_fbo_width  = {};

  //
  // The actual display resolution
  //
  int window_pix_height = {};
  int window_pix_width  = {};

  //
  // The number of tiles on screen.
  //
  int tiles_visible_across = {};
  int tiles_visible_down   = {};

  int ascii_pix_height = {};
  int ascii_pix_width  = {};
  int music_volume     = {};
  int sdl_delay        = {};
  int sound_volume     = {};

  SDL_Keysym key_wait       = {};
  SDL_Keysym key_console    = {};
  SDL_Keysym key_help       = {};
  SDL_Keysym key_load       = {};
  SDL_Keysym key_move_down  = {};
  SDL_Keysym key_move_left  = {};
  SDL_Keysym key_move_right = {};
  SDL_Keysym key_move_up    = {};
  SDL_Keysym key_quit       = {};
  SDL_Keysym key_save       = {};
  SDL_Keysym key_screenshot = {};
  SDL_Keysym key_unused1    = {};
  SDL_Keysym key_unused2    = {};
  SDL_Keysym key_unused3    = {};
  SDL_Keysym key_unused4    = {};
  SDL_Keysym key_unused5    = {};
  SDL_Keysym key_unused6    = {};
  SDL_Keysym key_unused7    = {};
  SDL_Keysym key_unused8    = {};
  SDL_Keysym key_unused9    = {};
  SDL_Keysym key_unused10   = {};
  SDL_Keysym key_unused11   = {};
  SDL_Keysym key_unused12   = {};
  SDL_Keysym key_abort      = {};
  SDL_Keysym key_throw      = {};
  SDL_Keysym key_fire       = {};
  SDL_Keysym key_inventory  = {};
  SDL_Keysym key_jump       = {};
  SDL_Keysym key_ascend     = {};
  SDL_Keysym key_descend    = {};
  SDL_Keysym key_zoom       = {};

  //
  // Ok, not really config...
  //
  HiScores hiscores;

  static void fini();
  void        reset();
};

class Game
{
public:
  //
  // Save file name, contains the date and other useful save slot info
  //
  std::string version = "" MYVER "";

  //
  // All levels
  //
  Levelsp levels {};

  //
  // Used to check for changes in the size of this struct.
  //
  int serialized_size {};

  //
  // If this was a saved game, which slot does it use.
  //
  uint8_t save_slot {};

  //
  // Title and seed name.
  //
  std::string save_meta;

  //
  // Saved file for this game.
  //
  std::string save_file;

  //
  // Appdata dir for saving logs.
  //
  std::string appdata;

  //
  // Save dir for saving games.
  //
  std::string saved_dir;

  //
  // User configuration.
  //
  Config config = {};

  /////////////////////////////////////////////////////////////////////////
  // not worth saving
  // | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
  // v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v
  /////////////////////////////////////////////////////////////////////////

  //
  // Game toughness
  //
  int difficulty = {};

  //
  // Current frame-rate
  //
  int fps_value = {};

  //
  // Zoom gfx level. Not saved as is regenerated.
  //
  int zoom = {MAP_ZOOM_DEF};

  //
  // If the fbo is smaller than the screen, a single map pixel takes up N on screen pixels
  //
  int single_pix_size = {};

  //
  // Remake the right bar next tick
  //
  bool request_to_remake_ui {};

  //
  // Only save the game once we get to the end of a tick to avoid saving things half way through a move
  //
  bool request_to_save_game {};

  //
  // Set when the mouse is moved
  //
  bool request_to_update_cursor {};

  //
  // Set when the player reaches the exit
  //
  bool request_reached_exit {};

  //
  // Set when the player reaches the entrance
  //
  bool request_reached_entrance {};

  //
  // Player is dead
  //
  bool        request_to_end_game {};
  std::string request_to_end_game_reason;

  //
  // Temporary. Global states
  //
  GameStateType state {STATE_INIT};

  //
  // Temporary. Dampens mouse clicks
  //
  int last_mouse_down {};

  //
  // These are the onscreen map pixel co-ords.
  //
  int visible_map_tl_x {};
  int visible_map_tl_y {};
  int visible_map_br_x {};
  int visible_map_br_y {};

  //
  // Temporary popup text that is overlaid over the map
  //
  std::array< std::array< GamePopups, MAP_HEIGHT >, MAP_WIDTH > popups;

  //
  // Stops clang tidy adding const for this function
  //
  bool compiler_non_const_hack {};

  //
  // Which player was finally chosen on the player menu
  //
  Thingp cand_player {};
  Tpp    chosen_player {};

  //
  // Which sacrifices were selected
  //
  Thingp                   cand_sacrifice_last {};
  std::map< Thingp, bool > cand_sacrifice;
  std::vector< Tpp >       chosen_sacrifice;

  //
  // Which player is hovering over
  //
  Thingp mouse_over_player {};
  Thingp mouse_over_sacrifice {};

  /////////////////////////////////////////////////////////////////////////
  // not worth saving
  // ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^
  // | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
  /////////////////////////////////////////////////////////////////////////

  explicit Game(const std::string &appdata);
  Game() = default;

  auto load_snapshot() -> bool;
  auto load(const std::string &file_to_load, class Game &target) -> bool;
  auto load(int slot) -> bool;
  auto save_config() -> bool;
  auto save_select() -> bool;
  auto save_snapshot() -> bool;
  auto save(const std::string &file_to_save) -> bool;
  auto save(int slot) -> bool;

  void cleanup();
  void create_levels();
  void destroy_levels();
  void display();
  void fini();
  void handle_game_request_to_remake_ui();
  void init();
  void load_select();
  void player_name_set(const char *player_name);
  void popup_cleanup();
  void popup_text_add(spoint p, const std::string &);
  void seed_clear();
  void seed_set(const char *seed = nullptr);
  void seed_previous_set() const;
  void start_playing();
  void state_change(GameStateType state, const std::string &why);
  void state_reset(const std::string &why);
  void tick();

  [[nodiscard]] auto load_config() const -> std::string;
};

static void game_map_zoom_update(Gamep g);

// NOLINTBEGIN
class Game *game;
// NOLINTEND

#include "game_load.hpp"
#include "game_save.hpp"

void Config::fini()
{
  log("game fini");
  TRACE_INDENT();
}

void Config::reset()
{
  log("game reset");
  TRACE_INDENT();

  config_font        = UI_FONT_8x8;
  config_font_height = 8;
  config_font_width  = 8;

  config_pix_height      = {};
  config_pix_width       = {};
  debug_mode             = false;
  fps_counter            = false;
  map_fbo_height         = {};
  map_fbo_width          = {};
  gfx_borderless         = true;
  gfx_fullscreen_desktop = true;
  gfx_fullscreen         = false;
  gfx_vsync_enable       = true;
  mouse_wheel_lr_negated = false;
  mouse_wheel_ud_negated = false;
  ui_term_height         = {TERM_HEIGHT_DEF};
  ui_term_width          = {TERM_WIDTH_DEF};
  version                = "" MYVER "";
  aspect_ratio           = {};
  window_pix_height      = {};
  window_pix_width       = {};
  tiles_visible_across   = MAP_TILES_ACROSS_DEF;
  tiles_visible_down     = MAP_TILES_DOWN_DEF;

  key_ascend.sym     = SDLK_LESS;
  key_console.sym    = SDLK_BACKQUOTE;
  key_descend.sym    = SDLK_GREATER;
  key_help.sym       = SDLK_h;
  key_inventory.sym  = SDLK_i;
  key_fire.sym       = SDLK_SPACE;
  key_jump.sym       = SDLK_j;
  key_throw.sym      = SDLK_t;
  key_abort.sym      = SDLK_ESCAPE;
  key_load.sym       = SDLK_F1;
  key_move_down.sym  = SDLK_s;
  key_move_left.sym  = SDLK_a;
  key_move_right.sym = SDLK_d;
  key_move_up.sym    = SDLK_w;
  key_quit.sym       = SDLK_q;
  key_save.sym       = SDLK_F12;
  key_screenshot.sym = SDLK_F10;
  key_wait.sym       = SDLK_PERIOD;
  key_zoom.sym       = SDLK_z;
  music_volume       = {MIX_MAX_VOLUME / 4};
  sdl_delay          = 1;
  sound_volume       = {MIX_MAX_VOLUME / 2};
}

void game_config_reset(Gamep g) { g->config.reset(); }

Game::Game(const std::string &vappdata) : save_slot(1)
{
  log("game load %s", vappdata.c_str());
  TRACE_INDENT();

  auto *g = this;

  config.reset();

  g->appdata = vappdata;

  saved_dir = appdata + DIR_SEP + "sacrecant" + DIR_SEP;

  if (! config.seed_name.empty()) {
    save_meta = "seed " + config.seed_name + ", ";
  }
  save_meta += string_timestamp();

  save_file = saved_dir + "saved-slot-" + std::to_string(save_slot);
}

void Game::init()
{
  log("game init");
  TRACE_INDENT();

  //
  // If no seed is specified, we do want to start from a different seed each time
  //
  OS_SRAND((uint32_t) std::time(nullptr));

  seed_set();

  level_raycast_init(this);

  level_light_precalculate(this);
  level_lunge_precalculate(this);

  player_name_set(nullptr);
}
void game_init(Gamep g) { g->init(); }

//
// Create a level with the given contents and start the game into playing state
//
[[nodiscard]] auto game_test_init(Gamep g, Levelp *l_out, LevelNum level_num, int w, int h, const char *contents, const Overrides &overrides)
    -> Levelsp
{
  log("test init");
  TRACE_INDENT();

  game_cleanup(g);

  game_state_change(g, STATE_INIT, "init test");

  //
  // We need a consistent seed for all tests, so damage doesn't vary
  //
  g->seed_set(TEST_SEED);

  g->player_name_set("Ser Testalot");

  auto *v = game_levels_set(g, levels_memory_alloc(g));

  game_test_init_level(g, v, l_out, level_num, w, h, contents, overrides);

  game_state_reset(g, "new game");

  game_start_playing(g);

  game_state_change(g, STATE_PLAYING, "new game");

  //
  // Need this to update visibility.
  //
  game_tick(g);

  return v;
}

//
// Create an additional level with the given contents and start the game into playing state
//
void game_test_init_level(Gamep g, Levelsp v, Levelp *l_out, LevelNum level_num, bpoint level_at, int w, int h, const char *contents,
                          const Overrides &overrides)
{
  log("test init level");
  TRACE_INDENT();

  auto *l = game_level_get(g, v, level_num);
  if (l_out != nullptr) {
    *l_out = l;
  }

  level_init(g, v, l, level_num);

  if (! level_populate(g, v, l, nullptr, w, h, contents, overrides)) {
    CROAK("level populate failed");
  }

  if (level_num >= LEVEL_ARR_IDX_MAX) {
    CROAK("too many levels deep, level_num %d", level_num);
  }

  if (level_at.x >= LEVEL_GRID_ACROSS) {
    CROAK("level oob (%d,%d)", level_at.x, level_at.y);
  }

  if (level_at.y >= LEVEL_GRID_DOWN) {
    CROAK("level oob (%d,%d)", level_at.x, level_at.y);
  }

  //
  // Assign the level into the level select grid
  //
  l->level_select_at = level_at;

  auto *s            = &v->level_select.data[ level_at.x ][ level_at.y ];
  s->level_num       = l->level_num;
  s->is_set          = 1U;
  l->level_select_at = level_at;

  //
  // Final level connectivity.
  //
  levels_finalize(g, v);
}

void game_test_init_level(Gamep g, Levelsp v, Levelp *l_out, LevelNum level_num, int w, int h, const char *contents, const Overrides &overrides)
{
  game_test_init_level(g, v, l_out, level_num, bpoint(0, level_num), w, h, contents, overrides);
}

void Game::fini()
{
  log("game fini");
  TRACE_INDENT();

  cleanup();
}
void game_fini(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }

  g->fini();

  OLDPTR(MTYPE_GAME, g);
  delete g;
  g    = nullptr;
  game = nullptr;
}

void Game::cleanup()
{
  log("game cleanup");
  TRACE_INDENT();

  state_change(STATE_QUITTING, "quitting");

  destroy_levels();

  level_raycast_fini();

  state_change(STATE_INIT, "init");

  //
  // Clear previous test error
  //
  if (g_opt_tests) {
    error_clear(this);
  }

  wid_console_clear(this);
}
void game_cleanup(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }

  g->cleanup();
}

void game_save_config(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }

  g->save_config();
}

[[nodiscard]] auto game_popups_get(Gamep g, int x, int y) -> std::list< GamePopup * > *
{
  TRACE();
  return &g->popups[ x ][ y ].all;
}

void game_popups_set(Gamep g, int x, int y, std::list< GamePopup * > &l)
{
  TRACE();
  g->popups[ x ][ y ].all = l;
}

[[nodiscard]] auto game_popups_present(Gamep g, int x, int y) -> bool
{
  TRACE();
  return ! g->popups[ x ][ y ].all.empty();
}

void Game::seed_clear()
{
  DBG("seed clear");
  TRACE_INDENT();

  config.seed_name   = "";
  config.seed_source = SEED_SOURCE_RANDOM;
}

void Game::seed_set(const char *maybe_seed)
{
  if (maybe_seed != nullptr) {
    DBG("set seed: %s", maybe_seed);
  } else {
    DBG("set seed: (none)");
  }

  TRACE_INDENT();

  if (! g_level_opt.level_name.empty()) {
    config.seed_name   = TEST_SEED;
    config.seed_source = SEED_SOURCE_TEST;
  } else if ((maybe_seed != nullptr) && (*maybe_seed != 0)) {
    config.seed_name   = std::string(maybe_seed);
    config.seed_source = SEED_SOURCE_USER;
    if (! g_opt_tests && ! g_opt_do_level_gen) {
      con("Set fixed seed '%s' from ui", config.seed_name.c_str());
    }
  } else if (! g_opt_seed_name.empty()) {
    config.seed_name   = g_opt_seed_name;
    config.seed_source = SEED_SOURCE_COMMAND_LINE;
    con("Set fixed seed '%s' from command line", config.seed_name.c_str());
  } else if (config.seed_name.empty()) {
    config.seed_name   = os_random_name(SIZEOF("4294967295") - 1);
    config.seed_source = SEED_SOURCE_RANDOM;
    con("Set random seed '%s', none set in config", config.seed_name.c_str());
  } else {
    switch (config.seed_source) {
      case SEED_SOURCE_TEST : //
        con("Set seed '%s' from testing option", config.seed_name.c_str());
        break;
      case SEED_SOURCE_COMMAND_LINE : //
        con("Set seed '%s' from previous save", config.seed_name.c_str());
        break;
      case SEED_SOURCE_USER : //
        con("Set seed '%s' from previous load", config.seed_name.c_str());
        break;
      case SEED_SOURCE_RANDOM :
        config.seed_name   = os_random_name(SIZEOF("4294967295") - 1);
        config.seed_source = SEED_SOURCE_RANDOM;
        con("Set random seed '%s', as none manually set", config.seed_name.c_str());
        break;
    }
  }

  //
  // If a number, use that as the seed, else convert the string to hash number
  //
  char *p         = nullptr; // NOLINT
  config.seed_num = strtol(config.seed_name.c_str(), &p, 10);
  if (*p != 0) {
    //
    // Conversion failed, not a number
    //
    config.seed_num = string_to_hash(config.seed_name);
  }

  log("set seed, name '%s', seed %u", config.seed_name.c_str(), config.seed_num);

  //
  // Limit to tested levels
  //
  PCG_SRAND(config.seed_num % LEVEL_TEST_MAX);
}

void Game::seed_previous_set() const
{
  TRACE();
  log("set previous seed?");

  switch (config.seed_source) {
    case SEED_SOURCE_TEST : //
      log("set previous seed?: no, test");
      break;
    case SEED_SOURCE_COMMAND_LINE : //
      log("set previous seed?: no, command line");
      break;
    case SEED_SOURCE_USER : //
      log("set previous seed?: no, user");
      break;
    case SEED_SOURCE_RANDOM :
      log("set previous seed, name '%s', seed %u", config.seed_name.c_str(), config.seed_num);
      g_opt_seed_name_previous = config.seed_name;
      break;
  }
}

void game_seed_set(Gamep g, const char *maybe_seed)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }

  g->seed_set(maybe_seed);
}

void game_seed_clear(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }

  g->seed_clear();
}

void game_seed_set(Gamep g, uint32_t seed)
{
  DBG("set seed: %u", seed);
  TRACE_INDENT();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }

  std::string const seed_name = std::to_string(seed);
  game_seed_set(g, seed_name.c_str());
}

[[nodiscard]] auto game_seed_name_get(Gamep g) -> const char *
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return "";
  }

  DBG("get seed: %s", g->config.seed_name.c_str());
  return g->config.seed_name.c_str();
}

[[nodiscard]] auto game_seed_source_get(Gamep g) -> SeedSource
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return SEED_SOURCE_RANDOM;
  }

  return g->config.seed_source;
}

[[nodiscard]] auto game_seed_num_get(Gamep g) -> uint32_t
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }

  return g->config.seed_num;
}

void Game::player_name_set(const char *maybe_player_name)
{
  TRACE();

  if (maybe_player_name != nullptr) {
    config.player_name = std::string(maybe_player_name);
    if (! g_opt_tests && ! g_opt_do_level_gen) {
      con("Set player name '%s' from ui", config.player_name.c_str());
    }
  } else if (! config.player_name.empty()) {
    con("Set player name '%s' from previous load", config.player_name.c_str());
  } else {
    config.player_name = "Ser Deadalot";
    con("Set default player name '%s'", config.player_name.c_str());
  }
}

void game_player_name_set(Gamep g, const char *maybe_player_name)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }

  g->player_name_set(maybe_player_name);
}

[[nodiscard]] auto game_player_name_get(Gamep g) -> const char *
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return "";
  }

  return g->config.player_name.c_str();
}

void Game::create_levels()
{
  log("create levels");
  TRACE_INDENT();

  auto *g = this;
  destroy_levels();

  //
  // Begin generation
  //
  state_change(STATE_GENERATING, "generate levels");

  //
  // Create the grid of empty levels. No actual levels exist yet.
  //
  level_select_grid_of_empty_levels(g);

  //
  // We shuld have levels memory now
  //
  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    ERR("no level memory created");
    return;
  }

  //
  // Crteate the actual levels
  //
  level_gen_create_levels(g, v);

  //
  // Assign level pointers to the level grid
  //
  level_select_assign_levels_to_grid(g, v);

  //
  // Final level connectivity.
  //
  levels_finalize(g, v);

  //
  // Sanity checks
  //
  LevelSelect const *s = &v->level_select;
  if (s == nullptr) {
    ERR("no level selection created");
    return;
  }

  auto *player = thing_player(g);
  if (player == nullptr) [[unlikely]] {
    ERR("no player pointer");
    return;
  }

  //
  // First level. Usually 0 but can be a specified level.
  //
  if (level_change(g, v, g_level_opt.level_num) == nullptr) {
    ERR("failed to change to chosen level %d", g_level_opt.level_num);
    return;
  }

  //
  // Update the tiles show in level selection
  //
  level_select_update_grid_tiles(g, v);

  if (g_opt_level_select_menu) {
    //
    // Start in level select?
    //
    if (level_change(g, v, LEVEL_ARR_IDX_LEVEL_SELECT) == nullptr) {
      ERR("failed to change to level selection");
      return;
    }
  }

  game_debug_info(g);

  state_change(STATE_GENERATED, "generated levels");
}
void game_create_levels(Gamep g) { g->create_levels(); }

void Game::start_playing()
{
  log("game started playing");
  TRACE();

  auto *g = this;
  if (g == nullptr) [[unlikely]] {
    return;
  }

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  (void) wid_topcon_init(g);
  (void) wid_botcon_init(g);

  auto *l = game_level_get(g, v);
  if (l == nullptr) [[unlikely]] {
    return;
  }

  wid_topcon_flush(g);
  wid_botcon_flush(g);
}
void game_start_playing(Gamep g) { g->start_playing(); }

void Game::destroy_levels()
{
  auto *g = this;
  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  log("levels destroy all");
  TRACE_INDENT();

  levels_destroy(g, v);

  //
  // Save previous seed
  //
  seed_previous_set();

  //
  // Create a new seed for the next run
  //
  seed_set();

  //
  // Remove all popups
  //
  game_popups_clear(g);
}
void game_destroy_levels(Gamep g) { g->destroy_levels(); }

static auto game_state_to_string(GameStateType state) -> std::string
{
  TRACE();
  return GameStateType_to_string(state);
}

//
// Reset to the default state, either playing the game, or starting up.
//
void Game::state_reset(const std::string &why)
{
  log("state reset: %s", why.c_str());
  TRACE_INDENT();

  auto *g = this;
  auto *v = game_levels_get(g);
  if (v != nullptr) {
    auto *l = game_level_get(g, v);
    if (l != nullptr) {
      if (level_is_level_select(g, v, l)) {
        state_change(STATE_LEVEL_SELECT_MENU, why);
      } else {
        state_change(STATE_PLAYING, why);
      }
    } else {
      state_change(STATE_MAIN_MENU, why);
    }
  } else {
    state_change(STATE_MAIN_MENU, why);
  }
}
void game_state_reset(Gamep g, const char *why) { g->state_reset(why); }

[[nodiscard]] auto game_state(Gamep g) -> GameStateType { return g->state; }

void Game::state_change(GameStateType new_state, const std::string &why)
{
  auto *g = this;
  if (g->state == new_state) {
    return;
  }

  auto old_state = state;

  //
  // Set here to stop recursion.
  //
  state = new_state;

  //
  // Why oh why change state
  //
  log("game state change: %s -> %s, reason: %s", game_state_to_string(old_state).c_str(), game_state_to_string(new_state).c_str(), why.c_str());
  TRACE_INDENT();

  //
  // Cleanup actions for the new state
  //
  switch (new_state) {
    case STATE_INIT :      [[fallthrough]];
    case STATE_MAIN_MENU : [[fallthrough]];
    case STATE_QUITTING :
    case STATE_GAME_OVER_MENU :
      wid_load_destroy(g);
      wid_main_menu_destroy(g);
      wid_quit_destroy(g);
      wid_save_destroy(g);
      wid_leftbar_fini(g);
      wid_rightbar_fini(g);
      wid_actionbar_fini(g);
      wid_topcon_fini(g);
      wid_botcon_fini(g);
      wid_dead_fini(g);

      game_request_to_remake_ui_unset(g);
      game_request_to_save_game_unset(g);
      game_request_to_update_cursor_unset(g);
      game_request_reached_entrance_unset(g);
      game_request_reached_exit_unset(g);
      break;
    case STATE_LEVEL_SELECT_MENU : wid_rightbar_fini(g); [[fallthrough]];
    case STATE_PLAYING :
      wid_load_destroy(g);
      wid_main_menu_destroy(g);
      wid_quit_destroy(g);
      wid_save_destroy(g);

      //
      // After a warning window closes, and we transition back to playing state,
      // the cursor is missing. We need to recreate it.
      //
      game_request_to_update_cursor_set(g);
      break;
    case STATE_THROW_ITEM :
      //
      // After a window closes, and we transition back to playing state,
      // the cursor is missing. We need to recreate it.
      //
      game_request_to_update_cursor_set(g);
      break;
    case STATE_DEAD_MENU :          [[fallthrough]];
    case STATE_MOVE_WARNING_MENU :  [[fallthrough]];
    case STATE_PLAYER_SELECT_MENU : [[fallthrough]];
    case STATE_KEYBOARD_MENU :      [[fallthrough]];
    case STATE_LOAD_MENU :          [[fallthrough]];
    case STATE_LOADED :             [[fallthrough]];
    case STATE_SAVE_MENU :          [[fallthrough]];
    case STATE_THROW_MENU :         [[fallthrough]];
    case STATE_QUIT_MENU :          [[fallthrough]];
    case STATE_INVENTORY_MENU :     [[fallthrough]];
    case STATE_COLLECT_MENU :       [[fallthrough]];
    case STATE_ITEM_MENU :          wid_actionbar_fini(g); break;
    case STATE_GENERATING :         [[fallthrough]];
    case STATE_GENERATED :          [[fallthrough]];
    case GAME_STATE_ENUM_MAX :      break;
  }

  //
  // Enter the new state
  //
  switch (new_state) {
    case STATE_MAIN_MENU :
      if (g_opt_quick_start) {
        //
        // Do nothing
        //
        log("do nothing, quick start chosen");
      } else if (g_opt_player_select_menu) {
        //
        // Do nothing
        //
      } else if (g_opt_level_select_menu) {
        //
        // Do nothing
        //
        log("do nothing, level select menu chosen");
      } else {
        log("select the main menu");
        wid_main_menu_select(g);
      }
      break;
    case STATE_PLAYER_SELECT_MENU :
      (void) wid_leftbar_init(g);
      (void) wid_rightbar_init(g);
      (void) wid_topcon_init(g);
      wid_actionbar_fini(g);
      break;
    case STATE_LEVEL_SELECT_MENU : [[fallthrough]];
    case STATE_PLAYING :           [[fallthrough]];
    case STATE_THROW_ITEM :
      //
      // When we transition back to playing, we need to update for the time spent in
      // another state not updating the frame counter.
      //
      level_tick_reset_frame_counter(g);

      switch (old_state) {
        case STATE_LOADED :
          (void) wid_leftbar_init(g);
          (void) wid_rightbar_init(g);
          (void) wid_topcon_init(g);
          (void) wid_botcon_init(g);
          (void) wid_actionbar_init(g);
          game_map_zoom_update(g);
          thing_player_init(g);
          break;
        case STATE_QUIT_MENU :         [[fallthrough]];
        case STATE_MOVE_WARNING_MENU : [[fallthrough]];
        case STATE_LOAD_MENU :         [[fallthrough]];
        case STATE_SAVE_MENU :         [[fallthrough]];
        case STATE_KEYBOARD_MENU :     [[fallthrough]];
        case STATE_MAIN_MENU :         [[fallthrough]];
        case STATE_INVENTORY_MENU :    [[fallthrough]];
        case STATE_COLLECT_MENU :      [[fallthrough]];
        case STATE_THROW_ITEM :        [[fallthrough]];
        case STATE_THROW_MENU :        [[fallthrough]];
        case STATE_ITEM_MENU :
          (void) wid_leftbar_init(g);
          (void) wid_rightbar_init(g);
          (void) wid_actionbar_init(g);
          break;
        case STATE_INIT :               [[fallthrough]];
        case STATE_QUITTING :           [[fallthrough]];
        case STATE_DEAD_MENU :          [[fallthrough]];
        case STATE_PLAYER_SELECT_MENU : [[fallthrough]]; ;
        case STATE_GAME_OVER_MENU :     [[fallthrough]];
        case STATE_PLAYING :            [[fallthrough]];
        case STATE_LEVEL_SELECT_MENU :  [[fallthrough]];
        case STATE_GENERATING :         [[fallthrough]];
        case STATE_GENERATED :          [[fallthrough]];
        case GAME_STATE_ENUM_MAX :      break;
      }
      break;
    case STATE_DEAD_MENU :      [[fallthrough]];
    case STATE_GAME_OVER_MENU : [[fallthrough]];
    case STATE_KEYBOARD_MENU :  [[fallthrough]];
    case STATE_LOAD_MENU :      [[fallthrough]];
    case STATE_THROW_MENU :     [[fallthrough]];
    case STATE_SAVE_MENU :      [[fallthrough]];
    case STATE_QUIT_MENU :      [[fallthrough]];
    case STATE_COLLECT_MENU :   [[fallthrough]];
    case STATE_INVENTORY_MENU : [[fallthrough]];
    case STATE_ITEM_MENU :
      //
      // Don't want the player to keep moving to an old path when we exit this menu
      //
      level_cursor_path_reset(g);
      break;
    case STATE_MOVE_WARNING_MENU :
      //
      // Don't reset the cursor path for this one
      //
      break;
    case STATE_QUITTING :      [[fallthrough]];
    case STATE_INIT :          [[fallthrough]];
    case STATE_LOADED :        [[fallthrough]];
    case STATE_GENERATING :    [[fallthrough]];
    case STATE_GENERATED :     [[fallthrough]];
    case GAME_STATE_ENUM_MAX : break;
  }

  //
  // In case some cleanup above changed state
  //
  state = new_state;
}
void game_state_change(Gamep g, GameStateType new_state, const char *why)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->state_change(new_state, std::string(why));
}

//
// Do something per tick in a given state
//
void Game::handle_game_request_to_remake_ui()
{
  TRACE();

  auto *g = this;

  if (! game_request_to_remake_ui_get(g)) {
    return;
  }

  auto *v = game_levels_get(g);

  switch (state) {
    case STATE_PLAYER_SELECT_MENU :
      if (v != nullptr) {
        (void) wid_leftbar_init(g);
        (void) wid_rightbar_init(g);
        wid_actionbar_fini(g);
      }
      break;
    case STATE_LEVEL_SELECT_MENU :
      if (v != nullptr) {
        (void) wid_leftbar_init(g);
        (void) wid_actionbar_init(g);
      }
      break;
    case STATE_DEAD_MENU :    [[fallthrough]];
    case STATE_PLAYING :      [[fallthrough]];
    case STATE_COLLECT_MENU : [[fallthrough]];
    case STATE_THROW_ITEM :   [[fallthrough]];
    case STATE_THROW_MENU :   [[fallthrough]];
    case STATE_INVENTORY_MENU :
      if (v != nullptr) {
        (void) wid_leftbar_init(g);
        (void) wid_rightbar_init(g);
        (void) wid_actionbar_init(g);
      }
      break;
    case STATE_GAME_OVER_MENU :    [[fallthrough]];
    case STATE_INIT :              [[fallthrough]];
    case STATE_MAIN_MENU :         [[fallthrough]];
    case STATE_QUITTING :          [[fallthrough]];
    case STATE_ITEM_MENU :         [[fallthrough]];
    case STATE_MOVE_WARNING_MENU : [[fallthrough]];
    case STATE_KEYBOARD_MENU :     [[fallthrough]];
    case STATE_LOAD_MENU :         [[fallthrough]];
    case STATE_LOADED :            [[fallthrough]];
    case STATE_SAVE_MENU :         [[fallthrough]];
    case STATE_QUIT_MENU :         [[fallthrough]];
    case STATE_GENERATING :        [[fallthrough]];
    case STATE_GENERATED :         [[fallthrough]];
    case GAME_STATE_ENUM_MAX :     break;
  }

  //
  // Common to all states
  //
  game_request_to_remake_ui_unset(g);
}

//
// Do something per tick in a given state
//
void Game::tick()
{
  TRACE();

  auto *g = this;
  auto *v = game_levels_get(g);
  if (v != nullptr) {
    switch (state) {
      case STATE_DEAD_MENU :
      case STATE_GAME_OVER_MENU :
        //
        // We want things to finish their animations when the player is dead,
        // so need to allow ticking to complete
        //
        [[fallthrough]];
      case STATE_PLAYING :            levels_tick(g, v); break;
      case STATE_PLAYER_SELECT_MENU : [[fallthrough]]; ;
      case STATE_LEVEL_SELECT_MENU :  [[fallthrough]];
      case STATE_INIT :               [[fallthrough]];
      case STATE_MAIN_MENU :          [[fallthrough]];
      case STATE_QUITTING :           [[fallthrough]];
      case STATE_MOVE_WARNING_MENU :  [[fallthrough]];
      case STATE_KEYBOARD_MENU :      [[fallthrough]];
      case STATE_LOAD_MENU :          [[fallthrough]];
      case STATE_LOADED :             [[fallthrough]];
      case STATE_SAVE_MENU :          [[fallthrough]];
      case STATE_QUIT_MENU :          [[fallthrough]];
      case STATE_INVENTORY_MENU :     [[fallthrough]];
      case STATE_COLLECT_MENU :       [[fallthrough]];
      case STATE_THROW_MENU :         [[fallthrough]];
      case STATE_THROW_ITEM :         [[fallthrough]];
      case STATE_ITEM_MENU :          [[fallthrough]];
      case STATE_GENERATING :         [[fallthrough]];
      case STATE_GENERATED :          [[fallthrough]];
      case GAME_STATE_ENUM_MAX :      break;
    }
  }

  handle_game_request_to_remake_ui();

  //
  // Create the dead widget?
  //
  if (game_request_to_end_game_get(g)) {
    game_request_to_end_game_unset(g);
    wid_dead_select(g, game_request_to_end_game_reason_get(g));
  }

  //
  // Clean up dead widgets
  //
  wid_gc_all(g);

  //
  // Check widgets are not getting too numerous
  //
  wid_sanity_check(g);
}

void game_tick(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->tick();
}

[[nodiscard]] auto game_tick_get(Gamep g, Levelsp v) -> uint32_t
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  if (v == nullptr) [[unlikely]] {
    ERR("no levels pointer");
    return 0;
  }
  return v->tick;
}

//
// Wait for completion of the tick.
//
[[nodiscard]] auto game_wait_for_tick_to_finish(Gamep g, Levelsp v, Levelp l) -> bool
{
  TRACE();

  auto started = time_ms();
#ifdef GITHUB_BUILD
  auto max_time = 600;
#else
  auto max_time = 60;
#endif

  if (v == nullptr) [[unlikely]] {
    ERR("no levels pointer");
    return false;
  }

  VERIFY(MTYPE_LEVELS, v);

  for (;;) {
    TRACE();

    IF_DEBUG2
    { //
      level_log(g, v, l, "Test: waiting for tick %u to finish", v->tick);
    }

    if (time_have_x_secs_passed_since(max_time, started)) {
      ERR("test timed out: %u secs", max_time);
      return false;
    }

    TRACE();
    game_tick(g);

    if (v == nullptr) [[unlikely]] {
      return true;
    }

    TRACE();
    if ((! static_cast< bool >(v->level_tick_in_progress_count)) && (! static_cast< bool >(v->level_tick_request_count))) {
      level_log(g, v, l, "Test: tick %u finished, stop waiting", v->tick);
      return true;
    }
  }
}

//
// Display the game
//
void Game::display()
{
  TRACE();

  auto *g = this;

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  auto *l = game_level_get(g, v);
  if (l == nullptr) [[unlikely]] {
    return;
  }

  switch (state) {
    case STATE_LEVEL_SELECT_MENU :
      [[fallthrough]];
      //
      // Needed else we can't see the level select things
      //
    case STATE_THROW_ITEM : [[fallthrough]];
    case STATE_PLAYING :    [[fallthrough]];
    case STATE_DEAD_MENU :  [[fallthrough]];
    case STATE_GAME_OVER_MENU :
      level_mouse_position_get(g, v, l);
      level_display(g, v, l);
      thing_player_event_loop(g, v, l);
      break;
    case STATE_INIT :               [[fallthrough]];
    case STATE_PLAYER_SELECT_MENU : [[fallthrough]];
    case STATE_MAIN_MENU :          [[fallthrough]];
    case STATE_QUITTING :           [[fallthrough]];
    case STATE_INVENTORY_MENU :     [[fallthrough]];
    case STATE_COLLECT_MENU :       [[fallthrough]];
    case STATE_MOVE_WARNING_MENU :  [[fallthrough]];
    case STATE_KEYBOARD_MENU :      [[fallthrough]];
    case STATE_THROW_MENU :         [[fallthrough]];
    case STATE_ITEM_MENU :          [[fallthrough]];
    case STATE_LOAD_MENU :          [[fallthrough]];
    case STATE_LOADED :             [[fallthrough]];
    case STATE_SAVE_MENU :          [[fallthrough]];
    case STATE_QUIT_MENU :          [[fallthrough]];
    case STATE_GENERATING :         [[fallthrough]];
    case STATE_GENERATED :          [[fallthrough]];
    case GAME_STATE_ENUM_MAX :      break;
  }
}

void game_display(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->display();
}

[[nodiscard]] auto game_load_config(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->load_config().empty() /* no error */;
}

[[nodiscard]] auto game_hiscores_get(Gamep g) -> class HiScores *
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return nullptr;
  }
  return &g->config.hiscores;
}

[[nodiscard]] auto game_hiscore_get(Gamep g) -> uint32_t
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.hiscores.hiscores[ 0 ].score;
}

void game_add_new_hiscore(Gamep g, uint32_t score, LevelNum level_num, const char *name, const char *reason)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.hiscores.add_new_hiscore(g, score, level_num, name, reason);
}

[[nodiscard]] auto game_is_new_hiscore(Gamep g, uint32_t score) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->config.hiscores.is_new_hiscore(score);
}

[[nodiscard]] auto game_is_new_highest_hiscore(Gamep g, uint32_t score) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->config.hiscores.is_new_highest_hiscore(score);
}

[[nodiscard]] auto game_place_str(Gamep g, uint32_t score) -> const char *
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return "";
  }
  return g->config.hiscores.place_str(score);
}

void game_visible_map_pix_get(Gamep g, int *visible_map_tl_x, int *visible_map_tl_y, int *visible_map_br_x, int *visible_map_br_y)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    *visible_map_tl_x = 0;
    *visible_map_tl_y = 0;
    *visible_map_br_x = 0;
    *visible_map_br_y = 0;
    ERR("no game pointer");
    return;
  }
  *visible_map_tl_x = g->visible_map_tl_x;
  *visible_map_tl_y = g->visible_map_tl_y;
  *visible_map_br_x = g->visible_map_br_x;
  *visible_map_br_y = g->visible_map_br_y;
}

void game_visible_map_pix_set(Gamep g, int visible_map_tl_x, int visible_map_tl_y, int visible_map_br_x, int visible_map_br_y)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    g->visible_map_tl_x = 0;
    g->visible_map_tl_y = 0;
    g->visible_map_br_x = 0;
    g->visible_map_br_y = 0;
    ERR("no game pointer");
    return;
  }
  g->visible_map_tl_x = visible_map_tl_x;
  g->visible_map_tl_y = visible_map_tl_y;
  g->visible_map_br_x = visible_map_br_x;
  g->visible_map_br_y = visible_map_br_y;
}

[[nodiscard]] auto game_tiles_visible_across_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return MAP_TILES_ACROSS_DEF;
  }
  return g->config.tiles_visible_across;
}
void game_tiles_visible_across_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.tiles_visible_across = val;
}

[[nodiscard]] auto game_tiles_visible_down_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return MAP_TILES_DOWN_DEF;
  }
  return g->config.tiles_visible_down;
}
void game_tiles_visible_down_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.tiles_visible_down = val;
}

[[nodiscard]] auto game_last_mouse_down_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->last_mouse_down;
}
void game_last_mouse_down_set(Gamep g, int val)
{
  TRACE();
  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->last_mouse_down = val;
}

[[nodiscard]] auto game_aspect_ratio_get(Gamep g) -> float
{
  TRACE();
  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 1;
  }
  return g->config.aspect_ratio;
}
void game_aspect_ratio_set(Gamep g, float val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.aspect_ratio = val;
}

[[nodiscard]] auto game_ui_term_height_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return TERM_HEIGHT_DEF;
  }
  return g->config.ui_term_height;
}
void game_ui_term_height_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.ui_term_height = val;
}

[[nodiscard]] auto game_ui_term_width_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return TERM_WIDTH_DEF;
  }
  return g->config.ui_term_width;
}
void game_ui_term_width_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.ui_term_width = val;
}

[[nodiscard]] auto game_debug_mode_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->config.debug_mode;
}
void game_debug_mode_set(Gamep g, bool val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.debug_mode = val;
}

[[nodiscard]] auto game_fps_counter_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->config.fps_counter;
}
void game_fps_counter_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.fps_counter = true;
}
void game_fps_counter_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.fps_counter = false;
}

[[nodiscard]] auto game_fps_value_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->fps_value;
}
void game_fps_value_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->fps_value = val;
}

[[nodiscard]] auto game_gfx_borderless_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->config.gfx_borderless;
}
void game_gfx_borderless_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.gfx_borderless = true;
}
void game_gfx_borderless_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.gfx_borderless = false;
}

[[nodiscard]] auto game_gfx_fullscreen_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->config.gfx_fullscreen;
}
void game_gfx_fullscreen_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.gfx_fullscreen = true;
}
void game_gfx_fullscreen_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.gfx_fullscreen = false;
}

[[nodiscard]] auto game_gfx_fullscreen_desktop_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->config.gfx_fullscreen_desktop;
}
void game_gfx_fullscreen_desktop_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.gfx_fullscreen_desktop = true;
}
void game_gfx_fullscreen_desktop_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.gfx_fullscreen_desktop = false;
}

[[nodiscard]] auto game_gfx_vsync_enable_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->config.gfx_vsync_enable;
}
void game_gfx_vsync_enable_set(Gamep g)
{
  TRACE();
  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.gfx_vsync_enable = true;
}
void game_gfx_vsync_enable_unset(Gamep g)
{
  TRACE();
  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.gfx_vsync_enable = false;
}

[[nodiscard]] auto game_mouse_wheel_lr_negated_get(Gamep g) -> bool
{
  TRACE();
  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->config.mouse_wheel_lr_negated;
}
void game_mouse_wheel_lr_negated_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.mouse_wheel_lr_negated = true;
}
void game_mouse_wheel_lr_negated_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.mouse_wheel_lr_negated = false;
}

[[nodiscard]] auto game_mouse_wheel_ud_negated_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->config.mouse_wheel_ud_negated;
}
void game_mouse_wheel_ud_negated_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.mouse_wheel_ud_negated = true;
}
void game_mouse_wheel_ud_negated_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.mouse_wheel_ud_negated = false;
}

[[nodiscard]] auto game_config_font_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.config_font;
}
void game_config_font_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.config_font = val;
}

[[nodiscard]] auto game_config_font_height_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.config_font_height;
}
void game_config_font_height_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.config_font_height = val;
}

[[nodiscard]] auto game_config_font_width_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.config_font_width;
}
void game_config_font_width_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.config_font_width = val;
}

[[nodiscard]] auto game_config_pix_height_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.config_pix_height;
}
void game_config_pix_height_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.config_pix_height = val;
}

[[nodiscard]] auto game_config_pix_width_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.config_pix_width;
}
void game_config_pix_width_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.config_pix_width = val;
}

[[nodiscard]] auto game_map_fbo_height_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.map_fbo_height;
}

[[nodiscard]] auto game_map_fbo_width_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.map_fbo_width;
}

void game_map_fbo_height_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.map_fbo_height = val;
}

void game_map_fbo_width_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.map_fbo_width = val;
}

[[nodiscard]] auto game_window_pix_height_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.window_pix_height;
}
void game_window_pix_height_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.window_pix_height = val;
}

[[nodiscard]] auto game_window_pix_width_get(Gamep g) -> int
{
  TRACE();
  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.window_pix_width;
}
void game_window_pix_width_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.window_pix_width = val;
}

[[nodiscard]] auto game_ascii_pix_height_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.ascii_pix_height;
}
void game_ascii_pix_height_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.ascii_pix_height = val;
}

[[nodiscard]] auto game_ascii_pix_width_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.ascii_pix_width;
}
void game_ascii_pix_width_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.ascii_pix_width = val;
}

[[nodiscard]] auto game_music_volume_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.music_volume;
}
void game_music_volume_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.music_volume = val;
}

[[nodiscard]] auto game_sdl_delay_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.sdl_delay;
}
void game_sdl_delay_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.sdl_delay = val;
}

[[nodiscard]] auto game_difficulty_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->difficulty;
}
void game_difficulty_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->difficulty = val;
}

[[nodiscard]] auto game_sound_volume_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 0;
  }
  return g->config.sound_volume;
}
void game_sound_volume_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.sound_volume = val;
}

[[nodiscard]] auto game_levels_get(Gamep g) -> Levelsp
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return nullptr;
  }
  if (g->levels != nullptr) {
    VERIFY(MTYPE_LEVELS, g->levels);
  }
  return g->levels;
}
[[nodiscard]] auto game_levels_set(Gamep g, Levelsp val) -> Levelsp
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return nullptr;
  }
  return g->levels = val;
}

[[nodiscard]] auto game_level_populate(Gamep g, Levelsp v, LevelNum n) -> Levelp
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("game_level_populate: no game pointer");
    return nullptr;
  }
  if (v == nullptr) [[unlikely]] {
    ERR("game_level_populate: no levels pointer");
    return nullptr;
  }
  if (n >= LEVEL_ARR_IDX_MAX) {
    ERR("game_level_populate: Exceeded max level arr idx: %u", n);
    return nullptr;
  }
  v->level_num = n;
  return game_level_get(g, v);
}

[[nodiscard]] auto game_key_wait_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_wait;
}
void game_key_wait_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }
  g->config.key_wait = key;
}

[[nodiscard]] auto game_key_console_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_console;
}
void game_key_console_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }
  g->config.key_console = key;
}

[[nodiscard]] auto game_key_help_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_help;
}
void game_key_help_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }
  g->config.key_help = key;
}

[[nodiscard]] auto game_key_load_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_load;
}
void game_key_load_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }
  g->config.key_load = key;
}

[[nodiscard]] auto game_key_move_down_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_move_down;
}
void game_key_move_down_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }
  g->config.key_move_down = key;
}

[[nodiscard]] auto game_key_move_left_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_move_left;
}
void game_key_move_left_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }
  g->config.key_move_left = key;
}

[[nodiscard]] auto game_key_move_right_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_move_right;
}
void game_key_move_right_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }
  g->config.key_move_right = key;
}

[[nodiscard]] auto game_key_move_up_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_move_up;
}
void game_key_move_up_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }
  g->config.key_move_up = key;
}

[[nodiscard]] auto game_key_quit_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_quit;
}
void game_key_quit_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }
  g->config.key_quit = key;
}

[[nodiscard]] auto game_key_save_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_save;
}
void game_key_save_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }
  g->config.key_save = key;
}

[[nodiscard]] auto game_key_screenshot_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_screenshot;
}
void game_key_screenshot_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return;
  }
  g->config.key_screenshot = key;
}

[[nodiscard]] auto game_key_unused1_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused1;
}
void game_key_unused1_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused1 = key;
}

[[nodiscard]] auto game_key_unused2_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused2;
}
void game_key_unused2_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused2 = key;
}

[[nodiscard]] auto game_key_unused3_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused3;
}
void game_key_unused3_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused3 = key;
}

[[nodiscard]] auto game_key_unused4_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused4;
}
void game_key_unused4_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused4 = key;
}

[[nodiscard]] auto game_key_unused5_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused5;
}
void game_key_unused5_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused5 = key;
}

[[nodiscard]] auto game_key_unused6_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused6;
}
void game_key_unused6_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused6 = key;
}

[[nodiscard]] auto game_key_unused7_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused7;
}
void game_key_unused7_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused7 = key;
}

[[nodiscard]] auto game_key_unused8_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused8;
}
void game_key_unused8_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused8 = key;
}

[[nodiscard]] auto game_key_unused9_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused9;
}
void game_key_unused9_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused9 = key;
}

[[nodiscard]] auto game_key_unused10_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused10;
}
void game_key_unused10_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused10 = key;
}

[[nodiscard]] auto game_key_unused11_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused11;
}
void game_key_unused11_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused11 = key;
}

[[nodiscard]] auto game_key_unused12_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_unused12;
}
void game_key_unused12_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_unused12 = key;
}

[[nodiscard]] auto game_key_abort_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_abort;
}
void game_key_abort_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_abort = key;
}

[[nodiscard]] auto game_key_throw_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_throw;
}
void game_key_throw_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_throw = key;
}

[[nodiscard]] auto game_key_fire_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_fire;
}
void game_key_fire_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_fire = key;
}

[[nodiscard]] auto game_key_inventory_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_inventory;
}
void game_key_inventory_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_inventory = key;
}

[[nodiscard]] auto game_key_jump_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_jump;
}
void game_key_jump_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_jump = key;
}

[[nodiscard]] auto game_key_ascend_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_ascend;
}
void game_key_ascend_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_ascend = key;
}

[[nodiscard]] auto game_key_descend_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_descend;
}
void game_key_descend_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_descend = key;
}

[[nodiscard]] auto game_key_zoom_get(Gamep g) -> SDL_Keysym
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return no_key;
  }
  return g->config.key_zoom;
}
void game_key_zoom_set(Gamep g, SDL_Keysym key)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->config.key_zoom = key;
}

[[nodiscard]] auto game_map_zoom_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return MAP_ZOOM_DEF;
  }
  if (g->zoom == 0) {
    return MAP_ZOOM_DEF;
  }
  return g->zoom;
}
void game_map_zoom_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }

  if (val >= MAP_ZOOM_MAX) {
    val = MAP_ZOOM_MAX - 1;
  }

  g->zoom = val;
}
[[nodiscard]] auto game_map_zoom_is_full_map_visible(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }
  return g->zoom == MAP_ZOOM_FULL_MAP;
}

[[nodiscard]] auto game_map_zoom_def_get(Gamep g) -> int
{
  TRACE();

  int visible_map_tl_x = 0;
  int visible_map_tl_y = 0;
  int visible_map_br_x = 0;
  int visible_map_br_y = 0;
  game_visible_map_pix_get(g, &visible_map_tl_x, &visible_map_tl_y, &visible_map_br_x, &visible_map_br_y);

  float const map_pix_width = visible_map_br_x - visible_map_tl_x;

  float zoom = map_pix_width / static_cast< float >(TILE_WIDTH) / static_cast< float >(MAP_TILES_ACROSS_DEF);

  zoom = std::max< float >(zoom, 2);

  if (zoom >= static_cast< float >(MAP_ZOOM_MAX)) {
    zoom = MAP_ZOOM_MAX - 1;
  }

  return static_cast< int >(zoom);
}

//
// Zoom update
//
static void game_map_zoom_update(Gamep g)
{
  DBG("zoom update");
  TRACE_INDENT();

  auto *v = game_levels_get(g);
  if (v == nullptr) [[unlikely]] {
    return;
  }

  //
  // Need to resize the map buffers and re-center
  //
  config_game_gfx_update(g);
  gl_init_fbo(g, FBO_MAP_BG_FLOOR_WATER_LAVA);
  gl_init_fbo(g, FBO_MAP_FG);
  gl_init_fbo(g, FBO_MAP_FG_OVERLAY);
  gl_init_fbo(g, FBO_MAP_BG_PREVIOUSLY_SEEN_TILES);
  gl_init_fbo(g, FBO_MAP_LAVA_OVERLAY);
  gl_init_fbo(g, FBO_FULL_SCREEN_PREVIOUSLY_SEEN_TILES);
  gl_init_fbo(g, FBO_FULL_SCREEN_VISIBLE_TILES);
  gl_init_fbo(g, FBO_FULL_SCREEN_LEVEL_CURR);
  gl_init_fbo(g, FBO_FULL_SCREEN_LEVEL_MERGED);
  gl_init_fbo(g, FBO_FULL_SCREEN_LEVEL_BELOW);
  gl_init_fbo(g, FBO_SPRITE1);

  //
  // If following the player already, then no need to re-center
  //
  if (! v->requested_auto_scroll) {
    auto *l = game_level_get(g, v);
    if (l != nullptr) {
      level_scroll_warp_to_focus(g, v, l);
    }
  }
}

//
// Zoom in/out
//
void game_map_zoom_toggle(Gamep g)
{
  DBG("zoom alt");
  TRACE_INDENT();

  if (game_map_zoom_is_full_map_visible(g)) {
    game_map_zoom_set(g, game_map_zoom_def_get(g));
  } else {
    game_map_zoom_set(g, MAP_ZOOM_FULL_MAP);
  }

  game_map_zoom_update(g);

  //
  // Restore the map pixel offset if we had previously been at this zoom level
  //
  auto *v = game_levels_get(g);
  if (v != nullptr) {
    auto zoom       = game_map_zoom_get(g);
    v->pixel_map_at = v->pixel_map_at_for_zoom[ zoom ];

    auto *l = game_level_get(g, v);
    if (l != nullptr) {
      level_bounds_set(g, v, l);
    }
  }
}

//
// Zoom in
//
void game_map_zoom_in(Gamep g)
{
  DBG("zoom in");
  TRACE_INDENT();

  game_map_zoom_set(g, game_map_zoom_def_get(g));
  game_map_zoom_update(g);
}

//
// Zoom out
//
void game_map_zoom_out(Gamep g)
{
  DBG("zoom out");
  TRACE_INDENT();

  game_map_zoom_set(g, MAP_ZOOM_FULL_MAP);
  game_map_zoom_update(g);
}

[[nodiscard]] auto game_cand_player_get(Gamep g) -> Tpp
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return nullptr;
  }

  Thingp t = g->cand_player;
  if (t == nullptr) {
    return nullptr;
  }

  return thing_tp(t);
}
[[nodiscard]] auto game_cand_player_get_thing(Gamep g) -> Thingp
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return nullptr;
  }

  return g->cand_player;
}
void game_cand_player_set(Gamep g, Thingp t)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->cand_player = t;
}
void game_cand_player_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->cand_player = {};
}
void game_player_clear(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->cand_player   = {};
  g->chosen_player = {};
}
[[nodiscard]] auto game_chosen_player_get(Gamep g) -> Tpp
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return nullptr;
  }
  return g->chosen_player;
}
void game_chosen_player_set(Gamep g, Tpp t)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->chosen_player = t;
}

[[nodiscard]] auto game_cand_sacrifice_get(Gamep g) -> std::vector< Tpp >
{
  TRACE();

  std::vector< Tpp > out;
  if (g == nullptr) [[unlikely]] {
    return out;
  }
  for (auto t : g->cand_sacrifice) {
    Thingp it = t.first;
    out.push_back(thing_tp(it));
  }
  return out;
}
[[nodiscard]] auto game_cand_sacrifice_get_last(Gamep g) -> Thingp
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return nullptr;
  }
  return g->cand_sacrifice_last;
}
void game_cand_sacrifice_set(Gamep g, Thingp t)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->cand_sacrifice[ t ] = true;
  g->cand_sacrifice_last = t;
}
void game_cand_sacrifice_unset(Gamep g, Thingp t)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->cand_sacrifice.erase(t);
  g->cand_sacrifice_last = {};
}
void game_sacrifice_clear(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->cand_sacrifice      = {};
  g->cand_sacrifice      = {};
  g->cand_sacrifice_last = {};
}
[[nodiscard]] auto game_chosen_sacrifice_get(Gamep g) -> std::vector< Tpp >
{
  TRACE();

  std::vector< Tpp > out;

  if (g == nullptr) [[unlikely]] {
    return out;
  }
  return g->chosen_sacrifice;
}
[[nodiscard]] auto game_cand_sacrifice_find(Gamep g, Thingp t) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return false;
  }

  if (g->cand_sacrifice.contains(t)) {
    return true;
  }
  return false;
}
void game_chosen_sacrifice_set(Gamep g, std::vector< Tpp > t)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->chosen_sacrifice = std::move(t);
}

[[nodiscard]] auto game_mouse_over_player_get(Gamep g) -> Thingp
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return nullptr;
  }
  return g->mouse_over_player;
}
void game_mouse_over_player_set(Gamep g, Thingp t)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->mouse_over_player = t;
}

[[nodiscard]] auto game_mouse_over_sacrifice_get(Gamep g) -> Thingp
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    return nullptr;
  }
  return g->mouse_over_sacrifice;
}
void game_mouse_over_sacrifice_set(Gamep g, Thingp t)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->mouse_over_sacrifice = t;
}

[[nodiscard]] auto game_map_single_pix_size_get(Gamep g) -> int
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return 1;
  }

  if (g->single_pix_size == 0) {
    return 1;
  }

  return g->single_pix_size;
}
void game_map_single_pix_size_set(Gamep g, int val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->single_pix_size = val;
}

[[nodiscard]] auto game_request_to_remake_ui_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return false;
  }

  return g->request_to_remake_ui;
}
void game_request_to_remake_ui_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_to_remake_ui = true;
}
void game_request_to_remake_ui_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_to_remake_ui = false;
}

[[nodiscard]] auto game_request_to_save_game_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return true;
  }

  return g->request_to_save_game;
}
void game_request_to_save_game_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_to_save_game = true;
}
void game_request_to_save_game_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_to_save_game = false;
}

[[nodiscard]] auto game_request_to_update_cursor_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return true;
  }

  return g->request_to_update_cursor;
}
void game_request_to_update_cursor_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_to_update_cursor = true;
}
void game_request_to_update_cursor_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_to_update_cursor = false;
}

[[nodiscard]] auto game_request_reached_exit_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return true;
  }

  return g->request_reached_exit;
}
void game_request_reached_exit_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_reached_exit = true;
}
void game_request_reached_exit_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_reached_exit = false;
}

[[nodiscard]] auto game_request_reached_entrance_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return true;
  }

  return g->request_reached_entrance;
}
void game_request_reached_entrance_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_reached_entrance = true;
}
void game_request_reached_entrance_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_reached_entrance = false;
}

[[nodiscard]] auto game_request_to_end_game_get(Gamep g) -> bool
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return true;
  }

  return g->request_to_end_game;
}
void game_request_to_end_game_set(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_to_end_game = true;
}
void game_request_to_end_game_unset(Gamep g)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_to_end_game = false;
}

[[nodiscard]] auto game_request_to_end_game_reason_get(Gamep g) -> std::string
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return "";
  }

  return g->request_to_end_game_reason;
}
void game_request_to_end_game_reason_set(Gamep g, const std::string &val)
{
  TRACE();

  if (g == nullptr) [[unlikely]] {
    ERR("no game pointer");
    return;
  }
  g->request_to_end_game_reason = val;
}
