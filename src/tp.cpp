//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_cpp_template.hpp"
#include "my_dice_rolls.hpp"
#include "my_game_defs.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_random.hpp"
#include "my_thing.hpp"
#include "my_tp.hpp"
#include "my_tp_class.hpp"
#include "my_tp_inlines.hpp"
#include "my_tps.hpp"
#include "my_types.hpp"
#include "my_ui.hpp"

#include <cstdint>
#include <initializer_list>
#include <map>
#include <string>
#include <utility>
#include <vector>

//
// Templates can be assigned dynamic IDs - however the levels are more reproducable it
// they have fixed numbers. This list helps to achieve that. It is optional for a template
// to be in this list
//
/* clang-format off */
static std::initializer_list< std::string > tps = {
    // clang-format off
    "", // ID 0 means unused
    "floor1", // Update FLOOR_VARIANTS
    "floor2", // Update FLOOR_VARIANTS
    "floor3", // Update FLOOR_VARIANTS
    "floor4", // Update FLOOR_VARIANTS
    "floor5", // Update FLOOR_VARIANTS
    "floor6", // Update FLOOR_VARIANTS
    "floor7", // Update FLOOR_VARIANTS
    "floor8", // Update FLOOR_VARIANTS
    "floor9", // Update FLOOR_VARIANTS
    "floor10", // Update FLOOR_VARIANTS
    "door_secret1", // Update WALL_VARIANTS
    "door_secret2", // Update WALL_VARIANTS
    "door_secret3", // Update WALL_VARIANTS
    "door_secret4", // Update WALL_VARIANTS
    "door_secret5", // Update WALL_VARIANTS
    "door_secret6", // Update WALL_VARIANTS
    "door_secret7", // Update WALL_VARIANTS
    "door_secret8", // Update WALL_VARIANTS
    "door_secret9", // Update WALL_VARIANTS
    "door_secret10", // Update WALL_VARIANTS
    "door_secret11", // Update WALL_VARIANTS
    "door_secret12", // Update WALL_VARIANTS
    "door_secret13", // Update WALL_VARIANTS
    "door_secret14", // Update WALL_VARIANTS
    "door_secret15", // Update WALL_VARIANTS
    "door_secret16", // Update WALL_VARIANTS
    "door_secret17", // Update WALL_VARIANTS
    "door_secret18", // Update WALL_VARIANTS
    "door_secret19", // Update WALL_VARIANTS
    "door_secret20", // Update WALL_VARIANTS
    "door_secret21", // Update WALL_VARIANTS
    "door_secret22", // Update WALL_VARIANTS
    "door_secret23", // Update WALL_VARIANTS
    "door_secret24", // Update WALL_VARIANTS
    "door_secret25", // Update WALL_VARIANTS
    "door_secret26", // Update WALL_VARIANTS
    "wall1", // Update WALL_VARIANTS
    "wall2", // Update WALL_VARIANTS
    "wall3", // Update WALL_VARIANTS
    "wall4", // Update WALL_VARIANTS
    "wall5", // Update WALL_VARIANTS
    "wall6", // Update WALL_VARIANTS
    "wall7", // Update WALL_VARIANTS
    "wall8", // Update WALL_VARIANTS
    "wall9", // Update WALL_VARIANTS
    "wall10", // Update WALL_VARIANTS
    "wall11", // Update WALL_VARIANTS
    "wall12", // Update WALL_VARIANTS
    "wall13", // Update WALL_VARIANTS
    "wall14", // Update WALL_VARIANTS
    "wall15", // Update WALL_VARIANTS
    "wall16", // Update WALL_VARIANTS
    "wall17", // Update WALL_VARIANTS
    "wall18", // Update WALL_VARIANTS
    "wall19", // Update WALL_VARIANTS
    "wall20", // Update WALL_VARIANTS
    "wall21", // Update WALL_VARIANTS
    "wall22", // Update WALL_VARIANTS
    "wall23", // Update WALL_VARIANTS
    "wall24", // Update WALL_VARIANTS
    "wall25", // Update WALL_VARIANTS
    "wall26", // Update WALL_VARIANTS
    "vault1", // Update VAULT_VARIANTS
    "vault2", // Update VAULT_VARIANTS
    "vault3", // Update VAULT_VARIANTS
    "vault4", // Update VAULT_VARIANTS
    "vault5", // Update VAULT_VARIANTS
    "vault6", // Update VAULT_VARIANTS
    "vault7", // Update VAULT_VARIANTS
    "vault8", // Update VAULT_VARIANTS
    "vault9", // Update VAULT_VARIANTS
    "vault10", // Update VAULT_VARIANTS
    "vault11", // Update VAULT_VARIANTS
    "vault12", // Update VAULT_VARIANTS
    "vault13", // Update VAULT_VARIANTS
    "vault14", // Update VAULT_VARIANTS
    "vault15", // Update VAULT_VARIANTS
    "vault16", // Update VAULT_VARIANTS
    "vault17", // Update VAULT_VARIANTS
    "vault18", // Update VAULT_VARIANTS
    "vault19", // Update VAULT_VARIANTS
    "vault20", // Update VAULT_VARIANTS
    "rock1",
    "border",
    /* begin shell marker1 */
    /* shell for i in $(find . -name "*.cpp" | xargs grep -h "tp_load(\"" | sort | awk '{print $4}' | cut -d\" -f2) */
    /* shell do */
    /* shell echo "    \"$i\"", */
    /* shell done */
    "argusul",
    "barrel",
    "beam_of_energy",
    "beam_of_fire",
    "blitzhound",
    "boost_devoted_thrust",
    "boost_healing_drain",
    "boots_fire",
    "boots_spider",
    "brazier",
    "bridge",
    "buff_bad_luck",
    "buff_good_luck",
    "buff_immune_fire",
    "buff_invis",
    "buff_levit",
    "buff_poison",
    "buff_prot",
    "buff_resistant_fire",
    "buff_stealth",
    "buff_tireless",
    "chasm",
    "chest",
    "chocolate_frog",
    "cleaner",
    "clover",
    "clown_meat",
    "coil_eel",
    "copious_chest",
    "corridor",
    "cursor_at",
    "cursor_path",
    "dirt",
    "door_locked",
    "door_unlocked",
    "effect_attack",
    "effect_blood",
    "effect_ripple",
    "entrance",
    "exit",
    "explosion",
    "fire",
    "foliage",
    "ghost_mob",
    "ghost",
    "glorp",
    "grass",
    "horseshoe",
    "key",
    "kobalos_mob",
    "kobalos",
    "lava_bg",
    "lava",
    "level_closed",
    "level_curr",
    "level_final",
    "level_locked",
    "level_next",
    "level_open",
    "level_select_bg",
    "mantisman",
    "mummy",
    "ogrik",
    "pale_eel",
    "pillar",
    "pirnana",
    "player1",
    "player2",
    "player3",
    "player4",
    "player5",
    "pot_disloc",
    "pot_healing",
    "pot_incin",
    "pot_invis",
    "pot_levit",
    "pot_prot",
    "pot_stealth",
    "pot_tireless",
    "proj_energy",
    "proj_fire",
    "reeds",
    "ring_life",
    "ring_war",
    "rubble",
    "sac_blood_pact",
    "sac_clumsy",
    "sac_defenceless",
    "sac_fumble_more",
    "sac_noisy",
    "sac_poison_blood",
    "sac_poor_vision",
    "sac_sickly_health",
    "sac_unlucky",
    "sac_weaponless",
    "skeleton_mob",
    "skeleton",
    "skullferno",
    "smoke",
    "spider_baby",
    "spider_giant",
    "spiderweb",
    "staff_energy",
    "staff_fire",
    "steam",
    "teleport",
    "trap_chasm",
    "voider",
    "wand_energy",
    "wand_fire",
    "water_deep",
    "water_shallow",
  /* end shell marker1 */
};
/* clang-format on */

