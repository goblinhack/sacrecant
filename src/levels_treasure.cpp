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
                    /* line */ (const char *) "x$......xg.....................x",
                    /* line */ (const char *) "x..@.....x$.............gx.....x",
                    /* line */ (const char *) "x.........x.............xg.....x",
                    /* line */ (const char *) "x..........x...........x.......x",
                    /* line */ (const char *) "x...x.......x.........x........x",
                    /* line */ (const char *) "x....x.......x.......x.........x",
                    /* line */ (const char *) "x.....x.......x.....x....xxxxx.x",
                    /* line */ (const char *) "xx.....x.......x...x.....xg....x",
                    /* line */ (const char *) "x.x.....x.......xgx......x.....x",
                    /* line */ (const char *) "x..x.....xxxxxx..x.......x.....x",
                    /* line */ (const char *) "x.......x................x.....x",
                    /* line */ (const char *) "x.....gx...........xxxxxxx.....x",
                    /* line */ (const char *) "x....gxg..........xx$....xxxx..x",
                    /* line */ (const char *) "x....xgx.........x..x..E.x.....x",
                    /* line */ (const char *) "x...x...x.......x....x...x.....x",
                    /* line */ (const char *) "x..x.....x.....x......x..x.....x",
                    /* line */ (const char *) "x..x......x...x....x.....x..xxxx",
                    /* line */ (const char *) "x..x.......x$x.x....x....x.....x",
                    /* line */ (const char *) "x..x........x...x....x...x.....x",
                    /* line */ (const char *) "x..x.............x....x..x.....x",
                    /* line */ (const char *) "x..x..............x....xxxxxxx.x",
                    /* line */ (const char *) "x..x.....xg........x...........x",
                    /* line */ (const char *) "x..xg....gx.........x....x.....x",
                    /* line */ (const char *) "x..gx......x.........x...x.....x",
                    /* line */ (const char *) "x....x......x.........x$$x.....x",
                    /* line */ (const char *) "x.....x......x.........x$x.....x",
                    /* line */ (const char *) "x......x......x.........xx.....x",
                    /* line */ (const char *) "x.......x......x.........x.....x",
                    /* line */ (const char *) "x........x......xg............$x",
                    /* line */ (const char *) "x$..............gx...........$gx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.9", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x.............x......x........Ex",
                    /* line */ (const char *) "x..@..........x.......x....x...x",
                    /* line */ (const char *) "x.............x...x....x....x..x",
                    /* line */ (const char *) "x....xxxxxxx.xx....x....x....x.x",
                    /* line */ (const char *) "x....x.........x....x....x...x.x",
                    /* line */ (const char *) "x....x..........x....x.......xgx",
                    /* line */ (const char *) "x...xx...........x....x...xxxxsx",
                    /* line */ (const char *) "x...gx............x....x..x$$$$x",
                    /* line */ (const char *) "x...xx.............xxx.xxxxxxxxx",
                    /* line */ (const char *) "x....x....................x....x",
                    /* line */ (const char *) "x....x....................x....x",
                    /* line */ (const char *) "x....x.........................x",
                    /* line */ (const char *) "x....x.......xx.xxx.......x....x",
                    /* line */ (const char *) "x....x.......xg..gx.......x....x",
                    /* line */ (const char *) "x....x............x.......x....x",
                    /* line */ (const char *) "x............x............x....x",
                    /* line */ (const char *) "x....x.......xg..gx.......x....x",
                    /* line */ (const char *) "x....x.......xxxxxx.......x....x",
                    /* line */ (const char *) "x....x....................x....x",
                    /* line */ (const char *) "x....x....................x....x",
                    /* line */ (const char *) "xx...x....................x....x",
                    /* line */ (const char *) "xg...xxx..................x....x",
                    /* line */ (const char *) "xx...x....................x....x",
                    /* line */ (const char *) "x....x....................x....x",
                    /* line */ (const char *) "x....x....................x....x",
                    /* line */ (const char *) "x....xg..................gx....x",
                    /* line */ (const char *) "x....gxxxxxxxxxx..xxxxxxxxg....x",
                    /* line */ (const char *) "x.x........x.........x......x..x",
                    /* line */ (const char *) "x...x......x.........x....x....x",
                    /* line */ (const char *) "x.........$x$.......$x$........x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.10", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..........................g.xEx",
                    /* line */ (const char *) "x..........................g.x.x",
                    /* line */ (const char *) "x.xxxxxxxxxxxxxxxxxxxxxxxxxx.x.x",
                    /* line */ (const char *) "x.x.g........................x.x",
                    /* line */ (const char *) "x.x.xxxxxxxxxxxxxxxxxxxxxxxxxx.x",
                    /* line */ (const char *) "x.x............................x",
                    /* line */ (const char *) "x.xxxxxxxxxxxxxxxxxxxxxxxxxxxx.x",
                    /* line */ (const char *) "x.x.........................$x.x",
                    /* line */ (const char *) "x.g..............xxxxxxxxxxxxx.x",
                    /* line */ (const char *) "x.x..............x.....x.....x.x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxx.x.....x.....xgx",
                    /* line */ (const char *) "x..............x.x..x..x.....x$x",
                    /* line */ (const char *) "x.x............x.x..x........xxx",
                    /* line */ (const char *) "x.x............x.x$gx..........x",
                    /* line */ (const char *) "x.x............xgxxxxgxxxxx.x..x",
                    /* line */ (const char *) "x.x.......................x.x..x",
                    /* line */ (const char *) "x.xxx.xxx..x.........x....x.x..x",
                    /* line */ (const char *) "xgx.....x...g........x....x.x..x",
                    /* line */ (const char *) "x.x.....x....x.......x$..gx.x..x",
                    /* line */ (const char *) "x.x.....x.....x.....xxxxxxx.x..x",
                    /* line */ (const char *) "xgxxx...x......x.....x......x..x",
                    /* line */ (const char *) "x...x...x.......g....x......x..x",
                    /* line */ (const char *) "x...x...x........x...x......x..x",
                    /* line */ (const char *) "x...x...x.........x.........x..x",
                    /* line */ (const char *) "x...x...x..........x........x..x",
                    /* line */ (const char *) "x...x...x...........g.......x..x",
                    /* line */ (const char *) "x...x...x............x.........x",
                    /* line */ (const char *) "x...x...x$..................x..x",
                    /* line */ (const char *) "x.@.x....xxxxxxxxxxxxxxxxxxxx..x",
                    /* line */ (const char *) "x...x..........................x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure1.11", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x.............................$x",
                    /* line */ (const char *) "x..@...........................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x.......................x......x",
                    /* line */ (const char *) "x......................xxx.....x",
                    /* line */ (const char *) "x.....x.................x......x",
                    /* line */ (const char *) "x....xxx.......................x",
                    /* line */ (const char *) "x.....x........................x",
                    /* line */ (const char *) "x.........g.........g..........x",
                    /* line */ (const char *) "x..........x.......x...........x",
                    /* line */ (const char *) "x...........x.....x............x",
                    /* line */ (const char *) "x............x...x.............x",
                    /* line */ (const char *) "x.............x.x..............x",
                    /* line */ (const char *) "x..............g...............x",
                    /* line */ (const char *) "x.............x.x..............x",
                    /* line */ (const char *) "x............x...x.............x",
                    /* line */ (const char *) "x...........x.....x............x",
                    /* line */ (const char *) "x..........x.......x...........x",
                    /* line */ (const char *) "x.........g.........g..........x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x.....x........................x",
                    /* line */ (const char *) "x....xxx.......................x",
                    /* line */ (const char *) "x.....x................x.......x",
                    /* line */ (const char *) "x.....................xxx......x",
                    /* line */ (const char *) "x......................x.......x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x$...........................$Ex",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.12", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xx$.............xx...........Exx",
                    /* line */ (const char *) "x$xx...........xxxx..........x$x",
                    /* line */ (const char *) "x..xx.........xx..xx........xx.x",
                    /* line */ (const char *) "x...xx.......xx............xx..x",
                    /* line */ (const char *) "x....xx.....xx......xx....xx...x",
                    /* line */ (const char *) "x.....xx...xx........xx..xx....x",
                    /* line */ (const char *) "x......xx.xx....g.....xxxx.....x",
                    /* line */ (const char *) "x.......xxx............xx......x",
                    /* line */ (const char *) "x.......xx............xxx......x",
                    /* line */ (const char *) "x......xxxx.............xx.....x",
                    /* line */ (const char *) "x.....xx..xx........xx...xx....x",
                    /* line */ (const char *) "x....xx....xx......xx..........x",
                    /* line */ (const char *) "x...........xx....xx.......xx..x",
                    /* line */ (const char *) "x..xx....g...xx..xx.........xx.x",
                    /* line */ (const char *) "x.xx....gxg...xxxx.....g.....xxx",
                    /* line */ (const char *) "xxx......g.....xx...........xx.x",
                    /* line */ (const char *) "x.xx..........xxxx.........xx..x",
                    /* line */ (const char *) "x..xx............xx.......xx...x",
                    /* line */ (const char *) "x...........xx....xx...........x",
                    /* line */ (const char *) "x....xx....xx...........xx.....x",
                    /* line */ (const char *) "x.....xx..xx........xx.xx......x",
                    /* line */ (const char *) "x......xxxx....gx....xxx.......x",
                    /* line */ (const char *) "x.......xx.....xg....xxx.......x",
                    /* line */ (const char *) "x......xxxx.........xx.xx......x",
                    /* line */ (const char *) "x.....xx..xx.......xx...xx.....x",
                    /* line */ (const char *) "x....g.....xx.....xx.....xx....x",
                    /* line */ (const char *) "x...xx......xx....x.......xx...x",
                    /* line */ (const char *) "x..xx........xx............xx..x",
                    /* line */ (const char *) "x.xx..........xxx...........xx$x",
                    /* line */ (const char *) "xxx...........@x.............$xx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.13", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xxxg........$CCCECCC$.......gxxx",
                    /* line */ (const char *) "xxxxCCCCCCCCCCCC.CCCCCCCCCCCxxxx",
                    /* line */ (const char *) "xgxxCCCCCC.CCCCC.CCCCC.CCCCxxxgx",
                    /* line */ (const char *) "x..xxx........CC.CC.......xxx..x",
                    /* line */ (const char *) "x...xxx........C.C.......xxx...x",
                    /* line */ (const char *) "x....xxx.......C.C......xxx....x",
                    /* line */ (const char *) "x.$...xxx..............xxx.....x",
                    /* line */ (const char *) "x......xxx.....bTb....xxx......x",
                    /* line */ (const char *) "x......xxxxxxxxxxxxxxxxxx....$.x",
                    /* line */ (const char *) "x......x.xxx..gbTbg.xxx.x......x",
                    /* line */ (const char *) "x......x..xx........xx..x......x",
                    /* line */ (const char *) "x......x....x......x....x......x",
                    /* line */ (const char *) "x......xg....b....b....gx......x",
                    /* line */ (const char *) "x.....bxb..............bxb.....x",
                    /* line */ (const char *) "x.....TxT......bb......TxT.....x",
                    /* line */ (const char *) "x.....bxb......bb......bxb.....x",
                    /* line */ (const char *) "x......xg..............gx......x",
                    /* line */ (const char *) "x......x.....b....b.....x......x",
                    /* line */ (const char *) "x......x....x......x....x......x",
                    /* line */ (const char *) "x......x..xx........xx..x......x",
                    /* line */ (const char *) "x......x$xxx.gbTbg..xxx.x......x",
                    /* line */ (const char *) "x......xxxxxxxxxxxxxxxxxx......x",
                    /* line */ (const char *) "x......xxx....bTb.....xxx......x",
                    /* line */ (const char *) "x.....xxx..............xxx.....x",
                    /* line */ (const char *) "x....xxx................xxx....x",
                    /* line */ (const char *) "x...xxx..................xxx...x",
                    /* line */ (const char *) "x..xxxxxxx................xxx..x",
                    /* line */ (const char *) "xgxxx....x.................xxxgx",
                    /* line */ (const char *) "xxxx..@..x.................gxxxx",
                    /* line */ (const char *) "xxx........................g$xxx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.14", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$.s.....g............x$.....x$x",
                    /* line */ (const char *) "xT.s.@...x............x......xsx",
                    /* line */ (const char *) "x$.s.....x............g........x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.x",
                    /* line */ (const char *) "x........x............g........x",
                    /* line */ (const char *) "x........g............x........x",
                    /* line */ (const char *) "x.xxxxxxxxxxxxxxsxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x........g............x.......$x",
                    /* line */ (const char *) "x........x............g........x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.x",
                    /* line */ (const char *) "x........x............g........x",
                    /* line */ (const char *) "x........g............x........x",
                    /* line */ (const char *) "x.xxxxxxxxxxxxxxsxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x........x............x........x",
                    /* line */ (const char *) "x$.......g............g........x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.x",
                    /* line */ (const char *) "x........x............g........x",
                    /* line */ (const char *) "x........g............x........x",
                    /* line */ (const char *) "x.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x........x............x........x",
                    /* line */ (const char *) "x........g............g........x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxsxxxxxxxxxxxxx.x",
                    /* line */ (const char *) "x........g............x........x",
                    /* line */ (const char *) "x........x............g........x",
                    /* line */ (const char *) "x.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x........x............x.......$x",
                    /* line */ (const char *) "xT.......g............g........x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxsxxxxxxxxxxxx..x",
                    /* line */ (const char *) "x........g............g........x",
                    /* line */ (const char *) "xE.......x............x........x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.15", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x....xT.......................xx",
                    /* line */ (const char *) "x..@.x.......x...............x$x",
                    /* line */ (const char *) "x....x......xkx.............x..x",
                    /* line */ (const char *) "x....x.......x.............x...x",
                    /* line */ (const char *) "x....x....................x....x",
                    /* line */ (const char *) "x....x...................x.....x",
                    /* line */ (const char *) "xx..xxxxx...............x......x",
                    /* line */ (const char *) "x......................x.......x",
                    /* line */ (const char *) "x.x.x.x.x.x.x.x.......x........x",
                    /* line */ (const char *) "x..........................x...x",
                    /* line */ (const char *) "x..x.x.x.x.x.x.x.x........xgx..x",
                    /* line */ (const char *) "x..................x.......x...x",
                    /* line */ (const char *) "x.........b......x.............x",
                    /* line */ (const char *) "x..................x...........x",
                    /* line */ (const char *) "x................x.............x",
                    /* line */ (const char *) "x....x.............x...........x",
                    /* line */ (const char *) "x...xgx..........x.......x.....x",
                    /* line */ (const char *) "x....x.............x....xgx....x",
                    /* line */ (const char *) "x................x.......x.....x",
                    /* line */ (const char *) "x..................x...........x",
                    /* line */ (const char *) "x................x.............x",
                    /* line */ (const char *) "x..b.....x.....................x",
                    /* line */ (const char *) "x.......xgx....................x",
                    /* line */ (const char *) "x........x.......b.............x",
                    /* line */ (const char *) "x.......................xxxxxxxx",
                    /* line */ (const char *) "x........b.............._b....gx",
                    /* line */ (const char *) "x..............x........_b....gx",
                    /* line */ (const char *) "x.............xgx......._......x",
                    /* line */ (const char *) "xxxxx..........x........_b....gx",
                    /* line */ (const char *) "xT.._..................._b....Ex",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.16", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..x$..x$..x$..x$..x$..x$..x$..x",
                    /* line */ (const char *) "x@.xxggxxggxxggxxggxxggxxggxxTEx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxTxx",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..x$..x$..x$..x$..x$..x$..x$..x",
                    /* line */ (const char *) "xE.xxggxxggxxggxxggxxggxxggxxggx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.17", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x....x............x.........x.kx",
                    /* line */ (const char *) "x..@.x.x........g.x....x....x..x",
                    /* line */ (const char *) "x....x...x.....x..x.........xxsx",
                    /* line */ (const char *) "x.xxxx.g........xxxxx.....x....x",
                    /* line */ (const char *) "x....x...x.x...................x",
                    /* line */ (const char *) "x...................xxxx....x..x",
                    /* line */ (const char *) "xxxxxx..xCCx...x.......x.......x",
                    /* line */ (const char *) "x..........C...........x.......x",
                    /* line */ (const char *) "x.x..x..x..x..x....x..........xx",
                    /* line */ (const char *) "x....x...................x.....x",
                    /* line */ (const char *) "x....x.....x.....g.......x..xxxx",
                    /* line */ (const char *) "x....x...x..........x....x.....x",
                    /* line */ (const char *) "x..x...........................x",
                    /* line */ (const char *) "x....x....xCCx...x...........x.x",
                    /* line */ (const char *) "xxxxxx...................x.....x",
                    /* line */ (const char *) "x....x.....x.x.......x...C....xx",
                    /* line */ (const char *) "xg................x..x...C.....x",
                    /* line */ (const char *) "x..x..x....x..g......x...x...x.x",
                    /* line */ (const char *) "x..C...........x.....x.........x",
                    /* line */ (const char *) "x..C.....xxx......x..xxxx...g..x",
                    /* line */ (const char *) "x..x.......x......g............x",
                    /* line */ (const char *) "x.......x..xxx.......x...x...x.x",
                    /* line */ (const char *) "x....x.........................x",
                    /* line */ (const char *) "x............x.................x",
                    /* line */ (const char *) "xxxxxx..g..x.....xCCCx......x..x",
                    /* line */ (const char *) "x........................x..xxxx",
                    /* line */ (const char *) "x..x.x.x...x..xxxxx..x.........x",
                    /* line */ (const char *) "x___...x........x.......x...x..x",
                    /* line */ (const char *) "x$$_.x.x.....x..x...x..........x",
                    /* line */ (const char *) "x$$_...x..x.....x.........x..E.x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.18", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x......x$Cg....................x",
                    /* line */ (const char *) "x..@...x$Cg....................x",
                    /* line */ (const char *) "x......x$Cg....................x",
                    /* line */ (const char *) "x....k.x$Cg....................x",
                    /* line */ (const char *) "x....xxxxxxxxxxxxxxxxxxxxxxxxxsx",
                    /* line */ (const char *) "x....xg..................gxg...x",
                    /* line */ (const char *) "x....x....................x....x",
                    /* line */ (const char *) "x....x....................x....x",
                    /* line */ (const char *) "x....x...xxxxxxxxxxxxxx...x....x",
                    /* line */ (const char *) "x....x...xg...........x...x....x",
                    /* line */ (const char *) "x...gx...x............x...x....x",
                    /* line */ (const char *) "x....x...x............x...x....x",
                    /* line */ (const char *) "x....x...x...xxxxxx...x...x....x",
                    /* line */ (const char *) "x....x...x.....CCCx...xCCCx....x",
                    /* line */ (const char *) "x....x...x...x.C.Ex...s...s....x",
                    /* line */ (const char *) "x....x...x...x.C..x...xCCCx....x",
                    /* line */ (const char *) "x....x...x...xgCCCx...x...x....x",
                    /* line */ (const char *) "x....x...x...xxxxxx...x...x....x",
                    /* line */ (const char *) "x....x...x............x...x....x",
                    /* line */ (const char *) "xg...x...x............x...x....x",
                    /* line */ (const char *) "x....x...x............C...x....x",
                    /* line */ (const char *) "x....x...xxxxxxxxxxxxxx...x....x",
                    /* line */ (const char *) "x....x....................x....x",
                    /* line */ (const char *) "x....x....................x....x",
                    /* line */ (const char *) "x....C...................gxg...x",
                    /* line */ (const char *) "x____xxxxxxxxxxxxxxxxxxxxxxsxxxx",
                    /* line */ (const char *) "x....x..s..............._......x",
                    /* line */ (const char *) "x....xk.x...............xxx....x",
                    /* line */ (const char *) "x....xxxx.................x....x",
                    /* line */ (const char *) "x$........................xg...x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.19", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x.............................$x",
                    /* line */ (const char *) "x..@.x.x.x.x.x.x.g.x.x.x.x.x.x.x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x.x.x.x.x.x.x.x.x.x.x.x.x.x.x..x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..x.x.x.x.g.x.x.x.x.x.x.x.x.x.x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "xxx.x.x.x.x.CCCCCCC.x.x.x.x.x..x",
                    /* line */ (const char *) "x...........CCCCCCC............x",
                    /* line */ (const char *) "x...........CCCCCCC............x",
                    /* line */ (const char *) "x...........CCCCCCC............x",
                    /* line */ (const char *) "x..x.x.x.x..CC$.CCC..x.g.x.x.xxx",
                    /* line */ (const char *) "x...........CCg................x",
                    /* line */ (const char *) "x.x.x.x.x.x.CC$.CCC.x.x.x.x.x..x",
                    /* line */ (const char *) "x...........CCCCCCC............x",
                    /* line */ (const char *) "x...........CCCCCCC............x",
                    /* line */ (const char *) "x...........CCCCCCC............x",
                    /* line */ (const char *) "x..x.x.x.x..CCCCCCC..x.g.x.x.x.x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "xxx.x.x.x.x.x.x.x.x.x.x.x.x.x..x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..x.x.x.x.g.x.x.x.x.x.x.x.x.x.x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x.x.g.x.x.x.x.x.x.x.x.x.x.x.x..x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x..x.x.x.x.x.x.x.x.x.x.g.x.x.x.x",
                    /* line */ (const char *) "x$............................Ex",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'g' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.20", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$g.........x......v.........gkx",
                    /* line */ (const char *) "xg........v....@.....v........gx",
                    /* line */ (const char *) "x...........v......v...........x",
                    /* line */ (const char *) "x.........v..........v.........x",
                    /* line */ (const char *) "x...........v......v...........x",
                    /* line */ (const char *) "x.........v..........v.........x",
                    /* line */ (const char *) "x...........v......v...........x",
                    /* line */ (const char *) "x.........v..........v.........x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x.x.x.x.x.x.x.x.x.x.x.x.x.x.x.xx",
                    /* line */ (const char *) "x______________________________x",
                    /* line */ (const char *) "x..x.x.x.x.x.x.x.x.x.x.x.x.x.x.x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x............................xxx",
                    /* line */ (const char *) "x....v.........................x",
                    /* line */ (const char *) "x...vgv........................x",
                    /* line */ (const char *) "x....v.........................x",
                    /* line */ (const char *) "x......................v.......x",
                    /* line */ (const char *) "xxx...................vgv......x",
                    /* line */ (const char *) "x......................v.......x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x........v.....................x",
                    /* line */ (const char *) "x.......vgv..................xxx",
                    /* line */ (const char *) "x........v.....................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCx",
                    /* line */ (const char *) "x............xx.xx.............x",
                    /* line */ (const char *) "x...........x.....x............x",
                    /* line */ (const char *) "xCC........x...E...x.........CCx",
                    /* line */ (const char *) "x$C........x.......x.........C$x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'e' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_random(is_amphibious); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "R$,WWWWWWRWWWWWWWWWWWWWWRWWWW,$R",
                    /* line */ (const char *) "R,,WWWWWWWWWWWWWWWWWWWWWWWWWW,,R",
                    /* line */ (const char *) "RWWWWWeWWWWWWWWeWWWWWWWRWWWWWWWR",
                    /* line */ (const char *) "RRWWWWWWWWWWWRWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWRWWWWWWWWWWWWRWWWWWWWWWWWWRR",
                    /* line */ (const char *) "RWWWWWWWWWWRWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RRWWWWWWWWWWWWWWWWWWWeWWWWWWWWRR",
                    /* line */ (const char *) "RWWWWWWWWeWWWWWWWWWRWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWRWWWWWWWWWWWWWWWWWWWWWWWW.R",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWRWWWWWWWWWWW..R",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWWWWWWWWWWWW...R",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWWWWWWWWWWW....R",
                    /* line */ (const char *) "RWW@WWWWWWWWWWRWWWWWWeWWWW...E.R",
                    /* line */ (const char *) "RWWWWWWWWWWWRWWWWWWWWWWWWWW....R",
                    /* line */ (const char *) "RWWWWWWWWWWWWWeWWWWWWWWWWWWW...R",
                    /* line */ (const char *) "RWWWRWWWWWWWWWWWWWWWWWWWWWWWW..R",
                    /* line */ (const char *) "RWWWWWWWeWWWWWWWWWWWWWWRWWWWWW.R",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWWRWWWWWWWRWWWWR",
                    /* line */ (const char *) "RRWWWWWWWWWWWWWWWRWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWRWWWWWWWWWWWWWWWWWRR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWRWWWWWWeWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWeWWWWWWWWWWWWWWWWRWWWWR",
                    /* line */ (const char *) "RWWWRWWWWWWWWWWWWWWWRWWWWWWWWWRR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWeWWWWWRWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWeWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWRWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "R,,WWWRWWWWWWWWWWWWWWWWWWWRWW,,R",
                    /* line */ (const char *) "R,,WWWWWWWWWWWRWWWWWWWWRWWWWW,$R",
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'e' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_random(is_amphibious); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x.........x...........x.......Tx",
                    /* line */ (const char *) "x.g....................x...g...x",
                    /* line */ (const char *) "x.........x.............x......x",
                    /* line */ (const char *) "x.........x..............x.....x",
                    /* line */ (const char *) "x.......xxxxx...vvvvvv....xx.xxx",
                    /* line */ (const char *) "x...........x...v$..$v....x....x",
                    /* line */ (const char *) "x.......x...x...v.T..v.........x",
                    /* line */ (const char *) "x.......x...x...vg..$v....x....x",
                    /* line */ (const char *) "x.......xx.xx...vvvvvv....xx.xxx",
                    /* line */ (const char *) "x......x.....x...........x.....x",
                    /* line */ (const char *) "x.....x.......x.........x......x",
                    /* line */ (const char *) "x....x.........x.......x.......x",
                    /* line */ (const char *) "x...x...........xWWeWWx........x",
                    /* line */ (const char *) "xxxx.............xxWxx.........x",
                    /* line */ (const char *) "x..x.............xWWWx.........x",
                    /* line */ (const char *) "x@...............xWWWx.........x",
                    /* line */ (const char *) "x..x.............xWWWx.........x",
                    /* line */ (const char *) "xxxx.............xxCxx.....g...x",
                    /* line */ (const char *) "x$.$x...........x.....x........x",
                    /* line */ (const char *) "x.T..x.........x...E...x.......x",
                    /* line */ (const char *) "x.....x.......x.........x......x",
                    /* line */ (const char *) "xCCCCCCx.....x...........x.....x",
                    /* line */ (const char *) "xg....g.xx.xxxxxxxxxxxxxxxxx.xxx",
                    /* line */ (const char *) "x.......x...x.............x....x",
                    /* line */ (const char *) "x.......x......................x",
                    /* line */ (const char *) "x.......x...x.............x....x",
                    /* line */ (const char *) "x.......xxxxx.............xxxxxx",
                    /* line */ (const char *) "x.........x....................x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "x.........x....................x",
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
