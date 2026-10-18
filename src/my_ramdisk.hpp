//
// Copyright goblinhack@gmail.com
//

#ifndef MY_RAMDISK_HPP
#define MY_RAMDISK_HPP

#include <cstdint>
#include <string>
#include <unordered_map>

using ramdisk_t = struct RamdiskT {
  const unsigned char *data;
  int                  len;
};

extern std::unordered_map< std::string, RamdiskT > ramdisk_data;

extern auto ramdisk_load(const char *filename, int *outlen) -> unsigned char *;
extern void ramdisk_init();

#endif