TpVec tp_vec;

// begin sort marker3 {
static TpVec tp_flag_vec[ THING_FLAG_ENUM_MAX ];
static TpVec tp_monst_vec[ MONST_GROUP_ENUM_MAX ];
// end sort marker3 }

static std::map< std::string, class Tp * > tp_name_map;

static bool tp_init_done;

static void tp_fixup();

Tp::Tp()
{
  TRACE_DEBUG();
  NEWPTR(MTYPE_TP, this, "Tp");
}

Tp::~Tp()
{
  TRACE_DEBUG();
  OLDPTR(MTYPE_TP, this);
}

[[nodiscard]] auto tp_find_mand(const std::string &val) -> Tpp
{
  TRACE_DEBUG();

  const std::string &name(val);
  auto               result = tp_name_map.find(name);

  if ((result != tp_name_map.end())) [[unlikely]] {
    return result->second;
  }

  CROAK("tp_find_mand: thing template %s not found", val.c_str());
  return nullptr;
}

[[nodiscard]] auto tp_find_opt(const std::string &val) -> Tpp
{
  TRACE_DEBUG();

  const std::string &name(val);
  auto               result = tp_name_map.find(name);

  if ((result != tp_name_map.end())) [[unlikely]] {
    return result->second;
  }

  return nullptr;
}

