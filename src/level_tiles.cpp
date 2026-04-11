//
// Copyright goblinhack@gmail.com
//

#include "my_bits.hpp"
#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_random.hpp"
#include "my_tile.hpp"

#include <cstring>
#include <map>

/* clang-format off */
#define IS_JOIN_ENUM(list_macro)                  \
    list_macro(IS_JOIN_BL,     "IS_JOIN_BL"),     \
    list_macro(IS_JOIN_BL2,    "IS_JOIN_BL2"),    \
    list_macro(IS_JOIN_BLOCK,  "IS_JOIN_BLOCK"),  \
    list_macro(IS_JOIN_BOT,    "IS_JOIN_BOT"),    \
    list_macro(IS_JOIN_BR,     "IS_JOIN_BR"),     \
    list_macro(IS_JOIN_BR2,    "IS_JOIN_BR2"),    \
    list_macro(IS_JOIN_HORIZ,  "IS_JOIN_HORIZ"),  \
    list_macro(IS_JOIN_HORIZ2, "IS_JOIN_HORIZ2"), \
    list_macro(IS_JOIN_LEFT,   "IS_JOIN_LEFT"),   \
    list_macro(IS_JOIN_NODE,   "IS_JOIN_NODE"),   \
    list_macro(IS_JOIN_RIGHT,  "IS_JOIN_RIGHT"),  \
    list_macro(IS_JOIN_T_1,    "IS_JOIN_T_1"),    \
    list_macro(IS_JOIN_T_2,    "IS_JOIN_T_2"),    \
    list_macro(IS_JOIN_T_3,    "IS_JOIN_T_3"),    \
    list_macro(IS_JOIN_T,      "IS_JOIN_T"),      \
    list_macro(IS_JOIN_T180_1, "IS_JOIN_T180_1"), \
    list_macro(IS_JOIN_T180_2, "IS_JOIN_T180_2"), \
    list_macro(IS_JOIN_T180_3, "IS_JOIN_T180_3"), \
    list_macro(IS_JOIN_T180,   "IS_JOIN_T180"),   \
    list_macro(IS_JOIN_T270_1, "IS_JOIN_T270_1"), \
    list_macro(IS_JOIN_T270_2, "IS_JOIN_T270_2"), \
    list_macro(IS_JOIN_T270_3, "IS_JOIN_T270_3"), \
    list_macro(IS_JOIN_T270,   "IS_JOIN_T270"),   \
    list_macro(IS_JOIN_T90_1,  "IS_JOIN_T90_1"),  \
    list_macro(IS_JOIN_T90_2,  "IS_JOIN_T90_2"),  \
    list_macro(IS_JOIN_T90_3,  "IS_JOIN_T90_3"),  \
    list_macro(IS_JOIN_T90,    "IS_JOIN_T90"),    \
    list_macro(IS_JOIN_TL,     "IS_JOIN_TL"),     \
    list_macro(IS_JOIN_TL2,    "IS_JOIN_TL2"),    \
    list_macro(IS_JOIN_TOP,    "IS_JOIN_TOP"),    \
    list_macro(IS_JOIN_TR,     "IS_JOIN_TR"),     \
    list_macro(IS_JOIN_TR2,    "IS_JOIN_TR2"),    \
    list_macro(IS_JOIN_VERT,   "IS_JOIN_VERT"),   \
    list_macro(IS_JOIN_VERT2,  "IS_JOIN_VERT2"),  \
    list_macro(IS_JOIN_X,      "IS_JOIN_X"),      \
    list_macro(IS_JOIN_X1_180, "IS_JOIN_X1_180"), \
    list_macro(IS_JOIN_X1_270, "IS_JOIN_X1_270"), \
    list_macro(IS_JOIN_X1_90,  "IS_JOIN_X1_90"),  \
    list_macro(IS_JOIN_X1,     "IS_JOIN_X1"),     \
    list_macro(IS_JOIN_X2_180, "IS_JOIN_X2_180"), \
    list_macro(IS_JOIN_X2_270, "IS_JOIN_X2_270"), \
    list_macro(IS_JOIN_X2_90,  "IS_JOIN_X2_90"),  \
    list_macro(IS_JOIN_X2,     "IS_JOIN_X2"),     \
    list_macro(IS_JOIN_X3_180, "IS_JOIN_X3_180"), \
    list_macro(IS_JOIN_X3,     "IS_JOIN_X3"),     \
    list_macro(IS_JOIN_X4_180, "IS_JOIN_X4_180"), \
    list_macro(IS_JOIN_X4_270, "IS_JOIN_X4_270"), \
    list_macro(IS_JOIN_X4_90,  "IS_JOIN_X4_90"),  \
    list_macro(IS_JOIN_X4,     "IS_JOIN_X4"),

/* clang-format on */

ENUM_DEF_H(IS_JOIN_ENUM, is_join_enum)
ENUM_DEF_C(IS_JOIN_ENUM, is_join_enum)

