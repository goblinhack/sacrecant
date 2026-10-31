//
// Copyright goblinhack@gmail.com
//

#include "my_hexdump.hpp"
#include "my_types.hpp"

#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

enum { HEX_DUMP_WIDTH = (16) };

//
// e.g.
// 00000000  02 00 00 00 09 00 00 00  76 65 63 2D 65 6C 65 6D |........vec-elem|
// 00000010  31 09 00 00 00 76 65 63  2D 65 6C 65 6D 32 02 00 |1....vec-elem2..|
// 00000020  00 00 0A 00 00 00 6C 69  73 74 2D 65 6C 65 6D 31 |......list-elem1|
// 00000030  0A 00 00 00 6C 69 73 74  2D 65 6C 65 6D 32 12 61 |....list-elem2.a|
// 00000040  72 72 2D 65 6C 65 6D 31  00 00 00 00 00 00 00 00 |rr-elem1........|
// 00000050  00 00 00 00 00 00 12 61  72 72 2D 65 6C 65 6D 32 |.......arr-elem2|
// 00000060  00 00 00 00 00 00 00 00  00 00 00 00 00 00 30 31 |..............01|
// 00000070  32 33 34 35 61 62 63 64  65 66 67 68 69 6A 6B 6C |2345abcdefghi
//
void hexdump(const uint8_t *addr, size_t len)
{
  int      skipping_blanks         = 0;
  uint8_t  empty[ HEX_DUMP_WIDTH ] = {0};
  uint8_t  buf[ HEX_DUMP_WIDTH + 1 ];
  auto    *pc = const_cast< uint8_t * >(addr);
  size_t   i  = 0;
  uint32_t x  = 0;

  std::cout << std::dec << len << " bytes:" << '\n';

  if (! static_cast< bool >(len)) {
    return;
  }

  for (i = 0, x = 0; i < len; i++, x++) {
    if ((i % HEX_DUMP_WIDTH) == 0) {
      if (skipping_blanks == 0) {
        if (i != 0) {
          std::cout << " |" << std::setw(HEX_DUMP_WIDTH) << buf << "|" << '\n';
        }
      }

      /*
       * Skip blank blocks.
       */
      if (memcmp(pc + i, empty, SIZEOF(empty)) == 0) {
        i += HEX_DUMP_WIDTH - 1;
        skipping_blanks = 1;
        buf[ 0 ]        = '\0';
        continue;
      }

      std::cout << "  " << std::setfill('0') << std::setw(4) << std::hex << i;

      x = 0;
    }

    if ((static_cast< bool >(x)) && (((i % (HEX_DUMP_WIDTH / 2))) == 0)) {
      std::cout << " ";
    }

    skipping_blanks = 0;

    std::cout << " " << std::setfill('0') << std::setw(2) << std::hex << static_cast< int >(pc[ i ]);

    if ((pc[ i ] < ' ') || (pc[ i ] > '~')) {
      buf[ i % HEX_DUMP_WIDTH ] = '.';
    } else {
      buf[ i % HEX_DUMP_WIDTH ] = pc[ i ];
    }

    buf[ (i % HEX_DUMP_WIDTH) + 1 ] = '\0';
  }

  if (! static_cast< bool >(buf[ 0 ])) {
    if (skipping_blanks != 0) {
      std::cout << "  *\n";
    }

    return;
  }

  while ((i % HEX_DUMP_WIDTH) != 0) {
    std::cout << "   ";
    if ((static_cast< bool >(i)) && (((i % (HEX_DUMP_WIDTH / 2))) == 0)) {
      std::cout << " ";
    }

    i++;
  }

  std::cout << " |" << std::setw(-HEX_DUMP_WIDTH) << buf << "|" << '\n';
}

void hexdump(std::vector< uint8_t > &v) { hexdump(v.data(), v.size()); }
