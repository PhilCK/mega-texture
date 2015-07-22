#include "sdl_common.hpp"


namespace
{
  std::function<void(const std::string&)> error_callback;
  std::string last_error = "";
}


namespace sdl {


void
log_error(const std::string &error)
{
  last_error = error;
}
  

void
log_debug_error_check(const std::string &msg)
{
  
}


std::string
get_last_error()
{
  return last_error;
}


void
set_error_callback(std::function<void(const std::string&)> set_error_callback)
{
  error_callback = set_error_callback;
}


void
clear_error_callback()
{
  // TODO: How
}


} // namespace