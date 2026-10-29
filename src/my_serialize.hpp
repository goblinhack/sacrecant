//
// Copyright goblinhack@gmail.com
//

#ifndef MY_SERIALIZE_HPP
#define MY_SERIALIZE_HPP

#include "c_plus_plus_serializer.hpp"
#include "my_hiscore.hpp"

#include "my_sdl_compat.hpp"

auto operator<<(std::ostream &out, Bits< const HiScore & > my) -> std::ostream &;  // save
auto operator>>(std::istream &in, Bits< HiScore & > my) -> std::istream &;         // load
auto operator<<(std::ostream &out, Bits< const HiScores & > my) -> std::ostream &; // save
auto operator>>(std::istream &in, Bits< HiScores & > my) -> std::istream &;        // load
auto operator>>(std::istream &in, Bits< SDL_Keysym & > my) -> std::istream &;
auto operator<<(std::ostream &out, Bits< const SDL_Keysym & > my) -> std::ostream &;

#endif
