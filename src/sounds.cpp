//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_sound.hpp"
#include "my_types.hpp"

void sounds_load(Gamep g)
{
  TRACE();

  (void) sound_load(0.1F, "data/sounds/interface/keypress.ogg", "keypress");
  (void) sound_load(0.2F, "data/sounds/interface/mouse_click.ogg", "click", 2);
  (void) sound_load(0.5F, "data/sounds/interface/error.ogg", "error");
  (void) sound_load(1.0F, "data/sounds/impacts/player_oof.ogg", "player_oof");
  (void) sound_load(1.0F, "data/sounds/impacts/glass_shatter.ogg", "glass_shatter");
  (void) sound_load(1.0F, "data/sounds/impacts/brazier.ogg", "brazier");
  (void) sound_load(1.0F, "data/sounds/impacts/player_hit.ogg", "player_hit");
  (void) sound_load(1.0F, "data/sounds/impacts/player_ouch.ogg", "player_ouch");
  (void) sound_load(1.0F, "data/sounds/impacts/player_punch.ogg", "player_punch");
  (void) sound_load(1.0F, "data/sounds/impacts/monst_death.ogg", "monst_death");
  (void) sound_load(1.0F, "data/sounds/misc/fall.ogg", "fall");
  (void) sound_load(0.2F, "data/sounds/misc/rock.ogg", "rock", 3);
  (void) sound_load(1.0F, "data/sounds/misc/teleport.ogg", "teleport");
  (void) sound_load(1.0F, "data/sounds/impacts/explosion.ogg", "explosion", 10 /* concurrent_max */);
  (void) sound_load(1.0F, "data/sounds/impacts/splash.ogg", "splash");
  (void) sound_load(1.0F, "data/sounds/interface/chest_open.ogg", "chest_open", 2);
  (void) sound_load(1.0F, "data/sounds/interface/bonus.ogg", "bonus", 2);
  (void) sound_load(1.0F, "data/sounds/interface/door_open.ogg", "door_open", 2);
  (void) sound_load(1.0F, "data/sounds/interface/door_stone.ogg", "door_open_secret", 2);
  (void) sound_load(1.0F, "data/sounds/interface/item_collect.ogg", "item_collect", 2);
  (void) sound_load(1.0F, "data/sounds/interface/item_drop.ogg", "item_drop", 2);
  (void) sound_load(1.0F, "data/sounds/misc/footstep.ogg", "footstep");
  (void) sound_load(1.0F, "data/sounds/misc/footstep_foliage.ogg", "footstep_foliage");
  (void) sound_load(1.0F, "data/sounds/missiles/projectile.ogg", "projectile", 5);
  (void) sound_load(1.0F, "data/sounds/missiles/beam_weapon.ogg", "beam_weapon", 5);
  (void) sound_load(1.0F, "data/sounds/treasure/key.ogg", "key");
  (void) sound_load(1.0F, "data/sounds/ambience/dungeon_ambience.ogg", "dungeon_ambience");
  (void) sound_load(0.1F, "data/sounds/ambience/bogland_ambience.ogg", "bogland_ambience");
  (void) sound_load(1.0F, "data/sounds/ambience/nethervoid_ambience.ogg", "nethervoid_ambience");
  (void) sound_load(0.5F, "data/sounds/ambience/graveyard_ambience.ogg", "graveyard_ambience");
  (void) sound_load(1.0F, "data/sounds/ambience/underhell_ambience.ogg", "underhell_ambience");
  (void) sound_load(1.0F, "data/sounds/creatures/hiss1.ogg", "hiss");
  (void) sound_load(1.0F, "data/sounds/creatures/squelch.ogg", "squelch");
  (void) sound_load(1.0F, "data/sounds/creatures/hiss2.ogg", "hiss");
  (void) sound_load(1.0F, "data/sounds/creatures/hiss3.ogg", "hiss");
  (void) sound_load(1.0F, "data/sounds/creatures/hiss4.ogg", "hiss");
  (void) sound_load(1.0F, "data/sounds/creatures/hiss5.ogg", "hiss");
  (void) sound_load(1.0F, "data/sounds/creatures/hiss6.ogg", "hiss");
  (void) sound_load(1.0F, "data/sounds/creatures/hiss7.ogg", "hiss");
  (void) sound_load(1.0F, "data/sounds/creatures/hiss8.ogg", "hiss");
  (void) sound_load(1.0F, "data/sounds/creatures/hiss9.ogg", "hiss");
  (void) sound_load(1.0F, "data/sounds/creatures/hiss10.ogg", "hiss");
}
