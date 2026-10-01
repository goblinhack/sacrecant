//
// Copyright goblinhack@gmail.com
//

#ifndef MY_CONFIG_HPP
#define MY_CONFIG_HPP

// NOLINTBEGIN

#define ENABLE_ASSERT              // Abort on errors
#define ENABLE_CRASH_HANDLER       // Intercept SEGV and other faults
#define ENABLE_DEBUG_TRACE         // (SLOW) Function tracing
#define ENABLE_PTRCHECK_HISTORY 5  // Per pointer history (when in --debug3 mode)
#define ENABLE_MAX_ERR_COUNT    10 // Maximum errors before we give up

#undef ENABLE_TILE_BOUNDS        // For collisions
#undef ENABLE_DEBUG_GFX_GL_BLEND // Use to debug specific blends
#undef ENABLE_DEBUG_MEM_LEAKS    // Check for leaks at exit
#undef ENABLE_DEBUG_PTRCHECK     // (VERY SLOW) Debug ptrcheck itself
#undef ENABLE_DEBUG_TILE         // Debug tile loading
#undef ENABLE_DEBUG_UI_FOCUS     // Which windows we are over
#undef ENABLE_DEBUG_UI           // User interface logs
#undef ENABLE_DEBUG_UI2          // User interface logs with inheritance (verbose)
#undef ENABLE_LOG_TIMESTAMPS     // Full timestamps with date in logs
#undef ENABLE_UI_ASCII_MOUSE     // Show an in-game mouse, not system mouse
#undef ENABLE_PER_THING_MEMORY   // Use malloc for things (for sanity checking memory access)
                                 // Will not work with save/load

// NOLINTEND

#endif
