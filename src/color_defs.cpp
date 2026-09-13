//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_color_defs.hpp"

// begin sort marker1 {
color BLACK;
color BLUE;
color GRAYBLUE;
color BLUE1;
color BLUE2;
color BLUE3;
color BLUE4;
color BROWN;
color BROWN1;
color BROWN2;
color BROWN3;
color BROWN4;
color COLOR_NONE;
color CYAN;
color CYAN1;
color CYAN2;
color CYAN3;
color CYAN4;
color GOLD;
color GOLD1;
color GOLD2;
color GOLD3;
color GOLD4;
color GRAY;
color GRAY10;
color GRAY20;
color GRAY30;
color GRAY40;
color GRAY5;
color GRAY50;
color GRAY60;
color GRAY70;
color GRAY80;
color GRAY90;
color GREEN;
color GREEN1;
color GREEN2;
color GREEN3;
color GREEN4;
color LIME;
color MAGENTA;
color MAGENTA1;
color MAGENTA2;
color MAGENTA3;
color MAGENTA4;
color NAVY;
color ORANGE;
color ORANGE1;
color ORANGE2;
color ORANGE3;
color ORANGE4;
color PINK;
color PINK1;
color PINK2;
color PINK3;
color PINK4;
color PURPLE;
color PURPLE1;
color PURPLE2;
color PURPLE3;
color PURPLE4;
color RED;
color RED1;
color RED2;
color RED3;
color RED4;
color TURQUOISE;
color TURQUOISE1;
color TURQUOISE2;
color TURQUOISE3;
color TURQUOISE4;
color WALL;
color WATER;
color WHITE;
color YELLOW;
color YELLOW1;
color YELLOW2;
color YELLOW3;
color YELLOW4;
// end sort marker1 }

static bool color_init_done;

void color_init()
{
  TRACE();

  color_init_done = true;

  // begin sort marker2 {
  color_set("black", &BLACK, 0, 0, 0, 255);
  color_set("blue", &BLUE, 0, 0, 255, 255);
  color_set("grayblue", &GRAYBLUE, 128, 170, 170, 255);
  color_set("blue1", &BLUE1, 0, 0, 255, 255);
  color_set("blue2", &BLUE2, 0, 0, 238, 255);
  color_set("blue3", &BLUE3, 0, 0, 205, 255);
  color_set("blue4", &BLUE4, 0, 0, 139, 255);
  color_set("brown", &BROWN, 165, 42, 42, 255);
  color_set("brown1", &BROWN1, 255, 64, 64, 255);
  color_set("brown2", &BROWN2, 238, 59, 59, 255);
  color_set("brown3", &BROWN3, 205, 51, 51, 255);
  color_set("brown4", &BROWN4, 139, 35, 35, 255);
  color_set("cyan", &CYAN, 0, 255, 255, 255);
  color_set("cyan1", &CYAN1, 0, 255, 255, 255);
  color_set("cyan2", &CYAN2, 0, 238, 238, 255);
  color_set("cyan3", &CYAN3, 0, 205, 205, 255);
  color_set("cyan4", &CYAN4, 0, 139, 139, 255);
  color_set("gold", &GOLD, 255, 215, 0, 255);
  color_set("gold1", &GOLD1, 255, 215, 0, 255);
  color_set("gold2", &GOLD2, 238, 201, 0, 255);
  color_set("gold3", &GOLD3, 205, 173, 0, 255);
  color_set("gold4", &GOLD4, 139, 117, 0, 255);
  color_set("gray", &GRAY, 190, 190, 190, 255);
  color_set("gray10", &GRAY10, 26, 26, 26, 255);
  color_set("gray20", &GRAY20, 51, 51, 51, 255);
  color_set("gray30", &GRAY30, 77, 77, 77, 255);
  color_set("gray40", &GRAY40, 102, 102, 102, 255);
  color_set("gray5", &GRAY5, 13, 13, 13, 255);
  color_set("gray50", &GRAY50, 127, 127, 127, 255);
  color_set("gray60", &GRAY60, 153, 153, 153, 255);
  color_set("gray70", &GRAY70, 179, 179, 179, 255);
  color_set("gray80", &GRAY80, 204, 204, 204, 255);
  color_set("gray90", &GRAY90, 229, 229, 229, 255);
  color_set("green", &GREEN, 0, 150, 100, 255);
  color_set("green1", &GREEN1, 0, 255, 0, 255);
  color_set("green2", &GREEN2, 0, 238, 0, 255);
  color_set("green3", &GREEN3, 0, 205, 0, 255);
  color_set("green4", &GREEN4, 0, 139, 0, 255);
  color_set("lime", &LIME, 50, 205, 50, 255);
  color_set("magenta", &MAGENTA, 255, 0, 255, 255);
  color_set("magenta1", &MAGENTA1, 255, 0, 255, 255);
  color_set("magenta2", &MAGENTA2, 238, 0, 238, 255);
  color_set("magenta3", &MAGENTA3, 205, 0, 205, 255);
  color_set("magenta4", &MAGENTA4, 139, 0, 139, 255);
  color_set("navy", &NAVY, 0, 0, 128, 255);
  color_set("none", &COLOR_NONE, 0, 0, 0, 0);
  color_set("orange", &ORANGE, 255, 165, 0, 255);
  color_set("orange1", &ORANGE1, 255, 165, 0, 255);
  color_set("orange2", &ORANGE2, 238, 154, 0, 255);
  color_set("orange3", &ORANGE3, 205, 133, 0, 255);
  color_set("orange4", &ORANGE4, 139, 90, 0, 255);
  color_set("pink", &PINK, 255, 192, 203, 255);
  color_set("pink1", &PINK1, 255, 181, 197, 255);
  color_set("pink2", &PINK2, 238, 169, 184, 255);
  color_set("pink3", &PINK3, 205, 145, 158, 255);
  color_set("pink4", &PINK4, 139, 99, 108, 255);
  color_set("purple", &PURPLE, 160, 32, 240, 255);
  color_set("purple1", &PURPLE1, 155, 48, 255, 255);
  color_set("purple2", &PURPLE2, 145, 44, 238, 255);
  color_set("purple3", &PURPLE3, 125, 38, 205, 255);
  color_set("purple4", &PURPLE4, 85, 26, 139, 255);
  color_set("red", &RED, 204, 42, 96, 255);
  color_set("red1", &RED1, 255, 0, 0, 255);
  color_set("red2", &RED2, 238, 0, 0, 255);
  color_set("red3", &RED3, 205, 0, 0, 255);
  color_set("red4", &RED4, 139, 0, 0, 255);
  color_set("turquoise", &TURQUOISE, 64, 224, 208, 255);
  color_set("turquoise1", &TURQUOISE1, 0, 245, 255, 255);
  color_set("turquoise2", &TURQUOISE2, 0, 229, 238, 255);
  color_set("turquoise3", &TURQUOISE3, 0, 197, 205, 255);
  color_set("turquoise4", &TURQUOISE4, 0, 134, 139, 255);
  color_set("wall", &WALL, 247, 188, 193, 255);
  color_set("water", &WATER, 84, 84, 232, 255);
  color_set("white", &WHITE, 255, 255, 255, 255);
  color_set("yellow", &YELLOW, 255, 255, 0, 255);
  color_set("yellow1", &YELLOW1, 255, 255, 0, 255);
  color_set("yellow2", &YELLOW2, 238, 238, 0, 255);
  color_set("yellow3", &YELLOW3, 205, 205, 0, 255);
  color_set("yellow4", &YELLOW4, 139, 139, 0, 255);
  // end sort marker2 }
}
