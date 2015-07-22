#ifndef SDL_INPUT_INCLUDED_A92B3254_3BD2_4EE7_AAF7_65555A4B6672
#define SDL_INPUT_INCLUDED_A92B3254_3BD2_4EE7_AAF7_65555A4B6672


#include "sdl_common.hpp"
#include "sdl_fwd.hpp"
#include "window.hpp"
#include <stdint.h>


namespace sdl {


class input final
{
public:

  explicit          input(const sdl::window &window);
  
  void              think();
  
  int32_t           get_mouse_delta_x() const;
  int32_t           get_mouse_delta_y() const;
  int32_t           get_mouse_x() const;
  int32_t           get_mouse_y() const;
  
  bool              set_mouse_hold(const bool set);
  bool              is_mouse_held() const;
  
  bool              set_text_stream(const bool set);
  bool              is_text_stream_enabled() const;
  
  bool              process_sdl_message(const SDL_Event &event);
  
private:

  int32_t           m_mouse_delta_x = 0;
  int32_t           m_mouse_delta_y = 0;
  sdl::sdl_event_callback_ownership m_callback_owner;
  

}; // class


} // namespace


#endif // include guard