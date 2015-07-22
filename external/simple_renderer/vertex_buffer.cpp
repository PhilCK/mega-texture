#include "vertex_buffer.hpp"
#include "shader.hpp"
#include "vertex_format.hpp"


namespace renderer {


vertex_buffer::vertex_buffer(const std::vector<float> &vertex_data, const bool is_dynamic)
{
  load_vertex_buffer(vertex_data, is_dynamic);
}


void
vertex_buffer::_destroy()
{
  if(m_vertex_buffer)
  {
  	glDeleteBuffers(1, &m_vertex_buffer);
  	m_vertex_buffer = 0;
    check_and_log_error("vertex_buffer::_destroy - destroying buffer.");
  }
}


vertex_buffer::~vertex_buffer()
{
  _destroy();
}


void
vertex_buffer::_move(vertex_buffer &this_one, vertex_buffer &other_one)
{
  _destroy();

  this_one.m_vertex_buffer   = other_one.m_vertex_buffer;
  this_one.m_buffer_size     = other_one.m_buffer_size;
  this_one.m_number_entries  = other_one.m_number_entries;

  other_one.m_vertex_buffer  = 0;
  other_one.m_buffer_size    = 0;
  other_one.m_number_entries = 0;
}


vertex_buffer::vertex_buffer(vertex_buffer&& other)
{
  _move(*this, other);
}


vertex_buffer&
vertex_buffer::operator=(vertex_buffer&& other)
{
  _move(*this, other);

  return *this;
}


bool
vertex_buffer::load_vertex_buffer(const std::vector<float> &vertex_data, const bool is_dynamic)
{
  if(is_valid())
  {
    log_error(GL_NO_ERROR, "vertex_buffer::load_shader - a vertex_buffer already exists, destroying old one.");
    _destroy();
  }

  glGenBuffers(1, &m_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), is_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  if(is_valid())
  {
    m_buffer_size = static_cast<GLuint>(vertex_data.size());
    m_number_entries = vertex_data.size();
    return true;
  }

  check_and_log_error("vertex_buffer::load_vertex_buffer - loading vertex buffer.");
  return false;
}


void
vertex_buffer::bind(const vertex_format &set_vert_fmt, const shader &set_shader) const
{
  if(!is_valid())
  {
    log_error(GL_NO_ERROR, "vertex_buffer::bind - buffer is not valid.");
    return;
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
  check_and_log_error("vertex_buffer::bind - binding buffer.");

  // Vertex Format
  {
    for(std::size_t i = 0; i < set_vert_fmt.get_number_of_elements(); ++i)
    {
      const GLint NOT_USED = -1;

      attribute attrib = set_vert_fmt.get_attribute(i);

      GLint index = glGetAttribLocation(set_shader.get_program_gl_id(), attrib.name.c_str());

      if(index != NOT_USED)
      {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
                              static_cast<GLint>(attrib.size),
                              attrib.type,
                              GL_FALSE,
                              set_vert_fmt.get_stride(),
                              (void*)attrib.pointer);

        check_and_log_error("vertex_buffer::bind - applying vertex format '" + attrib.name + "'.");
      }
    }
  }

  check_and_log_error("vertex_buffer::bind - applying vertex buffer.");
}	


} // namespace