[[nodiscard]] auto tp_id_get(Tpp tp) -> TpId
{
  TRACE_DEBUG(); // expensive

  return tp->id;
}

[[nodiscard]] auto tp_init() -> bool
{
  TRACE_DEBUG();

  tp_init_done = true;

  if (! templates_init()) {
    CROAK("templates_init failed");
  }

  tp_fixup();

  return true;
}

void tp_fini()
{
  TRACE_DEBUG();

  if (! tp_init_done) {
    return;
  }
  tp_init_done = false;

  for (auto &tp : tp_name_map) {
    delete tp.second;
  }

  tp_vec.clear();
  tp_name_map.clear();

  for (auto &c : tp_monst_vec) {
    c.clear();
  }

  for (auto &f : tp_flag_vec) {
    f.clear();
  }
}

//
// Assign template IDs
//
static void tp_assign_id(const std::string &tp_name, int *id_out)
{
  TRACE_DEBUG();

  static std::map< std::string, int > tp_preferred_id;

  static const std::vector< std::string > tp_arr(tps);

  static int  id;
  static bool init;

  //
  // Assign static IDs
  //
  if (! init) {
    init = true;
    for (const auto &t : tp_arr) {
      tp_preferred_id[ t ] = id++;
      if (id >= TP_ID_MAX) {
        CROAK("need to increase TP_ID_MAX");
      }
    }
  }

  //
  // Previously known
  //
  if (! tp_preferred_id.contains(tp_name)) {
    tp_preferred_id[ tp_name ] = *id_out = ++id;
    ERR("tp_assign_id: thing template not found [%s] Please edit tp.cpp and add it.", tp_name.c_str());
    return;
  }

  *id_out = tp_preferred_id[ tp_name ];
}

[[nodiscard]] auto tp_load(const std::string &val) -> Tpp
{
  TRACE_DEBUG();

  std::string const &name(val);

  int id = 0;
  tp_assign_id(val, &id);

  if (tp_find_opt(val) != nullptr) {
    ERR("tp_load: thing template name [%s] already loaded", val.c_str());
  }

  auto *tp      = new Tp();
  tp->name      = name;
  tp->tile_name = name;

  auto result = tp_name_map.insert(std::make_pair(name, tp));
  if (! result.second) {
    ERR("tp_load: thing insert name [%s] failed", val.c_str());
  }

  tp_vec.push_back(tp);
  tp->id = tp_vec.size();

  //
  // Finalizing is done in tp_fixup
  //

  return tp;
}

static void tp_fixup()
{
  TRACE_DEBUG();

  for (auto &tp : tp_vec) {
    //
    // Populate the flag map for quick lookup of things that share a flag
    //
    for (auto f = 0; f < THING_FLAG_ENUM_MAX; f++) {
      if (tp->flag[ f ] != 0) {
        tp_flag_vec[ f ].push_back(tp);
      }

      tp_temperature_init(tp);
      tp_collision_init(tp);
      tp_display_init(tp);

      if (tp_is_able_to_be_levitated(tp) || tp_is_able_to_be_invisible(tp)) {
        tp_flag_set(tp, is_able_to_be_buffed);
      }
    }
  }
}

[[nodiscard]] auto tp_first_tile(Tpp tp, ThingAnimType val) -> Tilep
{
  TRACE_DEBUG();

  if (tp == nullptr) [[unlikely]] {
    return nullptr;
  }

  auto *tiles = &tp->tiles[ val ];

  if ((tiles == nullptr) || tiles->empty()) {
    tp_err(tp, "class %d has no tiles", val);
    return nullptr;
  }

  //
  // Get the first anim tile.
  //
  return tp->tiles[ val ][ 0 ];
}

static auto tp_random_select_with_rarity(TpVec &m) -> Tpp
{
  TRACE_DEBUG();

  int tries = 100;
  while (tries-- > 0) {
    auto  index = PCG_RAND() % m.size();
    auto *tp    = m[ index ];

    if (tp == nullptr) [[unlikely]] {
      break;
    }

    switch (tp_rarity_get(tp)) {
      case THING_RARITY_COMMON : return tp;
      case THING_RARITY_UNCOMMON :
        if (d100() < 20) {
          return tp;
        }
        break;
      case THING_RARITY_RARE :
        if (d100() < 10) {
          return tp;
        }
        break;
      case THING_RARITY_VERY_RARE :
        if (d100() < 5) {
          return tp;
        }
        break;
      case THING_RARITY_UNIQUE :
        if (d1000() == 0) {
          return tp;
        }
        break;
      case THING_RARITY_ENUM_MAX : break;
    }
  }

  return nullptr;
}

