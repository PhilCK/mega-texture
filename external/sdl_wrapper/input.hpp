#ifndef SDL_INPUT_INCLUDED_A92B3254_3BD2_4EE7_AAF7_65555A4B6672
#define SDL_INPUT_INCLUDED_A92B3254_3BD2_4EE7_AAF7_65555A4B6672


#include "sdl_common.hpp"
#include "sdl_fwd.hpp"
#include "window.hpp"
#include <stdint.h>
#include <functional>
#include <map>


namespace sdl {

class input final
{
public:

  explicit          input();
                    ~input();
  
  void              think();
  
  inline int32_t    get_mouse_delta_x() const { return m_mouse_delta_x; }
  inline int32_t    get_mouse_delta_y() const { return m_mouse_delta_y; }
  int32_t           get_mouse_x() const;
  int32_t           get_mouse_y() const;
  
  bool              set_mouse_hold(const bool set);
  bool              is_mouse_held() const;
  
//  bool              set_text_stream(const bool set);
//  bool              is_text_stream_enabled() const;
  
  bool              is_key_down(uint32_t key_id);
  bool              is_key_down_on_frame(uint32_t key_id);
  
private:
  
  bool              _process_message(const SDL_Event &event);
  
private:

  int32_t           m_mouse_delta_x = 0;
  int32_t           m_mouse_delta_y = 0;
  
  enum class key_state {UP, DOWN,};
  std::map<uint32_t, key_state> m_key_states;
  std::map<uint32_t, key_state> m_frame_key_states;
  
  std::function<bool(const SDL_Event&)> m_message_callback = std::bind(&input::_process_message, this, std::placeholders::_1);

  
}; // class


} // namespace


#endif // include guard