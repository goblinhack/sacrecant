//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_ext.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

//
// Create all treasure levels
//
void levels_treasure(Gamep g)
{
  TRACE();

  {
    Overrides overrides;

    overrides[ 'p' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt"); };
    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x...........x................$Ex",
                    /* line */ (const char *) "x...........x................$$x",
                    /* line */ (const char *) "x.....@.....xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x.........p.xpppppppppppppppp..x",
                    /* line */ (const char *) "x.......p...xpppppppppppppppp..x",
                    /* line */ (const char *) "x.....p.....xpppppppppppppppp..x",
                    /* line */ (const char *) "x...p.......xpppppppppppppppp..x",
                    /* line */ (const char *) "x.p.........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x",
                    /* line */ (const char *) "x...........x..................x",
                    /* line */ (const char *) "x...........x..................x",
                    /* line */ (const char *) "x..........$x..................x",
                    /* line */ (const char *) "x.....xxxxxxx%%%%%%%%%%%%%%%%%%x",
                    /* line */ (const char *) "x.....x....................p..kx",
                    /* line */ (const char *) "x.....x.....$...g.....g...p....x",
                    /* line */ (const char *) "x.....x..................p.....x",
                    /* line */ (const char *) "x.....x.................p......x",
                    /* line */ (const char *) "x.....x.........g.....gp.....$$x",
                    /* line */ (const char *) "xE.k..%.....g.........p......$Ex",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("mantisman"); };
    overrides[ 'p' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.2", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x......x.......x...............x",
                    /* line */ (const char *) "x..@...x.......x...............x",
                    /* line */ (const char *) "x..............x...............x",
                    /* line */ (const char *) "x..............x.......x.......x",
                    /* line */ (const char *) "x..............x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x...............x.......x",
                    /* line */ (const char *) "x......x...............x...E...x",
                    /* line */ (const char *) "x......x...............x.......x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'p' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.3", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..@.E.........................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..@........................E..x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..@........................E..x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE4, "treasure4.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..@........................E..x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE5, "treasure5.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..@........................E..x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }
}
