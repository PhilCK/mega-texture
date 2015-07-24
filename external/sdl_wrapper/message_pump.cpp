#include "message_pump.hpp"
#include <functional>
#include <vector>
#include <mutex>


namespace
{
  std::mutex lock;
  std::vector<std::function<bool(const SDL_Event &event)>> callbacks;
}



namespace sdl {
  
  
void
message_callback_register(std::function<bool(const SDL_Event &)> callback)
{
  std::lock_guard<std::mutex> locker(lock);
  
  callbacks.emplace_back(callback);
}

  
void
message_callback_unregister(std::function<bool(const SDL_Event &)> &callback)
{
  std::lock_guard<std::mutex> locker(lock);
  
  for(auto it = callbacks.begin(); it != callbacks.end(); ++it)
  {
    if(it->target<void*>() == callback.target<void*>())
    {
      callbacks.erase(it);
      return;
    }
  }
}
  

void
message_pump()
{
  std::lock_guard<std::mutex> locker(lock);
  
  SDL_Event sdl_event;
  
  while (SDL_PollEvent(&sdl_event))
  {
    for(auto &cb : callbacks)
    {
      // If the event was swallowed then stop any more parsing of this event.
      if(cb(sdl_event))
      {
        break;
      }
    }
  }
}
  
  
} // namespace