static void level_assign_tiles_at(Gamep g, Levelsp v, Levelp l, const bpoint &p)
{
  TRACE();

  auto x = p.x;
  auto y = p.y;

  for (auto slot = 0; slot < MAP_SLOTS; slot++) {
    Tpp tp = nullptr;

    auto *t = thing_and_tp_get_at(g, v, l, p, slot, &tp);
    if (t == nullptr) {
      continue;
    }

    if (tp_is_tiled(tp)) {
      auto tile_name = tp_tile_name(tp);

      uint16_t const A = level_is_same_obj_type_at(g, v, l, bpoint(x - 1, y - 1), tp) ? 1 : 0;
      uint16_t const B = level_is_same_obj_type_at(g, v, l, bpoint(x, y - 1), tp) ? 1 : 0;
      uint16_t const C = level_is_same_obj_type_at(g, v, l, bpoint(x + 1, y - 1), tp) ? 1 : 0;
      uint16_t const D = level_is_same_obj_type_at(g, v, l, bpoint(x - 1, y), tp) ? 1 : 0;
      uint16_t const E = level_is_same_obj_type_at(g, v, l, bpoint(x, y), tp) ? 1 : 0;
      uint16_t const F = level_is_same_obj_type_at(g, v, l, bpoint(x + 1, y), tp) ? 1 : 0;
      uint16_t const G = level_is_same_obj_type_at(g, v, l, bpoint(x - 1, y + 1), tp) ? 1 : 0;
      uint16_t const H = level_is_same_obj_type_at(g, v, l, bpoint(x, y + 1), tp) ? 1 : 0;
      uint16_t const I = level_is_same_obj_type_at(g, v, l, bpoint(x + 1, y + 1), tp) ? 1 : 0;

      const uint16_t omask = (I << 8) | (H << 7) | (G << 6) | (F << 5) | (E << 4) | (D << 3) | (C << 2) | (B << 1) | (A << 0);

      uint8_t  block_type_score = 0;
      uint8_t  best_block_type  = 0;
      int8_t   block_type       = -1;
      uint16_t mask             = 0;

#define BLOCK(a, b, c, d, e, f, g, h, i, _index_)                                                                                          \
  mask = ((i) << 8) | ((h) << 7) | ((g) << 6) | ((f) << 5) | ((e) << 4) | ((d) << 3) | ((c) << 2) | ((b) << 1) | ((a) << 0);               \
                                                                                                                                           \
  if ((mask & omask) == mask) {                                                                                                            \
    uint32_t difference = mask ^ omask;                                                                                                    \
    BITCOUNT(difference);                                                                                                                  \
    block_type_score = 32 - difference;                                                                                                    \
    if (block_type_score > best_block_type) {                                                                                              \
      best_block_type = block_type_score;                                                                                                  \
      block_type      = _index_;                                                                                                           \
    }                                                                                                                                      \
  }

      BLOCK(1, 1, 1, 1, 1, 1, 1, 1, 1, IS_JOIN_BLOCK)
      BLOCK(0, 0, 0, 0, 1, 0, 0, 0, 0, IS_JOIN_NODE)
      BLOCK(0, 0, 0, 0, 0, 0, 0, 0, 0, IS_JOIN_NODE) // moving blocks
      BLOCK(0, 0, 0, 0, 1, 1, 0, 0, 0, IS_JOIN_LEFT)
      BLOCK(0, 0, 0, 0, 1, 0, 0, 1, 0, IS_JOIN_TOP)
      BLOCK(0, 0, 0, 1, 1, 0, 0, 0, 0, IS_JOIN_RIGHT)
      BLOCK(0, 1, 0, 0, 1, 0, 0, 0, 0, IS_JOIN_BOT)
      BLOCK(0, 0, 0, 1, 1, 1, 0, 0, 0, IS_JOIN_HORIZ)
      BLOCK(0, 1, 0, 0, 1, 0, 0, 1, 0, IS_JOIN_VERT)
      BLOCK(0, 0, 0, 0, 1, 1, 0, 1, 1, IS_JOIN_TL2)
      BLOCK(0, 1, 1, 0, 1, 1, 0, 0, 0, IS_JOIN_BL2)
      BLOCK(1, 1, 0, 1, 1, 0, 0, 0, 0, IS_JOIN_BR2)
      BLOCK(0, 0, 0, 1, 1, 0, 1, 1, 0, IS_JOIN_TR2)
      BLOCK(0, 0, 0, 0, 1, 1, 0, 1, 0, IS_JOIN_TL)
      BLOCK(0, 1, 0, 0, 1, 1, 0, 0, 0, IS_JOIN_BL)
      BLOCK(0, 1, 0, 1, 1, 0, 0, 0, 0, IS_JOIN_BR)
      BLOCK(0, 0, 0, 1, 1, 0, 0, 1, 0, IS_JOIN_TR)
      BLOCK(1, 1, 0, 1, 1, 0, 1, 1, 0, IS_JOIN_T90_3)
      BLOCK(1, 1, 1, 1, 1, 1, 0, 0, 0, IS_JOIN_T180_3)
      BLOCK(0, 1, 1, 0, 1, 1, 0, 1, 1, IS_JOIN_T270_3)
      BLOCK(0, 0, 0, 1, 1, 1, 1, 1, 1, IS_JOIN_T_3)
      BLOCK(0, 1, 0, 0, 1, 1, 0, 1, 0, IS_JOIN_T270)
      BLOCK(0, 1, 0, 1, 1, 1, 0, 0, 0, IS_JOIN_T180)
      BLOCK(0, 1, 0, 1, 1, 0, 0, 1, 0, IS_JOIN_T90)
      BLOCK(0, 0, 0, 1, 1, 1, 0, 1, 0, IS_JOIN_T)
      BLOCK(0, 1, 1, 0, 1, 1, 0, 1, 0, IS_JOIN_T270_2)
      BLOCK(1, 1, 0, 1, 1, 1, 0, 0, 0, IS_JOIN_T180_2)
      BLOCK(0, 1, 0, 1, 1, 0, 1, 1, 0, IS_JOIN_T90_2)
      BLOCK(0, 0, 0, 1, 1, 1, 0, 1, 1, IS_JOIN_T_2)
      BLOCK(0, 1, 0, 0, 1, 1, 0, 1, 1, IS_JOIN_T270_1)
      BLOCK(0, 1, 1, 1, 1, 1, 0, 0, 0, IS_JOIN_T180_1)
      BLOCK(1, 1, 0, 1, 1, 0, 0, 1, 0, IS_JOIN_T90_1)
      BLOCK(0, 0, 0, 1, 1, 1, 1, 1, 0, IS_JOIN_T_1)
      BLOCK(0, 1, 0, 1, 1, 1, 0, 1, 0, IS_JOIN_X)
      BLOCK(0, 1, 0, 1, 1, 1, 0, 1, 1, IS_JOIN_X1)
      BLOCK(0, 1, 1, 1, 1, 1, 0, 1, 0, IS_JOIN_X1_270)
      BLOCK(1, 1, 0, 1, 1, 1, 0, 1, 0, IS_JOIN_X1_180)
      BLOCK(0, 1, 0, 1, 1, 1, 1, 1, 0, IS_JOIN_X1_90)
      BLOCK(0, 1, 0, 1, 1, 1, 1, 1, 1, IS_JOIN_X2)
      BLOCK(0, 1, 1, 1, 1, 1, 0, 1, 1, IS_JOIN_X2_270)
      BLOCK(1, 1, 1, 1, 1, 1, 0, 1, 0, IS_JOIN_X2_180)
      BLOCK(1, 1, 0, 1, 1, 1, 1, 1, 0, IS_JOIN_X2_90)
      BLOCK(0, 1, 1, 1, 1, 1, 1, 1, 0, IS_JOIN_X3)
      BLOCK(1, 1, 0, 1, 1, 1, 0, 1, 1, IS_JOIN_X3_180)
      BLOCK(0, 1, 1, 1, 1, 1, 1, 1, 1, IS_JOIN_X4)
      BLOCK(1, 1, 1, 1, 1, 1, 0, 1, 1, IS_JOIN_X4_270)
      BLOCK(1, 1, 1, 1, 1, 1, 1, 1, 0, IS_JOIN_X4_180)
      BLOCK(1, 1, 0, 1, 1, 1, 1, 1, 1, IS_JOIN_X4_90)

      if (block_type == -1) {
        CROAK("no block type found for %s", tile_name.c_str());
      }

      auto which = tile_name + "." + is_join_enum_to_string(static_cast< is_join_enum >(block_type));

      if (tp_is_animated(tp)) {
        which += ".0";
      }

      auto *tile = tile_find_mand(which);
      if (tile != nullptr) {
        t->tile_index        = tile_global_index(tile);
        t->anim_type         = static_cast< ThingAnim >(block_type);
        t->anim_index        = PCG_RANDOM_RANGE_INCLUSIVE(0, tp_tiles_size(tp, t->anim_type) - 1);
        t->anim_ms_remaining = PCG_RANDOM_RANGE_INCLUSIVE(0, tile_delay_ms(tile));
      }
    }
  }
}

void level_assign_tiles(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  for (auto y = 0; y < MAP_HEIGHT; y++) {
    for (auto x = 0; x < MAP_WIDTH; x++) {
      bpoint const p(x, y);
      level_assign_tiles_at(g, v, l, p);
    }
  }
}

void level_update_tiles(Gamep g, Levelsp v, Levelp l)
{
  TRACE();

  for (auto y = 0; y < MAP_HEIGHT; y++) {
    for (auto x = 0; x < MAP_WIDTH; x++) {
      if (static_cast< bool >(l->is_modified_tile[ x ][ y ])) {
        level_assign_tiles_at(g, v, l, bpoint(x, y));
      }
    }
  }
}
