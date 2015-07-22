#include "draw_call.hpp"
#include "vertex_format.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "frame_buffer.hpp"
#include "shader.hpp"

#include <iostream>
namespace renderer {


void draw(shader &set_shader, vertex_format &set_vert_fmt, vertex_buffer &set_vertex_buffer)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  set_shader.bind();
  set_vertex_buffer.bind(set_vert_fmt, set_shader);

  const uint32_t number_of_vertices = set_vertex_buffer.get_number_entries() / set_vert_fmt.get_number_of_entires();

  glDrawArrays(GL_TRIANGLES, 0, number_of_vertices);
}


void draw(frame_buffer &set_frame_buffer, shader &set_shader, vertex_format &set_vert_fmt, vertex_buffer &set_vertex_buffer)
{
  set_frame_buffer.bind();
  set_shader.bind();
  set_vertex_buffer.bind(set_vert_fmt, set_shader);

  const uint32_t number_of_vertices = set_vertex_buffer.get_number_entries() / set_vert_fmt.get_number_of_entires();

  glDrawArrays(GL_TRIANGLES, 0, number_of_vertices);

  set_frame_buffer.unbind(); 
}


void draw(shader &set_shader, vertex_format &set_vert_fmt, vertex_buffer &set_vertex_buffer, const uint32_t start, const uint32_t size)
{
  set_shader.bind();
  set_vertex_buffer.bind(set_vert_fmt, set_shader);

  //const uint32_t number_of_vertices = set_vertex_buffer.get_number_entries() / set_vert_fmt.get_number_of_elements();
  //const uint32_t number_of_primitives = number_of_vertices / 3;

  glDrawArrays(GL_TRIANGLES, start, size);
}


void draw(shader &set_shader, vertex_format &set_vert_fmt, vertex_buffer &set_vertex_buffer, index_buffer &set_index_buffer)
{
  set_shader.bind();
  set_vertex_buffer.bind(set_vert_fmt, set_shader);
  set_index_buffer.bind();

  glDrawElements(GL_TRIANGLES, set_index_buffer.get_number_of_indices(), GL_UNSIGNED_INT, 0);
}


} // namespace