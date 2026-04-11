//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_main.hpp"

//
// Create all fragments
//
void fragments_init(Gamep g)
{
  TRACE();

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "     ",
                   /* frag   */ (const char *) ".... ",
                   /* frag   */ (const char *) ".... ",
                   /* frag   */ (const char *) ".... ",
                   /* frag   */ (const char *) ".... ", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "     ",
                           /* alt  */ (const char *) "..xx ",
                           /* alt  */ (const char *) "...x ",
                           /* alt  */ (const char *) ".x.. ",
                           /* alt  */ (const char *) ".... ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "     ",
                           /* alt  */ (const char *) "..xx ",
                           /* alt  */ (const char *) "...x ",
                           /* alt  */ (const char *) ".x.. ",
                           /* alt  */ (const char *) ".... ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "     ",
                           /* alt  */ (const char *) "..xx ",
                           /* alt  */ (const char *) "...x ",
                           /* alt  */ (const char *) ".... ",
                           /* alt  */ (const char *) ".... ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "     ",
                           /* alt  */ (const char *) ".xxx ",
                           /* alt  */ (const char *) ".xxx ",
                           /* alt  */ (const char *) "..xx ",
                           /* alt  */ (const char *) ".... ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "     ",
                           /* alt  */ (const char *) ".... ",
                           /* alt  */ (const char *) ".xx. ",
                           /* alt  */ (const char *) ".xx. ",
                           /* alt  */ (const char *) ".... ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "      ",
                   /* frag   */ (const char *) "..... ",
                   /* frag   */ (const char *) "..... ",
                   /* frag   */ (const char *) "..... ",
                   /* frag   */ (const char *) "..... ",
                   /* frag   */ (const char *) "..... ", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "      ",
                           /* alt  */ (const char *) "...xx ",
                           /* alt  */ (const char *) ".x..x ",
                           /* alt  */ (const char *) "..x.. ",
                           /* alt  */ (const char *) "...x. ",
                           /* alt  */ (const char *) "..... ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "      ",
                           /* alt  */ (const char *) "...xx ",
                           /* alt  */ (const char *) ".x..x ",
                           /* alt  */ (const char *) ".x... ",
                           /* alt  */ (const char *) ".xxx. ",
                           /* alt  */ (const char *) "..... ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "      ",
                           /* alt  */ (const char *) "...xx ",
                           /* alt  */ (const char *) "....x ",
                           /* alt  */ (const char *) "..... ",
                           /* alt  */ (const char *) "..... ",
                           /* alt  */ (const char *) "..... ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "      ",
                           /* alt  */ (const char *) "..xxx ",
                           /* alt  */ (const char *) "....x ",
                           /* alt  */ (const char *) "..x.x ",
                           /* alt  */ (const char *) "..... ",
                           /* alt  */ (const char *) "..... ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "     ",
                   /* frag   */ (const char *) "     ",
                   /* frag   */ (const char *) "     ",
                   /* frag   */ (const char *) "     ",
                   /* frag   */ (const char *) "     ", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "     ",
                           /* alt  */ (const char *) " ~   ",
                           /* alt  */ (const char *) "  ~~ ",
                           /* alt  */ (const char *) " ~ ~ ",
                           /* alt  */ (const char *) "     ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "     ",
                           /* alt  */ (const char *) " ~~  ",
                           /* alt  */ (const char *) "  ~~ ",
                           /* alt  */ (const char *) " ~ ~ ",
                           /* alt  */ (const char *) "     ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "  ~~~",
                           /* alt  */ (const char *) " ~~  ",
                           /* alt  */ (const char *) "  ~~ ",
                           /* alt  */ (const char *) " ~~~ ",
                           /* alt  */ (const char *) "~~   ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "~~~  ",
                           /* alt  */ (const char *) " ~~~ ",
                           /* alt  */ (const char *) " ~~~~",
                           /* alt  */ (const char *) " ~~~~",
                           /* alt  */ (const char *) "~~ ~~", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "~    ",
                           /* alt  */ (const char *) " ~~~ ",
                           /* alt  */ (const char *) "  ~~~",
                           /* alt  */ (const char *) "  ~~~",
                           /* alt  */ (const char *) "   ~~", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xT~~x",
                           /* alt  */ (const char *) "x~~~x",
                           /* alt  */ (const char *) "xm~$x",
                           /* alt  */ (const char *) "xxxxx", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xT~~x",
                           /* alt  */ (const char *) "x~~~x",
                           /* alt  */ (const char *) "x~~$x",
                           /* alt  */ (const char *) "xxxxx", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xTCCx",
                           /* alt  */ (const char *) "xCCCx",
                           /* alt  */ (const char *) "xCC$x",
                           /* alt  */ (const char *) "xxxxx", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xTLLx",
                           /* alt  */ (const char *) "xLLLx",
                           /* alt  */ (const char *) "xLL$x",
                           /* alt  */ (const char *) "xxxxx", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xT..x",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "x..$x",
                           /* alt  */ (const char *) "xxxxx", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x.$mx",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "x.T.x",
                           /* alt  */ (const char *) "xxxxx", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x.$.x",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "x.T.x",
                           /* alt  */ (const char *) "xxxxx", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x~$~x",
                           /* alt  */ (const char *) "x~~~x",
                           /* alt  */ (const char *) "x~T~x",
                           /* alt  */ (const char *) "xxxxx", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "....m",
                           /* alt  */ (const char *) ".CCC.",
                           /* alt  */ (const char *) ".C$C.",
                           /* alt  */ (const char *) ".CCC.",
                           /* alt  */ (const char *) "T....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "xxxxx",
                   /* frag   */ (const char *) "x....",
                   /* frag   */ (const char *) "x....",
                   /* frag   */ (const char *) "x....",
                   /* frag   */ (const char *) "x....", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xbb..",
                           /* alt  */ (const char *) "xb...",
                           /* alt  */ (const char *) "x....",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xbbx.",
                           /* alt  */ (const char *) "xb.x.",
                           /* alt  */ (const char *) "xxDx.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xm~~~",
                           /* alt  */ (const char *) "x~~~.",
                           /* alt  */ (const char *) "x~~..",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x....",
                           /* alt  */ (const char *) "x..x.",
                           /* alt  */ (const char *) "x.xx.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x..C.",
                           /* alt  */ (const char *) "x.bx.",
                           /* alt  */ (const char *) "x.xx.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xCC..",
                           /* alt  */ (const char *) "xC...",
                           /* alt  */ (const char *) "x..x.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x..x.",
                           /* alt  */ (const char *) "x....",
                           /* alt  */ (const char *) "xx.x.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x$mx.",
                           /* alt  */ (const char *) "x..C.",
                           /* alt  */ (const char *) "xxCx.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x....",
                           /* alt  */ (const char *) "x..x.",
                           /* alt  */ (const char *) "x..x.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x$.C.",
                           /* alt  */ (const char *) "x..C.",
                           /* alt  */ (const char *) "xCCC.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xTCC.",
                           /* alt  */ (const char *) "xCCC.",
                           /* alt  */ (const char *) "xCCC.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x$.C.",
                           /* alt  */ (const char *) "x.C..",
                           /* alt  */ (const char *) "xC...",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "vvvvx",
                           /* alt  */ (const char *) "v$.v.",
                           /* alt  */ (const char *) "v..v.",
                           /* alt  */ (const char *) "vvsv.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "vvvvx",
                           /* alt  */ (const char *) "v$mv.",
                           /* alt  */ (const char *) "v..v.",
                           /* alt  */ (const char *) "vvsv.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_V_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "vvvvx",
                           /* alt  */ (const char *) "vm$v.",
                           /* alt  */ (const char *) "v$mv.",
                           /* alt  */ (const char *) "vvsv.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_VV_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "vvvvx",
                           /* alt  */ (const char *) "v$$v.",
                           /* alt  */ (const char *) "v$$v.",
                           /* alt  */ (const char *) "vvsv.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_VV_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "vvvvx",
                           /* alt  */ (const char *) "vb$v.",
                           /* alt  */ (const char *) "v..v.",
                           /* alt  */ (const char *) "vvvx.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_VV_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "vvvvx",
                           /* alt  */ (const char *) "v$Cv.",
                           /* alt  */ (const char *) "vCCv.",
                           /* alt  */ (const char *) "vvsv.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_VV_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "vvvvx",
                           /* alt  */ (const char *) "vTCv.",
                           /* alt  */ (const char *) "vCCv.",
                           /* alt  */ (const char *) "vvsv.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_VV_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "vvvvx",
                           /* alt  */ (const char *) "vT.v.",
                           /* alt  */ (const char *) "v..v.",
                           /* alt  */ (const char *) "vvsv.",
                           /* alt  */ (const char *) "x....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "xxxxx",
                   /* frag   */ (const char *) "x...x",
                   /* frag   */ (const char *) "x...x",
                   /* frag   */ (const char *) "x...x",
                   /* frag   */ (const char *) "x...x", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "vvvvv",
                           /* alt  */ (const char *) "vB$Bv",
                           /* alt  */ (const char *) "v...v",
                           /* alt  */ (const char *) "vvsvv",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xB$Bx",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "xCCCx",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xx.xx",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x|T|x",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "xCTCx",
                           /* alt  */ (const char *) "xCCCx",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x.$.x",
                           /* alt  */ (const char *) "xCCCx",
                           /* alt  */ (const char *) "xCCCx",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxxx",
                           /* alt  */ (const char *) "x.$mx",
                           /* alt  */ (const char *) "xCCCx",
                           /* alt  */ (const char *) "xCCCx",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "xxxx",
                   /* frag   */ (const char *) "x..x",
                   /* frag   */ (const char *) "x..x",
                   /* frag   */ (const char *) "x..x",
                   /* frag   */ (const char *) "x..x", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "vvvv",
                           /* alt  */ (const char *) "vB$v",
                           /* alt  */ (const char *) "v..v",
                           /* alt  */ (const char *) "vvsv",
                           /* alt  */ (const char *) "x..x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxx",
                           /* alt  */ (const char *) "xB$x",
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "xCCx",
                           /* alt  */ (const char *) "x..x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxx",
                           /* alt  */ (const char *) "xx.x",
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "x..x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "xxxx",
                   /* frag   */ (const char *) "x..x",
                   /* frag   */ (const char *) "x..x",
                   /* frag   */ (const char *) "x..x", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x$Cx",
                           /* alt  */ (const char *) "xCCx",
                           /* alt  */ (const char *) "xCCx",
                           /* alt  */ (const char *) "x..x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "vvvv",
                           /* alt  */ (const char *) "v$sv",
                           /* alt  */ (const char *) "vvvv",
                           /* alt  */ (const char *) "x..x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "....",
                           /* alt  */ (const char *) "....",
                           /* alt  */ (const char *) "....",
                           /* alt  */ (const char *) "....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".$..",
                           /* alt  */ (const char *) "B..B",
                           /* alt  */ (const char *) "CCCC",
                           /* alt  */ (const char *) "....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxx",
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "x.xx",
                           /* alt  */ (const char *) "x..x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxxx",
                           /* alt  */ (const char *) "x.$x",
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "x..x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "xxx",
                   /* frag   */ (const char *) "x.x",
                   /* frag   */ (const char *) "x.x",
                   /* frag   */ (const char *) "x.x", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "x.x",
                           /* alt  */ (const char *) "x.x",
                           /* alt  */ (const char *) "x.x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "x.x",
                           /* alt  */ (const char *) "x.x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x$x",
                           /* alt  */ (const char *) "xCx",
                           /* alt  */ (const char *) "xCx",
                           /* alt  */ (const char *) "x.x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxx",
                           /* alt  */ (const char *) "x$x",
                           /* alt  */ (const char *) "xCx",
                           /* alt  */ (const char *) "x.x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "t$t",
                           /* alt  */ (const char *) "B.B",
                           /* alt  */ (const char *) "CCC",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "C$C",
                           /* alt  */ (const char *) "B.B",
                           /* alt  */ (const char *) "CCC",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxx",
                           /* alt  */ (const char *) "xmx",
                           /* alt  */ (const char *) "x.x",
                           /* alt  */ (const char *) "x.x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxx",
                           /* alt  */ (const char *) "x$x",
                           /* alt  */ (const char *) "x.x",
                           /* alt  */ (const char *) "x.x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "xxx",
                   /* frag   */ (const char *) ".x.",
                   /* frag   */ (const char *) ".x.",
                   /* frag   */ (const char *) ".x.", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxx",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) ".x.",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxx",
                           /* alt  */ (const char *) ".D.",
                           /* alt  */ (const char *) ".x.",
                           /* alt  */ (const char *) ".x.", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "xxx",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) ".m.",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "     ",
                   /* frag   */ (const char *) " ....",
                   /* frag   */ (const char *) " ....",
                   /* frag   */ (const char *) " ....",
                   /* frag   */ (const char *) " ....", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "$.   ",
                           /* alt  */ (const char *) "..|..",
                           /* alt  */ (const char *) " |...",
                           /* alt  */ (const char *) " ....",
                           /* alt  */ (const char *) " ....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "m.   ",
                           /* alt  */ (const char *) "..C..",
                           /* alt  */ (const char *) " CC..",
                           /* alt  */ (const char *) " ....",
                           /* alt  */ (const char *) " ....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "m.   ",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) " ....",
                           /* alt  */ (const char *) " ....",
                           /* alt  */ (const char *) " ....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "m..  ",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) " ....",
                           /* alt  */ (const char *) " ....",
                           /* alt  */ (const char *) " ....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".... ",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) " ....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "     ",
                           /* alt  */ (const char *) " ....",
                           /* alt  */ (const char *) " .x..",
                           /* alt  */ (const char *) " ....",
                           /* alt  */ (const char *) " ....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "$``  ",
                           /* alt  */ (const char *) "```..",
                           /* alt  */ (const char *) " ``..",
                           /* alt  */ (const char *) " `...",
                           /* alt  */ (const char *) " ...m", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "m``  ",
                           /* alt  */ (const char *) "```..",
                           /* alt  */ (const char *) " ``..",
                           /* alt  */ (const char *) " `...",
                           /* alt  */ (const char *) " ....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "C``  ",
                           /* alt  */ (const char *) "```..",
                           /* alt  */ (const char *) " ``..",
                           /* alt  */ (const char *) " `...",
                           /* alt  */ (const char *) " ....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) ".....",
                   /* frag   */ (const char *) ".....",
                   /* frag   */ (const char *) ".....",
                   /* frag   */ (const char *) ".....",
                   /* frag   */ (const char *) ".....", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".x...",
                           /* alt  */ (const char *) "..m..",
                           /* alt  */ (const char *) "...x.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".vsv.",
                           /* alt  */ (const char *) ".vTv.",
                           /* alt  */ (const char *) ".vvv.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".vvv.",
                           /* alt  */ (const char *) ".vCs.",
                           /* alt  */ (const char *) ".vvv.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".vvv.",
                           /* alt  */ (const char *) ".v$v.",
                           /* alt  */ (const char *) ".vvv.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".xmD.",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".x...",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) "...x.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".x...",
                           /* alt  */ (const char *) ".xx..",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".x...",
                           /* alt  */ (const char *) "..x..",
                           /* alt  */ (const char *) "...x.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) "..x..",
                           /* alt  */ (const char *) "..x..",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".x.x.",
                           /* alt  */ (const char *) ".x.x.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".CCC.",
                           /* alt  */ (const char *) ".C$C.",
                           /* alt  */ (const char *) ".CCC.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".x.x.",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".x.x.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".x.x.",
                           /* alt  */ (const char *) "..|..",
                           /* alt  */ (const char *) ".x.x.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".xCx.",
                           /* alt  */ (const char *) ".C|C.",
                           /* alt  */ (const char *) ".xCx.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "..~..",
                           /* alt  */ (const char *) ".x~x.",
                           /* alt  */ (const char *) "~~|~~",
                           /* alt  */ (const char *) ".x~x.",
                           /* alt  */ (const char *) "..~..", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".vvv.",
                           /* alt  */ (const char *) ".vmv.",
                           /* alt  */ (const char *) ".vsv.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".vvv.",
                           /* alt  */ (const char *) ".vMv.",
                           /* alt  */ (const char *) ".vsv.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) "..xx.",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".xx..",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) "..x..",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) "..x..",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".LLL.",
                           /* alt  */ (const char *) ".L|L.",
                           /* alt  */ (const char *) ".LLL.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) "..mx.",
                           /* alt  */ (const char *) "...x.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) "...x.",
                           /* alt  */ (const char *) "..gx.",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".|.|.",
                           /* alt  */ (const char *) "..m..",
                           /* alt  */ (const char *) ".|.|.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".CCm.",
                           /* alt  */ (const char *) ".CCC.",
                           /* alt  */ (const char *) "..CC.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".LLm.",
                           /* alt  */ (const char *) ".LLL.",
                           /* alt  */ (const char *) "..LL.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".~~m.",
                           /* alt  */ (const char *) ".~~~.",
                           /* alt  */ (const char *) "..~~.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) "..m..",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) "..M..",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".x.x.",
                           /* alt  */ (const char *) "..g..",
                           /* alt  */ (const char *) ".x.x.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_V_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".gxg.",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_VV_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".xxx.",
                           /* alt  */ (const char *) ".gxg.",
                           /* alt  */ (const char *) ".xgx.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_VVV_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) ".xgx.",
                           /* alt  */ (const char *) ".gxg.",
                           /* alt  */ (const char *) ".xgx.",
                           /* alt  */ (const char *) ".....", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "x...x",
                   /* frag   */ (const char *) "x...x",
                   /* frag   */ (const char *) "x...x",
                   /* frag   */ (const char *) "x...x",
                   /* frag   */ (const char *) "x...x", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "B...B",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "B...B",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "|...|",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "|...|",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "xx.xx",
                           /* alt  */ (const char *) "xxCxx",
                           /* alt  */ (const char *) "xx.xx",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "xxCxx",
                           /* alt  */ (const char *) "xx.xx",
                           /* alt  */ (const char *) "xxCxx",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "xC=Cx",
                           /* alt  */ (const char *) "xC=Cx",
                           /* alt  */ (const char *) "xC=Cx",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "xx.xx",
                           /* alt  */ (const char *) "xxtxx",
                           /* alt  */ (const char *) "xx.xx",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "xx.xx",
                           /* alt  */ (const char *) "xx.xx",
                           /* alt  */ (const char *) "xx.xx",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "xx.xx",
                           /* alt  */ (const char *) "xxmxx",
                           /* alt  */ (const char *) "xx.xx",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "v.vvv",
                           /* alt  */ (const char *) "v.v$v",
                           /* alt  */ (const char *) "v.vvv",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "xxx.x",
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "x.xxx",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x...x",
                           /* alt  */ (const char *) "xxx.x",
                           /* alt  */ (const char *) "x.C.x",
                           /* alt  */ (const char *) "x.xxx",
                           /* alt  */ (const char *) "x...x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x.~.x",
                           /* alt  */ (const char *) "x~~~x",
                           /* alt  */ (const char *) "x~~~x",
                           /* alt  */ (const char *) "x~~~x",
                           /* alt  */ (const char *) "x.~.x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "x..x",
                   /* frag   */ (const char *) "x..x",
                   /* frag   */ (const char *) "x..x",
                   /* frag   */ (const char *) "x..x",
                   /* frag   */ (const char *) "x..x", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "B..B",
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "B..B",
                           /* alt  */ (const char *) "x..x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "|..|",
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "|..|",
                           /* alt  */ (const char *) "x..x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "x.Cx",
                           /* alt  */ (const char *) "xCCx",
                           /* alt  */ (const char *) "xC.x",
                           /* alt  */ (const char *) "x..x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "xx.x",
                           /* alt  */ (const char *) "x..x",
                           /* alt  */ (const char *) "x.xx",
                           /* alt  */ (const char *) "x..x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "x~~x",
                           /* alt  */ (const char *) "xx~x",
                           /* alt  */ (const char *) "x~~x",
                           /* alt  */ (const char *) "x~xx",
                           /* alt  */ (const char *) "x~~x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "*xx*",
                   /* frag   */ (const char *) ".^^.",
                   /* frag   */ (const char *) "*xx*", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) " xx ",
                           /* alt  */ (const char *) "....",
                           /* alt  */ (const char *) " xx ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) " xx ",
                           /* alt  */ (const char *) ".D..",
                           /* alt  */ (const char *) " xx ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) " xx ",
                           /* alt  */ (const char *) ".##.",
                           /* alt  */ (const char *) " xx ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) " xx ",
                           /* alt  */ (const char *) ".DC.",
                           /* alt  */ (const char *) " xx ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) " x| ",
                           /* alt  */ (const char *) ".D..",
                           /* alt  */ (const char *) " x| ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) " xB ",
                           /* alt  */ (const char *) ".D..",
                           /* alt  */ (const char *) " xB ", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "...",
                   /* frag   */ (const char *) "...",
                   /* frag   */ (const char *) "...", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) ".x.",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) ".|.",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) ".m.",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) ".g.",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) ".M.",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) ".G.",
                           /* alt  */ (const char *) "...", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "~=~",
                   /* frag   */ (const char *) "~=~",
                   /* frag   */ (const char *) "~=~",
                   /* frag   */ (const char *) "~=~",
                   /* frag   */ (const char *) "~=~",
                   /* frag   */ (const char *) "~=~",
                   /* frag   */ (const char *) "~=~", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "B.B",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "B.B",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~m~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~.~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~~~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~",
                           /* alt  */ (const char *) "~=~", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "C=C",
                   /* frag   */ (const char *) "C=C",
                   /* frag   */ (const char *) "C=C",
                   /* frag   */ (const char *) "C=C",
                   /* frag   */ (const char *) "C=C",
                   /* frag   */ (const char *) "C=C",
                   /* frag   */ (const char *) "C=C", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "B.B",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "B.B",
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "C=C", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "B.B",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "...",
                           /* alt  */ (const char *) "B.B", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "CmC",
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "C=C", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "C.C",
                           /* alt  */ (const char *) "CCC",
                           /* alt  */ (const char *) "C.C",
                           /* alt  */ (const char *) "C=C",
                           /* alt  */ (const char *) "C=C", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "CC=CC",
                   /* frag   */ (const char *) "CC=CC",
                   /* frag   */ (const char *) "CC=CC",
                   /* frag   */ (const char *) "CC=CC",
                   /* frag   */ (const char *) "CC=CC",
                   /* frag   */ (const char *) "CC=CC",
                   /* frag   */ (const char *) "CC=CC", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "CC=CC",
                           /* alt  */ (const char *) "CC=CC",
                           /* alt  */ (const char *) "CB.BC",
                           /* alt  */ (const char *) "C...C",
                           /* alt  */ (const char *) "..m..",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) "B.$.B", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* frag   */ (const char *) "CC=CC",
                           /* frag   */ (const char *) "CC=CC",
                           /* frag   */ (const char *) ".CCC.",
                           /* frag   */ (const char *) ".CCC.",
                           /* frag   */ (const char *) "CC=CC",
                           /* frag   */ (const char *) "CC=CC",
                           /* frag   */ (const char *) "CC=CC", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "CC=CC",
                           /* alt  */ (const char *) "CC=CC",
                           /* alt  */ (const char *) "CC=CC",
                           /* alt  */ (const char *) "....C",
                           /* alt  */ (const char *) "....C",
                           /* alt  */ (const char *) ".....",
                           /* alt  */ (const char *) "$...m", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ROOM_FLAG_ALL, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "CC=CC",
                           /* alt  */ (const char *) "CC=CC",
                           /* alt  */ (const char *) "CC=CC",
                           /* alt  */ (const char *) "....C",
                           /* alt  */ (const char *) "....C",
                           /* alt  */ (const char *) "b....",
                           /* alt  */ (const char *) "bbb..", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_UNLIKELY, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "CC=CC",
                           /* alt  */ (const char *) "CC=CC",
                           /* alt  */ (const char *) "CC.CC",
                           /* alt  */ (const char *) "CC.CC",
                           /* alt  */ (const char *) "CC.CC",
                           /* alt  */ (const char *) "$...m",
                           /* alt  */ (const char *) "B...B", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }

  if (fragment_add(g, CHANCE_NORMAL, __FUNCTION__, __LINE__,
                   /* frag   */ (const char *) "xxxxxxx",
                   /* frag   */ (const char *) "x.....x",
                   /* frag   */ (const char *) "x.....x",
                   /* frag   */ (const char *) "x.....x",
                   /* frag   */ (const char *) "x.....x",
                   /* frag   */ (const char *) "x.....x", nullptr)) {
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "B..g..B",
                           /* alt  */ (const char *) ".......",
                           /* alt  */ (const char *) ".B...B$",
                           /* alt  */ (const char *) "xxCCCxx",
                           /* alt  */ (const char *) "xB...Bx",
                           /* alt  */ (const char *) "x.....x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "B..g..B",
                           /* alt  */ (const char *) ".......",
                           /* alt  */ (const char *) ".BC=CB$",
                           /* alt  */ (const char *) "xxC=Cxx",
                           /* alt  */ (const char *) "xBC=CBx",
                           /* alt  */ (const char *) "x.....x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "B..G..B",
                           /* alt  */ (const char *) ".......",
                           /* alt  */ (const char *) ".B...B$",
                           /* alt  */ (const char *) "xxCCCxx",
                           /* alt  */ (const char *) "xxCCCxx",
                           /* alt  */ (const char *) "x.....x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
    if (! fragment_alt_add(g, CHANCE_NORMAL, ~ROOM_FLAG_ENTRANCE, __FUNCTION__, __LINE__,
                           /* alt  */ (const char *) "B..gvvv",
                           /* alt  */ (const char *) "....s.v",
                           /* alt  */ (const char *) "B...v$v",
                           /* alt  */ (const char *) "xxxDvvv",
                           /* alt  */ (const char *) "x.B.B.x",
                           /* alt  */ (const char *) "x.....x", nullptr)) {
      CROAK("failed to add fragment alt");
    }
  }
}
