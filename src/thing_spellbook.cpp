//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_thing_inlines.hpp"
#include "my_types.hpp"

#include <cstring>

//
// Anything in the spellbook
//
[[nodiscard]] auto thing_spellbook_is_empty(Gamep g, Levelsp v, Levelp l, Thingp owner) -> bool
{
  TRACE();

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  FOR_ALL_SPELLBOOK_SPELLS(g, v, l, owner, spell)
  {
    if (spell != nullptr) {}
    return false;
  }

  return true;
}

//
// Add an spell to the spellbook
//
[[nodiscard]] auto thing_spellbook_add(Gamep g, Levelsp v, Levelp l, Thingp new_spell, Thingp owner) -> bool
{
  THING_DBG(g, v, l, new_spell, "spellbook add");
  TRACE();

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (new_spell == nullptr) {
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return false;
  }

  //
  // Look for a free slot
  //
  FOR_ALL_SPELLBOOK_SLOTS(g, v, l, owner, slot, spell)
  {
    if (spell != nullptr) {
      continue;
    }

    memset(slot, 0, sizeof(*slot));
    slot->thing_id = new_spell->id;
    slot->count    = 1;
    return true;
  }

  //
  // Out of slots
  //
  return false;
}

//
// Is this spell known?
//
[[nodiscard]] auto thing_spellbook_is_learned_spell(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp owner) -> bool
{
  TRACE();

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (spell == nullptr) {
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return false;
  }

  //
  // Look for a free slot
  //
  FOR_ALL_SPELLBOOK_SPELLS(g, v, l, owner, a_spell)
  {
    if (thing_tp(spell) == thing_tp(a_spell)) {
      return true;
    }
  }

  return false;
}

//
// Drop an spell to the spellbook
//
[[nodiscard]] auto thing_spellbook_remove(Gamep g, Levelsp v, Levelp l, Thingp drop_spell, Thingp owner) -> bool
{
  THING_DBG(g, v, l, drop_spell, "spellbook remove");
  TRACE();

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return false;
  }

  if (drop_spell == nullptr) {
    return false;
  }

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return false;
  }

  //
  // Look for the thing
  //
  FOR_ALL_SPELLBOOK_SLOTS(g, v, l, owner, slot, spell)
  {
    if (spell == nullptr) {
      continue;
    }

    if (drop_spell != spell) {
      continue;
    }

    THING_DBG(g, v, l, spell, "slot %d: count %d", _n_, slot->count);
    TRACE_INDENT();

    slot->count--;

    if (slot->count > 0) {
      THING_DBG(g, v, l, spell, "spell count remains");
      return true;
    }

    memset(slot, 0, sizeof(*slot));
    return true;
  }

  return false;
}

//
// Get a single spells slot count
//
[[nodiscard]] auto thing_spellbook_get_spell_count(Gamep g, Levelsp v, Levelp l, Thingp spell, Thingp owner) -> int
{
  TRACE();

  if (spell == nullptr) {
    return -1;
  }

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return -1;
  }

  //
  // Look for the thing
  //
  FOR_ALL_SPELLBOOK_SLOTS(g, v, l, owner, slot, an_spell)
  {
    if (spell == nullptr) {
      continue;
    }

    if (an_spell != spell) {
      continue;
    }

    return slot->count;
  }

  return -1;
}

//
// Get a single spells slot count
//
[[nodiscard]] auto thing_spellbook_get_spell_count(Gamep g, Levelsp v, Levelp l, Thingp owner) -> int
{
  TRACE();

  int count = 0;

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return 0;
  }

  FOR_ALL_SPELLBOOK_SLOTS(g, v, l, owner, slot, spell)
  {
    if (spell == nullptr) {
      continue;
    }

    count += slot->count;
  }

  return count;
}

//
// Get the nth spell
//
[[nodiscard]] auto thing_spellbook_get_spell_n(Gamep g, Levelsp v, Levelp l, Thingp owner, int index) -> Thingp
{
  TRACE();

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return nullptr;
  }

  int walk_index {};

  FOR_ALL_SPELLBOOK_SLOTS(g, v, l, owner, slot, spell)
  {
    if (spell == nullptr) {
      continue;
    }

    if (index == walk_index) {
      return spell;
    }

    walk_index++;
  }

  return nullptr;
}

//
// Drop an spell to the spellbook
//
void thing_spellbook_dump(Gamep g, Levelsp v, Levelp l, Thingp owner)
{
  IF_NODEBUG { return; }

  TRACE();

  if (! thing_is_player(owner) && ! thing_is_monst(owner)) {
    thing_err(g, v, l, owner, "unexpected thing for %s", __FUNCTION__);
    return;
  }

  auto *ext_struct = thing_ext_struct(g, v, owner);
  if (ext_struct == nullptr) {
    return;
  }

  FOR_ALL_SPELLBOOK_SLOTS(g, v, l, owner, slot, spell)
  {
    if (spell == nullptr) {
      THING_DBG(g, v, l, owner, "slot %d: -", _n_);
      continue;
    }

    auto s = to_string(g, v, l, spell);
    THING_DBG(g, v, l, owner, "slot %d: %s, count %d", _n_, s.c_str(), slot->count);
  }
}
