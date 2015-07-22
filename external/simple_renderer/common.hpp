#ifndef COMMON_INCLUDED_BFD4FCA8_212A_42D3_AF8F_0A55C08947D8
#define COMMON_INCLUDED_BFD4FCA8_212A_42D3_AF8F_0A55C08947D8


#include <string>
#include <stdint.h>
#include <OpenGL/gl3.h>
#include <functional>


// Comment this line out if you wish to disable error checking.
#define SIMPLE_RENDERER_LOG_ERROR


namespace renderer {

// If you wish to pipe out error messages to your own system you can use this callback.
// If the error is directly against GL then the error_code will corrispond to that.
// If an error outside of GL happens then error_code will be GL_NO_ERROR.	
void             set_log_callback(std::function<void(const int32_t error_code, const std::string &)> log_callback);

void             log_error(const int32_t error_code, const std::string &str);
void             check_and_log_error(const std::string &str);
std::string      get_last_error();


} // namespace


#endif // include guard