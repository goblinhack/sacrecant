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
// Create all fixed levels
//
void levels_fixed(Gamep g)
{
  TRACE();

  {
    Overrides overrides;

    overrides[ 'a' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("argusul"); };
    overrides[ 'B' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("boss1"); };
    overrides[ 'E' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("exit_closed"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_BOSS1, "boss1.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCC.......................CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CaCCCCCCCC.CCCCCCCCaC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCC...CCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCC.....CCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCC.......CCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCC..@......................BCEC",
                    /* line */ (const char *) "CCC.......CCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCC.....CCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCC...CCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CaCCCCCCCC.CCCCCCCCaC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCC.CCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.......................CCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCC....C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCC.$$.C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCC....C",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'B' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("boss2"); };
    overrides[ 'E' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("exit_closed"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_BOSS2, "boss2.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "R~~~~~~~~~WWWWWW~~~~~~~~~~~~~~~R",
                    /* line */ (const char *) "R~~WWWWWWWWWWWWWW~WWWWWWWWWWWW~R",
                    /* line */ (const char *) "R~WWWWWWWWWWWWWW~~WWWWWWWWWWWWWR",
                    /* line */ (const char *) "R~WWWWWWW,WWWW~~~~WWWWWWWWWWWWWR",
                    /* line */ (const char *) "R~WWWWWWWWWWW~~~~WWWW,WWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWW~~~~WWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWW~WWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWW~WWWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWW~~~WWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RRRRRR~WWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "R~~~~RR~WWWWBW,WWWWWWWWWWW~~~WWR",
                    /* line */ (const char *) "R~RR~RR~WWWWWWWWWWWWWWWWW~~RR~WR",
                    /* line */ (const char *) "R~~R~RRR~~WWWWWWWW~~~~WWW~~RRR~R",
                    /* line */ (const char *) "R~RR~RRRRRWWWWWWWWWWWWW~~~RRER~R",
                    /* line */ (const char *) "R@RR~~~~~~WWWWWWWW~~~WWWW~RRRR~R",
                    /* line */ (const char *) "RR~RRRRRRRWWWWWWWWWWWWWWWW~~R~~R",
                    /* line */ (const char *) "RWW~~RRR~~WWW,WWWWWWWWWWWW~~R~WR",
                    /* line */ (const char *) "RWWWW~R~~WWWWWWWWWWWWWWWWWW~~WWR",
                    /* line */ (const char *) "RWWWWW~~WWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWW~WWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWW~~~WWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWW~~~WWW,WWWWWWWR",
                    /* line */ (const char *) "R~WWWWWWWWWWWWWWWWWWWWW,WWWWWW~R",
                    /* line */ (const char *) "R~~WWWWWWWWWWWWWW,WWWWWWWWWWWW~R",
                    /* line */ (const char *) "R~~WWWWWWWWWWWWWWWWWWWWWWWWWW~~R",
                    /* line */ (const char *) "R~~WWWWWWWWWWWWWWWWWWWWWWWW~~~~R",
                    /* line */ (const char *) "R~~~WWWWWWWWWWWWWWWWWWW~~~~~R~~R",
                    /* line */ (const char *) "R~~~~WWWWWWWWWWWWWW~~~~~~~~RR~~R",
                    /* line */ (const char *) "R~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~R",
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skullferno"); };
    overrides[ 'B' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("boss3"); };
    overrides[ 'E' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("exit_closed"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_BOSS3, "boss3.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,R,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,RRR,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,R,,,,,,,,R,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,RRR,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,R,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,@,,,,,,,,,,,,,,,,,,B,,,,,EC",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,R,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,RRR,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,R,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,R,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,RRR,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,R,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,m,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'B' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("boss4"); };
    overrides[ 'E' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("exit_closed"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_BOSS4, "boss4.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "C```,,,,,,,,,,,,,,,,,,,,,,`````C",
                    /* line */ (const char *) "C``,,,,,,,,,,,,,,,,,,,,,,,,w```C",
                    /* line */ (const char *) "C`,,,,,,,,,,,,,,,,,,,,,,,,,,`w`C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,`,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,``,`,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,w``,|`,,,,,,,,,,,,,,`C",
                    /* line */ (const char *) "C,````,,,,,``w``,,,,,,,,,,,,,,`C",
                    /* line */ (const char *) "C,`,`,``,,,`w,,,,,,,,,,,,,,`,``C",
                    /* line */ (const char *) "C,,,,,,```,,,,,,,,,,,,,,,,````|C",
                    /* line */ (const char *) "C,,,,@,,``,,,,,,,,,,,,,,B,````EC",
                    /* line */ (const char *) "C,,,,,,```,,,,,,,`,,,,,,,,`,``|C",
                    /* line */ (const char *) "C,,,,,,`,`,,,,,,`,,,,,,,,,,,w``C",
                    /* line */ (const char *) "C,,,,,`w`,,,,,,````,,,,,,,,,w``C",
                    /* line */ (const char *) "C`,,`,```,,,,,,,`|w,,,,,,,,`,,`C",
                    /* line */ (const char *) "C,`,``,,,,,,,,,,`,`,,,,,,,,`,,`C",
                    /* line */ (const char *) "C``w,,,,,,,,,,,,,,`,,,,`,,,`,,,C",
                    /* line */ (const char *) "C`,,,,,,,,w,,,,,,,,,,,,w,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,`,ww,,,,,,,,,`,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,`,,,w,`,,,,,```,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,`,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,`,`,,,,,C",
                    /* line */ (const char *) "C,,,,,,,``,,,,,,,,,,,,w`,`,,,,,C",
                    /* line */ (const char *) "C,,,,,,,`,,,,,,,,,,,,,``,,,,,,,C",
                    /* line */ (const char *) "C`,,,,,,,,,,,,,,,,,,,,,,,,,,,,`C",
                    /* line */ (const char *) "C```,,,,,,,,,,,,,,,,,,,,,,,w,``C",
                    /* line */ (const char *) "C```,,,,,,,,,,,,,,,,,,,,,,`````C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'B' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("boss5"); };
    overrides[ 'E' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("exit_closed"); };
    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("ogrik"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_BOSS5, "boss5.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "R,,m,,,,,,,,,,,,,,,,,,,,,,,,,,,R",
                    /* line */ (const char *) "R,RRRRRRRRRRRRRRRRRRRRRRRRRRRR,R",
                    /* line */ (const char *) "R,R,,,,,,,,,,,,,,,,,,,,,,,,,,R,R",
                    /* line */ (const char *) "R,R,RRRRRRRRRRRRRRRRRRRRRRRR,R,R",
                    /* line */ (const char *) "R,R,R,,,,,,,,,,,,,,,,,,,,,,R,R,R",
                    /* line */ (const char *) "R,,,R,,RRRRR,,RRRRRRRRRRRR,R,R,R",
                    /* line */ (const char *) "R,R,R,RRLLLL..LLLLLLLLLLLR,R,R,R",
                    /* line */ (const char *) "R,R,R,RRLLLL..LLLLLLLLLLLR,R,R,R",
                    /* line */ (const char *) "R,R,R,RRLLLLLLLLLLLLLLLLLR,,,R,R",
                    /* line */ (const char *) "R,R,R,RLLLLLLLLLLLLLLLL,LR,R,R,R",
                    /* line */ (const char *) "R,R,R,,...LLLLLLLLLLLLLLR,,R,R,R",
                    /* line */ (const char *) "R,R,R,RLLLLLLLLEBLLLLL..,,,R,R,R",
                    /* line */ (const char *) "R,R,R,RLLLLLLLLLLLLLLLLLR,,R,R,R",
                    /* line */ (const char *) "R,R,R,RRLLLLLLLR.RLLLLLLLR,R,R,R",
                    /* line */ (const char *) "R,R,R,RRLLLLLLRR.RRLLLLLLR,R,R,R",
                    /* line */ (const char *) "R,R,R,RLLLLLLRR,,,RRRRRRRR,R,R,R",
                    /* line */ (const char *) "R,R,R,RRLLLLLRR,,,RRL$$$RR,R,R,R",
                    /* line */ (const char *) "R,R,R,,RLLLLLRR,,,RRL$$$$R,R,R,R",
                    /* line */ (const char *) "R,R,R,,RRRRRRR,,,,,RRRRRRR,R,R,R",
                    /* line */ (const char *) "R,R,R,,,,,,,,,,,,,,,,,,,,,,R,R,R",
                    /* line */ (const char *) "R,R,RRRRRRRRRR,,,,,RRRRRRRRR,R,R",
                    /* line */ (const char *) "R,R,,,,,,,,,,R,,,,,R,,,,,,,,,RmR",
                    /* line */ (const char *) "R,RRRRRRRRRRRR,,,,,RRRRRRRRR,R,R",
                    /* line */ (const char *) "R,,,,,,,,,,,,R,,,,,R,,,,,,,,,,,R",
                    /* line */ (const char *) "RRRRRRRRRRRRRR,,,,,RRRRRRRRRRRRR",
                    /* line */ (const char *) "             R,,,,,R            ",
                    /* line */ (const char *) "             R,,,,,R            ",
                    /* line */ (const char *) "             R,,,,,R            ",
                    /* line */ (const char *) "             R,,@,,R            ",
                    /* line */ (const char *) "             R,,,,,R            ",
                    /* line */ (const char *) "             RRRRRRR            ",
                    /* end */ nullptr);
  }
}
