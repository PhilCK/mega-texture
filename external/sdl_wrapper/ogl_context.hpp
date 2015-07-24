#ifndef SDL_OGL_CONTEXT_INCLUDED_1205DA41_7BC8_41AD_B776_01BD838772A4
#define SDL_OGL_CONTEXT_INCLUDED_1205DA41_7BC8_41AD_B776_01BD838772A4


#include "sdl_common.hpp"
#include "sdl_fwd.hpp"


namespace sdl {


class ogl_context
{


                    ogl_context(const ogl_context &) = delete;
                    ogl_context operator=(const ogl_context &) = delete;

public:

  explicit          ogl_context(const sdl::window &window);
                    ~ogl_context();

                    ogl_context(ogl_context&& other);
  ogl_context&      operator=(ogl_context&& other);

  inline bool       has_valid_context() const { return m_sdl_gl_context != nullptr; }
  
  void              think() {} // constant accross the other modules.  

private:

  void              _destroy();
  void              _move(ogl_context &other_one);

private:

  SDL_GLContext     m_sdl_gl_context;

}; // class


} // namespace


#endif // include guard