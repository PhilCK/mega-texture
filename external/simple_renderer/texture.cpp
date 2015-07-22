#include "texture.hpp"
#include <assert.h>


namespace renderer {


namespace texture_utils {


uint32_t
get_max_1d_texture_size()
{
  GLint texture_size = 0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texture_size);

  return static_cast<uint32_t>(texture_size);
}


uint32_t
get_max_2d_texture_size()
{
  GLint texture_size = 0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texture_size);

  return static_cast<uint32_t>(texture_size);
}


uint32_t
get_max_3d_texture_size()
{
  GLint texture_size = 0;
  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &texture_size);

  return static_cast<uint32_t>(texture_size);
}


uint32_t
get_max_texels()
{
  GLint texel_size = 0;
  glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &texel_size);

  return static_cast<uint32_t>(texel_size);
}


} // namespace


texture::texture(void* image_data, const uint32_t width, const uint32_t height, const uint32_t depth)
{
  load_data(image_data, width, height, depth);
}


void
texture::_destroy()
{
  if(m_texture_id)
  {
    glDeleteTextures(1, &m_texture_id);
    m_texture_id = 0;
    check_and_log_error("texture::_destroy - removeing texture.");
  }
}


texture::~texture()
{
  _destroy();
}


void
texture::_move(texture &other_one)
{
  texture &this_one = *this;

  _destroy();

  this_one.m_width      = other_one.m_width;
  this_one.m_height     = other_one.m_height;
  this_one.m_depth      = other_one.m_depth;
  this_one.m_texture_id = other_one.m_texture_id;
  this_one.m_dimention  = other_one.m_dimention;

  other_one.m_width      = 0;
  other_one.m_height     = 0;
  other_one.m_depth      = 0;
  other_one.m_texture_id = 0;
  other_one.m_dimention  = GL_TEXTURE_2D;  
}


texture::texture(texture&& other)
{
  _move(other);
}


texture&
texture::operator=(texture&& other)
{
  _move(other);
  return *this;
}


bool
texture::load_data(void* image_data, const uint32_t width, const uint32_t height, const uint32_t depth)
{
  if(is_valid())
  {
    _destroy();
  }

  // What dimention is the data, and checks.
  {
    // TODO: Is data pow2 still a thing at the texture level?

    if(width > 0 && height == 0 && depth == 0)
    {
      m_dimention = GL_TEXTURE_1D;
      assert(width <= texture_utils::get_max_1d_texture_size());

      if(width > texture_utils::get_max_1d_texture_size())
      {
        log_error(GL_NO_ERROR, "texture::load_data - texture is bigger than max 1D texture.");
        return false;
      }
    }
    else if(width > 0 && height > 0 && depth == 0)
    {
      m_dimention = GL_TEXTURE_2D;
      assert(width <= texture_utils::get_max_2d_texture_size());
      assert(height <= texture_utils::get_max_2d_texture_size());

      if(width > texture_utils::get_max_2d_texture_size() || height > texture_utils::get_max_2d_texture_size())
      {
        log_error(GL_NO_ERROR, "texture::load_data - texture is bigger than max 2D texture.");
        return false;
      }
    }
    else if(width > 0 && height > 0 && depth > 0)
    {
      m_dimention = GL_TEXTURE_3D;
      assert(width <= texture_utils::get_max_3d_texture_size());
      assert(height <= texture_utils::get_max_3d_texture_size());
      assert(depth <= texture_utils::get_max_3d_texture_size());

      if(width > texture_utils::get_max_3d_texture_size() || height > texture_utils::get_max_3d_texture_size() || depth > texture_utils::get_max_3d_texture_size())
      {
        log_error(GL_NO_ERROR, "texture::load_data - texture is bigger than max 3D texture.");
        return false;
      }      
    }
    else
    {
      log_error(GL_NO_ERROR, "texture::load_data - failed getting dimention.");
      return false;
    }
  } // dimention check.

  // Pass data to gl.
  const auto external_format = GL_RGBA;
  const auto internal_format = GL_RGBA;
  const auto format_type     = GL_UNSIGNED_BYTE;

  switch(m_dimention)
  {
    case(GL_TEXTURE_1D):
    {
      glGenTextures(1, &m_texture_id);
      glBindTexture(GL_TEXTURE_1D, m_texture_id);
      glTexImage1D(GL_TEXTURE_1D, 0, external_format, width, 0, internal_format, format_type, image_data);
      break;
    }
    case(GL_TEXTURE_2D):
    {
      glGenTextures(1, &m_texture_id);
      glBindTexture(GL_TEXTURE_2D, m_texture_id);
      glTexImage2D(GL_TEXTURE_2D, 0, external_format, width, height, 0, internal_format, format_type, image_data);
      break;
    }
    case(GL_TEXTURE_3D):
    {
      glGenTextures(1, &m_texture_id);
      glBindTexture(GL_TEXTURE_3D, m_texture_id);
      glTexImage3D(GL_TEXTURE_3D, 0, external_format, width, height, depth, 0, internal_format, format_type, image_data);
      break;
    }
    default:
      assert(false); // error
      log_error(GL_NO_ERROR, "texture::load_data - unkonwn dimention.");
      return false;
  }

  m_width = width;
  m_height = height;
  m_depth = depth;
  
  return true;
}


