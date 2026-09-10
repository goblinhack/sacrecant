//
// Copyright goblinhack@gmail.com
//

#include "my_bpoint.hpp"
#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_level_ext.hpp"
#include "my_tp.hpp"
#include "my_types.hpp"

Overrides no_overrides;

//
// Create all test levels
//
void levels_test(Gamep g)
{
  TRACE();

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "simple", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx.@x.R..xx",
                  /* line */ (const char *) "xx.xL....xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* end */ nullptr);

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("mummy"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "mob", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* line */ (const char *) "xx.......xx",
                    /* line */ (const char *) "xx..b|...xx",
                    /* line */ (const char *) "xx...g...xx",
                    /* line */ (const char *) "xx.@.|...xx",
                    /* line */ (const char *) "xx...g...xx",
                    /* line */ (const char *) "xx...|...xx",
                    /* line */ (const char *) "xx.......xx",
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ '$' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("pot_incin"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "chain_reaction", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* line */ (const char *) "x$$$$$$$$$x",
                    /* line */ (const char *) "x$........x",
                    /* line */ (const char *) "x$........x",
                    /* line */ (const char *) "x$........x",
                    /* line */ (const char *) "x$.@$$$$$$x",
                    /* line */ (const char *) "x$.......$x",
                    /* line */ (const char *) "x$.......$x",
                    /* line */ (const char *) "x$.......$x",
                    /* line */ (const char *) "x$$$$$$$$$x",
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ '$' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("pot_invis"); };
    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("kobalos"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "invisibility", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxx",
                    /* line */ (const char *) "x.......x",
                    /* line */ (const char *) "x......mx",
                    /* line */ (const char *) "x.......x",
                    /* line */ (const char *) "x.......x",
                    /* line */ (const char *) "x@$....mx",
                    /* line */ (const char *) "x.......x",
                    /* line */ (const char *) "x.......x",
                    /* line */ (const char *) "x..x....x",
                    /* line */ (const char *) "x..x....x",
                    /* line */ (const char *) "x..x....x",
                    /* line */ (const char *) "x..x....x",
                    /* line */ (const char *) "xxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'c' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("chocolate_frog"); };
    overrides[ 'd' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("clown_meat"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "treasure", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* line */ (const char *) "x.........x",
                    /* line */ (const char *) "x.$$$$$$$$x",
                    /* line */ (const char *) "x.$$$$$$$$x",
                    /* line */ (const char *) "x@$$$$$$$$x",
                    /* line */ (const char *) "x.$$$$$$$$x",
                    /* line */ (const char *) "x.........x",
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides const overrides;

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "generator", __FUNCTION__, __LINE__, no_overrides, 0,
                    /* line */ (const char *) "XXXXXXXXXXXXXX",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X.@..g.......X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "XXXXXXXXXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'M' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("cleaner"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "cleaner", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXXXXXXXXX",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X......@...M.X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "X............X",
                    /* line */ (const char *) "XXXXXXXXXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'M' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("voider"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "rubble", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXXXXXXXXX",
                    /* line */ (const char *) "X........r...X",
                    /* line */ (const char *) "X........r...X",
                    /* line */ (const char *) "X.@......r.M.X",
                    /* line */ (const char *) "X........r...X",
                    /* line */ (const char *) "X........r...X",
                    /* line */ (const char *) "XXXXXXXXXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'M' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("spiderweb"); };
    overrides[ '$' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("boots_spider"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "spiderweb", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXXXXXXXXX",
                    /* line */ (const char *) "XxxxxxxxxxxxxX",
                    /* line */ (const char *) "X.@$wwwwwww..X",
                    /* line */ (const char *) "XxxxxxxxxxxxxX",
                    /* line */ (const char *) "XXXXXXXXXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'M' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("spider_giant"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "spider", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXXXXXXXXX",
                    /* line */ (const char *) "XxxxxxxxxxxxxX",
                    /* line */ (const char *) "X.@........M.X",
                    /* line */ (const char *) "XxxxxxxxxxxxxX",
                    /* line */ (const char *) "XXXXXXXXXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("kobalos"); };
    overrides[ '$' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("pot_levit"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "test1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXXXXXXXXXXXXXXXXXXXXX",
                    /* line */ (const char *) "X.......CCC.bBb.LLL......X",
                    /* line */ (const char *) "X.......CCC.....LLL......X",
                    /* line */ (const char *) "X.......CCC.$...LLL......X",
                    /* line */ (const char *) "X.......CCC.@.b.LLL......X",
                    /* line */ (const char *) "X.......CCC.....LLL......X",
                    /* line */ (const char *) "X.......CCC.WWW.LLL......X",
                    /* line */ (const char *) "X.......CCC.WWW.LLL......X",
                    /* line */ (const char *) "XXXXXXXXXXXXXXXXXXXXXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("kobalos"); };
    overrides[ '$' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("pot_stealth"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "foliage", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXX",
                    /* line */ (const char *) "X``@$`X",
                    /* line */ (const char *) "X`````X",
                    /* line */ (const char *) "X`````X",
                    /* line */ (const char *) "X.....X",
                    /* line */ (const char *) "X.....X",
                    /* line */ (const char *) "X.....X",
                    /* line */ (const char *) "X.....X",
                    /* line */ (const char *) "X....mX",
                    /* line */ (const char *) "XXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("mantisman"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "test2", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXX",
                    /* line */ (const char *) "X.....X",
                    /* line */ (const char *) "X..v..X",
                    /* line */ (const char *) "X.v@v.X",
                    /* line */ (const char *) "X..v..X",
                    /* line */ (const char *) "X.....X",
                    /* line */ (const char *) "XXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("mantisman"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "test2b", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXX",
                    /* line */ (const char *) "X.===.X",
                    /* line */ (const char *) "X.CCC.X",
                    /* line */ (const char *) "X.CCC.X",
                    /* line */ (const char *) "X@CCCmX",
                    /* line */ (const char *) "XXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("spider_giant"); };
    overrides[ 'k' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("kobalos"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "avoid-water", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXXXXXXXXXX",
                    /* line */ (const char *) "X..~~.xkk.k.kkX",
                    /* line */ (const char *) "X..==.x.k..kk.X",
                    /* line */ (const char *) "X..~~.xk.k.kk.X",
                    /* line */ (const char *) "X..~~.xkk.k.k.X",
                    /* line */ (const char *) "X@.~~.xk.k.kkmX",
                    /* line */ (const char *) "XXXXXXXXXXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("ghost"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "avoid-chasm", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXXXXXXXXXX",
                    /* line */ (const char *) "X.CCC.CCC.CCC.X",
                    /* line */ (const char *) "X.===.CCC.===.X",
                    /* line */ (const char *) "X.CCC.CCC.CCC.X",
                    /* line */ (const char *) "X.CCC.===.CCC.X",
                    /* line */ (const char *) "X@CCC.CCC.CCCmX",
                    /* line */ (const char *) "XXXXXXXXXXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("mantisman"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "avoid-lava", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXXXXXXXXXX",
                    /* line */ (const char *) "X.LLL.LLL.LLL.X",
                    /* line */ (const char *) "X.===.LLL.===.X",
                    /* line */ (const char *) "X.LLL.LLL.LLL.X",
                    /* line */ (const char *) "X.LLL.===.LLL.X",
                    /* line */ (const char *) "X@LLL.LLL.LLLmX",
                    /* line */ (const char *) "XXXXXXXXXXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("mantisman"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "test4", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXX",
                    /* line */ (const char *) "X...x.X",
                    /* line */ (const char *) "X.x.x.X",
                    /* line */ (const char *) "X.x.xmX",
                    /* line */ (const char *) "X.x.x.X",
                    /* line */ (const char *) "X@x...X",
                    /* line */ (const char *) "XXXXXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("mantisman"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "test5", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "XXXXXXX",
                    /* line */ (const char *) "X.C...X",
                    /* line */ (const char *) "X.CC..X",
                    /* line */ (const char *) "X.CCC.X",
                    /* line */ (const char *) "X.CCC.X",
                    /* line */ (const char *) "X@CCCmX",
                    /* line */ (const char *) "XxxxXXX",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("mantisman"); };
    overrides[ 'n' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("ghost"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "monst.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..............x.......x.............x...x...x.x",
                    /* line */ (const char *) "x...xxxxxxxxxx.x.......x.........x...x.x.x.x.x.x",
                    /* line */ (const char *) "x............x.x.......x.........x...x.x.x.x.x.x",
                    /* line */ (const char *) "xxxxxxxxxxxxxx.x.................x...x.x.x.x.x.x",
                    /* line */ (const char *) "x...@```````.x.xxxxxxxxxxxxxxxxxxx...x.x.x.xxx.x",
                    /* line */ (const char *) "xxxxxxxxxxxxxx.........x.........x.....x.......x",
                    /* line */ (const char *) "x............x...~~~~~~x.........x..x..xxxxxxxxx",
                    /* line */ (const char *) "x............x.~~~~~~~~x..~~~....x..xxxx.......x",
                    /* line */ (const char *) "x............x..~~~~~~~x.~~~~....x.............x",
                    /* line */ (const char *) "x............x..~~~~~.~x.~~~~~...xxxxxxxxxxxxx.x",
                    /* line */ (const char *) "x............x~~~~~~~.~x.~~~.....x.........x...x",
                    /* line */ (const char *) "x................~~~~.~x.~~~~~~..x..x.x....x...x",
                    /* line */ (const char *) "x.....xxxxxx.x~~~~~~~..x.~~......x..x.x.xxxx...x",
                    /* line */ (const char *) "x............x.....~~....~.......x....x.x..x...x",
                    /* line */ (const char *) "x...``..xxxxxx....~~~.~~~~~~~~...xxxx.x.x..x.xxx",
                    /* line */ (const char *) "x...x.................................x........x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.xxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x......x.......................x...............x",
                    /* line */ (const char *) "x......x............xxxxxxxxx..x.xxxxxxxx......x",
                    /* line */ (const char *) "x......x............x.......x..x.x......x......x",
                    /* line */ (const char *) "x......x............x.......x..x.x......x......x",
                    /* line */ (const char *) "x......x............x.......x..x.x......x......x",
                    /* line */ (const char *) "x......x............x.......x..x.x.xxxxxxxxxxx.x",
                    /* line */ (const char *) "x...................x.xxxxxxxxxx...............x",
                    /* line */ (const char *) "x......x............x..........x.xxxxxxxx.xxxxxx",
                    /* line */ (const char *) "x......x............x..........x.x.............x",
                    /* line */ (const char *) "x......x............x............x.............x",
                    /* line */ (const char *) "x......x............x.xxxxxxxxxx.x.xxxxxxxxxxxxx",
                    /* line */ (const char *) "x......x.......................................x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx...x",
                    /* line */ (const char *) "x..........................x...................x",
                    /* line */ (const char *) "x..........................x...................x",
                    /* line */ (const char *) "x..........................x...................x",
                    /* line */ (const char *) "x..........................x...................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x.........xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x.........x....................................x",
                    /* line */ (const char *) "x.........x....................................x",
                    /* line */ (const char *) "x.........x....................................x",
                    /* line */ (const char *) "x.........x....................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x.........xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x.....................................x........x",
                    /* line */ (const char *) "x.....................................x........x",
                    /* line */ (const char *) "x.....................................x........x",
                    /* line */ (const char *) "x.....................................x........x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "monst", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x...xxxxxxxxx..................................x",
                    /* line */ (const char *) "x...x..........................................x",
                    /* line */ (const char *) "x...x..........................................x",
                    /* line */ (const char *) "x...x..........................................x",
                    /* line */ (const char *) "x...x..........................................x",
                    /* line */ (const char *) "x...x..........................................x",
                    /* line */ (const char *) "x.......E......................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "x..............................................x",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ '$' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("boots_fire"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "lava", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* line */ (const char *) "xx.......xx",
                    /* line */ (const char *) "xx...LL..xx",
                    /* line */ (const char *) "xx...LL..xx",
                    /* line */ (const char *) "xx.@$Lxxxxx",
                    /* line */ (const char *) "xx...LxLLxx",
                    /* line */ (const char *) "xx...LxLLxx",
                    /* line */ (const char *) "xx...LLLLxx",
                    /* line */ (const char *) "xx.......xx",
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* end */ nullptr);
  }

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "rock", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx...R...xx",
                  /* line */ (const char *) "xx...R...xx",
                  /* line */ (const char *) "xx.@.R...xx",
                  /* line */ (const char *) "xx...R...xx",
                  /* line */ (const char *) "xx...R...xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "border", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx...X...xx",
                  /* line */ (const char *) "xx...X...xx",
                  /* line */ (const char *) "xx.@.X...xx",
                  /* line */ (const char *) "xx...X...xx",
                  /* line */ (const char *) "xx...X...xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* end */ nullptr);

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skeleton"); };
    overrides[ 'M' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("mummy"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "chasm.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "x....xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "x..LLxCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "xxxxxxCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCxxxxxxxxxxxCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCx....@....xCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCx......R,.xCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCx.....R.,.xCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCx~~~.C.W,.xCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCx$.~.C.W..xCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCx...CCCLLxxCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCx...CCCLLxxCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCx..CCCCLLxxCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCx.``CCCLLxxCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCx.``CCCCLxxCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCxxxxxxxxxxxCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCxxxxxxxxxxxCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "chasm.2", __FUNCTION__, __LINE__, no_overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxx", // next level down
                    /* line */ (const char *) "xmmmmxxxxxx",
                    /* line */ (const char *) "xx$$$$$LLLx",
                    /* line */ (const char *) "xx$$$$$LLLx",
                    /* line */ (const char *) "xxL..$$LLLx",
                    /* line */ (const char *) "xxL..$$.LLx",
                    /* line */ (const char *) "xxL..$$..Lx",
                    /* line */ (const char *) "xxL......xx",
                    /* line */ (const char *) "xxL......xx",
                    /* line */ (const char *) "xxLELLLLLxx",
                    /* line */ (const char *) "xxL.LLLLLxx",
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* line */ (const char *) "xxxxxxxxxxx",
                    /* end */ nullptr);
  }

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "chasm", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "xxxxxxxxxxx", // all other chasm levels
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx.E.....xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "fire", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xx...!!!.xx",
                  /* line */ (const char *) "xx...b!b.xx",
                  /* line */ (const char *) "xx.@.!b!.xx",
                  /* line */ (const char *) "xx...b!b.xx",
                  /* line */ (const char *) "xx.E.!!!.xx",
                  /* line */ (const char *) "xx.......xx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* line */ (const char *) "xxxxxxxxxxx",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "firemaze", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) "  xxxxxxx  ",
                  /* line */ (const char *) "  xxxbbbx  ",
                  /* line */ (const char *) "  xxxbxbx  ",
                  /* line */ (const char *) "  x@!bxbx  ",
                  /* line */ (const char *) "  xxxxxbx  ",
                  /* line */ (const char *) "  xbbbbbx  ",
                  /* line */ (const char *) "  xxxxxxx  ",
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) "           ",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "bridge1", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x..CCC..x ",
                  /* line */ (const char *) " x.@====.x ",
                  /* line */ (const char *) " x..CCC..x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) "           ",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "bridge2", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x..LLL..x ",
                  /* line */ (const char *) " x.@====.x ",
                  /* line */ (const char *) " x..LLL..x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) "           ",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "shove", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x....C..x ",
                  /* line */ (const char *) " x@..BC..x ",
                  /* line */ (const char *) " x....C..x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) "           ",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "shove.9", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x..E....x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) "           ",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "barrel", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " xbbbbbb.x ",
                  /* line */ (const char *) " xb....b.x ",
                  /* line */ (const char *) " xb.@..b.x ",
                  /* line */ (const char *) " xb....b.x ",
                  /* line */ (const char *) " xbbbbbb.x ",
                  /* line */ (const char *) " x.......x ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) "           ",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "door_secret", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) " xxxxx     ",
                  /* line */ (const char *) " x...x     ",
                  /* line */ (const char *) " x...xxxxx ",
                  /* line */ (const char *) " x...x...x ",
                  /* line */ (const char *) " x..@s..mx ",
                  /* line */ (const char *) " x...x...x ",
                  /* line */ (const char *) " x...xxxxx ",
                  /* line */ (const char *) " x...x     ",
                  /* line */ (const char *) " xxxxx     ",
                  /* line */ (const char *) "           ",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "door_unlocked", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) " xxxxx     ",
                  /* line */ (const char *) " x...x     ",
                  /* line */ (const char *) " xmmmxxxxx ",
                  /* line */ (const char *) " x...x...x ",
                  /* line */ (const char *) " x@..D...x ",
                  /* line */ (const char *) " x...x...x ",
                  /* line */ (const char *) " xk..+...x ",
                  /* line */ (const char *) " x...x...x ",
                  /* line */ (const char *) " x...xxxxx ",
                  /* line */ (const char *) " x...x     ",
                  /* line */ (const char *) " xxxxx     ",
                  /* line */ (const char *) "           ",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "door_locked", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) " xxxxx     ",
                  /* line */ (const char *) " x...x     ",
                  /* line */ (const char *) " x...xxxxx ",
                  /* line */ (const char *) " x...x..mx ",
                  /* line */ (const char *) " x@.k+...x ",
                  /* line */ (const char *) " x...x...x ",
                  /* line */ (const char *) " x...xxxxx ",
                  /* line */ (const char *) " x...x     ",
                  /* line */ (const char *) " xxxxx     ",
                  /* line */ (const char *) "           ",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "item", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "           ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) " x...$$$$x ",
                  /* line */ (const char *) " x...$$$$x ",
                  /* line */ (const char *) " x...$$$$x ",
                  /* line */ (const char *) " x..@$$$$x ",
                  /* line */ (const char *) " x...$$$$x ",
                  /* line */ (const char *) " x...$$$$x ",
                  /* line */ (const char *) " x...$$$$x ",
                  /* line */ (const char *) " xxxxxxxxx ",
                  /* line */ (const char *) "           ",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "teleport", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "                   ",
                  /* line */ (const char *) "xxxxxx             ",
                  /* line */ (const char *) "x....x             ",
                  /* line */ (const char *) "x....xxxxxxxxxxxxxx",
                  /* line */ (const char *) "x.................x",
                  /* line */ (const char *) "x...bT..@.........x",
                  /* line */ (const char *) "x.................x",
                  /* line */ (const char *) "x....T............x",
                  /* line */ (const char *) "x.................x",
                  /* line */ (const char *) "xxxxxxxxxxxxxxxxxxx",
                  /* line */ (const char *) "                   ",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "teleport-large", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "xxxxxxCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCxxxxxxxx",
                  /* line */ (const char *) "x....xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCx......x",
                  /* line */ (const char *) "x....xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCx..T...x",
                  /* line */ (const char *) "x...TxCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCx......x",
                  /* line */ (const char *) "xxxx.xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCxxxxxxxx",
                  /* line */ (const char *) "CCCC.CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCC.CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCC.CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCxxxxxCCCCC",
                  /* line */ (const char *) "CCCC.CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCx...xCCCCC",
                  /* line */ (const char *) "CCCC....................................T.xCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCx...xCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCxxxxxCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCxxxxxxxxxCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCx.......xCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCx.@..T..xCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCx.......xCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCx.......xCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCxxxxxxxxxCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "xxxxxCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "x...xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "x.T.xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "x...xCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "xxxxxCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCxxxxx",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCx...x",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCx.T.x",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCx...x",
                  /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCxxxxx",
                  /* end */ nullptr);

  level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "empty", __FUNCTION__, __LINE__, no_overrides, 0,
                  /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x...xxxxxxxxx..................................x",
                  /* line */ (const char *) "x...x..........................................x",
                  /* line */ (const char *) "x...x...@.....g................................x",
                  /* line */ (const char *) "x...x..........................................x",
                  /* line */ (const char *) "x...x..........................................x",
                  /* line */ (const char *) "x...x..........................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "x..............................................x",
                  /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                  /* end */ nullptr);

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("coil_eel"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_TEST, "water", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x.....@....~m~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "x..........~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWx",
                    /* line */ (const char *) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                    /* end */ nullptr);
  }
}