static auto tp_random(Gamep g, Levelsp v, Levelp l, TpVec &m) -> Tpp
{
  TRACE_DEBUG();

  auto biome = BIOME_NONE;
  if (l != nullptr) {
    biome = level_to_biome(g, v, l);
  }

  int tries = 100;
  while (tries-- > 0) {
    auto *tp = tp_random_select_with_rarity(m);
    if (tp == nullptr) [[unlikely]] {
      break;
    }

    auto is_biome_dungeon    = tp_is_biome_dungeon(tp);
    auto is_biome_bogland    = tp_is_biome_bogland(tp);
    auto is_biome_nethervoid = tp_is_biome_nethervoid(tp);
    auto is_biome_graveyard  = tp_is_biome_graveyard(tp);
    auto is_biome_underhell  = tp_is_biome_underhell(tp);
    auto is_biome_restricted = is_biome_dungeon ||    // newline
                               is_biome_bogland ||    // newline
                               is_biome_nethervoid || // newline
                               is_biome_graveyard ||  // newline
                               is_biome_underhell;

    if (is_biome_restricted) {
      if (biome == BIOME_DUNGEON) {
        if (! is_biome_dungeon) {
          continue;
        }
      }

      if (biome == BIOME_BOGLAND) {
        if (! is_biome_bogland) {
          continue;
        }
      }

      if (biome == BIOME_NETHERVOID) {
        if (! is_biome_nethervoid) {
          continue;
        }
      }

      if (biome == BIOME_GRAVEYARD) {
        if (! is_biome_graveyard) {
          continue;
        }
      }

      if (biome == BIOME_UNDERHELL) {
        if (! is_biome_underhell) {
          continue;
        }
      }
    }

    //
    // If this thing has a limited chance of appearing, roll the dice.
    //
    auto chance = tp->chance_d1000_appearing;
    if (chance != 0) {
      auto roll = d1000();
      if (roll < chance) {
        return tp;
      }
    } else {
      return tp;
    }
  }

  //
  // Try again, but without the biome filter
  //
  tries = 100;
  while (tries-- > 0) {
    auto *tp = tp_random_select_with_rarity(m);
    if (tp == nullptr) [[unlikely]] {
      break;
    }

    //
    // If this thing has a limited chance of appearing, roll the dice.
    //
    auto chance = tp->chance_d1000_appearing;
    if (chance != 0) {
      auto roll = d1000();
      if (roll < chance) {
        return tp;
      }
    } else {
      return tp;
    }
  }

  //
  // Give in and return the first we find.
  //
  auto  index = PCG_RAND() % m.size();
  auto *tp    = m[ index ];
  return tp;
}

[[nodiscard]] auto tp_random_monst(Gamep g, Levelsp v, Levelp l, int c) -> Tpp
{
  TRACE_DEBUG();

  if (c >= MONST_GROUP_ENUM_MAX) {
    ERR("tp_random_monst: monst bad rating %d", c);
    return nullptr;
  }

  if ((tp_monst_vec[ c ].empty())) [[unlikely]] {
    ERR("tp_random_monst: no rating %d monsters found", c);
    return nullptr;
  }

  return tp_random(g, v, l, tp_monst_vec[ c ]);
}

[[nodiscard]] auto tp_random(Gamep g, Levelsp v, Levelp l, ThingFlagType f) -> Tpp
{
  TRACE_DEBUG();

  if ((tp_flag_vec[ f ].empty())) [[unlikely]] {
    ERR("tp_random: no tp found for ThingFlagType %d/%s", f, ThingFlagType_to_c_str(f));
    return nullptr;
  }

  if (f == is_treasure) {
    //
    // keep for tests
    //
  } else {
    if (g_opt_tests) {
      return tp_first(f);
    }
  }

  return tp_random(g, v, l, tp_flag_vec[ f ]);
}

[[nodiscard]] auto tp_variant(ThingFlagType f, int variant) -> Tpp
{
  TRACE_DEBUG();

  for (auto *tp : tp_flag_vec[ f ]) {
    if (tp_variant_get(tp) == variant) {
      return tp;
    }
  }

  ERR("tp_variant: failed to find %d/%s variant %d", f, ThingFlagType_to_c_str(f), variant);
  return nullptr;
}

