#include "frame_buffer.hpp"
#include <assert.h>


namespace renderer {


frame_buffer::frame_buffer(const uint32_t width, const uint32_t height)
{
  load_buffer(width, height);
}


void
frame_buffer::_destroy()
{
  if(m_frame_buffer)
  {
    glDeleteFramebuffers(1, &m_frame_buffer);
    m_frame_buffer = 0;
  }

  if(m_render_buffer)
  {
    glDeleteRenderbuffers(1, &m_render_buffer);
    m_render_buffer = 0;
  }

  if(m_color_buffer)
  {
    glDeleteTextures(1, &m_color_buffer);
    m_color_buffer = 0;
  }
}


frame_buffer::~frame_buffer()
{
  _destroy();
}


void
frame_buffer::_move(frame_buffer &this_one, frame_buffer &other_one)
{
  _destroy();

  this_one.m_width            = other_one.m_width;
  this_one.m_height           = other_one.m_height;
  this_one.m_view_port_width  = other_one.m_view_port_width;
  this_one.m_view_port_height = other_one.m_view_port_height;
  this_one.m_frame_buffer     = other_one.m_frame_buffer;
  this_one.m_color_buffer     = other_one.m_color_buffer;
  this_one.m_render_buffer    = other_one.m_render_buffer;

  other_one.m_width             = 0;
  other_one.m_height            = 0;
  other_one.m_view_port_width   = 0;
  other_one.m_view_port_height  = 0;
  other_one.m_frame_buffer      = 0;
  other_one.m_color_buffer      = 0;
  other_one.m_render_buffer     = 0;
}


frame_buffer::frame_buffer(frame_buffer&& other)
{
  _move(*this, other);
}


frame_buffer&
frame_buffer::operator=(frame_buffer&& other)
{
  _move(*this, other);
  return *this;
}


bool
frame_buffer::load_buffer(const uint32_t width, const uint32_t height)
{
  assert(!is_valid());

  if(is_valid())
  {
    log_error(GL_NO_ERROR, "frame_buffer::load_buffer - buffer has already been loaded.");
    return false;
  }

  m_width  = width;
  m_height = height;

  //setViewPort(width, height);

  glGenFramebuffers(1, &m_frame_buffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);

  glGenTextures(1, &m_color_buffer);
  glBindTexture(GL_TEXTURE_2D, m_color_buffer);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_buffer, 0);

  glGenRenderbuffers(1, &m_render_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer);

  check_and_log_error("framebuffer::load_buffer - Error creating buffer.");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return is_valid() ? true : false;
}


void
frame_buffer::bind()
{
  assert(is_valid());

  if(!is_valid())
  {
    return;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
  //glViewport(0, 0, m_view_port_width, m_view_port_height);

  check_and_log_error("framebuffer::bind - Error binding buffer.");
}


void
frame_buffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void
frame_buffer::clear(const bool color, const bool depth)
{
  assert(is_valid());

  if(!is_valid())
  {
    return;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
  //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  GLenum flags = 0;
  if(color) { flags |= GL_COLOR_BUFFER_BIT; }
  if(depth) { flags |= GL_DEPTH_BUFFER_BIT; }

  glClear(flags);
}


} // namespace