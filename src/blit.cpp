//
// Copyright goblinhack@gmail.com
//

#include "my_callstack.hpp"
#include "my_color.hpp"
#include "my_gl.hpp"
#include "my_level.hpp"
#include "my_ptrcheck.hpp"
#include "my_spoint.hpp"
#include "my_tile.hpp"
#include "my_types.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>

float   glapi_last_tex_right;
float   glapi_last_tex_bottom;
GLshort glapi_last_right;
GLshort glapi_last_bottom;

uint32_t NUMBER_BYTES_PER_VERTEX_2D = (SIZEOF(GLfloat) * NUMBER_DIMENSIONS_PER_COORD_2D) + (SIZEOF(GLshort) * NUMBER_DIMENSIONS_PER_COORD_2D)
                                    + (SIZEOF(GLubyte) * NUMBER_COMPONENTS_PER_COLOR);
//
// Two arrays, xy and uv.
//
float *gl_array_buf;
float *gl_array_buf_end;

//
// Where we are currently up to in writing to these buffers.
//
GLfloat *bufp;
GLfloat *bufp_end;
int      buf_tex;

void blit_init()
{
  TRACE_DEBUG();

  buf_tex = 0;

  if (gl_array_buf != nullptr) {
    bufp = gl_array_buf;
    return;
  }

  //
  // Our array size requirements.
  //
  uint32_t gl_array_size_required = 0;

  //
  // If the screen size has changed or this is the first run, allocate our
  // buffer if our size requirements have changed.
  //
  gl_array_size_required = 32 * 1024 * 1024;

  gl_array_buf = static_cast< __typeof__(gl_array_buf) >(MYZALLOC(gl_array_size_required, "GL xy buffer"));

  //
  // Make the end a bit smaller so we have plenty of headroom.
  //
  gl_array_buf_end
      = reinterpret_cast< __typeof__(gl_array_buf_end) >(reinterpret_cast< char * >(gl_array_buf)) + ((gl_array_size_required * 2) / 3);

  bufp     = gl_array_buf;
  bufp_end = gl_array_buf_end;
}

void blit_fini()
{
  TRACE_DEBUG();

  if (gl_array_buf != nullptr) {
    MYFREE(gl_array_buf);
    gl_array_buf = nullptr;
  }
}

static void blit_push(float **P, const float *p_end, bool first_vertex, float tex_left, float tex_top, float tex_right, float tex_bottom,
                      spoint tl, spoint tr, spoint bl, spoint br, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t a1, uint8_t r2, uint8_t g2,
                      uint8_t b2, uint8_t a2, uint8_t r3, uint8_t g3, uint8_t b3, uint8_t a3, uint8_t r4, uint8_t g4, uint8_t b4, uint8_t a4)
{
  TRACE_DEBUG();

  float *p = *P;

  if ((p >= p_end)) [[unlikely]] {
    log("overflow on gl bug %s", __FUNCTION__);
    return;
  }

  if ((! first_vertex)) [[likely]] {
    //
    // If there is a break in the triangle strip then make a degenerate triangle.
    //
    if ((glapi_last_right != bl.x) || (glapi_last_bottom != bl.y)) {
      GL_PUSH_TEXCOORD(p, glapi_last_tex_right, glapi_last_tex_bottom);
      GL_PUSH_VERTEX(p, glapi_last_right, glapi_last_bottom);
      GL_PUSH_RGBA(p, r4, g4, b4, a4);

      GL_PUSH_TEXCOORD(p, tex_left, tex_top);
      GL_PUSH_VERTEX(p, tl.x, tl.y);
      GL_PUSH_RGBA(p, r1, g1, b1, a1);
    }
  }

  GL_PUSH_TEXCOORD(p, tex_left, tex_top);
  GL_PUSH_VERTEX(p, tl.x, tl.y);
  GL_PUSH_RGBA(p, r1, g1, b1, a1);

  GL_PUSH_TEXCOORD(p, tex_left, tex_bottom);
  GL_PUSH_VERTEX(p, bl.x, bl.y);
  GL_PUSH_RGBA(p, r2, g2, b2, a2);

  GL_PUSH_TEXCOORD(p, tex_right, tex_top);
  GL_PUSH_VERTEX(p, tr.x, tr.y);
  GL_PUSH_RGBA(p, r3, g3, b3, a3);

  GL_PUSH_TEXCOORD(p, tex_right, tex_bottom);
  GL_PUSH_VERTEX(p, br.x, br.y);
  GL_PUSH_RGBA(p, r4, g4, b4, a4);

  glapi_last_tex_right  = tex_right;
  glapi_last_tex_bottom = tex_bottom;
  glapi_last_right      = br.x;
  glapi_last_bottom     = br.y;
  *P                    = p;
}

