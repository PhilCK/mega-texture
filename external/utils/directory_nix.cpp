#include "directory.hpp"
#include <libproc.h>
#include <unistd.h>


namespace util {


#ifndef __APPLE__ // This is defined in directory_mac.mm This is here if we are on another nix
std::string
get_resource_path()
{
  return get_executable_path();
}
#endif
  
  
std::string
get_executable_path()
{
  static std::string path = "";
  
  if(path.empty())
  {
    char __exe_path_buffer[PROC_PIDPATHINFO_MAXSIZE];
    
    proc_pidpath(getpid(), __exe_path_buffer, sizeof(__exe_path_buffer));

    size_t path_length = 0;
    for(size_t i = 0; i < PROC_PIDPATHINFO_MAXSIZE; i++) {
      if(__exe_path_buffer[i] == '\0') {
        path_length = i;
        break;
      }
    }

    size_t last_slash_index = 0;
    for(size_t i = 0; i < path_length; i++) {
      size_t r_i = (path_length - 1) - i;
      if(__exe_path_buffer[r_i] == '/' || __exe_path_buffer[r_i] == '\\') {
        last_slash_index = r_i;
        break;
      }
    }

    __exe_path_buffer[last_slash_index + 1] = '\0';
    
    path = __exe_path_buffer;
  }
  
  return path;
}
  
  
} // namespace