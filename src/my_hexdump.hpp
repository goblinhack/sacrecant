//
// Copyright goblinhack@gmail.com
//

#ifndef MY_HEXDUMP_HPP
#define MY_HEXDUMP_HPP

#include "my_types.hpp"

#include <vector>

auto hexdump(const uint8_t *addr, size_t len) -> void;
auto hexdump(std::vector< uint8_t > &v) -> void;

#endif