[[nodiscard]] auto tp_first(ThingFlagType f) -> Tpp
{
  TRACE_DEBUG();

  if ((tp_flag_vec[ f ].empty())) [[unlikely]] {
    ERR("tp_first: no tp found for ThingFlagType %d/%s", f, ThingFlagType_to_c_str(f));
    return nullptr;
  }
  return tp_flag_vec[ f ][ 0 ];
}

void tp_damage_set(Tpp tp, ThingEventType stat, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }

  if (stat >= THING_EVENT_ENUM_MAX) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, stat);
    return;
  }

  tp->damage[ stat ] = Dice(std::string(val));
}

//
// Get the damage roll string
//
[[nodiscard]] auto tp_damage_dice_roll_string(Tpp tp, ThingEventType val) -> std::string
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return "";
  }

  if (val >= THING_EVENT_ENUM_MAX) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return "";
  }

  return tp->damage[ val ].to_string();
}

[[nodiscard]] auto tp_damage_random_type_get(Tpp tp) -> ThingEventType
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return THING_EVENT_NONE;
  }

  std::vector< ThingEventType > cands;

  for (auto v = 0; v < THING_EVENT_ENUM_MAX; v++) {
    if (tp->damage[ v ].initialized) {
      cands.push_back(static_cast< ThingEventType >(v));
    }
  }

  if (cands.empty()) {
    tp_err(tp, "no damage types set");
    return THING_EVENT_NONE;
  }

  return pcg_rand_one_of(cands);
}

void tp_chance_set(Tpp tp, ThingChanceType ev, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }

  if (ev >= THING_CHANCE_ENUM_MAX) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, ev);
    return;
  }

  tp->chance[ ev ] = Dice(std::string(val));
}

//
// Roll for chance
//
[[nodiscard]] auto tp_chance(Tpp tp, ThingChanceType val) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }

  if (val >= THING_CHANCE_ENUM_MAX) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return 0;
  }

  return tp->chance[ val ].roll();
}

void tp_stat_set(Tpp tp, ThingStatType stat, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }

  if (stat >= THING_STAT_ENUM_MAX) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, stat);
    return;
  }

  tp->stat[ stat ] = Dice(std::string(val));
}

//
// Roll for stat
//
[[nodiscard]] auto tp_stat(Tpp tp, ThingStatType val) -> int
{
  TRACE_DEBUG();

  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }

  if (val >= THING_STAT_ENUM_MAX) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return 0;
  }

  if (! tp->stat[ val ].initialized) {
    return THING_STAT_DEFAULT;
  }

  return tp->stat[ val ].roll();
}

//
// Roll for chance of success
//
[[nodiscard]] auto tp_chance_success(Tpp tp, ThingChanceType val) -> bool
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }

  if (val >= THING_CHANCE_ENUM_MAX) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return false;
  }

  auto roll = tp->chance[ val ].roll();
  if (roll == 0) {
    return false;
  }

  if (compiler_unused) {
    tp_con(tp, " tp_chance_success roll %d vs %d", roll, tp->chance[ val ].max_roll());
  }

  return roll == tp->chance[ val ].max_roll();
}

//
// Roll for chance of failure
//
[[nodiscard]] auto tp_chance_fail(Tpp tp, ThingChanceType val) -> bool
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }

  if (val >= THING_CHANCE_ENUM_MAX) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return false;
  }

  auto roll = tp->chance[ val ].roll();

  if (compiler_unused) {
    tp_con(tp, " tp_chance_fail roll %d vs %d", roll, 1);
  }

  return roll == 1;
}

[[nodiscard]] auto tp_tiles_get(Tpp tp, ThingAnimType val, int index) -> Tilep
{
  TRACE_DEBUG();

  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return nullptr;
  }

  if (std::cmp_greater_equal(index, tp->tiles[ val ].size())) {
    tp_err(tp, "tp_tiles_get: tile overflow tp %s class %s/%d index %d", tp->name.c_str(), ThingAnimType_to_string(val).c_str(), val, index);
    return nullptr;
  }

  return tp->tiles[ val ][ index ];
}

void tp_tiles_push_back(Tpp tp, ThingAnimType val, Tilep tile)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }

  if (val >= THING_ANIM_ENUM_MAX) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return;
  }

  tp->tiles[ val ].push_back(tile);
}

[[nodiscard]] auto tp_tiles_size(Tpp tp, ThingAnimType val) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return static_cast< int >(tp->tiles[ val ].size());
}

[[nodiscard]] auto tp_name(Tpp tp) -> std::string
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return "<noname>";
  }
  return tp->name;
}

[[nodiscard]] auto tp_tile_name(Tpp tp) -> std::string
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return "<notile_name>";
  }
  return tp->tile_name;
}

