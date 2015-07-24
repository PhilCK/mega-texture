#ifndef MESSAGE_PUMP_INCLUDED_977F16E3_A92A_4BEF_AD8E_36A3FF74AF2A
#define MESSAGE_PUMP_INCLUDED_977F16E3_A92A_4BEF_AD8E_36A3FF74AF2A


#include "sdl_common.hpp"
#include <functional>


namespace sdl {


void message_callback_register(std::function<bool(const SDL_Event &event)>);
void message_callback_unregister(std::function<bool(const SDL_Event &event)> &callback);
void message_pump();

  
} // namespace


#endif // include guard