static void blit_push(float **P, float *p_end, bool first_vertex, float tex_left, float tex_top, float tex_right, float tex_bottom, GLshort left,
                      GLshort top, GLshort right, GLshort bottom, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t a1, uint8_t r2, uint8_t g2,
                      uint8_t b2, uint8_t a2, uint8_t r3, uint8_t g3, uint8_t b3, uint8_t a3, uint8_t r4, uint8_t g4, uint8_t b4, uint8_t a4)
{
  TRACE_DEBUG();

  spoint const tl(left, top);
  spoint const tr(right, top);
  spoint const bl(left, bottom);
  spoint const br(right, bottom);

  blit_push(P, p_end, first_vertex, tex_left, tex_top, tex_right, tex_bottom, tl, tr, bl, br, r1, g1, b1, a1, r2, g2, b2, a2, r3, g3, b3, a3, r4,
            g4, b4, a4);
}

static bool first_vertex;

void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort left, GLshort top, GLshort right, GLshort bottom,
          const color &c)
{
  TRACE_DEBUG();

  if (buf_tex == 0) [[unlikely]] {
    blit_init();
    first_vertex = true;
  } else if (buf_tex != tex) [[unlikely]] {
    blit_flush();
    first_vertex = true;
  } else {
    first_vertex = false;
  }

  buf_tex = tex;

  uint8_t const r = c.r;
  uint8_t const g = c.g;
  uint8_t const b = c.b;
  uint8_t const a = c.a;

  blit_push(&bufp, bufp_end, first_vertex, texMinX, texMinY, texMaxX, texMaxY, left, top, right, bottom, r, g, b, a, r, g, b, a, r, g, b, a, r,
            g, b, a);
}

void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort pixMinX, GLshort pixMinY, GLshort pixMaxX,
          GLshort pixMaxY, const color &c, LightPixels *light_pixels, bool blit_flush_per_line)
{
  TRACE_DEBUG();

  if (buf_tex == 0) [[unlikely]] {
    blit_init();
    first_vertex = true;
  } else if (buf_tex != tex) [[unlikely]] {
    blit_flush();
    first_vertex = true;
  } else {
    first_vertex = false;
  }

  buf_tex = tex;

  const float texDiffX = (texMaxX - texMinX) / static_cast< float > LIGHT_PIXEL;
  const float texDiffY = (texMinY - texMaxY) / static_cast< float > LIGHT_PIXEL;
  const float pixDiffX = (static_cast< float >(pixMaxX) - static_cast< float >(pixMinX)) / static_cast< float > LIGHT_PIXEL;
  const float pixDiffY = (static_cast< float >(pixMinY) - static_cast< float >(pixMaxY)) / static_cast< float > LIGHT_PIXEL;
  const uint8_t                                                                                                 a = 255;

  const float cr = static_cast< float >(c.r) / 255.0F;
  const float cg = static_cast< float >(c.g) / 255.0F;
  const float cb = static_cast< float >(c.b) / 255.0F;

  //
  // Fewer triangle breaks doing y rows first
  //
  for (auto y = 0; y < LIGHT_PIXEL; y++) {

    const float texMaxY2 = texMaxY + (y * texDiffY);
    const float texMinY2 = texMaxY + ((y + 1) * texDiffY);
    const auto  pixMaxY2 = static_cast< GLshort >(static_cast< float >(pixMaxY) + (static_cast< float >(y) * pixDiffY));
    const auto  pixMinY2 = static_cast< GLshort >(static_cast< float >(pixMaxY) + (static_cast< float >(y + 1) * pixDiffY));

    for (auto x = 0; x < LIGHT_PIXEL; x++) {

      auto *const   pixel = &light_pixels->pixel[ x ][ y ];
      const uint8_t r     = static_cast< int >(std::max(pixel->r, pixel->player_r) * cr);
      const uint8_t g     = static_cast< int >(std::max(pixel->g, pixel->player_g) * cg);
      const uint8_t b     = static_cast< int >(std::max(pixel->b, pixel->player_b) * cb);

      float const texMinX2 = texMinX + (x * texDiffX);
      float const texMaxX2 = texMinX + ((x + 1) * texDiffX);
      auto        pixMinX2 = static_cast< GLshort >(static_cast< float >(pixMinX) + (static_cast< float >(x) * pixDiffX));
      auto        pixMaxX2 = static_cast< GLshort >(static_cast< float >(pixMinX) + (static_cast< float >(x + 1) * pixDiffX));

      blit_push(&bufp, bufp_end, first_vertex, texMinX2, texMinY2, texMaxX2, texMaxY2, pixMinX2, pixMinY2, pixMaxX2, pixMaxY2, r, g, b, a, r, g,
                b, a, r, g, b, a, r, g, b, a);
    }

    //
    // If we have alpha values in the texture, the end of one triangle line and the start of another creates
    // a visible strip
    //
    if (blit_flush_per_line) {
      buf_tex = tex;
      blit_flush();
    }
  }
}

