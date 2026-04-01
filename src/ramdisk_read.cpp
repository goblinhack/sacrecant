//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_main.hpp"
#include "my_ptrcheck.hpp"
#include "my_ramdisk.hpp"

#include <cstdlib>
#include <cstring>

std::unordered_map< std::string, RamdiskT > ramdisk_data;

auto ramdisk_load(const char *filename, int *outlen) -> uint8_t *
{
  TRACE();

  auto f = ramdisk_data.find(filename);
  if (f == ramdisk_data.end()) {
    return nullptr;
  }

  auto ramfile = f->second;

  auto *out = MYMALLOC(ramfile.len + 1, "ramdisk load");
  if (out == nullptr) {
    ERR("no memory for loading ramdisk out, %s", filename);
    return nullptr;
  }

  memcpy(out, const_cast<uint8_t *>(ramfile.data), ramfile.len);
  *(static_cast<uint8_t *>(out) + ramfile.len) = 0;
  *outlen                                = ramfile.len;

  return static_cast<uint8_t *>(out);
}
