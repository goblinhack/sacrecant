//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_ext.hpp"
#include "my_thing.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

//
// Create all treasure levels
//
void levels_treasure(Gamep g)
{
  TRACE();

  {
    LevelGenOverrides overrides;

    overrides.o[ 'p' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt"); };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    ",
                    /* line */ (const char *) "xk..........xz...............$Ex    ",
                    /* line */ (const char *) "x...........x................$$x    ",
                    /* line */ (const char *) "x.....@.....xpppppppppppppppp..x    ",
                    /* line */ (const char *) "xx_________xxpppppppppppppppp..x    ",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x    ",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x    ",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x    ",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x    ",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x    ",
                    /* line */ (const char *) "x...........xpppppppppppppppp..xxxxx",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x..Bx",
                    /* line */ (const char *) "x...........xpppppppppppppppp..s..Zx",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x..Bx",
                    /* line */ (const char *) "x.........p.xpppppppppppppppp..xxxxx",
                    /* line */ (const char *) "x.......p...xpppppppppppppppp..x    ",
                    /* line */ (const char *) "x.....p.....xpppppppppppppppp..x    ",
                    /* line */ (const char *) "x...p.......xpppppppppppppppp..x    ",
                    /* line */ (const char *) "x.p.........xpppppppppppppppp..x    ",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x    ",
                    /* line */ (const char *) "x...........xpppppppppppppppp..x    ",
                    /* line */ (const char *) "x...........x..................x    ",
                    /* line */ (const char *) "x...........x..................x    ",
                    /* line */ (const char *) "x..........$x..................x    ",
                    /* line */ (const char *) "x.....xxxxxxx__________________x    ",
                    /* line */ (const char *) "x.....x....................p..kx    ",
                    /* line */ (const char *) "x.....x.....$...g.....g...p....x    ",
                    /* line */ (const char *) "x.....x..................p.....x    ",
                    /* line */ (const char *) "x.....x.................p......x    ",
                    /* line */ (const char *) "x.....x.........g.....gp.....$$x    ",
                    /* line */ (const char *) "xE.k.._.....g.........p......$Ex    ",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    ",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'm' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("mantisman"); };
    overrides.o[ 'p' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt"); };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.2", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x......x.......x.......x.......x",
                    /* line */ (const char *) "x......x.xxxxx.x.......x.......x",
                    /* line */ (const char *) "x......x....zx.x...............x",
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
    LevelGenOverrides overrides;

    overrides.o[ 'p' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt"); };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x....x......x.....x...CCCCCC...x",
                    /* line */ (const char *) "x....x......x.....x...CCCCCC...x",
                    /* line */ (const char *) "x....x......x.....x...CC..CC...x",
                    /* line */ (const char *) "x....x......x.....x...CC.zCC...x",
                    /* line */ (const char *) "x....x......x.....x...CC..CC...x",
                    /* line */ (const char *) "x....x......x.....x...CCxCCC...x",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x...x...x...xz$...Ex..gx...x...x",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x......x.......x..........xxxxxx",
                    /* line */ (const char *) "x.............xzx........._....x",
                    /* line */ (const char *) "x..............x.........._..x.x",
                    /* line */ (const char *) "x$........................_.gxEx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x...x..gx...CCC....xg..x...x...x",
                    /* line */ (const char *) "x...x...x...CzC....x...x...x...x",
                    /* line */ (const char *) "x...x...x...CCC....x...x...x...x",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x..........x.....x...z..x...T..x",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.8", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$......xzg.CCCCCCCC...........x",
                    /* line */ (const char *) "x..@....xx$.CCCCCCCC....gx.....x",
                    /* line */ (const char *) "x........xxsxxxxxxxxsxxxxg.....x",
                    /* line */ (const char *) "x.........xx.......x...x.......x",
                    /* line */ (const char *) "x...x......xx......x..x........x",
                    /* line */ (const char *) "x....x......xx.....s.x.........x",
                    /* line */ (const char *) "x.....x......xx....xx....xxxxx.x",
                    /* line */ (const char *) "xx.....x......xx...x.....xg....x",
                    /* line */ (const char *) "x.x.....x......xxgx......x.....x",
                    /* line */ (const char *) "x..x.....xxxxx..xx.......x.....x",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.9", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x.............x......x........Ex",
                    /* line */ (const char *) "x..@..........x.......x....x...x",
                    /* line */ (const char *) "x.............x...x....x....x..x",
                    /* line */ (const char *) "x....xxxxxxx.xx....x....x....x.x",
                    /* line */ (const char *) "x....xz.s......x....x....x...x.x",
                    /* line */ (const char *) "x....x..x.......x....x.......xgx",
                    /* line */ (const char *) "x...xxxxx........x....x...xxxxsx",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x.g..xxxxxxxxxxx.xxxxxxxxxxxxx.x",
                    /* line */ (const char *) "x.x..s...z.....x.x.....x.....x.x",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x.............xgx..............x",
                    /* line */ (const char *) "x.............gzg..............x",
                    /* line */ (const char *) "x.............xgx..............x",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.12", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xxz.............xx...........Exx",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.13", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xz.g........$CCCECCC$.......g.zx",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x........g.....z......x........x",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.15", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x....xT.......................xx",
                    /* line */ (const char *) "x..@.x.......x...............x$x",
                    /* line */ (const char *) "x....x......xkx.............x..x",
                    /* line */ (const char *) "x....x.......x.............x...x",
                    /* line */ (const char *) "x....x....................x..xsx",
                    /* line */ (const char *) "x....x...................x...x.x",
                    /* line */ (const char *) "xx..xxxxx...............x....x.x",
                    /* line */ (const char *) "x......................x.....x.x",
                    /* line */ (const char *) "x.x.x.x.x.x.x.x.......x......x.x",
                    /* line */ (const char *) "x..........................x.x.x",
                    /* line */ (const char *) "x..x.x.x.x.x.x.x.x........xgxx.x",
                    /* line */ (const char *) "x..................x.......x.x.x",
                    /* line */ (const char *) "x.........b......x...........x.x",
                    /* line */ (const char *) "x..................x.........x.x",
                    /* line */ (const char *) "x................x...........x.x",
                    /* line */ (const char *) "x....x.............x.........x.x",
                    /* line */ (const char *) "x...xgx..........x.......x...x.x",
                    /* line */ (const char *) "x....x.............x....xgx..x.x",
                    /* line */ (const char *) "x................x.......x...x.x",
                    /* line */ (const char *) "x..................x.........x.x",
                    /* line */ (const char *) "x................x...........x.x",
                    /* line */ (const char *) "x..b.....x..............xxxxxxsx",
                    /* line */ (const char *) "x.......xgx.............xg.CCC.x",
                    /* line */ (const char *) "x........x.......b......xz.CCC.x",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x..xx..xx..xx..xz..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..xx..xx..xx..xx..xx..xx..xx..x",
                    /* line */ (const char *) "x..x$..x$..x$..x$..x$..x$..x$..x",
                    /* line */ (const char *) "xE.xxggxxggxxggxxggxxggxxggxxggx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x....x...................x....zx",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x....x...x...x.C.Ex...s.z.s....x",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x.x.x.x.x.x.CC$.CCC.x.x.x.x.x.zx",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

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
                    /* line */ (const char *) "x........v...................xxx",
                    /* line */ (const char *) "x.......vgv..................xzx",
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
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.21", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "                            ",
                    /* line */ (const char *) " xxxxx  xxxxxxxxxxxx  xxxxx ",
                    /* line */ (const char *) " x@.$x  x$.........x  xE.$x ",
                    /* line */ (const char *) " xxx.x  xxx.xxxxxx.x  xxx.x ",
                    /* line */ (const char *) "   x.x    x.x    x.x    x.x ",
                    /* line */ (const char *) " xxx.x  xxx.x  xxx.x  xxx.x ",
                    /* line */ (const char *) " x..gx  x..gx  x..gx  x..gx ",
                    /* line */ (const char *) " x.xxx  x.xxx  x.xxx  x.xxx ",
                    /* line */ (const char *) " x.x    x.x    x.x    x.x   ",
                    /* line */ (const char *) " x.xxx  x.xxx  x.xxx  x.xxx ",
                    /* line */ (const char *) " x...x  x...x  x...x  x...x ",
                    /* line */ (const char *) " xxx.x  xxx.x  xxx.xxxxxx.x ",
                    /* line */ (const char *) "   x.x    x.x    x.s.zz.s.x ",
                    /* line */ (const char *) " xxx.x  xxx.x  xxx.xxxxxx.x ",
                    /* line */ (const char *) " x..gx  x..gx  x..gx  x..gx ",
                    /* line */ (const char *) " x.xxx  x.xxx  x.xxx  x.xxx ",
                    /* line */ (const char *) " x.x    x.x    x.x    x.x   ",
                    /* line */ (const char *) " x.xxx  x.xxx  x.xxx  x.xxx ",
                    /* line */ (const char *) " x...x  x...x  x...x  x...x ",
                    /* line */ (const char *) " xxx.x  xxx.x  xxx.x  xxx.x ",
                    /* line */ (const char *) "   x.x    x.x    x.x    x.x ",
                    /* line */ (const char *) " xxx.x  xxx.x  xxx.x  xxx.x ",
                    /* line */ (const char *) " x..gx  x..gx  x..gx  x..gx ",
                    /* line */ (const char *) " x.xxx  x.xxx  x.xxx  x.xxx ",
                    /* line */ (const char *) " x.x    x.x xxxx.x    x.x   ",
                    /* line */ (const char *) " x.xxxxxx.xxx$$x.xxxxxx.xxx ",
                    /* line */ (const char *) " x........gss$$x..........x ",
                    /* line */ (const char *) " xxxxxxxxxxxxxxxxxxxxxxxxxx ",
                    /* line */ (const char *) "                            ",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE1, "treasure1.22", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "                            ",
                    /* line */ (const char *) " xxxxx  xxxxxxxxxxxx  xxxxx ",
                    /* line */ (const char *) " x@.$x  x$.........x  xE.$x ",
                    /* line */ (const char *) " xxx.x  xxx.xxxxxx.x  xxx.x ",
                    /* line */ (const char *) "   x.x    x.x    x.x    x.x ",
                    /* line */ (const char *) " xxx.x  xxx.x  xxx.x  xxx.x ",
                    /* line */ (const char *) " x..gx  x..gx  x..gx  x..gx ",
                    /* line */ (const char *) " x.xxx  x.xxx  x.xxx  x.xxx ",
                    /* line */ (const char *) " x.x    x.x    x.x    x.x   ",
                    /* line */ (const char *) " x.xxx  x.xxx  x.xxx  x.xxx ",
                    /* line */ (const char *) " x...x  x...x  x...x  x...x ",
                    /* line */ (const char *) " xxx.x  xxx.x  xxx.x  xxx.x ",
                    /* line */ (const char *) "   x.x    x.x    x.x    x.x ",
                    /* line */ (const char *) " xxx.x  xxx.x  xxx.x  xxx.x ",
                    /* line */ (const char *) " x..gx  x..gx  x..gx  x..gx ",
                    /* line */ (const char *) " x.xxx  x.xxx  x.xxx  x.xxx ",
                    /* line */ (const char *) " x.x    x.x    x.x    x.x   ",
                    /* line */ (const char *) " x.xxx  x.xxx  x.xxx  x.xxx ",
                    /* line */ (const char *) " x...x  x...x  x...x  x...x ",
                    /* line */ (const char *) " xxx.x  xxx.x  xxx.x  xxx.x ",
                    /* line */ (const char *) "   x.x    x.x    x.x    x.x ",
                    /* line */ (const char *) " xxx.x  xxx.x  xxx.x  xxx.x ",
                    /* line */ (const char *) " x..gx  x..gx  x..gx  x..gx ",
                    /* line */ (const char *) " x.xxx  x.xxx  x.xxx  x.xxx ",
                    /* line */ (const char *) " x.x    x.x xxxx.x    x.x   ",
                    /* line */ (const char *) " x.xxxxxx.xxx$$x.xxxxxx.xxx ",
                    /* line */ (const char *) " x........gss$$x........szx ",
                    /* line */ (const char *) " xxxxxxxxxxxxxxxxxxxxxxxxxx ",
                    /* line */ (const char *) "                            ",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "R$,WWWWWWRWWRWWWWWWRWWWWRWWWW,zR",
                    /* line */ (const char *) "R,,WWWWWWWWWWWWWWWWRWWWWWWWWW,,R",
                    /* line */ (const char *) "RWWWWWeRWWWWRWWeWWRWWWWRWWWWWWWR",
                    /* line */ (const char *) "RRWWWWWWRWWWWRWWWWWWeWWRWWWWWWWR",
                    /* line */ (const char *) "RWWWRWWWWWWRWWWWWRWWWWWWRWWeWWRR",
                    /* line */ (const char *) "RWWeWWWWWWWRWWWWWWWWWWWWWRWWWWWR",
                    /* line */ (const char *) "RRWWWWWWWWRWWWWWWWWWWeWWWWWWWWRR",
                    /* line */ (const char *) "RWWWWWWWWRWWWWWRWWWRWWWWWWeWWRWR",
                    /* line */ (const char *) "RWWWWRWWRWWWRWWWWWWWWWWWRWWRWW.R",
                    /* line */ (const char *) "RWWWWWWRWWWWWRWWWRWWWWWRWWWWW..R",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWWWWWWRWWWWR...R",
                    /* line */ (const char *) "R$WWWWRWWWWWWWWWWWWRWWWWWWR....R",
                    /* line */ (const char *) "RWW@WWWRWWWWWWRWWWWWReWWWR...E.R",
                    /* line */ (const char *) "R$WWWWWWRWWWRWWWWWWWWRWWWWR....R",
                    /* line */ (const char *) "RWWWWWWWWRWWRWeWWWRWWWRWeWWR...R",
                    /* line */ (const char *) "RWWWRRWWWWRRRWWWWRWWWWWRWWWWR..R",
                    /* line */ (const char *) "RWWWWWRWeWWWWWWWWRWWWWWRWWeWWR.R",
                    /* line */ (const char *) "RWWWWWRWWWWRWWRWWWRWWWWWWWRWWRWR",
                    /* line */ (const char *) "RRWeWRWRWWWWRRRRRRWWWWWWWReWWWWR",
                    /* line */ (const char *) "RWWWWRWWRWWWRWWWWWRWWWWWRWWWWWRR",
                    /* line */ (const char *) "RWWWWWWWWWWWRWRWWWWWWeWRWWeWWWWR",
                    /* line */ (const char *) "RWWWWWWWWeWRWWWWWWWWWWWWWWRWeWWR",
                    /* line */ (const char *) "RWWWRWWWWWWWWWWWWWWWRWWWWWWWWWRR",
                    /* line */ (const char *) "RWWRWWRWWeWWWWWWWWRRWWWWRWWWeWWR",
                    /* line */ (const char *) "RWRWWWWRWWWRWWWWWRWWWWWRWWWWWeWR",
                    /* line */ (const char *) "RRWWeWWWWWRWRWWWWWWWWWRWRWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWRWWWeRWWRWWRWeWWWWR",
                    /* line */ (const char *) "RWWWWWWWRWWWWWRWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "R,,WWWRWWWWWWWWRWWWWWWWeWWRWW,,R",
                    /* line */ (const char *) "R,,WWWWWWWWWWWRWWWWWWWWRWWWWW,$R",
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.2", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "vvvvRRRRRRRRRRRRRRRRRRRRRRRRvvvv",
                    /* line */ (const char *) "v$,vWWWWW,WWRWWWWWWRWWWWRWWWv,$R",
                    /* line */ (const char *) "v,,vWWWWW,WWWWWWWWWRWWWWWWWWv,,R",
                    /* line */ (const char *) "vv+vWWeRW,WWRWWeWWRWWWWRWWWWv+vv",
                    /* line */ (const char *) "RR,,,,W,,,,,,,,,,,,,,,,,,,,,,,WR",
                    /* line */ (const char *) "RWWWRWWWW,WRWWWWWRWWWWWWRWWeWWRR",
                    /* line */ (const char *) "RWWeWWvvv+vvvWWWWWWWWWWWWRWWWWWR",
                    /* line */ (const char *) "RRWWWWvg,,,,vWWWWWWWWeWWWWWWWWRR",
                    /* line */ (const char *) "RWWWWWv,,,,,vWWRWWWRWWWWWWeWWRWR",
                    /* line */ (const char *) "RWWWWRv$,,,$vWWWWWWWWWWWRW_____R",
                    /* line */ (const char *) "RWWWWWvvvvvvvRWWWRWWWWWRWW_,,,gR",
                    /* line */ (const char *) "R,WWWWWWWWWWWWWWWWWWWWRWWW_,,,,R",
                    /* line */ (const char *) "R,,,WWRWWWWWWWWWWWWRWWWWWW_,,,,R",
                    /* line */ (const char *) "R,,@WWWRWWWWWWRWWWWWReWWWR_,,E,R",
                    /* line */ (const char *) "R,,,WWWWRWWWRWWWWWWWWRWWWW_,,,,R",
                    /* line */ (const char *) "R,WWWWWWWRWWRWeWWvvvvvRWeW_,,,,R",
                    /* line */ (const char *) "RWWWRRWWWWRRRWWWWv$,gvWRWW_,,,gR",
                    /* line */ (const char *) "RWWWWWRWeWRWWWWWWv,,,vWRWW____,R",
                    /* line */ (const char *) "RWWWWWRWWWWRWWRWWv,,,+,,,,,RRRWR",
                    /* line */ (const char *) "RRWeWRWRWWWWRRRRRv,,,vWWWReWWRWR",
                    /* line */ (const char *) "RWWWWRWWRWWWRWWWWv$,gvWWRWWWWRWR",
                    /* line */ (const char *) "RWWWWWWWWWWWRWRWWvvvvvWRWWeWWRzR",
                    /* line */ (const char *) "RWWWWWWWWeWRWWWWWWWWWWWWWWRWeRWR",
                    /* line */ (const char *) "RWWWRWWWWWWWWWWWWWWWRWWWWWWRRWWR",
                    /* line */ (const char *) "RWWRWWRWWeWWWWWWWWRRWWWWRWWWeWWR",
                    /* line */ (const char *) "RWRWWWWRvvvvvvvWWRWWWWWRWWWWWeWR",
                    /* line */ (const char *) "RRWWeWWWv$g,g$vWWWWWWWRWRWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWv,,,,,vWWeRWWRWWRWeWWWWR",
                    /* line */ (const char *) "vvvvWWWWv,,,,,vWWWWWWWWWWWWWvvvv",
                    /* line */ (const char *) "v,,vWWRWvvv+vvvRWWWWWWWeWWRWv,,v",
                    /* line */ (const char *) "v,,+,,,,,,,,,,,,,,,,,,,,,,,,+,$v",
                    /* line */ (const char *) "vvvvRRRRRRRRRRRRRRRRRRRRRRRRvvvv",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'p' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.3", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$..$_ppppppC.....C....$C$....Ex",
                    /* line */ (const char *) "x...._ppppppC..g..C..g..C......x",
                    /* line */ (const char *) "x...._ppppppC.....C.....C......x",
                    /* line */ (const char *) "x...._ppppppC.....C.....C......x",
                    /* line */ (const char *) "x.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C..g...C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C..g..C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C..g..C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxgx",
                    /* line */ (const char *) "xg...C......C.....C.....s......x",
                    /* line */ (const char *) "xz...C......C.....C.....s......x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxx......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C......C.....C.....C......x",
                    /* line */ (const char *) "x....C$.....C....$C.....C$.....x",
                    /* line */ (const char *) "x....C............C.....C......x",
                    /* line */ (const char *) "x.@..C...g.....g..C.....C..g...x",
                    /* line */ (const char *) "x....x............C.....C......x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'p' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.4", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$....k....................g...x",
                    /* line */ (const char *) "x..........................g...x",
                    /* line */ (const char *) "x$....@....................g...x",
                    /* line */ (const char *) "x___________________________...x",
                    /* line */ (const char *) "x.........................._...x",
                    /* line */ (const char *) "x.........................._...x",
                    /* line */ (const char *) "x.......g.................._...x",
                    /* line */ (const char *) "x...____________________..._...x",
                    /* line */ (const char *) "x..._............g....._..._...x",
                    /* line */ (const char *) "x..._.................._g.._..gx",
                    /* line */ (const char *) "x..g_......g..........._..._...x",
                    /* line */ (const char *) "x..._...____________..._..._...x",
                    /* line */ (const char *) "x..._..._pppppppppp_..._..._...x",
                    /* line */ (const char *) "x..._..g_pppppppppp_g.._..._...x",
                    /* line */ (const char *) "x..._..._pppppppppp_..._..._...x",
                    /* line */ (const char *) "x..._..._ppp________..._..._...x",
                    /* line */ (const char *) "x..._..._ppp_CCCCCC_..._..._...x",
                    /* line */ (const char *) "x..._..._ppp_...CCC_..._..._g..x",
                    /* line */ (const char *) "x..._..._ppp_z$.CCE_..g_..._...x",
                    /* line */ (const char *) "x..._g..____________..._..g_...x",
                    /* line */ (const char *) "x..._...xg............._..._...x",
                    /* line */ (const char *) "x..._..._$g...........k_..._...x",
                    /* line */ (const char *) "xg.._...________________..._...x",
                    /* line */ (const char *) "x..._......................_...x",
                    /* line */ (const char *) "x..._g....................._...x",
                    /* line */ (const char *) "x..._$g...................._...x",
                    /* line */ (const char *) "x...________________________...x",
                    /* line */ (const char *) "x..............................x",
                    /* line */ (const char *) "xg.............................x",
                    /* line */ (const char *) "x$g...........................$x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.5", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xE,WWWWWWWWWWWWWWWWWWWWWWWWWWW$x",
                    /* line */ (const char *) "x,,WWWWWWWWWWWxzWWWWWWWWWxeWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWxWWWWWWWWWxxWWWWx",
                    /* line */ (const char *) "xWWWWWWxWWWWWWWWWxWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWxxWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWWWWW$WWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWWexkWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWeWWeWWxxxWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWeWWWWWWWxWWWWWWxeWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWeWWWWWWWeWWWWWWxWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWW,,WWWeWWeWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWxWeWWW,,,,WWWeWWWWWWWWx",
                    /* line */ (const char *) "x$WWWWWWWWWWWW,,@,,,WWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWeWWWW,,,,,WWeWWWWWWWWx",
                    /* line */ (const char *) "xWWWWxWWWWWWWWWW,,,WWWWWxeWWWWWx",
                    /* line */ (const char *) "xWWWeWWWWWeWWWWWWWWWWeWWxxWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWxxWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWex$WWeWWeWWWWWWWW$WWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWeWWWWWWWWWWeWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWeWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWeWWWWWWWWWeWWWWWWWWWxWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWxWWWWWWWWeWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWWWeWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWeWWWWWWWWWWWWWW$x",
                    /* line */ (const char *) "xWWWWWWxWWWWWWWWWWWWWWWWWWvvvvvv",
                    /* line */ (const char *) "xWWWWW,WWWWWWWWWWWWWWWWWWW_WWWWv",
                    /* line */ (const char *) "xWWW,,,,WWWWWWWWWWWWWWWWWW_WWxWv",
                    /* line */ (const char *) "x$WWW,,WWWWWWWWWWWWWWWWWWW_WexEv",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxvvvvvv",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.6", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xeW,,,WWxWWWWWWWWeWeWxWWeWWWeWWWWWWWWWWWxWWW,,$x",
                    /* line */ (const char *) "xWW,@,WWWWWWWWeWWWWWWeWWWeWWexWWWWWeWWWWWWWW,Ezx",
                    /* line */ (const char *) "xeW,,,WWWWWWWWWexeWWWWWeWWWeWWWWWWWxWWWWWWWW,,$x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.7", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x,,,,WxWWWWWWWWeeeeeWWWW,,,,,,gx",
                    /* line */ (const char *) "x,@,WWWWWWWWWxWWWWWWWWW,,,,,,g,x",
                    /* line */ (const char *) "x,,,WWxxxxxxxxxxxxxxxxxxxxxxg,,x",
                    /* line */ (const char *) "x,,WWWWWWWWWWx,,WWWWWWWWWWWx,,,x",
                    /* line */ (const char *) "xWWWWWxxxxWWWx,WeeeeeWWWWWWx,,,x",
                    /* line */ (const char *) "xxxxxxx$,xWWxxWWWWWWWWWWWW,x,,,x",
                    /* line */ (const char *) "xg,,,,,,,xWeWxWWWxWWWWWWW,,x,,Wx",
                    /* line */ (const char *) "x,,,WWWWWxWeWxWWWxWWWWWW,,$x,WWx",
                    /* line */ (const char *) "x,,WWWWWWxxeWxWWWxxxxxxxxxxxWWWx",
                    /* line */ (const char *) "x,WWWWWWWWWeWxWWWxWWWWWWWWWWWW,x",
                    /* line */ (const char *) "xxxxxxxxxxWeWx,WWWWeeeeeWWWW,,,x",
                    /* line */ (const char *) "x,,WWWWWWWWeWx,,WxWWWWWWWWW,,,,x",
                    /* line */ (const char *) "x,WWxxxxxxWWWxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xWWWx$gg$xWWWxWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWx,,,,xWWWWWWWWWWWWWWWeeWeeWx",
                    /* line */ (const char *) "xWWWx,,,,xWWWWWWWWWxWxWxWWWWWWWx",
                    /* line */ (const char *) "xWWWxx__xxxxxxxxxxxxWxWxWxxxxxxx",
                    /* line */ (const char *) "xWWWWWeWWeWWWWWWWWWxWxWxWx$,,,gx",
                    /* line */ (const char *) "xWWWeWWWWWWeWWWWWWWxWxWxWx,E,,,x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxWxWxWx,,,,Wx",
                    /* line */ (const char *) "x$,,,WWWWWWWWWWWWWWWWxWxWx,,,WWx",
                    /* line */ (const char *) "x,,,WWWWWWWWWeWWWWWWWxWxWx,WWWWx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxWxWxWWWWzx",
                    /* line */ (const char *) "x$$,g,,WWWWWWWWWeWWWWWWxWxWxxxxx",
                    /* line */ (const char *) "x$,g,,,WWWWeWWWeeWeWWWWxWxWWWWWx",
                    /* line */ (const char *) "x,g,,,,WWWWWWWWWWWWWWWWxWxWWWeWx",
                    /* line */ (const char *) "xg,,,,WWWxxxxxxxxxxxxxxxWxWeWWWx",
                    /* line */ (const char *) "x,,,,WWWWx$,,WWWWWWWWeWWWxWWWWWx",
                    /* line */ (const char *) "x,,,WWWWWx,,WWWWWWeWeWWWWxxxxxWx",
                    /* line */ (const char *) "xWWWWWWWWx,WWWWWWWWWWWeWWWWWWWWx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.8", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWWWWWWWWWW,,gx",
                    /* line */ (const char *) "xW@WWWWWWWWWWWWWWWWWWWWWWWWW,$,x",
                    /* line */ (const char *) "xWWWWWWW,WWWWWWWWWWWWWWWWWWWW,,x",
                    /* line */ (const char *) "xWWWWWW,$,WWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWW,g,,,WWWWWWWWWW,WWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWW,,,,,WWWWWWWW,,,WWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWW,WWWWWWWWW,,$,,WWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWW,WWWWWWWWWW,g,,,WWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWWWW,,,,WWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWWWW,,WWWWWWWx",
                    /* line */ (const char *) "xWWW,WWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xW,,,,WWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xW,g,,,WWWWWWWWWWWWWWWWWWWWWWW,x",
                    /* line */ (const char *) "xW$,,,WWWWWWWWWWWWWWWWWWWWWWW,$x",
                    /* line */ (const char *) "xWW,,WWWWWWWWWWWWWWWWWWWWWWWW,zx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWW,,WWWWWWWW,,x",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWW,,,,,WWWWWWW,x",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWW,,g$,,WWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWW,,,,W,WWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWW,,,WWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWW,WWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWW,,WWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWW,,,,WWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWW,,g,,,WWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWW,,$WW,WWWWWWWWWWWWWWWW,,x",
                    /* line */ (const char *) "xWWWWWWWW,,,,WWWWWWWWWWWWWWW,,,x",
                    /* line */ (const char *) "x,,WWWWWWW,,WWWWWWWWWWWWWWW,,E,x",
                    /* line */ (const char *) "x$,WWWWWWWWWWWWWWWWWWWWWWWW,,,$x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.9", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x,,,@,,,,WWWWWWWWWWWWWWWWW,,,,gx",
                    /* line */ (const char *) "xsxxxxxxxxxxxxxxxxxxxxxxxxxxxx,x",
                    /* line */ (const char *) "xWWWW,,,_,,,,,,,,g,,,,,,,,x,zx,x",
                    /* line */ (const char *) "xWWWW,,,_,,,,,,,,,g,,,,,,,x,,xkx",
                    /* line */ (const char *) "xWWW,,,,_,,,,,,,,,,g,,,,,,x,,x,x",
                    /* line */ (const char *) "xWW,,,,,_,,,,,,,,,,,g,,,,,xxsx,x",
                    /* line */ (const char *) "xWW,,,,,_,,,,,,,,,,,,g,,,,,,,xWx",
                    /* line */ (const char *) "xWxxxxxxx____________________xWx",
                    /* line */ (const char *) "xWx$,,,$x,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWx,gEg,x,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWx,,,,,x,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWx,,,,,x,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWxWWWWWx,,,,,,,,,,,,,,,,,,,,x,x",
                    /* line */ (const char *) "x,xWWWWWx,,,,,,,,,,,,,,,,,,,,x$x",
                    /* line */ (const char *) "x$xWWWWWx,,,,,,,,,,,,,,,,,,,,x,x",
                    /* line */ (const char *) "x,xWWWWWx,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWxWWWWWx,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWxWWWWWx,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWxWWWWWx,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWxWWWWWx,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWxWWWWWx,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWxWWWWWx,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWxWWWWWx,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWxWWWWWx,,,,,,,,,,,,,,,,,,,,xWx",
                    /* line */ (const char *) "xWxWWWWWx,,,,,,,,,,,,,,,,,,,,x,x",
                    /* line */ (const char *) "x,xWWWWWWWWW,,,,,,,,,,,,,,,,gx,x",
                    /* line */ (const char *) "x,xWWWWWWWWWWWWW,,,,,,,,,,,g$x,x",
                    /* line */ (const char *) "x,xWWWWWWWWWWWWWWWWWWWW,,,g$$x,x",
                    /* line */ (const char *) "xkxxxxxxxxxxxxxxxxxxxxxxxxxxxx,x",
                    /* line */ (const char *) "xg,,,,WWWWWWWW,$,WWWWWWW,,,,,,gx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.10", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x@,$x,,,,x,,,,+,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,x,k,,x,,,,x,,g,,,,,,,,g,,,,x",
                    /* line */ (const char *) "x,,,x,,,,x,,,,x,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "xx,xxxxx,xxx,xxxxxxxxxxxxxx____x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,x,,,,,,,$$x,,,,x",
                    /* line */ (const char *) "xWxxxx,,,,,,,,,,,x,,,g,,,$x,,,,x",
                    /* line */ (const char *) "xWxk,$x,,,,,,,,,,,x,,,,,,,x,g,,x",
                    /* line */ (const char *) "xWx,,,,x,,,,,,,,,,,x,,,,,,x,,,,x",
                    /* line */ (const char *) "xWx,,,,,x,,,,,,,,,,,x,,,,,x,,,,x",
                    /* line */ (const char *) "xWWx,,,,,x,,,,x,,,,,,x,,,,x,,,,x",
                    /* line */ (const char *) "xWWWx,,,,x,,,xzx,,,,,,x,,,_,,,,x",
                    /* line */ (const char *) "xWWWWx,,,x,,,,x,,,,,,,,x,,_,,,,x",
                    /* line */ (const char *) "xeWWWWx,,x,,,,,,,,,,,,x,x,_,,,,x",
                    /* line */ (const char *) "xeWeWWWx,x,,,,,,,,,,,x,,,x_,,,,x",
                    /* line */ (const char *) "xWWeWeWx+xxxxxxxxxxxx,,,,,x,,,,x",
                    /* line */ (const char *) "xWWeWWWx,,,,,,,,,,,,x,,$,,x,,,,x",
                    /* line */ (const char *) "xWWWWW,x,,,x,x,x,x,,x,,,,,x,,,,x",
                    /* line */ (const char *) "xWWWW,,x,,,,,,,,,,,,x,,$,,x,,,,x",
                    /* line */ (const char *) "xWWWW,,xxx,x,x,x,x,xx,,,,,x,,g,x",
                    /* line */ (const char *) "xWWW,,,gx,,,,,,,,,,,,x,,,x,,,,,x",
                    /* line */ (const char *) "xWW,,,g,,x,,,,,x,,,,,,x,x,,,,,,x",
                    /* line */ (const char *) "xWW,,x,,,,x,,,,,,,,,,,,x,,,,,,,x",
                    /* line */ (const char *) "xWWWxW,,,WWxWW,,,,,WWWWWxWW,,,Wx",
                    /* line */ (const char *) "xWWxWW,,,WWWxWWWWWWWWWWWWxWWWWWx",
                    /* line */ (const char *) "xWxWW,,,WWWxWWWWWWWxWWWWWWxggggx",
                    /* line */ (const char *) "xgWWWW,WWWxWWWWWWWWWxWWWWWWxWWWx",
                    /* line */ (const char *) "xWWWWWWWWxWWWWxWWWWWWxWWWWWWxWWx",
                    /* line */ (const char *) "xWWWWWWWxWWWWWWxWWWWWWxWWWWWWxEx",
                    /* line */ (const char *) "xWWWWWWxWWWW,WWWxWWWWWWgWWWWW$xx",
                    /* line */ (const char *) "xWWWWWgWWWW,,,WWWxWWWWWWWWWWW$$x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.11", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x,,,,,WWWWWxWWWWWWWWW,,,WWWW,,$x",
                    /* line */ (const char *) "x,,@,WWWWWxWWWWWxWWWWWWWWWWWW,,x",
                    /* line */ (const char *) "x,,,,WWWWxWWWWWWWxWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x,WWWWWWxWW,WWWWWWxWWWWWxWWWWWWx",
                    /* line */ (const char *) "xWWWWWWxWWWWWWWWWWWxWWWxgxWWWWWx",
                    /* line */ (const char *) "xWWWWWxWWWWWWWWWWWWWxWxWxWxWWWWx",
                    /* line */ (const char *) "xWWWWxgxWWWWWWW,WWWWWxWWWWWxWWWx",
                    /* line */ (const char *) "xWWWWWxWWWWWWWWWWWWWWWWWWWWWxWWx",
                    /* line */ (const char *) "x,WWWWWWWWgWWWWWWWWWgWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWWxxxxxxxWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWWWxWWWWWWWWWxWWx",
                    /* line */ (const char *) "xWWxxxxxxxxxxxWWWxWWWWWWWWWWWx$x",
                    /* line */ (const char *) "xWWWWWWWWWWWWWxgxWWWWWWWWWWWWWxx",
                    /* line */ (const char *) "xxWWWWWWWWWWWWgzgWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWxWWWWW,WWWWWxgxWWWWWxWWWWWWWxx",
                    /* line */ (const char *) "xWWxWWWWWWWWWxWWWxWWWxWWWWWWWxWx",
                    /* line */ (const char *) "xWWWxWWWWWWWxWWWWWxWxWWWWWWWxWWx",
                    /* line */ (const char *) "xWWWWxWWWWWxWWWWWWWxWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWgWxWWWWWWWgWWW,WWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWxWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWWWWxWWWWWxWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWxWWWWWWWxWWWWWWxWWWWWxWWWx",
                    /* line */ (const char *) "xWWWWxgxWWWWWWxWWWWWWWxWWWxWWWWx",
                    /* line */ (const char *) "x,WWWWxWWWWWWWxWWWWWWWWxWxWWWW,x",
                    /* line */ (const char *) "xWWWWWWxWWWWWWxWWWWWWWxgxWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWxWWWWWxWWWWWWxWxWWWWWWWx",
                    /* line */ (const char *) "xxWWWWWWWxWWWWxWWWWWxWWWWWWggggx",
                    /* line */ (const char *) "x,xWWWWWWWxWWWWWWWWxWWWWWWWg,,,x",
                    /* line */ (const char *) "x$,xWWWWWWWxWWWWWWxWWWWWWWWg,,$x",
                    /* line */ (const char *) "x$,,xWWWWWWWWW,,,WWWWWWWWWWg,$Ex",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.12", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xzxWWWWWWWWW,,,WWWWWWWWWWWx",
                    /* line */ (const char *) "xWxWWWWWWWWW,@,WWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWW,,,WWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWW,,,WWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWW,,,WWWeWWWWWWWx",
                    /* line */ (const char *) "xWWWWWeWWWWW,,,WWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWW,,,WWWWeWWWWWWx",
                    /* line */ (const char *) "x,,WWeWWWWWW,,,WWWWWWWWW,,x",
                    /* line */ (const char *) "x$,WWWWWWWWW,,,WWWWWeWWW,kx",
                    /* line */ (const char *) "x,,WWWWWWWWW,,,WWWWWWWWW,,x",
                    /* line */ (const char *) "xWWWWWWWWWWW,,,WWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWWWWW,,,WWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWWWeWWW,,,WWeWWWWWWWWx",
                    /* line */ (const char *) "xWWeWWWWWWWW,,,WWWWWWWeWWWx",
                    /* line */ (const char *) "xWWWWWWWWxxx___xxxWWWWWWWWx",
                    /* line */ (const char *) "x,,,,,,,xx,,,,,,,xx,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,x,,,,,,,,,x,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,xx,,,,,,,,,xx,,,,,,x",
                    /* line */ (const char *) "x,,,,,,x,,,,,,,,,,,x,,,,,,x",
                    /* line */ (const char *) "x,,,,,,xx,,,,,,,,,xx,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,xxgggggggxx,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,xxxxExxxx,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,xxxxxxx,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,gg$$$gg,,,,,,,,,x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.13", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x,,,,k,,,,_,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,@,,,,,,,_,,,,g,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,_,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "xxxxxx,,,,_,,,,g,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,szxxxxx,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,xxx,,,,xxxxx,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "xWWWWWWWWWWWWWWWxxxxx,,,,,,,,,gx",
                    /* line */ (const char *) "xWWWWWeWWWWWWWWWWWWWWxxxxx,,,g,x",
                    /* line */ (const char *) "xWWWWWWWWWWeWWWWWWWWWWWWWWxxxx.x",
                    /* line */ (const char *) "x.xxxxWWWWWWWWeWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWWxxxxxWWWWWWWWWeWWWWWWWWWWx",
                    /* line */ (const char *) "xWWWWeWWWWWxxxxxWWWWWWWeWWWWWWWx",
                    /* line */ (const char *) "xWWWeWeWWWWWWWWWxxxxxWWWWWWWWW,x",
                    /* line */ (const char *) "x,WWWWWWWWWeWWWWWWWWWxxxxxWWW,$x",
                    /* line */ (const char *) "x$,WWWWWWWWWWWWWeWWWWWWWWWxxxxxx",
                    /* line */ (const char *) "xxxxxxWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x$$,,,xxxxx,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x$,,,,,,,,,xxxxx,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,xxxxx,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,xxxxx,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,,,,,,xxxx.x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,,,,,,g,,,,x",
                    /* line */ (const char *) "xE,,,,,,,,,,,,,,,,,,,,,,,,g,,,,x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.14", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x,,,,,xWeWWWWWWWWeWWeWWWeWe,,,gx",
                    /* line */ (const char *) "x,@,,,xWWeWWxxxxxxxWxxxxxxxxxx,x",
                    /* line */ (const char *) "x,,,,,WWWWeWx$,,,$xWx$$$,,,,gx,x",
                    /* line */ (const char *) "x$,,,,xWWWWex,,,,,xTx$$$,,,,gx,x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxsxxxxxxxxxsxx,x",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,,,x$,,,,,,x,,x",
                    /* line */ (const char *) "x,T,,,x,,,,,,,,,,,,,x,,,,,,x,,Wx",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,,,x,,,,,x,WWWx",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,,,x,,,,xWWWWWx",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,,,x,,,xWWWWWWx",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,,,x,,xWWWWWWWx",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,,,s,xWWWWWWWWx",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,,,xxWWWWWWWW,x",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,,,xWWWWWWWW,,x",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,,xWWWWWWWW,,,x",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,xWWWWWWWW,,,,x",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,,,WWWWW,,,,,,x",
                    /* line */ (const char *) "x,,,,,x+xxxxxxxxxxx,,WW,,,,,,,gx",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,x,,,,,,,xx+xxx",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,,x,,,,,,,x,,,,x",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,,gx,,,,,,,x,,,,x",
                    /* line */ (const char *) "x,,,,,x,,,,,,,,,g$x,,,,,,,x,,,,x",
                    /* line */ (const char *) "x,,,,,xx+xxxxxxxxxx,,,,,,,x,,,,x",
                    /* line */ (const char *) "x,,,,,,,,x,,,,,,,,,,,,,,,,x,,,,x",
                    /* line */ (const char *) "x,,,,,,,,x,,,,,,,,,,,,,,,gx,,,,x",
                    /* line */ (const char *) "x,,,,,,,,x,,,,,,,,xxxxxxxxx,,,,x",
                    /* line */ (const char *) "x,,,,,,,,x,,,,,,,,x$$,,,,,,,,,gx",
                    /* line */ (const char *) "xg,,,,,,,x,,,,,,,,x$,,,,,,,,,g,x",
                    /* line */ (const char *) "x$g,,,,,,x,,,,,,,,x,,,,,,,,,g,,x",
                    /* line */ (const char *) "xz$g,,,,,+,,,,,,,,+,,,,,,,,g,,Ex",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.15", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,@,,,,,,,,,,,,,,,,,,,,,,,,,,k,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x______________________________x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCx",
                    /* line */ (const char *) "xzgxgxgxgxgxgxgxgxgxgxgxgxgxgxgx",
                    /* line */ (const char *) "xCCCCCCCCCCCCCCC,CCCCCCCCCCCCCCx",
                    /* line */ (const char *) "x,,,,,,,,,,,,,CC,CC,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,CC,CC,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,CC,CC,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,CC,CC,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,CCkCC,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,CC,CC,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,CCCCC,,,,,,,,,,,,x",
                    /* line */ (const char *) "x_____________xxxxx____________x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,,,,,,,,,E,x",
                    /* line */ (const char *) "x,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,x",
                    /* line */ (const char *) "xxgx$xgxgx$xgxgxgxgx$xgxgxgxgxgx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.16", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "RWWWWWeWWWWWWWeWWWWWWWeWWWWWWWWR",
                    /* line */ (const char *) "RWWRRRRRRWWRRRRRRWeRRRRRReWRRWWR",
                    /* line */ (const char *) "RWWRReWRRWWRRWWRRWWRRWWRRWWRRWeR",
                    /* line */ (const char *) "RWWRRWeRRWWRRWWRRWWRRWWRRWWRRWWR",
                    /* line */ (const char *) "RWWRRWWRReWRRWWRRWWRRWWRRWWRRWeR",
                    /* line */ (const char *) "RWWRRWWRRWeRRWWRRWWRRWWRRWWRRWeR",
                    /* line */ (const char *) "RWWRRWWRRWeRRWWRRWWRRWWRRWWRRWWR",
                    /* line */ (const char *) "RWWRRWWRRWWRRWWRRWWRRWWRRWWRRWWR",
                    /* line */ (const char *) "RWWRRWWRRWWRRWWRRWWRRWWRRWWRRWWR",
                    /* line */ (const char *) "R,,R$WWR$WWR$WWR$WWR$WWR$WWR$WWR",
                    /* line */ (const char *) "R@,RRggRRggRRggRRggRRggRRggRRWER",
                    /* line */ (const char *) "R,,RRWWRRWWRRWWRRWWRRWWRRWWRRWWR",
                    /* line */ (const char *) "RWWRRWWRRWeRRWWRRWWRRWWRRWWRReWR",
                    /* line */ (const char *) "RWWRRWWRRWWRRWWRRWWRRWWRRWWRRWWR",
                    /* line */ (const char *) "RWWRReWRRWWRRWWRRWWRRWWRRWWRRWWR",
                    /* line */ (const char *) "RWWRRWWRRWWRRWWRRWWRRWWRRWWRRWWR",
                    /* line */ (const char *) "RWWRRWWRRWWRRWWRRWWRRWWRRWWRRWWR",
                    /* line */ (const char *) "RWWR$WWR$WWR$WWR$WWR$WWR$WWRzWWR",
                    /* line */ (const char *) "RWWRRggRRRRRRggRRRRRRggRRRRRRggR",
                    /* line */ (const char *) "RWWWWWeWWWWWWWeWWWWWWWeWWWWWWWWR",
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.17", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "RWWWWWeWWWWWWWeWWWWWWWeWWWWWWWWR",
                    /* line */ (const char *) "RWWRRRRRRRRRRRRRRRRRRRRRRRRRRWWR",
                    /* line */ (const char *) "RWWRReWWWWWWWWWRRWWRRWWRR$$RRWeR",
                    /* line */ (const char *) "RWWRRWeWWWWWWWWRRWWRRWWRReWRRWWR",
                    /* line */ (const char *) "RWWRRWWWWeWWWWWRRWWRWWWRRWeRRWeR",
                    /* line */ (const char *) "RWWRRWWWWWeRWWWRRWWWWWWRReWRRWeR",
                    /* line */ (const char *) "RWWRRWWWWWeRRWWRRWWWWWWRReeRRWWR",
                    /* line */ (const char *) "RWWRRWWWWWWRRWWRRWWRWWWRRWRRRWWR",
                    /* line */ (const char *) "RWWRRRWWWWWRRWWRRWWRRWWRRWWRRWWR",
                    /* line */ (const char *) "R,,R$WWWWWWR$WWR$WWR$WWRRRWRzWWR",
                    /* line */ (const char *) "R@,RRRWWWWWRRRRRRggRRggRRggRRWWR",
                    /* line */ (const char *) "R,,RRWWWWWWRRWWWWWWRRWWWRWWRRWWR",
                    /* line */ (const char *) "RWWRRWWWWWeRRWWWWWWRRWWWRRWRReWR",
                    /* line */ (const char *) "RWWRRWWWWWWRRWWWRRWRRWWWWWWRRWWR",
                    /* line */ (const char *) "RWWRReWWWWWRWWWWRWWRRWWWRRWRRWWR",
                    /* line */ (const char *) "RWWRRWWWWWWWWWWRRWWRRWWWRWWRRWWR",
                    /* line */ (const char *) "RWWRRWWWWWWWWWWRRWWRRWWRRWWRRWWR",
                    /* line */ (const char *) "RWWR$WWWWWWWWWWR$WWR$WWR$WER$WWR",
                    /* line */ (const char *) "RWWRRggRRRRRRRRRRRRRRRRRRRRRRggR",
                    /* line */ (const char *) "RWWWWWeWWWWWWWeWWWWWWWeWWWWWWWWR",
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.18", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRR",
                    /* line */ (const char *) "RWWWW,,@,,WWWWR",
                    /* line */ (const char *) "RWWWWW,,,WWWWWR",
                    /* line */ (const char *) "ReWWRR,,,RRWWeR",
                    /* line */ (const char *) "RWWRRRR,RRRRWWR",
                    /* line */ (const char *) "RWRRRRRCRRRRRWR",
                    /* line */ (const char *) "RWRRR,ggg,RRReR",
                    /* line */ (const char *) "RWWRR,,,,,RRWWR",
                    /* line */ (const char *) "RWWWWWW,WWWWWWR",
                    /* line */ (const char *) "ReWWRRW,WRRWWWR",
                    /* line */ (const char *) "RWWRRRR,RRRRWeR",
                    /* line */ (const char *) "ReRRRRRCRRRRRWR",
                    /* line */ (const char *) "RWRRR,ggg,RRRWR",
                    /* line */ (const char *) "RWWRR,,,,,RRWWR",
                    /* line */ (const char *) "RWWWWWW,WWWWWeR",
                    /* line */ (const char *) "ReWWRRW,WRRWWeR",
                    /* line */ (const char *) "RWWRRRR,RRRRWeR",
                    /* line */ (const char *) "RWRRRRRCRRRRRWR",
                    /* line */ (const char *) "ReRRR,ggg,RRReR",
                    /* line */ (const char *) "ReWRR,,,,,RRWWR",
                    /* line */ (const char *) "RWWWWWW,WWWWWWR",
                    /* line */ (const char *) "ReWWRRW,WRRWWWR",
                    /* line */ (const char *) "RWWRRRR,RRRRWWR",
                    /* line */ (const char *) "RWRRRRRCRRRRRWR",
                    /* line */ (const char *) "ReRRR,ggg,RRReR",
                    /* line */ (const char *) "RWWRR,,,,,RRWeR",
                    /* line */ (const char *) "ReWWWWW,WWWWWeR",
                    /* line */ (const char *) "ReRWWW,,,WWWWWR",
                    /* line */ (const char *) "RzRWW,,E,,WWWWR",
                    /* line */ (const char *) "RRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'e' ].do_thing_spawn = [](Gamep g_in, Levelsp v, Levelp l, const bpoint &at, ThingEvent *e) -> Thingp {
      (void) thing_spawn(g_in, v, l, tp_find_random(is_water_dweller), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_deep), at, e);
      (void) thing_spawn(g_in, v, l, tp_first(is_water_shallow), at, e);
      return thing_spawn(g_in, v, l, tp_first(is_dirt), at, e);
    };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.19", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,@,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "R$$,gsCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCsg,$$R",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "R$$,gsCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCsg,$$R",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "Rz$,gsCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCsg,$$R",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "R$$,gsCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCsg,$$R",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "R$$,gsCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCsg,$zR",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "RRRRRR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRRRRR",
                    /* line */ (const char *) "R$$,gsCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCsg,$$R",
                    /* line */ (const char *) "RRR,RR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RR,RRR",
                    /* line */ (const char *) "RRR,RR,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RR,RRR",
                    /* line */ (const char *) "RRR,,s,,,,,,,,,,,,,,E,,,,,,,,,,,,,,,s,,RRR",
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE2, "treasure2.20", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "R$g,,,,,,,,,R,,,,,,R,,,,,,,,,gkR",
                    /* line */ (const char *) "Rg,,,,,,,,R,,,,@,,,,,R,,,,,,,,gR",
                    /* line */ (const char *) "R,,,,,,,,,,,R,,,,,,R,,,,,,,,,,,R",
                    /* line */ (const char *) "R,,,,,,,,,R,,,R,,R,,,R,,,,,,,,,R",
                    /* line */ (const char *) "R,,,,,,,,,,,R,,,,,,R,,,,,,,,,,,R",
                    /* line */ (const char *) "R,,,,,,,,,R,,,R,,R,,,R,,,,,,,,,R",
                    /* line */ (const char *) "R,,,,,,,,,,,R,,,,,,R,,,,,,,,,,,R",
                    /* line */ (const char *) "R,,,,,,,,,R,,,R,,R,,,R,,,,,,,,,R",
                    /* line */ (const char *) "R,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCCCCCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCCCCCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCCCCCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCCCCCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCCCCCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCCCCCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCCCCCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCg,gCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCg$$$gCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCg$$$gCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCg,gCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCC,CCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCC,CCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCC,CCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCC,CCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCC,CCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,CCCCCCCCCCCCC,CCCCCCCCCCCCCC,R",
                    /* line */ (const char *) "R,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,R",
                    /* line */ (const char *) "R,,,,,,,,,,,,RR,RR,,,,,,,,,,,,,R",
                    /* line */ (const char *) "R,,,,,,,,,,,R,,,,,R,,,,,,,,,,,,R",
                    /* line */ (const char *) "RCC,,,,,,,,R,,,E,,,R,,,,,,,,,CCR",
                    /* line */ (const char *) "R$C,,,,,,,,R,,,,,,,R,,,,,,,,,CzR",
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("haunt_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCg.gCCCCCCCCCCCCCCg..CCCCCCCCC",
                    /* line */ (const char *) "CCC.T.CCCCCCCCCCCCCC.T.C_....$.C",
                    /* line */ (const char *) "CCCg.gCCCCCCCCCCCCCC...C_......C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCC_......C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCC____......C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCCkCCCCCCCCCC.........gC",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCC_....CCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCC._...CC....zC",
                    /* line */ (const char *) "C.@...............g._...CC..E..C",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCC._...CC....zC",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCC_....CCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCkCCCCCCCCCC.........gC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCC____......C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCC_......C",
                    /* line */ (const char *) "CCC$$$CCCCCCCCCCCCCC...C_......C",
                    /* line */ (const char *) "CCC$T$CCCCCCCCCCCCCC.T.C_.....$C",
                    /* line */ (const char *) "CC$$$$CCCCCCCCCCCCCCg..CCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.2", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCg.gCCCCCCCCCCCCCCg..CCCCCCCCC",
                    /* line */ (const char *) "CCC.T.CCCC...........T.C_....$.C",
                    /* line */ (const char *) "CCCg.gCCCC.CCCCCCCCC...C_......C",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCCCCCC_......C",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCCC____......C",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCCkCCCCCCCCCC.........gC",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCC_....CCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCC._...CC.....C",
                    /* line */ (const char *) "C.@...............g._...CC..E.zC",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCC._...CC.....C",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCC_....CCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCkCCCCCCCCCC.........gC",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCCC..........C",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCCC____......C",
                    /* line */ (const char *) "CCCCCCCCCC.CCCCCCCCCCCCC_......C",
                    /* line */ (const char *) "CCC$$$CCCC.CCCCCCCCC...C_......C",
                    /* line */ (const char *) "CCC$T$CCCC...........T.C_.....$C",
                    /* line */ (const char *) "CCC$$$CCCCCCCCCCCCCCg..CCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.3", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCzCCC....C....C..@...C......C",
                    /* line */ (const char *) "CC.CCC.....C....C....C.......C",
                    /* line */ (const char *) "CC.CC.......C....C..C........C",
                    /* line */ (const char *) "CC.C.........C.C...C.........C",
                    /* line */ (const char *) "CC.C.x.x......C..CC..........C",
                    /* line */ (const char *) "CCCC..g........C=C...x.x.....C",
                    /* line */ (const char *) "Csxx.x.x.......C=C....g......C",
                    /* line */ (const char *) "C$$xCCCCCCCCCCCC=C...x.x.....C",
                    /* line */ (const char *) "C$$xCCCCCCCCCCCC=C...........C",
                    /* line */ (const char *) "C$$xCCCCCCCCCCCC=CCCCCCCCCCCCC",
                    /* line */ (const char *) "Csxx....=======C=C=======CCCCC",
                    /* line */ (const char *) "Cb....CCCCCCCCCC=CCCCCCCCCCCCC",
                    /* line */ (const char *) "Cb....CC......bC=Cb......CCCCC",
                    /* line */ (const char *) "Cb....CC....g..C=C..g....CCCCC",
                    /* line */ (const char *) "CCCCCCCC.......C=C.......CCCCC",
                    /* line */ (const char *) "C..g...C.......C=C.......C...C",
                    /* line */ (const char *) "C...g..C.......C=C.......C.$.C",
                    /* line */ (const char *) "C....g.C.$.....C=C.....$.C...C",
                    /* line */ (const char *) "C.C....C$.$....C=C....$.$C...C",
                    /* line */ (const char *) "C.C=CCCCCCCCCCCC=CCCCCCCCC...C",
                    /* line */ (const char *) "C.C....CCCCCCCCC=CCCCCCCCCCC=C",
                    /* line */ (const char *) "C.Cg...C........=........C...C",
                    /* line */ (const char *) "C.C.G..C......C..........C...C",
                    /* line */ (const char *) "C.C..g.C......C..........C...C",
                    /* line */ (const char *) "C.C....C.....CCC.........C..EC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.4", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxCCCCCCCCCCCCCCCCCCxxxxxxxx",
                    /* line */ (const char *) "x$..gxxCCCCCCCCCCCCCCCCCx$.T..$x",
                    /* line */ (const char *) "x.....xCCCCCCxxxxxCCCCCCx......x",
                    /* line */ (const char *) "x....TxCCCCCCxE.bxCCCCCCx......x",
                    /* line */ (const char *) "xb...xxCCCCCCx.T.xCCCCCCxg....gx",
                    /* line */ (const char *) "xxxx.xCCCCCCCxb.$xCCCCCCxxxxxxxx",
                    /* line */ (const char *) "CCCC=CCCCCCCCxxxxxCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCC=CCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCC=CCCCCCCCCCCCCCCCCxxxxxCCCCC",
                    /* line */ (const char *) "CCCC=CCCCCCCCCCCCCCCCCx...xCCCCC",
                    /* line */ (const char *) "CCCC==================..T.xCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCC=CCCx...xCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCC=CCCxxxxxCCCCC",
                    /* line */ (const char *) "CCxxxxxCCCxxxxCCCC=CCCCCCCCCCCCC",
                    /* line */ (const char *) "CCxg..xCCCxT.xCCxx=xxxxxxCCCCCCC",
                    /* line */ (const char *) "CCx.T.xCCCx.zxCCx.......xCCCCCCC",
                    /* line */ (const char *) "CCx..gxCCCxxxxCCx..@.T..xCCCCCCC",
                    /* line */ (const char *) "CCxxxxxCCCCCCCCCxb......xCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCxbb.....xCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCxxxxxx=xxCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCC=CCCCCCCCC",
                    /* line */ (const char *) "xxxxxCCCCCCCCCCCCCCCCC=CCCCCCCCC",
                    /* line */ (const char *) "xbbbxCCCCCCCCCCCCCCCCC=CCCCCCCCC",
                    /* line */ (const char *) "xbTb===================CCCCCCCCC",
                    /* line */ (const char *) "xbbbxCCCCCCCCCCCCCCCCC=CCCCCCCCC",
                    /* line */ (const char *) "xxxxxCCCCCxxxxxxCCCCCC=CCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCxg..gxCCCCCC=CCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCx....xCCCCCC=CCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCx.T..xCCCCCC=CCCCxxxxx",
                    /* line */ (const char *) "xxxxxxxxxxx....xCCCCCC======...x",
                    /* line */ (const char *) "x$$$......sg..gxCCCCCCCCCCCx.T.x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxCCCCCCCCCCCx...x",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.5", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCC...............CCCCCCCC",
                    /* line */ (const char *) "CCCCCC..........@..........CCCCC",
                    /* line */ (const char *) "CCCC.........................CCC",
                    /* line */ (const char *) "CCC.............R.............CC",
                    /* line */ (const char *) "CCC............RRR............CC",
                    /* line */ (const char *) "CC............RRzRR............C",
                    /* line */ (const char *) "CC...........RR...RR...........C",
                    /* line */ (const char *) "CC..........RR.....RR..........C",
                    /* line */ (const char *) "CCC........RR.......RR........CC",
                    /* line */ (const char *) "CCC........R.........R........CC",
                    /* line */ (const char *) "CCCC........................CCCC",
                    /* line */ (const char *) "CCCCCC.....................CCCCC",
                    /* line */ (const char *) "CCCCCCCCCg.............gCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCC=CCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCC=CCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCC...CCC=CCC..gCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCC.$.CCC=CCC.k.CCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCg..CCC=CCC...CCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCC=CCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCC=CCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCC...CCC=CCC..gCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCC.$.CCC=CCC.k.CCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCg..CCC=CCC...CCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCC=CCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCC=CCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCC......ggg......CCCCCCCC",
                    /* line */ (const char *) "CCCCCC.....................CCCCC",
                    /* line */ (const char *) "CCCC.........................CCC",
                    /* line */ (const char *) "CCC...........................CC",
                    /* line */ (const char *) "CCCR........R_______R........RCC",
                    /* line */ (const char *) "CC$.R.......R.......R.......R.$C",
                    /* line */ (const char *) "CC$..R.......R.....R.......R..$C",
                    /* line */ (const char *) "CC$...R.......R...R.......R...$C",
                    /* line */ (const char *) "CCC....R.......RgR.......R....CC",
                    /* line */ (const char *) "CCC.....R.......R.......R.....CC",
                    /* line */ (const char *) "CCCC.....R.....CCC.....R....CCCC",
                    /* line */ (const char *) "CCCCCC....R....CEC....R....CCCCC",
                    /* line */ (const char *) "CCCCCCCCC..R..gCCCg..R..CCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCC...CCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCC.z.CCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCC...CCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.6", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCC...CCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCC.@.CCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCC...CCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCC...CCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCC...CCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCC...CCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCC...............CCCCCCCC",
                    /* line */ (const char *) "CCCCCC.....................CCCCC",
                    /* line */ (const char *) "CCCC.........................CCC",
                    /* line */ (const char *) "CCC...........................CC",
                    /* line */ (const char *) "CCC.............R.............CC",
                    /* line */ (const char *) "CC.............RgR.............C",
                    /* line */ (const char *) "CC............R...R............C",
                    /* line */ (const char *) "CC...........R.....R...........C",
                    /* line */ (const char *) "CCC.........R.......R.........CC",
                    /* line */ (const char *) "CCC.........R.......R.........CC",
                    /* line */ (const char *) "CCC.........R.......R.........CC",
                    /* line */ (const char *) "CCCR........R.......R........RCC",
                    /* line */ (const char *) "CC$.R.......R.......R.......R.$C",
                    /* line */ (const char *) "CC$..R.......R.....R.......R..$C",
                    /* line */ (const char *) "CC$...R.......R...R.......R...$C",
                    /* line */ (const char *) "CCC....R.......RzR.......R....CC",
                    /* line */ (const char *) "CCC.....R.......R.......R.....CC",
                    /* line */ (const char *) "CCCC.....R.....CCC.....R....CCCC",
                    /* line */ (const char *) "CCCCCC....R....CEC....R....CCCCC",
                    /* line */ (const char *) "CCCCCCCCC..R..gCCCg..R..CCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.7", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCC...............CCCCCCCC",
                    /* line */ (const char *) "CCCCCC..........@..........CCCCC",
                    /* line */ (const char *) "CCCC.........................CCC",
                    /* line */ (const char *) "CCC...........................CC",
                    /* line */ (const char *) "CCC........CCC.....CCC........CC",
                    /* line */ (const char *) "CC........CCzCC...CCzCC........C",
                    /* line */ (const char *) "CC.........C.CC...CC.C.........C",
                    /* line */ (const char *) "CC.........CCCC...CCCC.........C",
                    /* line */ (const char *) "CCC.........CC.....CC.........CC",
                    /* line */ (const char *) "CCC...........................CC",
                    /* line */ (const char *) "CCC...........................CC",
                    /* line */ (const char *) "CCC...........................CC",
                    /* line */ (const char *) "CCCC.........................CCC",
                    /* line */ (const char *) "CCCC.........................CCC",
                    /* line */ (const char *) "CCCC.........................CCC",
                    /* line */ (const char *) "CCCC.........................CCC",
                    /* line */ (const char *) "CCCC.........................CCC",
                    /* line */ (const char *) "CCCC.........................CCC",
                    /* line */ (const char *) "CCCCC....CgCgCgCgCgCgCgC....CCCC",
                    /* line */ (const char *) "CCCCC......C$C$C$C$C$C......CCCC",
                    /* line */ (const char *) "CCCCCC.....................CCCCC",
                    /* line */ (const char *) "CCCCCCC.........E.........CCCCCC",
                    /* line */ (const char *) "CCCCCCCCCC.............CCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };
    overrides.o[ 'a' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("argusul"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.8", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC.$.CCCC.....CCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCaCCCCC......CCCC...CCCC.T.....CC",
                    /* line */ (const char *) "CCCC@.......CCCCCCCCC.$....CCCCCCCCCCC....g..CC",
                    /* line */ (const char *) "CCCC........CC...CCCC......CC......CCC.......CC",
                    /* line */ (const char *) "CCCC........CC.g..CCC......CC...$..CC........CC",
                    /* line */ (const char *) "CCCC......T.CC.....CC......CC......CC........CC",
                    /* line */ (const char *) "CCCC........CC.....CCC.....CC......CC........CC",
                    /* line */ (const char *) "CCCCCCCCCCCCCC......CCCC...CC......CC.....CCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCC...T..CCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "C..........CCCCC....CCCC...CCCCCCCCCCCCCCCC.zCC",
                    /* line */ (const char *) "C........g.CCCCCCCCCCCCC...CCCCCC.....CCCCa..CC",
                    /* line */ (const char *) "C..........CCCCCCCCCCCC.....CCCCC...T.CCCCCCCCC",
                    /* line */ (const char *) "CCCC....CCCCC......CCC........CCC.....CCCCCCCCC",
                    /* line */ (const char *) "C..........CC....$.CCC........CCC.............C",
                    /* line */ (const char *) "C.T........CC.......CC.........CC.............C",
                    /* line */ (const char *) "C..........CC.......CC..g......CC.............C",
                    /* line */ (const char *) "CCCCCCCCCCCCCC......CC.........CC.............C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCC.....CCCCCCCCCCCCC..........g..C",
                    /* line */ (const char *) "CaCCCCCC......CC....CCCCCCCCCCCCC.............C",
                    /* line */ (const char *) "CCCCCCCC...T..CCCCCCCCC....CCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCC...C.g....CCCCCCCCC.....CCCCCCCCCCCC......C",
                    /* line */ (const char *) "CCCC...C......CC.....CC......CC.......CC.g....C",
                    /* line */ (const char *) "CCCC...C.....CC...g..CC.T....CC.g.....CC......C",
                    /* line */ (const char *) "CCCCCCCC...CCCC......CC......CC.......CC....E.C",
                    /* line */ (const char *) "C$...CCCCCCCCCC......CC...CCCCC.....T.CC......C",
                    /* line */ (const char *) "C.$..CCCCCCCCCCC....CCCCCCCCCCC.......CCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'a' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("argusul"); };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.9", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CC$$CCC......CCCC...CCCC...CCCC..C",
                    /* line */ (const char *) "CC..===......====.g.====...====..C",
                    /* line */ (const char *) "CC$$CCC......CCCC...CCCC...CCCC..C",
                    /* line */ (const char *) "CCCCCCCCC=CCCCCCCCCCCCCCCCCCCC...C",
                    /* line */ (const char *) "CCCCCCCCC=CCCCCCCCCCCCCCCCCCCC...C",
                    /* line */ (const char *) "CCCCCCCCC=CCCCCCCCCCCCCCC...==...C",
                    /* line */ (const char *) "CC...CCC...CC..CC.....CCC...CCCCCC",
                    /* line */ (const char *) "CC...===...CC..==...g.===...CC.zCC",
                    /* line */ (const char *) "CC...CCC...CCCCCC.....CCCCCCCC..CC",
                    /* line */ (const char *) "CCCCCCCCC=CCCCCCCCC...CCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCC=CCC...CCC...CCC...CCC..C",
                    /* line */ (const char *) "CCCCCCCCC=CCC.g.CCC...CCC.g.===..C",
                    /* line */ (const char *) "C...CCCC...CC...CCCC.CCCC...CCC..C",
                    /* line */ (const char *) "C.@.====.g.CCC=CCCCC.CCCCCCCCCC.EC",
                    /* line */ (const char *) "C...CCCC...CCC=CCCCC.CCCCCCCCCC..C",
                    /* line */ (const char *) "CCCCCCCCC=CCCC=CCCC...CCC....CC..C",
                    /* line */ (const char *) "CCCCCCCCC=CCC..====.g.===..g.CC..C",
                    /* line */ (const char *) "CCCCCCCCC=CCC..CCCC...CCC....CCCCC",
                    /* line */ (const char *) "CCCCCCCCC=CCCC=CCCCC=CCCCCCCCCCCCC",
                    /* line */ (const char *) "CC...CCC...CCC...CCC=CCCCCCCCCCCCC",
                    /* line */ (const char *) "CC...===...CCC.g.==...CCCCC.....CC",
                    /* line */ (const char *) "CC...CCC...CCC...CC.g.=====..g..CC",
                    /* line */ (const char *) "CCC=CCCCC=CCCCCCCCC...CCCCC.....CC",
                    /* line */ (const char *) "CCC=CCCCC=CCCCCCCCCC=CCCCCCCCC=CCC",
                    /* line */ (const char *) "CCC=CCCC....CCCCCCC....CCCCCCC=CCC",
                    /* line */ (const char *) "CCC=CCCC....=======....CCCCCCC=CCC",
                    /* line */ (const char *) "CCC=CCCCCC..CCCCCCCCCCCCCCCCCC=CCC",
                    /* line */ (const char *) "CC......CCCCCCCCC...CCCCCCCC....CC",
                    /* line */ (const char *) "CC...g..=========...========.g$$CC",
                    /* line */ (const char *) "CC......CCCCCCCCC...CCCCCCCC..$$CC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("argusul"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.10", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CC..CCC......CCCC...CCCC...CCCC..C",
                    /* line */ (const char *) "CCg.===......====.g.====...====..C",
                    /* line */ (const char *) "CC..CCC......CCCC...CCCC...CCCC..C",
                    /* line */ (const char *) "CCCCCCCCC=CCCCCCCCCCCCCCCCCCCC...C",
                    /* line */ (const char *) "CCCCCCCCC=CCCCCCCCCCCCCCCCCCCC...C",
                    /* line */ (const char *) "CCCCCCCCC=CCCCCCCCCCCCCCC...==...C",
                    /* line */ (const char *) "CC...CCC...CC$$CC.....CCC...CCCCCC",
                    /* line */ (const char *) "CC...===...CC$$CC...g.CCC...CCz.CC",
                    /* line */ (const char *) "CC...CCC...CCCCCC.....CCCC=CCC..CC",
                    /* line */ (const char *) "CCC=CCCCC=CCCCCCCCC...CCCC=CCCCCCC",
                    /* line */ (const char *) "CCC=CCCCC=CCC...CCC...CCC...CCC..C",
                    /* line */ (const char *) "CCC=CCCCC=CCC.g.CCC...CCC.g.===..C",
                    /* line */ (const char *) "C...CCCC...CC...CCCC.CCCC...CCC..C",
                    /* line */ (const char *) "C.@.====.g.CCC=CCCCC.CCCCCCCCCC.EC",
                    /* line */ (const char *) "C...CCCC...CCC=CCCCC.CCCCCCCCCC..C",
                    /* line */ (const char *) "CCC=CCCCC=CCCC=CCCC...CCC....CC..C",
                    /* line */ (const char *) "CCC=CCCCC=CCC..====.g.===..g.CC..C",
                    /* line */ (const char *) "CCC=CCCCC=CCC..CCCC...CCC....CCCCC",
                    /* line */ (const char *) "CCC=CCCCC=CCCC=CCCCC=CCCCCCC=CCCCC",
                    /* line */ (const char *) "CC...CCC...CCC...CCC=CCCCCCC=CCCCC",
                    /* line */ (const char *) "CC...===...CCC.g.==...CCCCC.....CC",
                    /* line */ (const char *) "CC...CCC...CCC...CC.g.=====..g..CC",
                    /* line */ (const char *) "CCC=CCCCC=CCCCCCCCC...CCCCC.....CC",
                    /* line */ (const char *) "CCC=CCCCC=CCCCCCCCCC=CCCCCCCCC=CCC",
                    /* line */ (const char *) "CCC=CCCC....CCCCCCC....CCCCCCC=CCC",
                    /* line */ (const char *) "CCC=CCCC....=======....CCCCCCC=CCC",
                    /* line */ (const char *) "CCC=CCCCCC..CCCCCCCCCCCCCCCCCC=CCC",
                    /* line */ (const char *) "CC......CCCCCCCCC...CCCCCCCC....CC",
                    /* line */ (const char *) "CC...g..=========...========.g..CC",
                    /* line */ (const char *) "CC......CCCCCCCCC...CCCCCCCC....CC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };
    overrides.o[ 'k' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.11", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CC@.$CCCC$.........CCCCEk$CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CC..gCCCC..gCCCC..gCCCC..gCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC...CCCC...CCCC...CCCC...CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CC..gCCCC..gCCCC..gCCCC..gCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC...CCCC...CCCC...CCCC...CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CC..gCCCC..gCCCC..gCCCC..gCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC........CCCCCC..........CC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCC...CCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCC.z.CCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCC...CCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("argusul"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.12", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCRCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CC@.$CCCC$.........CCCzE.$CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CCCC.CCRCCC.CCCCCC.CCCRCC.CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CC..gCCCC..gCCCC..gCCCC..gCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC.CCRCCC.CCRCCC.CCRCCC.CRCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC...CCCC...CCCC...CCCC...CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CCCC.CRCCCC.CRCCCC.CCRCCC.CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CC..gCCCC..gCCCC..gCCCC..gCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCRCCC.CCRC",
                    /* line */ (const char *) "CC.CCCRCC.CCCCCC.CCCCRC.CCCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC...CCCC...CCCC...CCCC...CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CRCC.CCRCCC.CCRCCC.CCRCCC.CC",
                    /* line */ (const char *) "CCCC.CCCCCC.CCCCCC.CCCCCC.CC",
                    /* line */ (const char *) "CC..gCCCC..gCCCC..gCCCC..gCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCRCC.CCRCCC.CCCC",
                    /* line */ (const char *) "CC.CCCCCC.CCCCCC.CCCCCC.CCCC",
                    /* line */ (const char *) "CC$......$CCCCCC.........$CC",
                    /* line */ (const char *) "CCCCCCCCCCCRCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.13", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCC",
                    /* line */ (const char *) "C.CCC$===$CCCEC",
                    /* line */ (const char *) "C.CCC.CCC.===.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.===.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.===.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCCgC",
                    /* line */ (const char *) "C.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCCgC",
                    /* line */ (const char *) "C.CCC.===.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCCgC",
                    /* line */ (const char *) "C.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCCgC",
                    /* line */ (const char *) "C.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC$C",
                    /* line */ (const char *) "C@CCC.===.CCCzC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("argusul"); };
    overrides.o[ 'f' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skullferno"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.14", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "C.CCC$===$CCCCCC.CRC$===$CCCEC",
                    /* line */ (const char *) "C.CCC.CCC.===fCC.CCC.CCC.===fC",
                    /* line */ (const char *) "C.CCC.CCC.CCC.CC.CRC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC.CC.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC.CC.CRC.CCC.CCC.C",
                    /* line */ (const char *) "C.===.CCC.CCC.CC.===.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC.CC.CRC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.===.CC.CCC.CCC.===.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC.CC.CRC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC.CC.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCCgCC.CRC.CCC.CCCgC",
                    /* line */ (const char *) "C.CCC.CCC.CCC.CC.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCCgCC.CRC.CCC.CCCgC",
                    /* line */ (const char *) "C.CCC.===.CCC.CC.CCC.===.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCCgCC.CRC.CCC.CCCgC",
                    /* line */ (const char *) "C.CCC.CCC.CCC.CC.CCC.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCCgCC.CRC.CCC.CCCgC",
                    /* line */ (const char *) "C.CCC.CCC.CCC.CC.===.CCC.CCC.C",
                    /* line */ (const char *) "C.CCC.CCC.CCC$CC$CRC.CCC.CCC$C",
                    /* line */ (const char *) "C@CCC.===.CCC$CC$CCC.===.CCCzC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'f' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skullferno"); };
    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE3, "treasure3.15", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCC$$$$CCCCCCCC",
                    /* line */ (const char *) "CCCCC.....===========.......CCCCCCCC",
                    /* line */ (const char *) "CCCCC.CCCCCCCCCCCCCCCCCC....CCCCCCCC",
                    /* line */ (const char *) "CCCCC.CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCC...CCCCCCCCCCCCCCCCCfCCCCCCCCCCC",
                    /* line */ (const char *) "CCCC.g......................CCCCCCCC",
                    /* line */ (const char *) "CCCC...CCCCCCCCCCCCCCCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCCCC=CCCCCCCCCCCCCCCCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCCCC=CCCCCCCCC....CCCCCCCC=CCCCCCCC",
                    /* line */ (const char *) "CCCCC=CCCCCCCCC.zz.CCCCCCCC=CCCCCCCC",
                    /* line */ (const char *) "CCCC...CCCCCCCC....CCCCCCCC=CCCCCCCC",
                    /* line */ (const char *) "CCCC...CCCCCCCCCCCCCCCCCCCC=CCCCCCCC",
                    /* line */ (const char *) "CCCC$$$CCCCCCCCCCCCCCCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCCCCCfCCCCCCCCCC...fCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCC...CCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCCCCCC...CCCCCCC...CCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "C@......C....C....C....C......====.E",
                    /* line */ (const char *) "CCC.CCCCCCCC...CCCCCCC...CC.CCCCCCCC",
                    /* line */ (const char *) "CCC.CCCCCCCCCCCCCCCCCCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCC.CCfCCCCCCCCCCCCCCCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCC.CCCCCCCCCCCCCCCCCCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCC.CCCCCCCCCCCCCCCCCCCCfCC.CCCCCCCC",
                    /* line */ (const char *) "CCC.CCCCCCCCCfCCCCCCCCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCC.CCCCCCCCCCCCCCCCCCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCC=CCCCCCCCCCCCCCCCCCCCCCC.CCCCCCCC",
                    /* line */ (const char *) "CCC=CCCCCCCCC...CCCCCCCCCC...CCCCCCC",
                    /* line */ (const char *) "CCC=CCCCCCCCC.z..CCCCCCCCC.g.CCCCCCC",
                    /* line */ (const char *) "CCC.CCCCCCCCC..z.CCCCCCCCC...CCCCCCC",
                    /* line */ (const char *) "CCC.CCCCCCCCCC...CCCCCCCCCC=CCCCCCCC",
                    /* line */ (const char *) "CCC.CCCCCCCCCCCCCCCCCCCCfCC=CCCCCCCC",
                    /* line */ (const char *) "CCC.CCCCCCCCCCCCCCCCCCCCCCC=CCCCCCCC",
                    /* line */ (const char *) "CCC.CCCCCCCCCCCCCCCCCCCCCCC=CCCCCCCC",
                    /* line */ (const char *) "CCC.CCCCCCCCCCCCCCCCCCCCCCC=CCCCCCCC",
                    /* line */ (const char *) "CCC....CCCCCCCCCCCCCCCCCCC...CCCCCCC",
                    /* line */ (const char *) "CCC..$$CCCCCCCCCCCCCCCCCCC...CCCCCCC",
                    /* line */ (const char *) "CCC..$$CCCCCCCCCCCCCCCCCCC$$$CCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE4, "treasure4.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$,,,,,,,,,`,,,,~,,$,,,,,,,,,,~x",
                    /* line */ (const char *) "x,,,g`````,,``,~~,,```,,``,,,,sx",
                    /* line */ (const char *) "x,,``..``.`.`.`~.`.`.`..`.`..`,x",
                    /* line */ (const char *) "x,``.`.`.`.`.~~`.`.`..g.`..``,,x",
                    /* line */ (const char *) "x,,```````,,~`,`,,,```,,``,,C,,x",
                    /* line */ (const char *) "x,,,,`.`..`.~.`..g.````..CCC,,sx",
                    /* line */ (const char *) "x,,,````.`.~.`..`.`.`.`.C``,,,,x",
                    /* line */ (const char *) "xCCCCCCCCC`.``.`.`.`.`CC``,,,,,x",
                    /* line */ (const char *) "x,,@,,,,,CC,,CCCCCC,,CC,,,,,E,,x",
                    /* line */ (const char *) "x,,,,,,``CCCCCC.`zCCCC`.`.`,,,,x",
                    /* line */ (const char *) "x,,,,`````,,``CCCCC``C,,``,,,,sx",
                    /* line */ (const char *) "x,,``..`~.`.`.`..`.`.`C.`.`..`,x",
                    /* line */ (const char *) "x,``.`.`~`.g.`.`.`.`..C.`..``,,x",
                    /* line */ (const char *) "x,,````~``,,``,`,,,```,C``,g,,,x",
                    /* line */ (const char *) "x,,,,`.~..`.`.`..g.````.C`.`,,sx",
                    /* line */ (const char *) "x,,,``~`.`.`.`..`.`.`.`C.C`,,,,x",
                    /* line */ (const char *) "x,,,,,~``.`.`.`..`.`.`C``C,,,,,x",
                    /* line */ (const char *) "x,,,,g~,,$,,,`````$`.C.`..C`.$,x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE4, "treasure4.2", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xz,,,,,,,,,`,,,,C,,$,,,,,,,,,,~x",
                    /* line */ (const char *) "x,,,g`````,,``,CC,,```,,``,,,,sx",
                    /* line */ (const char *) "x,,``..``.`.`.`C.`.`.`..`.`..`,x",
                    /* line */ (const char *) "x,``.`.`.`.`.CC`.`.`..g.`..``,,x",
                    /* line */ (const char *) "x,,```````,CCC,`,,,```,,``,,C,,x",
                    /* line */ (const char *) "x,,,,`.`..CCC.`..g.````..CCC,,sx",
                    /* line */ (const char *) "xCCCCCCCCCCC.`..`.`.`.`.C``,,,,x",
                    /* line */ (const char *) "xCCCCCCCCCC.``.`.`.`.`CC``,,,,,x",
                    /* line */ (const char *) "x,,@,,,CCCC,,`,`,,,,,CC,,,,,E,,x",
                    /* line */ (const char *) "x,,,,,,`CCCCCCC.`.CCCC`.`.`,,,,x",
                    /* line */ (const char *) "x,,,,```CC,,``CCCCC``C,,``,,,,sx",
                    /* line */ (const char *) "x,,``..`CC`.`.`..`.`.`C.`.`..`,x",
                    /* line */ (const char *) "x,``.`.`C`.g.`.`.`.`..C.`..``,,x",
                    /* line */ (const char *) "x,,````C``,,``,`,,,```,C``,g,,,x",
                    /* line */ (const char *) "x,,,,`.C..`.`.`..g.````.C`.`,,sx",
                    /* line */ (const char *) "x,,,``C`.`.`.`..`.`.`.`C.C`,,,,x",
                    /* line */ (const char *) "x,,,,,C``.`.`.`..`.`.`C``C,,,,,x",
                    /* line */ (const char *) "x,,,,gC,,$,,,`````$`.C.`..C`.$,x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE4, "treasure4.3", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$,,,,,,,,,`,,,,~,,$,,,,,,,,,,zx",
                    /* line */ (const char *) "x,,,g`````,,``,~~,,```,,``,,,,sx",
                    /* line */ (const char *) "x,,``..``.`.`.`~.`.`.`..`.`..`,x",
                    /* line */ (const char *) "x,``.`.`.`.`.~~`.`.`..g.`..``,,x",
                    /* line */ (const char *) "x,,```````,,~`,`,,,```,,``,,~,,x",
                    /* line */ (const char *) "x,,,,`.`..`.~.`..g.````..~~~,,sx",
                    /* line */ (const char *) "x,,,````.`.~.`..`.`.`.`.~``,,,,x",
                    /* line */ (const char *) "x~~~~~~~~~`.``.`.`.`.`~~``,,,,,x",
                    /* line */ (const char *) "x,,@,,,,,~~,,`,`,,,,,~~,,,,,E,,x",
                    /* line */ (const char *) "x,,,,,,``~~~~~~.`.~~~~`.`.`,,,,x",
                    /* line */ (const char *) "x,,,,`````,,``~~~~~``~,,``,,,,sx",
                    /* line */ (const char *) "x,,``..`~.`.`.`..`.`.`~.`.`..`,x",
                    /* line */ (const char *) "x,``.`.`~`.g.`.`.`.`..~.`..``,,x",
                    /* line */ (const char *) "x,,````~``,,``,`,,,```,~``,g,,,x",
                    /* line */ (const char *) "x,,,,`.~..`.`.`..g.````.~`.`,,sx",
                    /* line */ (const char *) "x,,,``~`.`.`.`..`.`.`.`~.~`,,,,x",
                    /* line */ (const char *) "x,,,,,~``.`.`.`..`.`.`~``~,,,,,x",
                    /* line */ (const char *) "x,,,,g~,,$,,,`````$`.~.`..~`.$,x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE4, "treasure4.4", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "            xxxxxxxx            ",
                    /* line */ (const char *) "            x$,gg`Ex            ",
                    /* line */ (const char *) "    xxxx    x`,```,x            ",
                    /* line */ (const char *) "    x.zx    x`````,x            ",
                    /* line */ (const char *) "    xxxx    x`,`,`,x            ",
                    /* line */ (const char *) "            x`,```,x      xxx   ",
                    /* line */ (const char *) "            xCCCCCCx      xzx   ",
                    /* line */ (const char *) "xxxxxxxxxxxxx`,`,`,xxxxxxxxsxxxx",
                    /* line */ (const char *) "x$,,,,,,,,,`,,,,~,,$,,,,,,,,,,~x",
                    /* line */ (const char *) "x,,,g`````,,``,~~,,```,,``,,,,sx",
                    /* line */ (const char *) "x,,,,,~``.`.`.`..`.`.`````,,,,,x",
                    /* line */ (const char *) "x,,,,g~,,$,,,`````$`.`.`..``.$,x",
                    /* line */ (const char *) "xxxxxxxxxxxxx`,`,`,xxxxxxxxxxxxx",
                    /* line */ (const char *) "            xCCCCCCx            ",
                    /* line */ (const char *) "            x`````,x            ",
                    /* line */ (const char *) "            x`````,x            ",
                    /* line */ (const char *) "            x```,`,x            ",
                    /* line */ (const char *) "            x`````,x            ",
                    /* line */ (const char *) "            x`,```,x            ",
                    /* line */ (const char *) "            x```,`,x            ",
                    /* line */ (const char *) "            x`````,x            ",
                    /* line */ (const char *) "            x`,```,x            ",
                    /* line */ (const char *) "            x`````,x            ",
                    /* line */ (const char *) "            x`,@,`,x            ",
                    /* line */ (const char *) "            x`,`,`,x            ",
                    /* line */ (const char *) "            xxxxxxxx            ",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE4, "treasure4.5", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "            xxxxxxxx            ",
                    /* line */ (const char *) "            x$,gg`Ex            ",
                    /* line */ (const char *) "            x`,```,x            ",
                    /* line */ (const char *) "            x`,```,x            ",
                    /* line */ (const char *) "            xCCCCCCx            ",
                    /* line */ (const char *) "xxxxxxxxxxxxx`,`,`,xxxxxxxxxxxxx",
                    /* line */ (const char *) "x$,,,,,,,,,`,,,,~,,$,,,,,,,,,,~x",
                    /* line */ (const char *) "x,,,g`````,,``,~~,,```,,``,,,,sx",
                    /* line */ (const char *) "x,,,,,~``.`.`.`..`.`.`````,,,,,x",
                    /* line */ (const char *) "x,,,,g~,,$,,,`````$`.`.`..``.$,x",
                    /* line */ (const char *) "xsxxxxxxxxxxx`,`,`,xxxxxxxxxxxxx",
                    /* line */ (const char *) "xzx         xCCCCCCx            ",
                    /* line */ (const char *) "xxxxxxxxxxxxx`,`,`,xxxxxxxxxxxxx",
                    /* line */ (const char *) "x$,,,,,,,,,`,,,,~,,$,,,,,,,,,,~x",
                    /* line */ (const char *) "x,,,g`````,,``,~~,,```,,``,,,,sx",
                    /* line */ (const char *) "x,,,,,~``.`.`.`..`.`.`````,,,,,x",
                    /* line */ (const char *) "x,,,,g~,,$,,,`````$`.`.`..``.$,x",
                    /* line */ (const char *) "xxxxxxxxxxxxx`,`,`,xxxxxxxxxxxxx",
                    /* line */ (const char *) "            xCCCCCCx            ",
                    /* line */ (const char *) "            x`````,x            ",
                    /* line */ (const char *) "            x`,@,`,x            ",
                    /* line */ (const char *) "            x`,`,`,x            ",
                    /* line */ (const char *) "            xxxxxxxx            ",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE4, "treasure4.6", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "xz,C,,,,,,,`,,,gEg,$,,,,,,,C,,$`x",
                    /* line */ (const char *) "x,C,,`````,,``,``,,```,,``,,C,``x",
                    /* line */ (const char *) "xC,``..``.`.`.``.`.`.`..`.`..C,`x",
                    /* line */ (const char *) "x,``.`.`.`.`.```.`.`..`.`..``,C`x",
                    /* line */ (const char *) "x,,```````,,``,`,,,```,,``,,`,,`x",
                    /* line */ (const char *) "x,,,,`.`..`.`.`..`.````..```,,``x",
                    /* line */ (const char *) "x,,,````.`.`.`..`.`.`.`.```,,,,`x",
                    /* line */ (const char *) "x````````s,s,s,s,s,s,s,s````````x",
                    /* line */ (const char *) "x`````,,,,,,,,,,,,,,,,,,,,,`````x",
                    /* line */ (const char *) "x```,,,,,,,,,,,,,,,,,,,,,,,,,```x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x```,,,,,,,,,,,,,,,,,,,,,,,,````x",
                    /* line */ (const char *) "x`````,,,,,,,,,,,,,,,,,,,,,`````x",
                    /* line */ (const char *) "x````````,,,,,`````,,,,,`````C``x",
                    /* line */ (const char *) "x``C``,````,`R,`,`,R,```,`,,,,,`x",
                    /* line */ (const char *) "x``,`,`,`,`,,`,`,`,`,`,```C,,,,`x",
                    /* line */ (const char *) "x`````C`,`,`,R,`,`,R,```,`,`,`,`x",
                    /* line */ (const char *) "x``,```````,`,`,`,`,`,````,,,C,`x",
                    /* line */ (const char *) "x$``````C`,`,R,`@`,R,`````C,,,,`x",
                    /* line */ (const char *) "x``C```````,``,`,`,`,`,`,`,,,,$`x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE4, "treasure4.7", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$,C,,,,,,,`,,,gEg,$,,,,,,,C,,z`x",
                    /* line */ (const char *) "x,C,,`````,,``,``,,```,,``,,C,``x",
                    /* line */ (const char *) "xC,``..``.`.`.``.`.`.`..`.`..C,`x",
                    /* line */ (const char *) "x,``.`.`.`.`.```.`.`..`.`..``,C`x",
                    /* line */ (const char *) "x,,```````,,``,`,,,```,,``,,`,,`x",
                    /* line */ (const char *) "x,,,,`.`..`.`.`..`.````..```,,``x",
                    /* line */ (const char *) "x,,,````.`.`.`..`.`.`.`.```,,,,`x",
                    /* line */ (const char *) "x````````s,s,s,s,s,s,s,s````````x",
                    /* line */ (const char *) "x`````,,,,,,,,,,,,,,,,,,,,,`````x",
                    /* line */ (const char *) "x```,,,,,,,,,,,,,,,,,,,,,,,,,```x",
                    /* line */ (const char *) "x``,,,,,,,,,```,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,```,,,,,```,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,```,,,,,,,``x",
                    /* line */ (const char *) "x``CCCCCC,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,CCCCCC,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,CCCCCC,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,CCCCCC,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,CCCCCC,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,CCCCCC,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,CCC``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x``,,,,,,,,,,,,,,,,,,,,,,,,,,,``x",
                    /* line */ (const char *) "x```,,,,,,,,,,,,,,,,,,,,,,,,````x",
                    /* line */ (const char *) "x`````,,,,,,,,,,,,,,,,,,,,,`````x",
                    /* line */ (const char *) "x````````,,,,,`````,,,,,`````C``x",
                    /* line */ (const char *) "x``C```````,`R,`,`,R,``,``,,,,,`x",
                    /* line */ (const char *) "x``,`,`,`,`,,`,`,`,`,`````C,,,,`x",
                    /* line */ (const char *) "x`````C`,`,`,R,`,`,R,`,`,`,`,`,`x",
                    /* line */ (const char *) "x``````````,`,`,`,`,`,````,,,C,`x",
                    /* line */ (const char *) "x$``````C`,`,R,`@`,R,`````C,,,,`x",
                    /* line */ (const char *) "x``C```````,``,`,`,`,`,`,`,,,,$`x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE4, "treasure4.8", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$,,,,,,,,,`,,,z~,,$,,,,,,,,,,~x",
                    /* line */ (const char *) "x,,,g`````,,``,~~,,```,,``,,,,sx",
                    /* line */ (const char *) "x,,``..``.`.`.`~.`.`.`..`.`E.`,x",
                    /* line */ (const char *) "x,``.`.`.`.`.~~`.`.`..g.`..``,,x",
                    /* line */ (const char *) "x,,```````,,~`,`,,,```,,``,,C,,x",
                    /* line */ (const char *) "x,,,,`.`..`.~.`..g.````..CCC,,sx",
                    /* line */ (const char *) "x,,,````.`.~.`..`.`.`.`.C``,,,,x",
                    /* line */ (const char *) "xCCCCCCCCC`.``.`.`.`.`CC``,,,,,x",
                    /* line */ (const char *) "x,,,,,,,,CC,,`,`,,,,,CC,,,,,,,,x",
                    /* line */ (const char *) "x,,,,,,``CCCCCC.`.CCCC`.`.`,,,,x",
                    /* line */ (const char *) "x,,,,`````,,``CCCCC``C,,``,,,,sx",
                    /* line */ (const char *) "x,,``..`~.`.`.`..`.`.`C.`.`..`,x",
                    /* line */ (const char *) "x,``.`.`~`.g.`.`.`.`..C.`..``,,x",
                    /* line */ (const char *) "x,``.`.`.`.`.~~`.`.`..g.`..``,,x",
                    /* line */ (const char *) "x,,```````,,~`,`,,,```,,``,,C,,x",
                    /* line */ (const char *) "x,,,,`.`..`.~.`..g.````..CCC,,sx",
                    /* line */ (const char *) "x,,,````.`.~.`..`.`.`.`.C``,,,,x",
                    /* line */ (const char *) "x,,,,,,,,CC,,`,`,,,,,CC,,,,,E,,x",
                    /* line */ (const char *) "x,,``..`~.`.`.`.C`.`.`C.`.`..`,x",
                    /* line */ (const char *) "x,``.`.`~`.g.`.`CCC`..C.`..``,,x",
                    /* line */ (const char *) "x,,````~``,,``CCCCCC``,C``,g,,,x",
                    /* line */ (const char *) "x,,,,`.~..`.`.CC.`.C```.C`.`,,sx",
                    /* line */ (const char *) "x,,,``~`.`.`.CC.`.`CC.`C.C`,,,,x",
                    /* line */ (const char *) "x,,,,,~``.`.CCC.@`.CC`C``C,,,,,x",
                    /* line */ (const char *) "x,,,,g~,,$,CCCC```$CCCC`..C`.$,x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE4, "treasure4.9", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$,,,,,,,,,`,,,,~E,$,,,,,,,,,,~x",
                    /* line */ (const char *) "x,,,g`````,,``,~~,,```,,``,,,,sx",
                    /* line */ (const char *) "x,,``..``.`.`.`~.`.`.`..`.`..`,x",
                    /* line */ (const char *) "x,``.`.`.`.`.~~`.`.`..g.`..``,,x",
                    /* line */ (const char *) "x,,```````,,~`,`,,,```,,``,,C,,x",
                    /* line */ (const char *) "x,,,,`.`..`.~.`..g.````..CCC,,sx",
                    /* line */ (const char *) "x,,,````.`.~.`..`.`.`.`.C``,,,,x",
                    /* line */ (const char *) "x,,$`..`~.`.`.`..`.`.`C.`.`.$`,x",
                    /* line */ (const char *) "x,``.`.`~`.g.`.`.`.`..C.`z.``,,x",
                    /* line */ (const char *) "x,,````~``,,``,`,,,```,C``,g,,,x",
                    /* line */ (const char *) "x,,,,`.~..`.`.`..g.````.C`.`,,sx",
                    /* line */ (const char *) "x,,,``~`.`.`.`..`.`.`.`C.C`,,,,x",
                    /* line */ (const char *) "x,$,,,~``.`.`.`.@`.`.`C``C,,,,,x",
                    /* line */ (const char *) "x,,,,g~,,$,,,`````$`.C.`..C`.$,x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE4, "treasure4.10", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x$$,g`````,,``,C,C,```,,``,,,$sx",
                    /* line */ (const char *) "x$,,,,,,,,,`,,,CEC,$,,,,,,,,,,$x",
                    /* line */ (const char *) "x,,,g`````,,``,C,C,```,,``,,,,sx",
                    /* line */ (const char *) "x,,``..``.`.`.`C.C.`.`..`.`..`,x",
                    /* line */ (const char *) "x,``.`.`.`.`.,,C.C.`..g.`..``,,x",
                    /* line */ (const char *) "x,,```````,,,`CC,CC```,,``,,C,,x",
                    /* line */ (const char *) "x,,z,`.$..`.,.CC.CC````..CCC,,sx",
                    /* line */ (const char *) "x,,```````,,,`CC,CC```,,``,,C,,x",
                    /* line */ (const char *) "x,,,````.`.,.`CCCCC.`.`.C``,,,,x",
                    /* line */ (const char *) "xCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCx",
                    /* line */ (const char *) "xCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCx",
                    /* line */ (const char *) "xCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCx",
                    /* line */ (const char *) "xCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCx",
                    /* line */ (const char *) "xCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCx",
                    /* line */ (const char *) "xCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCx",
                    /* line */ (const char *) "x,,,,`````,,``CCCCC``C,,``,,,,sx",
                    /* line */ (const char *) "x,,``..`,.`.`.`..`.`.`C.`.`..`,x",
                    /* line */ (const char *) "x,``.$.`,`.g.`.`.`.`..C.`..``,,x",
                    /* line */ (const char *) "x,,````,``,,``,`,,,```,C``,g,,,x",
                    /* line */ (const char *) "x,,,,`.,..`.`.`..`.````.C`.`,,sx",
                    /* line */ (const char *) "x,,,``,`.`.`.`..`.`.`.`C.C`,,,,x",
                    /* line */ (const char *) "x,,,,,,``.`.`.`.@`.`.`C``C,,,,,x",
                    /* line */ (const char *) "x,,,,g,,,$,,,`````$`.C.`..C`.$,x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };
    overrides.o[ 'o' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("ogrik"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE5, "treasure5.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "RRRLLLLLRRLLLLLLLLLRRRRRLLLLLLLLLRRRLLRR",
                    /* line */ (const char *) "RRRLRLLLLLLLLLLLRLLLLLLLLLLzLLLRLLLLLLRR",
                    /* line */ (const char *) "RLLLLLLLzLRLLLLLLLLLLLLLRLLLLLLLLLLLRLLR",
                    /* line */ (const char *) "RLLRLLLLLLLLLLLLLLLLLLLLLLLLLRLLLLLLLLLR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RRLLL$....$L........BLB....LLL.....LLLLR",
                    /* line */ (const char *) "RRLLL..@...L.....g...L......L...g..LLLLR",
                    /* line */ (const char *) "RLLLL.....LL........LL.....BLB.....LLLLR",
                    /* line */ (const char *) "RLLRL..B.LLLL.B....LLL......LL...B.LRLLR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLLLL......LLLLLLLLLLLR",
                    /* line */ (const char *) "RLLLL..B...LL.BLL...LL..B..LLL...BLLLLLR",
                    /* line */ (const char *) "RLRLL......L...L.....LLLLLLLL......LLLLR",
                    /* line */ (const char *) "RLLLL.....BLB..L..g..LL.B..LL...g..LLRRR",
                    /* line */ (const char *) "RLLRL.B....L...L.....L......L......LLLRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLL..o..L.....BLB..o..LLLLR",
                    /* line */ (const char *) "RRLRL.B...LLL..L.....L..g...L......LLRLR",
                    /* line */ (const char *) "RRLLL.........BLB....L.....LL......LRLLR",
                    /* line */ (const char *) "RLLLLL...g.....L.....LLLLLLLLL.....LLLLR",
                    /* line */ (const char *) "RLLLLL........LL.....LL....LLL..B..LRLRR",
                    /* line */ (const char *) "RLRLLLLLLLLLLLLLL...BLB.....LLLLLLLLLLRR",
                    /* line */ (const char *) "RLLLLLL.......LLLL...L......LL..B.LLLLRR",
                    /* line */ (const char *) "RLLLLL....g....LLLLLLL......L......LLLLR",
                    /* line */ (const char *) "RLLLL...g...g..L..$$$LB....BLB.....LRRLR",
                    /* line */ (const char *) "RRLLL.........BLB.$$$LLLLLLLL......LLLLR",
                    /* line */ (const char *) "RRLRL.....B....L..$$$LB....LLL..g..LLRRR",
                    /* line */ (const char *) "RRLLLLLLLLLLLLLLLLLLLL......L......LLLRR",
                    /* line */ (const char *) "RLLLLL....B...LLL...LL.....BLB.....LLLRR",
                    /* line */ (const char *) "RLLLL...............BLB.....L......LRLLR",
                    /* line */ (const char *) "RLLLL..g.............L......L.....LLLLLR",
                    /* line */ (const char *) "RLRLLL.......B..LL..LL....LLLL..B.LLLLLR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLRLRR",
                    /* line */ (const char *) "RLLLLL...L......LLL......LLL....B.LLLLLR",
                    /* line */ (const char *) "RRLRL.o.BLB......L..g....BLB.......LLLLR",
                    /* line */ (const char *) "RRLLL....L...g..BLB.......L...g....LLRRR",
                    /* line */ (const char *) "RLLLLE..LLL.....LLL......LLL......LLLLLR",
                    /* line */ (const char *) "RRLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLRLR",
                    /* line */ (const char *) "RRLRLLLLLLLRLLLRLLLLLLLLLLLLLLLLLLLLRLLR",
                    /* line */ (const char *) "RRLLRLLLLRLLLLLLLLLLLLRLLLRLLLLLRLLLRLLR",
                    /* line */ (const char *) "RLLLLLLLLLRRLLLRLLLLLRLLzLLLLRLLLLLRR$$R",
                    /* line */ (const char *) "RRLRRLLLLLLLLLLRRLLLLLLLLLLLRRRLLLLR$$$R",
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };
    overrides.o[ 'o' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("ogrik"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE5, "treasure5.2", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "RRRLLLLLRRLLLLLLLLLRRRRRRRRRRRRRLLLLLLLRRRLLRR",
                    /* line */ (const char *) "RRRLRLLLLLLLLLLLRLLLLLLLLLLLLLLLLLLLLRLLLLLLRR",
                    /* line */ (const char *) "RLLLLLLLLLRLLLLLLLLLLLLLzLLLLLRLLLLLLLLLLLRLLR",
                    /* line */ (const char *) "RLLRLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLRLLLLLLLLLR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RRLLL$,,,,,R,,,,,,,,,R,,,,,,,,,,,,,,,,o,BLLLLR",
                    /* line */ (const char *) "RRLLL,@,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,o,ELLL,R",
                    /* line */ (const char *) "RLLLL$,,,,,,,,,,R,,,,,,,,,,,,,,,,,,,,,o,BLLL,R",
                    /* line */ (const char *) "RRLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLR,R",
                    /* line */ (const char *) "RRRLRLLLLLLLLLLLRLLLLLLLLLLLLLLLLLLLLRLLLLLR,R",
                    /* line */ (const char *) "RLLLLLLLLLRLLLLLLLLzLLLLLLLLLLRLLLLLLLLLLLRR,R",
                    /* line */ (const char *) "RLLRLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLRLLLLLLR$,R",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLR$$R",
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };
    overrides.o[ 'o' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("ogrik"); };
    overrides.o[ 'f' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skullferno"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE5, "treasure5.3", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "RRRLLLLLRRLLLLLLLLLRRRRRRRRRRRRRLLLLLLLRRRLLRR",
                    /* line */ (const char *) "RRRLRLLLLLLLLLLLRLLLLLLLLLLLLLLLLLLLLRLLLLLLRR",
                    /* line */ (const char *) "RLLLLLLLLLRLLLLLLLLLLLLLLLLLLLRLLLLLLLLLLLRLLR",
                    /* line */ (const char *) "RLLRLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLRLLLLLLLLLR",
                    /* line */ (const char *) "RLLLLLLLLLLLLzLLLLLL$,,,,,$LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,R,,,,,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,,,,R,,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,,,,,,,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,,,,,R,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RRLLLB,,,,,R,,,,,,,,,R,,,,,,,,,,,,,,,,o,,LLLLR",
                    /* line */ (const char *) "RRLLL,@,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,o,,LLLLR",
                    /* line */ (const char *) "RLLLL$,,,,,,,,,,R,,,,,,,,,,,,,,R,,,,,,o,,LLLLR",
                    /* line */ (const char *) "RLLLLB,,,,,,,,,,,,,,,,B,B,,,,,,,,,,,,,o,,LLLLR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,RR,RR,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,RR,RR,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,RR,RR,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,RR,RR,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,RR,RR,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLzLLLLLL,RR,RR,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,RR,RR,LLLLLLzLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,RR,RR,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,RR,RR,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,RR,RR,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,RR,RR,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,RR,RR,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLLoRRoRRoLLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLLoRRoRRoLLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RRRLRLLLLLLLLLLLRLLLLLLLLLLLLLLLLLLLLRLLLLLLLR",
                    /* line */ (const char *) "RRRLRLLLLLLLLLLLRLLLLLLLLLLLLLLLLLLLLRLLLLLLLR",
                    /* line */ (const char *) "RRRLRLLLLLLLLLLLRLLLLLL,LLLLLLLzLLL,,,LLLLLLLR",
                    /* line */ (const char *) "RRRLRLL,,,,LLLLLRLLLLLLLLLLLLLLLLL,,$RLLLLLLLR",
                    /* line */ (const char *) "RRRLRLLL,$$,LLLLRLLLLLLLLLLLLLLLLL,$,,LLLLLLLR",
                    /* line */ (const char *) "RRRLRLLLL,,,,LLLRLLLLLLLLLLLLLLLLLL,,,LLLLLLLR",
                    /* line */ (const char *) "RRLLLLLLLLLLLLLLLLLLB,,,,,BLLLLLLLLLLLLLLLLRLR",
                    /* line */ (const char *) "RLLLLLLLLLLLLLLLLLLL,,,E,,,LLLLLLLLLLLLLLLLRRR",
                    /* line */ (const char *) "RRLLLLLLLLLLLLLLLLLLB,,,,,BLLLLLLLLLLLLLLLLRLR",
                    /* line */ (const char *) "RRRLRLLLLLLLLLLLRLLLLLLLLLLLLLLLLLLLLRLLLLLLLR",
                    /* line */ (const char *) "RLLLLLLLLLRLLLLLLLLLLLLLLLLLLLRLLLLLLLLLLLRLLR",
                    /* line */ (const char *) "RLLRLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLRLLLLLLLLLR",
                    /* line */ (const char *) "RfLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLRfR",
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    LevelGenOverrides overrides;

    overrides.o[ 'g' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("bat_mob"); };
    overrides.o[ 's' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton_mob"); };
    overrides.o[ 'o' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("ogrik"); };
    overrides.o[ 'f' ].get_tp = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skullferno"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TREASURE5, "treasure5.4", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "LLRLLLLLLLLLRLRLLLLLRLLLLLLL",
                    /* line */ (const char *) "LRLLLLLLLLLLLLLLRLLLRLLLLLLL",
                    /* line */ (const char *) "LL@,$LLLL$,,,,,,,,,LLLLEf$LL",
                    /* line */ (const char *) "LLLL,LLLLLL,LLLLLL,LLLLLL,LL",
                    /* line */ (const char *) "LLLL,LLRLLL,LLLLLL,LLLRLLoLL",
                    /* line */ (const char *) "LLLL,LLLLLL,LLLLLL,LLLLLL,LL",
                    /* line */ (const char *) "LL,,LLLLL,,gLLLL,,LLLLL,,LLL",
                    /* line */ (const char *) "LL,LLLLLL,LLLLLL,LLLzLL,LLLL",
                    /* line */ (const char *) "LL,LLRLLL,LLRLLL,LLRLLL,LRLL",
                    /* line */ (const char *) "LL,LLLLLL,LLLLLL,LLLLLL,LLLL",
                    /* line */ (const char *) "LL,,,LLLL,,,LLLL,,,LLLL,,,LL",
                    /* line */ (const char *) "LLLL,LLLLLL,LLLLLL,LLLLLL,LL",
                    /* line */ (const char *) "LLLL,LRLLLL,LRLLLL,LLRLLL,LL",
                    /* line */ (const char *) "LLLL,LLLLLL,LLLLLL,LLLLLL,LL",
                    /* line */ (const char *) "LL,,LLLLL,,gLLLL,,LLLLL,,LLL",
                    /* line */ (const char *) "LL,LLLLLL,LLLLLL,LLRLLL,LLRL",
                    /* line */ (const char *) "LL,LLLRLL,LLLLLL,LLLzRL,LLLL",
                    /* line */ (const char *) "LL,LLLLLL,LLLLLL,LLLLLL,LLLL",
                    /* line */ (const char *) "LL,,oLLLL,,,LLLL,,,LLLL,,,LL",
                    /* line */ (const char *) "LLLL,LLLLLLoLLLLLL,LLLLLL,LL",
                    /* line */ (const char *) "LRLL,LLRLLL,LLRLLL,LLRLLL,LL",
                    /* line */ (const char *) "LLLL,LLLLLL,LLLLLLoLLLLLL,LL",
                    /* line */ (const char *) "LL,,LLLLL,,gLLLL,,LLLLL,,LLL",
                    /* line */ (const char *) "LL,LLLLLL,LLLLLL,LLLLLL,LLLL",
                    /* line */ (const char *) "LL,LLLLLL,LLLRLL,LLRLLL,LLLL",
                    /* line */ (const char *) "LL,LLLLLL,LLLLLL,LLLLLL,LLLL",
                    /* line */ (const char *) "LL$,,,,,,$LLLLLL,,,,,,,,,$LL",
                    /* line */ (const char *) "LLLLLLLLLLLRLLLLLLLLLLLLLLLL",
                    /* line */ (const char *) "LLLLLLLLLLLLLLLLLLLLLLLLLLLL",
                    /* end */ nullptr);
  }
}