void tp_tile_name_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->tile_name = std::string(val);
}

void tp_name_short_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->name_short = std::string(val);
  if (tp->name_short.size() > UI_MAX_THING_NAME_SHORT) {
    tp_err(tp, "string [%s] too long for short names", val.c_str());
    return;
  }
}

[[nodiscard]] auto tp_name_short(Tpp tp) -> std::string
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return "<noshortname>";
  }
  if (tp->name_short.empty()) {
    return tp_name(tp);
  }
  return tp->name_short;
}

void tp_name_long_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->name_long = std::string(val);
  if (tp->name_long.size() > UI_MAX_THING_NAME_LONG) {
    tp_err(tp, "string [%s] too long for long names", val.c_str());
    return;
  }
}

[[nodiscard]] auto tp_name_long(Tpp tp) -> std::string
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return "<nolongname>";
  }
  if (tp->name_long.empty()) {
    return tp_name_short(tp);
  }
  return tp->name_long;
}

void tp_name_apostrophize_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->name_apostrophize = std::string(val);
}

[[nodiscard]] auto tp_name_apostrophize(Tpp tp) -> std::string
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return "<noapostrophizename>";
  }
  if (tp->name_apostrophize.empty()) {
    return tp_name_long(tp);
  }
  return tp->name_apostrophize;
}

void tp_name_a_or_an_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->name_a_or_an = std::string(val);
}

[[nodiscard]] auto tp_name_a_or_an(Tpp tp) -> std::string
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return "<noa_or_anname>";
  }
  if (tp->name_a_or_an.empty()) {
    return tp_name_long(tp);
  }
  return tp->name_a_or_an;
}

void tp_name_pluralize_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->name_pluralize = std::string(val);
}

[[nodiscard]] auto tp_name_pluralize(Tpp tp) -> std::string
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return "<nopluralizename>";
  }
  if (tp->name_pluralize.empty()) {
    return tp_name_long(tp);
  }
  return tp->name_pluralize;
}

void tp_name_real_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->name_real = std::string(val);
}

[[nodiscard]] auto tp_name_real(Tpp tp) -> std::string
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return "<norealname>";
  }
  if (tp->name_real.empty()) {
    return tp_name_long(tp);
  }
  return tp->name_real;
}

void tp_light_color_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->light_color = color_find(val.c_str());
}

void tp_light_color_apply(Tpp tp)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  GLCOLOR(tp->light_color);
}

[[nodiscard]] auto tp_light_color(Tpp tp) -> color
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return WHITE;
  }
#endif
  return tp->light_color;
}

void tp_flag_set(Tpp tp, ThingFlagType f, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->flag[ f ] = val;
}

void tp_z_depth_set(Tpp tp, MapZDepthType val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->z_depth = val;
}

[[nodiscard]] auto tp_z_depth_get(Tpp tp) -> MapZDepthType
{
#ifdef DEBUG_BUILD
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return MAP_Z_DEPTH_FLOOR;
  }
#endif
  return tp->z_depth;
}

void tp_speed_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->speed = val;
}

[[nodiscard]] auto tp_speed_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->speed;
}

void tp_weight_set(Tpp tp, uint32_t val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->weight = val;
}

[[nodiscard]] auto tp_weight_get(Tpp tp) -> uint32_t
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->weight;
}

void tp_monst_group_add(Tpp tp, ThingMonstGroup val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }

  if (static_cast< int >(val) >= static_cast< int >(MONST_GROUP_ENUM_MAX)) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return;
  }

  if (tp->is_monst_group[ val ]) {
    return;
  }

  tp->is_monst_group[ val ] = true;

  //
  // Keep track of all things in each group so we can select random monsters from them
  //
  tp_monst_vec[ val ].push_back(tp);

  switch (val) {
    case MOB_GROUP1 :   tp_flag_set(tp, is_mob1); break;
    case MOB_GROUP2 :   tp_flag_set(tp, is_mob2); break;
    case MONST_GROUP1 : tp_flag_set(tp, is_monst1); break;
    case MONST_GROUP2 : tp_flag_set(tp, is_monst2); break;
    default :           tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val); return;
  }
}

void tp_special_attack_add(Tpp tp, TpSpecialAttack val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }

  if (tp->special_attacks.contains(val.type)) {
    tp_err(tp, "damage type %s already set", val.type.c_str());
    return;
  }

  val.dice = Dice(val.roll);

  tp->special_attacks[ val.type ] = val;
}

