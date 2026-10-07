//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_fpoint.hpp"
#include "my_game_defs.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_math.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

#include <cmath>

//
// Can't shoot too far
//
static auto thing_beam_weapon_truncate(const fpoint &from, fpoint &to) -> float
{
  float const how_far_i_want_to_shoot = distance(from, to);
  float const how_far_i_can_shoot     = THING_BEAM_WEAPON_TILES_MAX;

  if (how_far_i_want_to_shoot > how_far_i_can_shoot) {
    //
    // Yep. Trying to shoot too far.
    //
    fpoint u = to - from;
    u.unit();
    u *= how_far_i_can_shoot;

    to = from + u;
  }

  return distance(from, to);
}

auto thing_beam_weapon_fire_at(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp what, const fpoint target, const fpoint offset) -> bool
{
  THING_DBG(g, v, l, me, "fire beam weapon at @%f,%f offset %f,%f", target.x, target.y, offset.x, offset.y);
  TRACE_INDENT();

  //
  // No firing when dead!
  //
  if (thing_is_dead(me)) {
    THING_DBG(g, v, l, me, "fire beam weapon, no as firer dead");
    return false;
  }

  //
  // Can't shoot too far
  //
  auto   to       = target;
  fpoint fbeam_at = thing_real_at(g, v, l, me) + offset;

  auto distance = thing_beam_weapon_truncate(fbeam_at, to);
  THING_DBG(g, v, l, me, "distance to fire: %f", distance);

  auto delta = to - fbeam_at;
  if ((delta.x == 0) && (delta.y == 0)) {
    delta.x = 1;
  }

  auto  angle = angle_radians(delta);
  float s     = 0;
  float c     = 0;
  SINCOSF(angle, &s, &c);

  //
  // Need a small fraction to account for comparisons of very similar floats where
  // we end up shooting the player upon firing
  //
  float const self_offset = thing_collision_radius(me) + tp_collision_radius(what) + THING_COLLISION_FIRING_OFFSET;
  fbeam_at.x += c * self_offset;
  fbeam_at.y += s * self_offset;

  //
  // Create all the fragments of the beam_weapon weapon
  //
  auto        beam_target_distance = ceil(distance);
  const float avoid_gaps_in_tiles  = 0.94F;
  int         index {};

  for (auto step = 0; step < beam_target_distance; step++) {
    if (compiler_unused) {
      thing_topcon(g, v, l, me, "%f,%f step %d", fbeam_at.x, fbeam_at.y, step);
    }

    ThingEvent e {};
    e.reason        = "missile spawn";
    e.missile_index = index++;

    auto *beam_weapon = thing_spawn_missile(g, v, l, me, what, fbeam_at, &e);
    if (beam_weapon == nullptr) {
      break;
    }

    beam_weapon->angle = static_cast< f16 >(angle);

    //
    // Special handling for teleporting of a beam_weapon weapon
    //
    if (level_is_teleport_bool(g, v, l, thing_at(g, v, l, beam_weapon))) {
      if (thing_teleport_handle(g, v, l, beam_weapon)) {
        fbeam_at = thing_real_at(g, v, l, beam_weapon);
      }
    }

    bool last = (step == beam_target_distance - 1);
    if (level_is_obs_to_beam(g, v, l, thing_at(g, v, l, beam_weapon)) != nullptr) {
      last = true;
    }

    if (last) {
      beam_weapon->anim_index = THING_BEAM_WEAPON_TILES_MAX - 1;
      break;
    }
    beam_weapon->anim_index = step;

    //
    // Set up the next fragment
    //
    fbeam_at.x += c * avoid_gaps_in_tiles;
    fbeam_at.y += s * avoid_gaps_in_tiles;
  }

  //
  // Set my direction based on where I fire
  //
  bpoint const dir    = make_bpoint(fbeam_at);
  bpoint const source = thing_at(g, v, l, me);
  thing_set_dir_from_delta(g, v, l, me, dir.x - source.x, dir.y - source.y);

  return true;
}

auto thing_beam_weapon_fire_at(Gamep g, Levelsp v, Levelp l, Thingp me, Tpp what, const bpoint target, const fpoint offset) -> bool
{
  return thing_beam_weapon_fire_at(g, v, l, me, what, make_fpoint(target), offset);
}
