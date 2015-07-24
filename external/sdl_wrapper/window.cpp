#include "window.hpp"
#include "message_pump.hpp"
#include <assert.h>
#include <iostream>


namespace
{
  const int32_t fullscreen_mode = SDL_WINDOW_FULLSCREEN_DESKTOP; // rather than SDL_WINDOW_FULLSCREEN
}


namespace sdl {


window::window(const uint32_t width, const uint32_t height, const bool is_fullscreen, const std::string &title)
{
  // Reg for callbacks
  sdl::message_callback_register(m_message_callback);
  
  // Init window
  const Uint32 init_flags = SDL_INIT_EVERYTHING;

	if(SDL_Init(init_flags) != 0)
	{
    assert(false);
    sdl::log_debug_error_check();
    return;
	}

  const Uint32 default_window_flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;
  const Uint32 window_flags = is_fullscreen ? default_window_flags | fullscreen_mode : default_window_flags;

  m_sdl_window = SDL_CreateWindow(title.c_str(),
								SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								width,
								height,
								window_flags);

  if(!m_sdl_window)
  {
    assert(false);
    sdl::log_debug_error_check();
  }
}


window::~window()
{
  if(m_message_callback)
  {
    sdl::message_callback_unregister(m_message_callback);
  }
  
  if(m_sdl_window)
  {
    SDL_DestroyWindow(m_sdl_window);
    sdl::log_debug_error_check();
  }
}


void
window::_move(window &this_one, window &other_one)
{
  std::lock_guard<std::mutex> lock_other(other_one.m_lock);
  std::lock_guard<std::mutex> lock_me(this_one.m_lock);

  this_one.m_sdl_window = other_one.m_sdl_window;
  other_one.m_sdl_window = nullptr;
}


window::window(window&& other)
{
  _move(*this, other);
}


window&
window::operator=(window&& other)
{
  _move(*this, other);  
  return *this;
}


bool
window::set_resolution(const uint32_t width, const uint32_t height)
{
  std::lock_guard<std::mutex> lock(m_lock);

  int32_t curr_width, curr_height;
  SDL_GetWindowSize(m_sdl_window, &curr_width, &curr_height);
  sdl::log_debug_error_check();

  if ((curr_width == width) && (curr_height == height))
  {
    return false;
  }

  SDL_SetWindowSize(m_sdl_window, static_cast<int32_t>(width), static_cast<int32_t>(height));
  sdl::log_debug_error_check();
  
  return true;
}


uint32_t
window::get_width() const
{
  std::lock_guard<std::mutex> lock(m_lock);

	int w, h;
	
	SDL_GetWindowSize(m_sdl_window, &w, &h);
  sdl::log_debug_error_check();
    
  return static_cast<uint32_t>(w);
}


uint32_t
window::get_height() const
{
  std::lock_guard<std::mutex> lock(m_lock);

	int w, h;
	
	SDL_GetWindowSize(m_sdl_window, &w, &h);
  sdl::log_debug_error_check();
  
  return static_cast<uint32_t>(h);
}


bool
window::set_fullscreen(const bool set)
{
  std::lock_guard<std::mutex> lock(m_lock);

  SDL_Surface* surface = SDL_GetWindowSurface(m_sdl_window);
  assert(surface);
  
  if(!surface)
  {
    sdl::log_debug_error_check();
    sdl::log_error("Failed to get SDL_Surface.");
  }
  
  if((surface->flags & fullscreen_mode) == set)
  {
    return false;
  }

  if(SDL_SetWindowFullscreen(m_sdl_window, set ? fullscreen_mode : 0) < 0)
  {
    assert(false);
    sdl::log_debug_error_check();
    sdl::log_error("Failed to set window to fullscreen.");
  }
  

	return true;
}


bool
window::is_fullscreen() const
{
  SDL_Surface* surface = nullptr;
  {
    std::lock_guard<std::mutex> lock(m_lock);
    surface = SDL_GetWindowSurface(m_sdl_window);
  }

  assert(surface);
  
  if(!surface)
  {
    sdl::log_debug_error_check();
    return false; // Although this is an error. not confirmation of non-fullscreen.
  }
  
  return surface->flags & fullscreen_mode;
}


bool
window::set_title(const std::string &str)
{
  {
    std::lock_guard<std::mutex> lock(m_lock);
    SDL_SetWindowTitle(m_sdl_window, str.c_str());
  }
  
  if(get_title() != str)
  {
    assert(false);
    sdl::log_debug_error_check();
    return false;
  }
	
	return true;
}


void
window::flip_buffer()
{
  SDL_GL_SwapWindow(m_sdl_window);
  sdl::log_debug_error_check();
}


std::string
window::get_title() const
{
  std::lock_guard<std::mutex> lock(m_lock);

  const std::string title = SDL_GetWindowTitle(m_sdl_window);
  sdl::log_debug_error_check();

  return title;
}


bool
window::_process_message(const SDL_Event &sdl_event)
{
  std::lock_guard<std::mutex> lock(m_lock);

  // Check for quit message.
  if (sdl_event.type == SDL_QUIT)
  {
    m_quit_requested = true;
  }

  if (sdl_event.type == SDL_WINDOWEVENT)
  {
    switch (sdl_event.window.event)
    {
      case(SDL_WINDOWEVENT_SHOWN): // When shown for the first time.
      {
        break;
      }

      case(SDL_WINDOWEVENT_HIDDEN):
      {
        break;
      }

      case(SDL_WINDOWEVENT_EXPOSED): // Called from start and also when window comes back from minimized. 
      {
        break;
      }

      case(SDL_WINDOWEVENT_MOVED):
      {
        break;
      }

      case(SDL_WINDOWEVENT_SIZE_CHANGED):
      {
        break;
      }

      case(SDL_WINDOWEVENT_RESIZED): // Called when size window dragged or max button pressed. and sometimes when SetWindowSize() is called.
      {
        break;
      }

      case(SDL_WINDOWEVENT_MAXIMIZED):
      {
        break;
      }

      case(SDL_WINDOWEVENT_MINIMIZED):
      {
        break;
      }

      case(SDL_WINDOWEVENT_RESTORED): // When minimised and brought back onto the screen.
      {
        break;
      }

      case(SDL_WINDOWEVENT_ENTER): // When mouse pointer hovers over. Even if not in focus.
      {
        break;
      }

      case(SDL_WINDOWEVENT_LEAVE): // When mouse pointer leaves the window. Even if not in focus.
      {
        break;
      }

      case(SDL_WINDOWEVENT_FOCUS_GAINED):
      {
        break;
      }

      case(SDL_WINDOWEVENT_FOCUS_LOST):
      {
        break;
      }

      case(SDL_WINDOWEVENT_CLOSE): // When x hit.
      {
        break;
      }
    }
  }
  
  
  // Loop through all the hooks.
//    for(auto &hook : m_hooks)
//    {
//      const bool swallow = hook(sdl_event);
//      
//      if(swallow)
//      {
//        break;
//      }
//    }
  
  return false;
}


} // namespace