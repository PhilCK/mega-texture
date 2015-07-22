#include "ogl_context.hpp"
#include "window.hpp"
#include <assert.h>
#include <string>
#include <array>


namespace sdl {


ogl_context::ogl_context(const sdl::window &window)
{
  assert(window.has_valid_context());
  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
  
  m_sdl_gl_context = SDL_GL_CreateContext(window.get_sdl_window());
  //SDL_ERROR("Renderer::Renderer() - SDL_GL_CreateContext");
  assert(m_sdl_gl_context);

  SDL_GL_MakeCurrent(window.get_sdl_window(), m_sdl_gl_context);
  //SDL_ERROR("Renderer::Renderer() - SDL_GL_MakeCurrent");
  
}


void
ogl_context::_destroy()
{
  if(m_sdl_gl_context)
  {
    SDL_GL_DeleteContext(m_sdl_gl_context);
  }
}


ogl_context::~ogl_context()
{
  _destroy();
}


void
ogl_context::_move(ogl_context &other_one)
{
  _destroy();

  ogl_context &this_one = *this;

  this_one.m_sdl_gl_context = other_one.m_sdl_gl_context;
}


ogl_context::ogl_context(ogl_context&& other)
{
  _move(other);
}


ogl_context&
ogl_context::operator=(ogl_context&& other)
{
  _move(other);
  return *this;
}


} // namespace