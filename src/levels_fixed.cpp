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
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCC.......................................CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CaCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCaC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCC...CCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCC.....CCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCC.......CCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCC..@......................................BCEC",
                    /* line */ (const char *) "CCC.......CCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCC.....CCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCC...CCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.CaCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCaC.CCC",
                    /* line */ (const char *) "CCCCCC.CCCCCCCCCCCCCCCCCC.CCCCCCCCCCCCCCCCCC.CCC",
                    /* line */ (const char *) "CCCCCC.......................................CCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC....C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC.$$.C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC....C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'B' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("boss2"); };
    overrides[ 'E' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("exit_closed"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_BOSS2, "boss2.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* line */ (const char *) "R~~~~~~~~~~~~~~WWWWWWWWWWWWWWWWW~~~~~~~~~~~~~~~R",
                    /* line */ (const char *) "R~~~~~~~~WWWWWWWWWWWWWWWWWWWWWWWWWW~~~~~~~~~~~~R",
                    /* line */ (const char *) "R~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW~~~~~~~~~~R",
                    /* line */ (const char *) "R~~WWWWWWWWWWWWWWWWWWWWWWWWW,WWWWWWWWWWW~~~~~~~R",
                    /* line */ (const char *) "R~WWWWWWWWWWWWWWWWWWWW~~WWWWWWWWWWWWWWWWW~~~~~~R",
                    /* line */ (const char *) "R~WWWWWWWWWWWWWWWWWW~~~~WWWWWWWWWWWWWWWWWWWWW~~R",
                    /* line */ (const char *) "R~WWWWWWWWWWWWWWW~~~~~R~WWWWWWWWWWWWWWWWWWWWWW~R",
                    /* line */ (const char *) "R~~WWWWWWWWWWWWW~RRR~R~~WWWWWWWWWWWWWWWWWWWWWW~R",
                    /* line */ (const char *) "R~~WWWWWWWWWWWW~RR$RR~~WWWWWWWWWWWWWWWWWWWWWWW~R",
                    /* line */ (const char *) "R~~WWWWWWWWWWW~~RRRR~~~WWWWWWWWWW~WWWWWWWWWWWW~R",
                    /* line */ (const char *) "R~WWWWWWWWWWWW~~RRRR~RWWWWWWWWWW~~WWWWWWWWWWWWWR",
                    /* line */ (const char *) "R~WWWWWWW,WWWWW~RRRWWWWWWWWWWW~~~~WWWWWWWWWWWWWR",
                    /* line */ (const char *) "R~WWWWWWWWWWWWWWWWWWWWWWWWWWW~~~~WWWW,WWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWWR~WWWWWWWW~~~~WWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWW~~WW~R~~WWWWWW~WWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWW~WWWWWWWW~~~~RR~R~WWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWW~~~WWWWWWW~~~~~RW~WWWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RRRRRR~WWWWWWWW~~~W~WWWWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "R~~~~RR~WWWWWWWW~WWWWWWWWWWWBW,WWWWWWWWWWW~~~WWR",
                    /* line */ (const char *) "R~RR~RR~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW~~RR~WR",
                    /* line */ (const char *) "R~~R~RRR~~~WWWWWWWWWWWW~~WWWWWWWWW~~~~WWW~~RRR~R",
                    /* line */ (const char *) "R~RR~RRRRRR~~~~WWWWWWWWWWWWWWWWWWWWWWWW~~~RRER~R",
                    /* line */ (const char *) "R@RR~~~~~~~~WWWWWWWWWWWW~~WWWWWWWW~~~WWWW~RRRR~R",
                    /* line */ (const char *) "RR~RRRRRRR~~WWW,WWWWWWWWWWWWWWWWWWWWWWWWWW~~R~~R",
                    /* line */ (const char *) "RWW~~RRR~~~~WWWWWWW~WWWWWWWWW,WWWWWWWWWWWW~~R~WR",
                    /* line */ (const char *) "RWWWW~R~~WWWWWWWWW~~~WWWWWWWWWWWWWWWWWWWWWW~~WWR",
                    /* line */ (const char *) "RWWWWW~~WWWWWWWWW~~R~~WWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWW~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWW~WWWWWWWWWWWWWWW~WWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWW,WWWWWWWWWWWWWWWWWWW~~~WWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW~~~WWW,WWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW~WWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWWW~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWW~~~WWWWWWWWWWWWWWWW~WWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "RWWWWWWWW~~~WWWWWWWWWWWWWW~~~WWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "R~WWWWWWWWW~WWWWWWWWWWWWW~R~~WWWWWWWWWWWWWWWWWWR",
                    /* line */ (const char *) "R~WWWWWWWWWWWWWWWWWWWWWW~~~~WWWWWWWWWWWWWWWWWW~R",
                    /* line */ (const char *) "R~WWWWWWWWWWWWWWWWWWWWWW~~~WWWWWWWWWWWWWWWWWWW~R",
                    /* line */ (const char *) "R~WWWWWWWWWWWWWWWWWWWWWW~WWWWWWWWWWWWWW,WWWWWW~R",
                    /* line */ (const char *) "R~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW,WWWWWWWWWWWW~R",
                    /* line */ (const char *) "R~~WWWWWWWWWWW,WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW~~R",
                    /* line */ (const char *) "R~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW~~~~R",
                    /* line */ (const char *) "R~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW~~~~~R~~R",
                    /* line */ (const char *) "R~~~~WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW~~~~~~~~RR~~R",
                    /* line */ (const char *) "R~~~~~~~~~~WWWWWWWWWWWWW~~~~~~~~~~~~~~~~~~~~~~~R",
                    /* line */ (const char *) "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'm' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("skullferno"); };
    overrides[ 'B' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("boss3"); };
    overrides[ 'E' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("exit_closed"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_BOSS3, "boss3.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,R,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRR,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,R,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,m,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,m,,,,,,,,,,m,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,R,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,R,,,,,,,,,,,,,,,,,,,,RRR,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,RRR,,,,,,,,,,,,,,,,,,,,R,,,,,,,,R,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,R,,,,,,,,,,,,,,,,,,,,,,,,,,,,,RRR,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,R,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,R,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,RRR,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,m,,,,,,,,,,,,R,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,@,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,B,,,,,EC",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,R,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,RRR,,,,,,,m,,,,,,,,,,,,,,,,R,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,R,,,,,,,,,,,,,,,,,,,,,,,,RRR,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,R,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,R,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,RRR,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,R,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,R,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,m,,,,,,,,,,,,,,,,,,,,,,,m,,,,RRR,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,R,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,R,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,RRR,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,R,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,m,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'B' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("boss4"); };
    overrides[ 'E' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("exit_closed"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_BOSS4, "boss4.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "C```,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,`````C",
                    /* line */ (const char *) "C``,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,````C",
                    /* line */ (const char *) "C`,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,```C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,`C",
                    /* line */ (const char *) "C,,,,,,,,,```,,,,,,,,,,,,,,,,,`,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,|`,,,,,,,,,,,,,,,```,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,``,,,,,,,,,,,,,,````,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,`,```,,,,,,,,,,,,,,,`|`,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,``,`,,,,,,,,,,,,,,,,,```,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,````,,,,,,,,,,,,,,,,,`,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,`,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,``,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,`,`,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,`|``,,,,,,,,,,,,`,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,`,``,,,,,,,,,,``,`,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,`,,,,,,,,,``,|`,,,,,,,,,,,,,,`C",
                    /* line */ (const char *) "C,````,,,,,,,,,,,,,,,,,,,,,`````,,,,,,,,,,,,,,`C",
                    /* line */ (const char *) "C,`,`,``,,,,,,,,,,,,,,,,,,,`,,,,,,,,,,,,,,,`,``C",
                    /* line */ (const char *) "C,,,,,,```,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,````|C",
                    /* line */ (const char *) "C,,,,@,,````,,,,,,,,,,,,,,,,,,,,,,,,,,,,B,````EC",
                    /* line */ (const char *) "C,,,,,,`````,,,,,,,,,,,,,,,,,,,,,`,,,,,,,,`,``|C",
                    /* line */ (const char *) "C,,,,,,`,```,,,,,,````,,,,,,,,,,`,,,,,,,,,,,,``C",
                    /* line */ (const char *) "C,,,,,`,`,,,,,,,,,``|``,,,,,,,,````,,,,,,,,,,``C",
                    /* line */ (const char *) "C`,,`,```,,,,,,,,,``,``,,,,,,,,,`|`,,,,,,,,`,,`C",
                    /* line */ (const char *) "C,`,``,,,,,,,,,,,,,,,,`,,,,,,,,,`,`,,,,,,,,`,,`C",
                    /* line */ (const char *) "C```,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,`,,,,`,,,`,,,C",
                    /* line */ (const char *) "C`,,,,,,,,``,,,,,,,,,,,,,,,,,,,,,,,,,,,`,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,`|``,,,,,,,,,,,,,,,,,,,,,,,,,`,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,```,,,,,,,,,,,,,,,,,,,`,,,,,```,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,`,,,,,,,,,,,,,,,,,,```,,,,`,`,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,``|`,,,,`,`,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,``,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,`,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,```,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,``|`,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,```,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,`,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,`,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,`,,,,,,,,,,,,,,,,,,,,,,,,,,,,`,`,,,,,C",
                    /* line */ (const char *) "C,,,,,,,````,,,,,,,,,,,,,,,,,,,,,,,,,,``,`,,,,,C",
                    /* line */ (const char *) "C,,,,,,,`,``,,,,,,,,,,,,,,,,,,,,,,,,,,``,,,,,,,C",
                    /* line */ (const char *) "C`,,,,,,,,``,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,`C",
                    /* line */ (const char *) "C```,,,,,,`,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,`,``C",
                    /* line */ (const char *) "C```,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,`````C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }

  {
    Overrides overrides;

    overrides[ 'B' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("boss4"); };
    overrides[ 'E' ] = [](char /*c*/, bpoint /*p*/) -> Tpp { return tp_find_mand("exit_closed"); };

    level_fixed_add(g, CHANCE_NORMAL, LEVEL_TYPE_BOSS5, "boss5.1", __FUNCTION__, __LINE__, overrides, 0,
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,@,,,,,B,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,EC",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "C,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,C",
                    /* line */ (const char *) "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
                    /* end */ nullptr);
  }
}
