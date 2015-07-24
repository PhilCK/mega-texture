#ifndef SDL_WINDOW_INCLUDED_09D60A47_F1DA_4146_8F2B_5EF5C099CD60
#define SDL_WINDOW_INCLUDED_09D60A47_F1DA_4146_8F2B_5EF5C099CD60


#include "sdl_common.hpp"
#include "sdl_fwd.hpp"
#include <string>
#include <stdint.h>
#include <functional>
#include <vector>
#include <mutex>
#include <memory>


namespace sdl {


class window final
{

                      window(const window &) = delete;
                      window operator=(const window &) = delete;

public:


  explicit            window(const uint32_t width = 640,
                             const uint32_t height = 480,
                             const bool is_fullscreen = false,
                             const std::string& title = "App");
  
                      ~window();
  
                      window(window&& other);
  window&             operator=(window&& other);

  bool                set_resolution(const uint32_t width, const uint32_t height);
  uint32_t            get_width() const;
  uint32_t            get_height() const;

  bool                set_fullscreen(const bool set);
  bool                is_fullscreen() const;

  bool                set_title(const std::string &str);
  std::string         get_title() const;

  void                think() {} // constant accross the other modules.
  void                flip_buffer();

  inline bool         wants_to_quit() const       { return m_quit_requested;        }
  inline SDL_Window*  get_sdl_window() const      { return m_sdl_window;            }
  inline bool         has_valid_context() const   { return m_sdl_window != nullptr; }

private:

  bool                _process_message(const SDL_Event &event);
  void                _move(window &this_one, window &other_one);

private:
  
  SDL_Window                            *m_sdl_window      = nullptr;
  bool                                  m_quit_requested   = false;
  std::function<bool(const SDL_Event&)> m_message_callback = std::bind(&window::_process_message, this, std::placeholders::_1);
  mutable std::mutex                    m_lock;

}; // class


} // namespace


#endif // include guard