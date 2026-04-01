//
// Changed by goblinhack@gmail.com
//

#include "my_age_map_inlines.hpp"
#include "my_callstack.hpp"
#include "my_globals.hpp"
#include "my_level.hpp"
#include "my_level_inlines.hpp"
#include "my_main.hpp"

void age_map_incr(AgeMap *m, const bpoint &pov, const uint8_t val)
{
#ifdef DEBUG_BUILD
  if (is_oob(pov)) [[unlikely]] {
    ERR("overflow");
    return;
  }
#endif

  if (m != nullptr) {
    age_map_incr(m, pov.x, pov.y, static_cast< uint8_t >(val));
  }
}

[[nodiscard]] auto age_map_get(const AgeMap *m, const bpoint &pov) -> uint8_t
{
#ifdef DEBUG_BUILD
  if (is_oob(pov)) [[unlikely]] {
    ERR("overflow");
    return 0U;
  }
#endif
  if (m == nullptr) {
    return 0U;
  }

  return age_map_get(m, pov.x, pov.y);
}
