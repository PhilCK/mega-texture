#ifndef SDL_COMMON_INCLUDED_3DA33932_47CB_4400_A549_64B8C18B64D7
#define SDL_COMMON_INCLUDED_3DA33932_47CB_4400_A549_64B8C18B64D7


#include <SDL2/SDL.h>
#include <string>
#include <functional>


namespace sdl {


void            log_error(const std::string &error);
void            log_debug_error_check(const std::string &msg = "");  
std::string     get_last_error();

void            set_error_callback(std::function<void(const std::string&)>);
void            clear_error_callback();


} // namespace 


#endif // include guard