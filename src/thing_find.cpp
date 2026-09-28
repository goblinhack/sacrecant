//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_level.hpp"
#include "my_main.hpp"
#include "my_thing.hpp"
#include "my_types.hpp"
#include <cinttypes>

//
// Slower version, packed with extra warnings
//
[[nodiscard]] auto thing_find_non_inline(Gamep g, Levelsp v, ThingId id) -> Thingp
{
  TRACE_DEBUG(); // expensive

  ThingIdPacked id_packed = {};
  id_packed.a.val         = id;
  auto arr_index          = id_packed.c.arr_index;

  auto *t = &v->thing_body[ arr_index ];
  if (t == nullptr) [[unlikely]] {
    CROAK("thing not found as id %08" PRIX32 //
          " (level: %" PRIu32                //
          " id: %08" PRIX32                  //
          " entropy: %08" PRIX32             //
          ")",                               //
          id,                                //
          id_packed.b.level_num,             //
          id_packed.b.per_level_id,          //
          id_packed.b.entropy);
  }

  if ((t->id != id)) [[unlikely]] {
    ThingIdPacked id_found = {};
    id_found.a.val         = t->id;

    CROAK("thing %p found as id %08" PRIX32                      //
          " (level: %" PRIu32                                    //
          " id: %08" PRIX32                                      //
          " entropy: %08" PRIX32                                 //
          "), but entropy mismatch with expected id, %08" PRIX32 //
          " (level: %" PRIu32                                    //
          " id: %08" PRIX32                                      //
          " entropy: %08" PRIX32                                 //
          ")",                                                   //
          (void *) t,
          t->id,                    //
          id_found.b.level_num,     //
          id_found.b.per_level_id,  //
          id_found.b.entropy,       //
          id,                       //
          id_packed.b.level_num,    //
          id_packed.b.per_level_id, //
          id_packed.b.entropy);
  }

  return t;
}
