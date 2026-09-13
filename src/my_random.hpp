#ifndef MY_RANDOM_HPP
#define MY_RANDOM_HPP

////////////////////////////////////////////////////////////////////////////
// Random numbers
////////////////////////////////////////////////////////////////////////////

//
// Enhanced random number mob.
//
#include "my_pcg_basic.hpp"

//
// For randomness that matters for reproducibility.
//
#define PCG_RAND()                       pcg32_random(__FUNCTION__, __LINE__)
#define PCG_SRAND(a)                     pcg32_srandom(a, a)
#define PCG_RANDOM_RANGE(a, b)           ((b) > (a) ? pcg32_boundedrand(__FUNCTION__, __LINE__, (b) - (a)) + (a) : (a))
#define PCG_RANDOM_RANGE_INCLUSIVE(a, b) ((b) > (a) ? pcg32_boundedrand(__FUNCTION__, __LINE__, (b) - (a) + 1) + (a) : (a))

//
// For randomness that does not matter for reproducibility.
//
#define OS_RAND()                       rand()
#define OS_SRAND(a)                     srand(a)
#define OS_RANDOM_RANGE(a, b)           ((b) > (a) ? rand() % ((b) - (a)) + (a) : (a))
#define OS_RANDOM_RANGE_INCLUSIVE(a, b) ((b) > (a) ? rand() % ((b) - (a) + 1) + (a) : (a))

#endif