namespace
{

bool
_update_texture(const GLenum dimention, const GLuint tex_id, void* image_data, const uint32_t tex_width, const uint32_t tex_height, const uint32_t tex_depth, const uint32_t offset_x, const uint32_t offset_y, const uint32_t offset_z)
{
  const auto internal_format = GL_RGBA;
  const auto format_type     = GL_UNSIGNED_BYTE;

  glBindTexture(dimention, tex_id);

  switch(dimention)
  {
    case(GL_TEXTURE_1D):
    {
      glTexSubImage1D(GL_TEXTURE_1D, offset_x, 0, tex_width, internal_format, format_type, image_data);
      break;
    }
    case(GL_TEXTURE_2D):
    {
      glTexSubImage2D(GL_TEXTURE_2D, 0, offset_x, offset_y, tex_width, tex_height, internal_format, format_type, image_data);
      break;
    }
    case(GL_TEXTURE_3D):
    {
      glTexSubImage3D(GL_TEXTURE_3D, 0, offset_x, offset_y, offset_z, tex_width, tex_height, tex_depth, internal_format, format_type, image_data);
      break;
    }    
    default:
      assert(false); // What did you do!
      log_error(GL_NO_ERROR, "texture::update_data error getting dimention.");
      check_and_log_error("texture::update_data error updating texture.");
      return false;
  };

  glBindTexture(dimention, 0);
  check_and_log_error("texture::update_data error updating texture.");

  return true;
}

} // namespace


bool
texture::update_data(void* image_data, const uint32_t width, const uint32_t offset_x)
{
  if(!is_valid())
  {
    return false;
  }

  return _update_texture(m_dimention, m_texture_id, image_data, width, 1, 1, offset_x, 0, 0);
}


bool
texture::update_data(void* image_data, const uint32_t width, const uint32_t height, const uint32_t offset_x, const uint32_t offset_y)
{
  if(!is_valid())
  {
    return false;
  }

  return _update_texture(m_dimention, m_texture_id, image_data, width, height, 1, offset_x, offset_y, 0);
}


bool
texture::update_data(void* image_data, const uint32_t width, const uint32_t height, const uint32_t depth, const uint32_t offset_x, const uint32_t offset_y, const uint32_t offset_z)
{
  if(!is_valid())
  {
    return false;
  }

  return _update_texture(m_dimention, m_texture_id, image_data, width, height, depth, offset_x, offset_y, offset_z);
}


} // namespace