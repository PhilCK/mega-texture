#include "common.hpp"
#include <iostream>


namespace
{
  std::function<void(const int32_t error_code, const std::string &)> log_callback;
  std::string last_error_message;
}


namespace renderer {



void
check_and_log_error(const std::string &str)
{
#ifdef SIMPLE_RENDERER_LOG_ERROR

  GLenum gl_error_code = glGetError();

  if(gl_error_code != GL_NO_ERROR)
  {
  	log_error(gl_error_code, str);
  }

#endif
}


void
log_error(const int32_t error_code, const std::string &str)
{
  last_error_message = std::to_string(error_code) + " : " + str;

  if(log_callback)
  {
    log_callback(error_code, str);
  }
}


void
set_log_callback(std::function<void(const int32_t error_code, const std::string &)> set_log_callback)
{
  log_callback = set_log_callback;
}


std::string
get_last_error()
{
  return "foop";
}


} // namespace