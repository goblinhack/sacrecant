//
// Copyright goblinhack@gmail.com
//

#ifndef MY_UI_WID_POPUP_HPP
#define MY_UI_WID_POPUP_HPP

#include <map>
#include <string>
#include <vector>

#include "my_wid_text_box.hpp"

class WidPopup
{
private:
  std::map< uint32_t, std::string > wid_popup_lines;
  std::vector< Widp >               children;

public:
  Widp        wid_popup_container {};
  Widp        wid_title {};
  WidTextBox *wid_text_area {};

  spoint      tl;
  spoint      br;
  Tilep       title_tile {};
  std::string background;
  int         outer_w {};
  int         outer_h {};
  int         inner_w {};
  int         inner_h {};
  std::string name;

  ~WidPopup();
  WidPopup(Gamep g, std::string name, spoint tl, spoint br, Tilep title_tile = nullptr, std::string background = "", bool horiz_scroll = true,
           bool vert_scoll = true, int scroll_height = -1);

  WidPopup(Gamep g, Widp parent, std::string name, spoint tl, spoint br, Tilep title_tile = nullptr, std::string background = "",
           bool horiz_scroll = true, bool vert_scoll = true, int scroll_height = -1);

  auto log(Gamep g, const std::string &s, wid_text_format format = TEXT_FORMAT_NONE, const std::string &c = "") const -> Widp;
  auto log_empty_line(Gamep g) const -> Widp;

  //
  // Get rid of trailing empty lines
  //
  void compress(Gamep g) const;
};

#endif
