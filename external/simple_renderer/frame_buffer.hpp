#ifndef FRAME_BUFFER_INCLUDED_08DDDDEB_BA61_44F5_9DD0_8932689F7174
#define FRAME_BUFFER_INCLUDED_08DDDDEB_BA61_44F5_9DD0_8932689F7174


#include "common.hpp"


namespace renderer {


class frame_buffer
{

                        frame_buffer(const frame_buffer &) = delete;
                        frame_buffer operator=(const frame_buffer &) = delete;

public:

  explicit              frame_buffer() {}
  explicit              frame_buffer(const uint32_t width, const uint32_t height);
                        ~frame_buffer();

                        frame_buffer(frame_buffer&& other);
  frame_buffer&         operator=(frame_buffer&& other);

  bool                  load_buffer(const uint32_t width, const uint32_t height);
  void                  bind();
  void                  unbind();
  void                  clear(const bool color = true, const bool depth = true);

  inline bool           is_valid() const   { return m_frame_buffer != 0; }
  inline GLuint         get_gl_id() const  { return m_frame_buffer;      }
  inline uint32_t       get_width() const  { return m_width;             }
  inline uint32_t       get_height() const { return m_height;            }

private:

  void                  _move(frame_buffer &this_one, frame_buffer &other_one);
  void                  _destroy();

private:

  uint32_t              m_width              = 0;
  uint32_t              m_height             = 0;
  GLsizei               m_view_port_width    = 0;
  GLsizei               m_view_port_height   = 0;
  GLuint                m_frame_buffer       = 0;
  GLuint                m_color_buffer       = 0;
  GLuint                m_render_buffer      = 0;

};


} // namespace


#endif // include guard