#include "renderer.hpp"
#include "common.hpp"


namespace
{
  GLuint vao_id = 0;
}


namespace renderer {


void
initialize()
{
  if(!vao_id)
  {
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);  

    reset();
  }
}


void
de_initialize()
{
  if(vao_id)
  {
    glDeleteVertexArrays(1, &vao_id);
  }
}


void
reset()
{
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
}


void
depth(const bool set)
{
  if(set)
  {
    glEnable(GL_DEPTH_TEST);
  }
  else
  {
    glDisable(GL_DEPTH_TEST);
  }
}


void
clear(const bool color, const bool depth)
{
  GLbitfield flags = 0;
  flags |= color ? GL_COLOR_BUFFER_BIT : 0;
  flags |= depth ? GL_DEPTH_BUFFER_BIT : 0;

  glClear(flags);
}


void
clear_color(const float red, const float green, const float blue)
{
   glClearColor(red, green, blue, 1.f);
}


} // namespace