#include "vertex_format.hpp"
#include <assert.h>


namespace
{
  std::size_t number_of_elements(renderer::attr_type type)
  {
    switch(type)
    {
      case(renderer::attr_type::FLOAT):  return 1; break;
      case(renderer::attr_type::FLOAT2): return 2; break;
      case(renderer::attr_type::FLOAT3): return 3; break;
      case(renderer::attr_type::FLOAT4): return 4; break;
      case(renderer::attr_type::INT):    return 1; break;
      case(renderer::attr_type::UINT):   return 1; break;
    }

    assert(false); // shouldn't have got here.
    return 0; // For waring message.
  }

  GLenum gl_type(renderer::attr_type type)
  {
    switch(type)
    {
      case(renderer::attr_type::FLOAT):  return GL_FLOAT; break;
      case(renderer::attr_type::FLOAT2): return GL_FLOAT; break;
      case(renderer::attr_type::FLOAT3): return GL_FLOAT; break;
      case(renderer::attr_type::FLOAT4): return GL_FLOAT; break;
      case(renderer::attr_type::INT):    return GL_INT;   break;
      case(renderer::attr_type::UINT):   return GL_UNSIGNED_INT; break;
    }

    assert(false); // shouldn't have got here.
    return 0; // For waring message.
  }

  uint32_t size_of_gl_type(GLenum type)
  {
    switch(type)
    {
      case(GL_FLOAT):         return sizeof(GLfloat); break;
      case(GL_INT):           return sizeof(GLint);   break;
      case(GL_UNSIGNED_INT):  return sizeof(GLuint);  break;
    }

    assert(false); // shouldn't have got here.
    return 0; // For waring message.
  }
}


namespace renderer {


vertex_format::vertex_format(const std::vector<attr_format_desc> &desc)
{
  load_format(desc);
}


bool vertex_format::load_format(const std::vector<attr_format_desc> &desc)
{
  if(is_valid())
  {
    assert(false);
    return false;
  }

  for(const auto &attr : desc)
  {
    attribute attr_info;

    attr_info.name       = attr.name;
    attr_info.size       = static_cast<uint32_t>(number_of_elements(attr.type));
    attr_info.type       = gl_type(attr.type);
    attr_info.pointer    = m_stride;
    m_stride            += (number_of_elements(attr.type) * sizeof(attr_info.type));
    m_number_of_entires += number_of_elements(attr.type);

    m_format.emplace_back(attr_info);
  }

  return true;
}

} // namespace