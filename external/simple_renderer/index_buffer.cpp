#include "index_buffer.hpp"


namespace renderer {


index_buffer::index_buffer()
{

}


index_buffer::index_buffer(const std::vector<uint32_t> &index)
{
  load_index_buffer(index);
}


void
index_buffer::_destroy()
{
  if(is_valid())
  {
    glDeleteBuffers(1, &m_index_buffer);
  }
}


index_buffer::~index_buffer()
{
  _destroy();
}


void
index_buffer::_move(index_buffer &other_one)
{
  _destroy();

  index_buffer &this_one       = *this;
  this_one.m_index_buffer      = other_one.m_index_buffer;
  this_one.m_number_of_indices = other_one.m_number_of_indices;
  
  other_one.m_index_buffer      = 0;
  other_one.m_number_of_indices = 0;
}


index_buffer::index_buffer(index_buffer&& other)
{
  _move(other);
}


index_buffer&
index_buffer::operator=(index_buffer&& other)
{
  _move(other);
  return *this;
}


bool
index_buffer::load_index_buffer(const std::vector<uint32_t> &index)
{
  if(m_index_buffer)
  {
    _destroy();
  }
  
  m_number_of_indices = static_cast<uint32_t>(index.size());

  glGenBuffers(1, &m_index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), &index[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  check_and_log_error("index_buffer::load_index_buffer - loading buffer.");

  return  (is_valid() ? true : false);
}


void
index_buffer::bind()
{
  if(is_valid())
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
    check_and_log_error("index_buffer::bind - failed binding buffer");
  }
}


}