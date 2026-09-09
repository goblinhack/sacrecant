//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_sound.hpp"
#include "my_types.hpp"

void sounds_load(Gamep g)
{
  TRACE();

  //
  // Critical UI sounds
  //
  (void) sound_load(0.1F /* volume */, "data/sounds/ui/keypress.ogg", "keypress");
  (void) sound_load(0.2F /* volume */, "data/sounds/ui/mouse_click.ogg", "click", 2 /* concurrent_max */);
  (void) sound_load(0.5F /* volume */, "data/sounds/ui/error.ogg", "error");
  (void) sound_load(1.0F /* volume */, "data/sounds/ui/chest_open.ogg", "chest_open", 2 /* concurrent_max */);
  (void) sound_load(1.0F /* volume */, "data/sounds/ui/bonus.ogg", "bonus", 2 /* concurrent_max */);
  (void) sound_load(0.5F /* volume */, "data/sounds/ui/start.ogg", "start", 2 /* concurrent_max */);
  (void) sound_load(0.5F /* volume */, "data/sounds/ui/enter.ogg", "enter", 2 /* concurrent_max */);
  (void) sound_load(0.5F /* volume */, "data/sounds/ui/select.ogg", "select", 2 /* concurrent_max */);
  (void) sound_load(1.0F /* volume */, "data/sounds/ui/door_open.ogg", "door_open", 2 /* concurrent_max */);
  (void) sound_load(1.0F /* volume */, "data/sounds/ui/door_stone.ogg", "door_open_secret", 2 /* concurrent_max */);
  (void) sound_load(1.0F /* volume */, "data/sounds/ui/item_collect.ogg", "item_collect", 2 /* concurrent_max */);
  (void) sound_load(1.0F /* volume */, "data/sounds/ui/item_drop.ogg", "item_drop", 2 /* concurrent_max */);

  (void) sound_load(1.0F /* volume */, "data/sounds/impacts/player_oof.ogg", "player_oof");
  (void) sound_load(1.0F /* volume */, "data/sounds/impacts/glass_shatter.ogg", "glass_shatter");
  (void) sound_load(1.0F /* volume */, "data/sounds/impacts/brazier.ogg", "brazier");
  (void) sound_load(1.0F /* volume */, "data/sounds/impacts/player_hit.ogg", "player_hit");
  (void) sound_load(1.0F /* volume */, "data/sounds/impacts/player_ouch.ogg", "player_ouch");
  (void) sound_load(1.0F /* volume */, "data/sounds/impacts/player_punch.ogg", "player_punch");
  (void) sound_load(1.0F /* volume */, "data/sounds/impacts/monst_death.ogg", "monst_death");
  (void) sound_load(1.0F /* volume */, "data/sounds/misc/fall.ogg", "fall");
  (void) sound_load(0.2F /* volume */, "data/sounds/misc/rock.ogg", "rock", 3 /* concurrent_max */);
  (void) sound_load(1.0F /* volume */, "data/sounds/misc/teleport.ogg", "teleport");
  (void) sound_load(1.0F /* volume */, "data/sounds/impacts/explosion.ogg", "explosion", 10 /* concurrent_max */);
  (void) sound_load(1.0F /* volume */, "data/sounds/impacts/splash.ogg", "splash");
  (void) sound_load(1.0F /* volume */, "data/sounds/misc/footstep.ogg", "footstep");
  (void) sound_load(1.0F /* volume */, "data/sounds/misc/footstep_foliage.ogg", "footstep_foliage");
  (void) sound_load(1.0F /* volume */, "data/sounds/missiles/projectile.ogg", "projectile", 5 /* concurrent_max */);
  (void) sound_load(1.0F /* volume */, "data/sounds/missiles/beam_weapon.ogg", "beam_weapon", 5 /* concurrent_max */);
  (void) sound_load(1.0F /* volume */, "data/sounds/treasure/key.ogg", "key");
  (void) sound_load(1.0F /* volume */, "data/sounds/ambience/dungeon_ambience.ogg", "dungeon_ambience");
  (void) sound_load(0.1F /* volume */, "data/sounds/ambience/bogland_ambience.ogg", "bogland_ambience");
  (void) sound_load(1.0F /* volume */, "data/sounds/ambience/nethervoid_ambience.ogg", "nethervoid_ambience");
  (void) sound_load(0.5F /* volume */, "data/sounds/ambience/graveyard_ambience.ogg", "graveyard_ambience");
  (void) sound_load(1.0F /* volume */, "data/sounds/ambience/underhell_ambience.ogg", "underhell_ambience");
  (void) sound_load(1.0F /* volume */, "data/sounds/creatures/hiss1.ogg", "hiss");
  (void) sound_load(1.0F /* volume */, "data/sounds/creatures/squelch.ogg", "squelch");
  (void) sound_load(1.0F /* volume */, "data/sounds/creatures/hiss2.ogg", "hiss");
  (void) sound_load(1.0F /* volume */, "data/sounds/creatures/hiss3.ogg", "hiss");
  (void) sound_load(1.0F /* volume */, "data/sounds/creatures/hiss4.ogg", "hiss");
  (void) sound_load(1.0F /* volume */, "data/sounds/creatures/hiss5.ogg", "hiss");
  (void) sound_load(1.0F /* volume */, "data/sounds/creatures/hiss6.ogg", "hiss");
  (void) sound_load(1.0F /* volume */, "data/sounds/creatures/hiss7.ogg", "hiss");
  (void) sound_load(1.0F /* volume */, "data/sounds/creatures/hiss8.ogg", "hiss");
  (void) sound_load(1.0F /* volume */, "data/sounds/creatures/hiss9.ogg", "hiss");
  (void) sound_load(1.0F /* volume */, "data/sounds/creatures/hiss10.ogg", "hiss");
}
