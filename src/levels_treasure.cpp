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
                    /* line */ (const char *) "xk..........x................$Ex",
                    /* line */ (const char *) "x...........x................$$x",
                    /* line */ (const char *) "x.....@.....xpppppppppppppppp..x",
                    /* line */ (const char *) "xx_________xxpppppppppppppppp..x",
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
                    /* line */ (const char *) "x.....xxxxxxx__________________x",
                    /* line */ (const char *) "x.....x....................p..kx",
                    /* line */ (const char *) "x.....x.....$...g.....g...p....x",
                    /* line */ (const char *) "x.....x..................p.....x",
                    /* line */ (const char *) "x.....x.................p......x",
                    /* line */ (const char *) "x.....x.........g.....gp.....$$x",
                    /* line */ (const char *) "xE.k.._.....g.........p......$Ex",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("mantisman"); };
    overrides[ 'p' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt"); };
    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.2", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.xxxxx.x.......x.......x",
                    /* line */ (const char *) "x......x....$x.x...............x",
                    /* line */ (const char *) "x..@...xxxxxxx.x...............x",
                    /* line */ (const char *) "x..............x...............x",
                    /* line */ (const char *) "x..............x......gxg......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x..m...x.......x.......x.......x",
                    /* line */ (const char *) "x......xg......x......gx.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x....m.xg......xg......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......xg.....gx",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "xm....mx.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x......gx.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x......gx.......x.......x",
                    /* line */ (const char *) "xmmmmmmx...............x.......x",
                    /* line */ (const char *) "xmmmmmmxg.............gx...E...x",
                    /* line */ (const char *) "x$....$x$g...........g$x$.....$x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'p' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt"); };
    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.3", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$..$xppppppx.....x....$x$....Ex",
                    /* line */ (const char *) "x...._ppppppx..g.....g..x......x",
                    /* line */ (const char *) "x...._ppppppx...........x......x",
                    /* line */ (const char *) "x...._ppppppx...........x......x",
                    /* line */ (const char *) "xk...xppppppx.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x......x.....x.....x......x",
                    /* line */ (const char *) "x....x$.....x....$x.....x$.....x",
                    /* line */ (const char *) "x....x............x............x",
                    /* line */ (const char *) "x.@..x...g.....g..x........g...x",
                    /* line */ (const char *) "x....x............x............x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.4", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..@...........................x",
                    /* line */ (const char *) "x..........................g...x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxx...x",
                    /* line */ (const char *) "x..........................x...x",
                    /* line */ (const char *) "x..........................x...x",
                    /* line */ (const char *) "x.......g..................x...x",
                    /* line */ (const char *) "x...xxxxxxxxxxxxxxxxxxxx...x...x",
                    /* line */ (const char *) "x...x............g.....x...x...x",
                    /* line */ (const char *) "x...x..................xg..x..gx",
                    /* line */ (const char *) "x..gx......g...........x...x...x",
                    /* line */ (const char *) "x...x...xxxxxxxxxxxx...x...x...x",
                    /* line */ (const char *) "x...x...x..........x...x...x...x",
                    /* line */ (const char *) "x...x..gx..........xg..x...x...x",
                    /* line */ (const char *) "x...x...x..........x...x...x...x",
                    /* line */ (const char *) "x...x...x...x______x...x...x...x",
                    /* line */ (const char *) "x...x...x...x......x...x...x...x",
                    /* line */ (const char *) "x...x...x...x......x...x...xg..x",
                    /* line */ (const char *) "x...x...x...x$$...Ex..gx...x...x",
                    /* line */ (const char *) "x...xg..x...xxxxxxxx...x..gx...x",
                    /* line */ (const char *) "x...x...xg.............x...x...x",
                    /* line */ (const char *) "x...x...x$g............x...x...x",
                    /* line */ (const char *) "xg..x...xxxxxxxxxxxxxxxx...x...x",
                    /* line */ (const char *) "x...x......................x...x",
                    /* line */ (const char *) "x...xg.....................x...x",
                    /* line */ (const char *) "x...x$g....................x...x",
                    /* line */ (const char *) "x...xxxxxxxxxxxxxxxxxxxxxxxx...x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "xg.............................x",
                    /* line */ (const char *) "xkg............................x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.5", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x.............................$x",
                    /* line */ (const char *) "x..@.....................xg....x",
                    /* line */ (const char *) "x........................xx....x",
                    /* line */ (const char *) "x......x.........x.............x",
                    /* line */ (const char *) "x.....xx.......................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x...................gxk........x",
                    /* line */ (const char *) "x...................xxx........x",
                    /* line */ (const char *) "x.....g.......x......xg........x",
                    /* line */ (const char *) "x..........................x...x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x........x.....................x",
                    /* line */ (const char *) "x$.............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x....x..................xg.....x",
                    /* line */ (const char *) "x.......................xx.....x",
                    /* line */ (const char *) "x...........xx.................x",
                    /* line */ (const char *) "x...........x$.................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..g...................x.......x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x................x.............x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x.............................$x",
                    /* line */ (const char *) "x......x..................xxxxxx",
                    /* line */ (const char *) "x........................._....x",
                    /* line */ (const char *) "x........................._..x.x",
                    /* line */ (const char *) "x$........................_.gxEx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.6", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..@...........................x",
                    /* line */ (const char *) "x..........................g...x",
                    /* line */ (const char *) "x...xxxxxxxxxxxxxxxxxxxxxxxx...x",
                    /* line */ (const char *) "x...xg.....................x...x",
                    /* line */ (const char *) "x...x$g....................x...x",
                    /* line */ (const char *) "x...x$$g...................x...x",
                    /* line */ (const char *) "x...xxxxxxxxxxxxxxxxxxxx...x...x",
                    /* line */ (const char *) "x...x............g.....x...x...x",
                    /* line */ (const char *) "x...x..................xg..x..gx",
                    /* line */ (const char *) "x..gx......g...........x...x...x",
                    /* line */ (const char *) "x...x...xxxxxxxxxxxx...x...x...x",
                    /* line */ (const char *) "x...x...xg........gx...x...x...x",
                    /* line */ (const char *) "x...x..gx..........xg..x...x...x",
                    /* line */ (const char *) "x...x...x..........x...x...x...x",
                    /* line */ (const char *) "x...x...x..........x...x...x...x",
                    /* line */ (const char *) "x...x...x..........x...x...x...x",
                    /* line */ (const char *) "x...x...x..........x...x...x...x",
                    /* line */ (const char *) "x...x...x...g$$...Ex..gx...x...x",
                    /* line */ (const char *) "x...xg..x___xxxxxxxx...x..gsg..x",
                    /* line */ (const char *) "x...x...xg.............x...x...x",
                    /* line */ (const char *) "x...x...x$g............x...x...x",
                    /* line */ (const char *) "xg..x...xxxxxxxxxxxxxxxx...x...x",
                    /* line */ (const char *) "x...x...x$g............x...x...x",
                    /* line */ (const char *) "x...x...x$g............x...x...x",
                    /* line */ (const char *) "x...x...x$g............sg..x...x",
                    /* line */ (const char *) "x...x...xxxxxxxxxxxxxxxxxxxx...x",
                    /* line */ (const char *) "x...x..........................x",
                    /* line */ (const char *) "x...x..........................x",
                    /* line */ (const char *) "xE$$x..........................x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.7", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$....$x........TxT.....x......x",
                    /* line */ (const char *) "x..@...x.....g...x......x...g..x",
                    /* line */ (const char *) "x......x.........x.....TxT.....x",
                    /* line */ (const char *) "x..T...x..T......x......x....T.x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxx......xxxxxxxx",
                    /* line */ (const char *) "x..T...x..Tx.....x..T...x....T.x",
                    /* line */ (const char *) "x......x...x.....xxxxxxxx......x",
                    /* line */ (const char *) "x.....TxT..x..g..x..T...x...g..x",
                    /* line */ (const char *) "x.T....x...x.....x......x......x",
                    /* line */ (const char *) "xxxxxxxxxxxx.....x.....TxT.....x",
                    /* line */ (const char *) "x.T........x.....x..g...x......x",
                    /* line */ (const char *) "x.........TxT....x......x......x",
                    /* line */ (const char *) "x....g.....x.....xxxxxxxx......x",
                    /* line */ (const char *) "x..........x.....x......x...T..x",
                    /* line */ (const char *) "xxxxxxxxxxxx....TxT.....xxxxxxxx",
                    /* line */ (const char *) "x..........x.....x......x...T..x",
                    /* line */ (const char *) "x.....g....xxxxxxx......x......x",
                    /* line */ (const char *) "x...g...g..x..$$$xT....TxT.....x",
                    /* line */ (const char *) "x.........TxT.$$$xxxxxxxx......x",
                    /* line */ (const char *) "x.....T....x..$$$xT.....x...g..x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxx......x......x",
                    /* line */ (const char *) "x.....T..........x.....TxT.....x",
                    /* line */ (const char *) "x...............TxT.....x......x",
                    /* line */ (const char *) "x..g.............x......x......x",
                    /* line */ (const char *) "x........T.......x......x...T..x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x....x.......x........x.....T..x",
                    /* line */ (const char *) "x...TxT......x..g....TxT.......x",
                    /* line */ (const char *) "x....x...g..TxT.......x...g....x",
                    /* line */ (const char *) "xE...x.......x........x........x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.8", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.9", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.10", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.11", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.12", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.13", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.14", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.15", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.16", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.17", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.18", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.19", __FUNCTION__, __LINE__, overrides, 0,
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

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.20", __FUNCTION__, __LINE__, overrides, 0,
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
    Overrides const overrides;

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
    Overrides const overrides;

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
    Overrides const overrides;

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
    Overrides const overrides;

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
