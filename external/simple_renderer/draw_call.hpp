#ifndef DRAW_CALL_INCLUDED_3592DEB3_2FAC_4538_977A_8E6D7BD736A8
#define DRAW_CALL_INCLUDED_3592DEB3_2FAC_4538_977A_8E6D7BD736A8


#include "renderer_fwd.hpp"
#include <stdint.h>

/*
  These are draw helpers.
  They are only simple methods that bind the various objects etc.
*/
namespace renderer {


void draw(shader &set_shader, vertex_format &set_vert_fmt, vertex_buffer &set_vertex_buffer);
void draw(frame_buffer &set_frame_buffer, shader &set_shader, vertex_format &set_vert_fmt, vertex_buffer &set_vertex_buffer);
void draw(shader &set_shader, vertex_format &set_vert_fmt, vertex_buffer &set_vertex_buffer, const uint32_t start, const uint32_t size);
void draw(shader &set_shader, vertex_format &set_vert_fmt, vertex_buffer &set_vertex_buffer, index_buffer &set_index_buffer);


} // namespace


#endif // include guard