auto tp_special_attack_get_random(Tpp tp, TpSpecialAttack &out) -> bool
{
  TRACE_DEBUG();

  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }

  if (tp->special_attacks.empty()) {
    return false;
  }

  for (const auto &d : tp->special_attacks) {
    auto val = d.second;
    if (val.d100 != 0U) {
      if (d100() < val.d100) {
        out = val;
        return true;
      }
    }
  }

  return false;
}

void tp_is_immune_to_add(Tpp tp, ThingEventType val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }

  if (static_cast< int >(val) >= static_cast< int >(THING_EVENT_ENUM_MAX)) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return;
  }

  if (tp->is_immune[ val ]) {
    return;
  }

  tp->is_immune[ val ] = true;
}

[[nodiscard]] auto tp_is_immune_to(Tpp tp, ThingEventType val) -> bool
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }

  if (static_cast< int >(val) >= static_cast< int >(THING_EVENT_ENUM_MAX)) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return false;
  }

  return tp->is_immune[ val ];
}

void tp_is_resistant_to_add(Tpp tp, ThingEventType val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }

  if (static_cast< int >(val) >= static_cast< int >(THING_EVENT_ENUM_MAX)) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return;
  }

  if (tp->is_resistant[ val ]) {
    return;
  }

  tp->is_resistant[ val ] = true;
}

[[nodiscard]] auto tp_is_resistant_to(Tpp tp, ThingEventType val) -> bool
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }

  if (static_cast< int >(val) >= static_cast< int >(THING_EVENT_ENUM_MAX)) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return false;
  }

  return tp->is_resistant[ val ];
}

void tp_is_prone_to_add(Tpp tp, ThingEventType val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }

  if (static_cast< int >(val) >= static_cast< int >(THING_EVENT_ENUM_MAX)) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return;
  }

  if (tp->is_prone[ val ]) {
    return;
  }

  tp->is_prone[ val ] = true;
}

[[nodiscard]] auto tp_is_prone_to(Tpp tp, ThingEventType val) -> bool
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return false;
  }

  if (static_cast< int >(val) >= static_cast< int >(THING_EVENT_ENUM_MAX)) {
    tp_err(tp, "bad value in tp for %s, %d", __FUNCTION__, val);
    return false;
  }

  return tp->is_prone[ val ];
}

void tp_health_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->health_initial = Dice(std::string(val));
}

[[nodiscard]] auto tp_health_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->health_initial.roll();
}

[[nodiscard]] auto tp_health_max_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->health_initial.max_roll();
}

void tp_stamina_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->stamina_initial = Dice(std::string(val));
}

[[nodiscard]] auto tp_stamina_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->stamina;
}

[[nodiscard]] auto tp_stamina_max_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->stamina_initial.max_roll();
}

void tp_temperature_initial_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->temperature_initial = val;
}

void tp_temperature_burns_at_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->temperature_burns_at = val;
}

void tp_temperature_melts_at_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->temperature_melts_at = val;
}

void tp_temperature_damage_at_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->temperature_damage_at = val;
}

[[nodiscard]] auto tp_temperature_damage_at_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->temperature_damage_at;
}

void tp_value1_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->value1 = val;
}

[[nodiscard]] auto tp_value1_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->value1;
}

void tp_value2_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->value2 = val;
}

[[nodiscard]] auto tp_value2_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->value2;
}

void tp_value3_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->value3 = val;
}

[[nodiscard]] auto tp_value3_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->value3;
}

void tp_value4_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->value4 = val;
}

[[nodiscard]] auto tp_value4_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->value4;
}

void tp_value5_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->value5 = val;
}

[[nodiscard]] auto tp_value5_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->value5;
}

void tp_crit_roll_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->crit_roll = val;
}

[[nodiscard]] auto tp_crit_roll_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->crit_roll;
}

void tp_fumble_roll_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->fumble_roll = val;
}

[[nodiscard]] auto tp_fumble_roll_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->fumble_roll;
}

void tp_difficulty_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->difficulty = val;
}

[[nodiscard]] auto tp_difficulty_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->difficulty;
}

void tp_mana_max_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->mana_max = val;
}

[[nodiscard]] auto tp_mana_max_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->mana_max;
}

void tp_mana_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->mana = val;
}

[[nodiscard]] auto tp_mana_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->mana;
}

void tp_sac_points_max_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->sac_points_max = val;
}

[[nodiscard]] auto tp_sac_points_max_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->sac_points_max;
}

void tp_sac_points_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->sac_points = val;
}

[[nodiscard]] auto tp_sac_points_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->sac_points;
}

void tp_jump_attack_pct_chance_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->jump_attack_pct_chance = val;
}