void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort pixMinX, GLshort pixMinY, GLshort pixMaxX,
          GLshort pixMaxY, LightPixels *light_pixels, bool blit_flush_per_line)
{
  TRACE_DEBUG();

  if (buf_tex == 0) [[unlikely]] {
    blit_init();
    first_vertex = true;
  } else if (buf_tex != tex) [[unlikely]] {
    blit_flush();
    first_vertex = true;
  } else {
    first_vertex = false;
  }

  buf_tex = tex;

  const float texDiffX = (texMaxX - texMinX) / static_cast< float > LIGHT_PIXEL;
  const float texDiffY = (texMinY - texMaxY) / static_cast< float > LIGHT_PIXEL;
  const float pixDiffX = (static_cast< float >(pixMaxX) - static_cast< float >(pixMinX)) / static_cast< float > LIGHT_PIXEL;
  const float pixDiffY = (static_cast< float >(pixMinY) - static_cast< float >(pixMaxY)) / static_cast< float > LIGHT_PIXEL;
  const uint8_t                                                                                                 a = 255;

  //
  // Fewer triangle breaks doing y rows first
  //
  for (auto y = 0; y < LIGHT_PIXEL; y++) {

    const float texMaxY2 = texMaxY + (y * texDiffY);
    const float texMinY2 = texMaxY + ((y + 1) * texDiffY);
    const auto  pixMaxY2 = static_cast< GLshort >(static_cast< float >(pixMaxY) + (static_cast< float >(y) * pixDiffY));
    const auto  pixMinY2 = static_cast< GLshort >(static_cast< float >(pixMaxY) + (static_cast< float >(y + 1) * pixDiffY));

    for (auto x = 0; x < LIGHT_PIXEL; x++) {

      auto *const   pixel = &light_pixels->pixel[ x ][ y ];
      uint8_t const r     = std::max(pixel->r, pixel->player_r);
      uint8_t const g     = std::max(pixel->g, pixel->player_g);
      uint8_t const b     = std::max(pixel->b, pixel->player_b);

      float const texMinX2 = texMinX + (x * texDiffX);
      float const texMaxX2 = texMinX + ((x + 1) * texDiffX);
      auto        pixMinX2 = static_cast< GLshort >(static_cast< float >(pixMinX) + (static_cast< float >(x) * pixDiffX));
      auto        pixMaxX2 = static_cast< GLshort >(static_cast< float >(pixMinX) + (static_cast< float >(x + 1) * pixDiffX));

      blit_push(&bufp, bufp_end, first_vertex, texMinX2, texMinY2, texMaxX2, texMaxY2, pixMinX2, pixMinY2, pixMaxX2, pixMaxY2, r, g, b, a, r, g,
                b, a, r, g, b, a, r, g, b, a);
    }

    //
    // If we have alpha values in the texture, the end of one triangle line and the start of another creates
    // a visible strip
    //
    if (blit_flush_per_line) {
      buf_tex = tex;
      blit_flush();
    }
  }
}

void blit(int tex, float texMinX, float texMinY, float texMaxX, float texMaxY, GLshort left, GLshort top, GLshort right, GLshort bottom,
          const color &color_bl, const color &color_br, const color &color_tl, const color &color_tr)
{
  TRACE_DEBUG();

  if (buf_tex == 0) [[unlikely]] {
    blit_init();
    first_vertex = true;
  } else if (buf_tex != tex) [[unlikely]] {
    blit_flush();
    first_vertex = true;
  } else {
    first_vertex = false;
  }

  buf_tex = tex;

  blit_push(&bufp, bufp_end, first_vertex, texMinX, texMinY, texMaxX, texMaxY, left, top, right, bottom, color_tl.r, color_tl.g, color_tl.b,
            color_tl.a, color_bl.r, color_bl.g, color_bl.b, color_bl.a, color_tr.r, color_tr.g, color_tr.b, color_tr.a, color_br.r, color_br.g,
            color_br.b, color_br.a);
}

void blit(int tex, GLshort left, GLshort top, GLshort right, GLshort bottom, const color &c)
{
  TRACE_DEBUG();

  blit(tex, 0.0, 1.0, 1.0, 0.0, left, top, right, bottom, c);
}

void blit_fbo(Gamep g, FboEnum fbo, color c)
{
  TRACE_DEBUG();

  int tex_width  = 0;
  int tex_height = 0;
  fbo_get_size(g, fbo, tex_width, tex_height);
  blit_init();
  blit(g_fbo_tex_id[ fbo ], 0.0, 1.0, 1.0, 0.0, 0, 0, tex_width, tex_height, c);
  blit_flush();
}

void blit_fbo(Gamep g, FboEnum fbo, int tl_x, int tl_y, int br_x, int br_y, color c)
{
  TRACE_DEBUG();

  blit_init();
  blit(g_fbo_tex_id[ fbo ], 0.0, 1.0, 1.0, 0.0, tl_x, tl_y, br_x, br_y, c);
  blit_flush();
}

void blit_flush_triangle_fan() { blit_flush_triangle_fan(gl_array_buf, bufp); }

void blit_flush_colored_triangle_fan()
{
  TRACE_DEBUG();

  blit_flush_colored_triangle_fan(gl_array_buf, bufp);
}
