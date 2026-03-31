//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_game.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_serialize.hpp"
#include "my_time.hpp"

HiScores::HiScores()
{
  TRACE();
  if (static_cast< int >(hiscores.size()) > HiScore::max) {
    hiscores.resize(HiScore::max);
  } else {
    while (static_cast< int >(hiscores.size()) < HiScore::max) {
      hiscores.emplace_back("AAA", "", "",
                            0, // score
                            0  // levels_completed
      );
    }
  }
}

HiScores::~HiScores() { TRACE(); }

void HiScores::add_new_hiscore(Gamep g, uint32_t score, LevelNum level_num, const std::string &name, const std::string &reason)
{
  TRACE();
  auto        h = hiscores.begin();
  std::string hiscore_name;

  hiscore_name = name;

  while (h != hiscores.end()) {
    if (score > h->score) {
      hiscores.insert(h, HiScore(hiscore_name, reason, current_date(), score, level_num));

      hiscores.resize(HiScore::max);

      DBG("save hiscore config");
      game_save_config(g);
      return;
    }

    h++;
  }
}

auto HiScores::is_new_hiscore(uint32_t score) -> bool
{
  TRACE();
  auto h = hiscores.begin();

  if (score == 0) {
    //
    // Poor player.
    //
    return false;
  }

  while (h != hiscores.end()) {
    if (score > h->score) {
      return true;
    }
    h++;
  }

  return false;
}

auto HiScores::is_new_highest_hiscore(uint32_t score) -> bool
{
  auto h = hiscores.begin();

  if (score == 0) {
    //
    // Poor player.
    //
    return false;
  }

  while (h != hiscores.end()) {
    return score > h->score;
  }
  return false;
}

auto HiScores::place_str(uint32_t score) -> const char *
{
  // NOLINTBEGIN
  const char *which[ HiScore::max_displayed ] = {
      "first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth", "ninth", "tenth",
  };
  // NOLINTEND

  if (score == 0) {
    //
    // Poor player.
    //
    return "";
  }

  auto h = hiscores.begin();

  while (h != hiscores.end()) {
    if (score > h->score) {
      return which[ h - hiscores.begin() ];
    }
    h++;
  }
  return "";
}

auto operator>>(std::istream &in, Bits< HiScore & > my) -> std::istream &
{
  TRACE();
  in >> bits(my.t.name);
  in >> bits(my.t.reason);
  in >> bits(my.t.when);
  in >> bits(my.t.score);
  in >> bits(my.t.levels_completed);
  DBG("read hiscore: who:%s score:%d demise:%s when:%s", my.t.name.c_str(), my.t.score, my.t.reason.c_str(), my.t.when.c_str());
  return in;
}

auto operator<<(std::ostream &out, Bits< const HiScore & > const my) -> std::ostream &
{
  TRACE();
  out << bits(my.t.name);
  out << bits(my.t.reason);
  out << bits(my.t.when);
  out << bits(my.t.score);
  out << bits(my.t.levels_completed);
  DBG("saved hiscore: who:%s score:%d demise:%s when:%s", my.t.name.c_str(), my.t.score, my.t.reason.c_str(), my.t.when.c_str());
  return out;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// It looks almost identical to the above. However const is missing here. The compiler
// will if it does not find a functon matching this non const, will use some other
// means of writing the HiScore and hence loading will fail.
//
// DO NOT REMOVE THIS FUNCTION.
//
// vvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
//////////////////////////////////////////////////////////////////////////////////////
static auto operator<<(std::ostream &out, Bits< HiScore & > const my) -> std::ostream &
{
  TRACE();
  out << bits(my.t.name);
  out << bits(my.t.reason);
  out << bits(my.t.when);
  out << bits(my.t.score);
  out << bits(my.t.levels_completed);
  DBG("saved Hiscore: who:%s score:%d demise:%s when:%s", my.t.name.c_str(), my.t.score, my.t.reason.c_str(), my.t.when.c_str());
  return out;
}
//////////////////////////////////////////////////////////////////////////////////////
//
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
// DO NOT REMOVE THIS FUNCTION.
//
// It looks almost identical to the above. However const is missing here. The compiler
// will if it does not find a functon matching this non const, will use some other
// means of writing the HiScore and hence loading will fail.
//
//////////////////////////////////////////////////////////////////////////////////////

auto operator>>(std::istream &in, Bits< HiScores & > my) -> std::istream &
{
  TRACE();
  my.t.hiscores.resize(0);
  in >> bits(my.t.hiscores);

  return in;
}

auto operator<<(std::ostream &out, Bits< const HiScores & > const my) -> std::ostream &
{
  TRACE();
  out << bits(my.t.hiscores);
  return out;
}