[[nodiscard]] auto tp_jump_attack_pct_chance_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->jump_attack_pct_chance;
}

void tp_items_collected_max_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->items_collected_max = val;
}

[[nodiscard]] auto tp_items_collected_max_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->items_collected_max;
}

void tp_attack_count_max_per_tick_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->attack_max_per_tick = val;
}

[[nodiscard]] auto tp_attack_count_max_per_tick_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->attack_max_per_tick;
}

void tp_hearing_threshold_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->hearing_threshold = val;
}

[[nodiscard]] auto tp_hearing_threshold_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->hearing_threshold;
}

void tp_distance_throw_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->distance_throw = val;
}

[[nodiscard]] auto tp_distance_throw_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->distance_throw;
}

void tp_charge_count_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->charge_count = val;
}

[[nodiscard]] auto tp_charge_count_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->charge_count;
}

void tp_distance_avoid_target_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->distance_avoid_target = val;
}

[[nodiscard]] auto tp_distance_avoid_target_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->distance_avoid_target;
}

void tp_score_value_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->score_value = val;
}

[[nodiscard]] auto tp_score_value_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->score_value;
}

void tp_minion_max_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (val > THING_MINION_MAX) {
    tp_err(tp, "minion max too high: %d", val);
    return;
  }

  tp->minion_max = val;
}

[[nodiscard]] auto tp_minion_max_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->minion_max;
}

void tp_missile_count_max_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  if (val > THING_MISSILE_MAX) {
    tp_err(tp, "missile max too high: %d", val);
    return;
  }

  tp->missile_count_max = val;
}

[[nodiscard]] auto tp_missile_count_max_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->missile_count_max;
}

void tp_distance_minion_from_mob_max_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->distance_minion_from_mob_max = val;
}

[[nodiscard]] auto tp_distance_minion_from_mob_max_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->distance_minion_from_mob_max;
}

void tp_distance_light_penetration_pixels_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->distance_light_penetration_pixels = val;
}

[[nodiscard]] auto tp_distance_light_penetration_pixels_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->distance_light_penetration_pixels;
}

void tp_distance_vision_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->distance_vision = val;
}

[[nodiscard]] auto tp_distance_vision_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->distance_vision;
}

void tp_temperature_thermal_conductivity_set(Tpp tp, float val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->temperature_thermal_conductivity = val;
}

[[nodiscard]] auto tp_temperature_thermal_conductivity_get(Tpp tp) -> float
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->temperature_thermal_conductivity;
}

void tp_temperature_heat_capacity_set(Tpp tp, float val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->temperature_heat_capacity = val;
}

[[nodiscard]] auto tp_temperature_heat_capacity_get(Tpp tp) -> float
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->temperature_heat_capacity;
}

void tp_variant_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->variant = val;
}

[[nodiscard]] auto tp_variant_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->variant;
}

void tp_priority_set(Tpp tp, ThingPriorityType val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->priority = val;
}

[[nodiscard]] auto tp_priority_get(Tpp tp) -> ThingPriorityType
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return THING_PRIORITY_LOWEST;
  }
  return tp->priority;
}

void tp_rarity_set(Tpp tp, ThingRarityType val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->rarity = val;
}

[[nodiscard]] auto tp_rarity_get(Tpp tp) -> ThingRarityType
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return THING_RARITY_COMMON;
  }
  return tp->rarity;
}

void tp_distance_jump_set(Tpp tp, int val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->distance_jump = val;
}

[[nodiscard]] auto tp_distance_jump_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->distance_jump;
}

void tp_lifespan_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->lifespan = Dice(std::string(val));
}

[[nodiscard]] auto tp_lifespan_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->lifespan.roll();
}

[[nodiscard]] auto tp_lifespan_max_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->lifespan.max_roll();
}

void tp_ticks_to_stay_dead_set(Tpp tp, const std::string &val)
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return;
  }
  tp->ticks_to_stay_dead = Dice(std::string(val));
}

[[nodiscard]] auto tp_ticks_to_stay_dead_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->ticks_to_stay_dead.roll();
}

[[nodiscard]] auto tp_ticks_to_stay_dead_max_get(Tpp tp) -> int
{
  TRACE_DEBUG();
  if (tp == nullptr) [[unlikely]] {
    ERR("no thing template pointer");
    return 0;
  }
  return tp->ticks_to_stay_dead.max_roll();
}

[[nodiscard]] auto tp_collision_radius(Tpp t) -> float
{
  TRACE_DEBUG();

  return tp_is_collision_circle_small(t) ? 0.25 : 0.5;
}
