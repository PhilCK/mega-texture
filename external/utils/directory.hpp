#ifndef DIRECTORY_INCLUDED_5551C33B_D835_42D2_99FF_661889A068A0
#define DIRECTORY_INCLUDED_5551C33B_D835_42D2_99FF_661889A068A0


#include <string>


namespace util {
  
//! Returns the resource folder on mac, exe_path on others. Prefer this.
std::string           get_resource_path();
  
//! Returns the executable path with trailing slash. Prefer resource path.
std::string           get_executable_path();
  
  
} // namespace


#endif // include guard