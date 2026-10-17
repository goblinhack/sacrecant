#ifndef MY_RANDOM_HPP
#define MY_RANDOM_HPP

#ifdef USE_MORE_MODERN_RAND
#include <cmath>
#include <limits>
#include <random>
#endif

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
#ifdef USE_MORE_MODERN_RAND
extern std::minstd_rand simple_rand;

#define OS_RAND()                       simple_rand()
#define OS_SRAND(a)                     simple_rand.seed(a)
#define OS_RANDOM_RANGE(a, b)           ((b) > (a) ? simple_rand() % ((b) - (a)) + (a) : (a))
#define OS_RANDOM_RANGE_INCLUSIVE(a, b) ((b) > (a) ? simple_rand() % ((b) - (a) + 1) + (a) : (a))
#else
#define OS_RAND()                       rand()
#define OS_SRAND(a)                     srand(a)
#define OS_RANDOM_RANGE(a, b)           ((b) > (a) ? rand() % ((b) - (a)) + (a) : (a))
#define OS_RANDOM_RANGE_INCLUSIVE(a, b) ((b) > (a) ? rand() % ((b) - (a) + 1) + (a) : (a))
#endif

#endif
