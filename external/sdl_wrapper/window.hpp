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


/*
  hook_into_message_pump will return this object inside a unique_ptr.
  Its recomended that any object that wishes access to the callback. Keep
  this unique_ptr as a member variable. It will automattically unregester the
  call back.
*/
class sdl_event_callback_ownership final
{

  sdl_event_callback_ownership(const sdl_event_callback_ownership &) = delete;
  sdl_event_callback_ownership operator=(const sdl_event_callback_ownership &) = delete;
  sdl_event_callback_ownership(sdl_event_callback_ownership&& other) = delete;
  sdl_event_callback_ownership& operator=(sdl_event_callback_ownership&& other) = delete;

public:

  sdl_event_callback_ownership()
  {
    
  }

  ~sdl_event_callback_ownership()
  {
    if(un_register_callback)
    {
      un_register_callback();
    }
  }

private:

  friend class window;

  std::function<bool(const SDL_Event &evt)> event_callback;
  std::function<void()> un_register_callback;
};  


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

  void                flip_buffer();
  
  void                pump_messages();

  void                hook_into_message_pump(std::function<bool(const SDL_Event& event)> message_hook);

  template<typename T>
  std::unique_ptr<sdl_event_callback_ownership> hook_into_message_pump(T *instance, bool (T::*callback)(const SDL_Event&))
  {
    std::unique_ptr<sdl_event_callback_ownership> callback_obj( new sdl_event_callback_ownership());

    std::function<bool(const SDL_Event &evt)> c = std::bind(callback, instance, std::placeholders::_1);
    callback_obj->event_callback.swap(c);

    return callback_obj;
  }
  
  template<typename T>
  void hook_into_message_pump(T *instance, bool (T::*callback)(const SDL_Event&))
  {
    std::function<bool(const SDL_Event &evt)> evt_handler = std::bind(callback, instance, std::placeholders::_1);
    
    hook_into_message_pump(evt_handler);
  }

  bool                unregister_message_pump_hook();
  
  inline bool         wants_to_quit() const { return m_quit_requested; }
  
  inline SDL_Window*  get_sdl_window() const { return m_sdl_window; }
  inline bool         has_valid_context() const { return m_sdl_window != nullptr; }

private:

  void                _move(window &this_one, window &other_one);

private:

  std::vector<std::function<bool(const SDL_Event& event)> > m_hooks;
  std::vector<std::reference_wrapper<sdl_event_callback_ownership>> m_callback_hooks;
  SDL_Window          *m_sdl_window    = nullptr;
  bool                m_quit_requested = false;
  mutable std::mutex  m_lock;

}; // class


} // namespace


#endif // include guard