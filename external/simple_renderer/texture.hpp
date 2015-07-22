#ifndef TEXTURE_INCLUDED_2C5BD8B8_1F7A_4A72_A8AC_6036FE8777EE
#define TEXTURE_INCLUDED_2C5BD8B8_1F7A_4A72_A8AC_6036FE8777EE


#include "common.hpp"
#include <stdint.h>


namespace renderer {


namespace texture_utils
{

uint32_t get_max_1d_texture_size();
uint32_t get_max_2d_texture_size();
uint32_t get_max_3d_texture_size();
uint32_t get_max_texels();

}


class texture
{

                      texture(const texture &) = delete;
                      texture operator=(const texture &) = delete;

public:
  
  explicit            texture() {}
  explicit            texture(void* image_data, const uint32_t width, const uint32_t height = 0, const uint32_t depth = 0);
                      ~texture();

                      texture(texture&& other);
  texture&            operator=(texture&& other);

  bool                load_data(void* image_data, const uint32_t width, const uint32_t height = 0, const uint32_t depth = 0);
  bool                update_data(void* image_data, const uint32_t width, const uint32_t offset_x);
  bool                update_data(void* image_data, const uint32_t width, const uint32_t height, const uint32_t offset_x, const uint32_t offset_y);
  bool                update_data(void* image_data, const uint32_t width, const uint32_t height, const uint32_t depth, const uint32_t offset_x, const uint32_t offset_y, const uint32_t offset_z);

  inline uint32_t     get_width() const { return m_width; }
  inline uint32_t     get_height() const { return m_height; }
  inline uint32_t     get_depth() const { return m_depth; }
  inline bool         is_valid() const { return m_texture_id != 0; }
  inline GLuint       get_gl_id() const { return m_texture_id; }
  inline GLenum       get_gl_dimention() const { return m_dimention; }

private:

  void                _move(texture &other_one);
  void                _destroy();

private:

  uint32_t            m_width      = 0;
  uint32_t            m_height     = 0;
  uint32_t            m_depth      = 0;
  GLuint              m_texture_id = 0;
  GLenum              m_dimention  = GL_TEXTURE_2D;


}; // class


} // namespace


#endif // include guard