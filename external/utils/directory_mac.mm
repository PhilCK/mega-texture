#include "directory.hpp"
#import <dirent.h>
#import <Foundation/Foundation.h>


namespace util {


std::string
get_resource_path()
{
  static std::string path;
  
  if(path.empty())
  {
    NSBundle *bundle = [NSBundle mainBundle];
    
    if(bundle == nil) {
      return "";
    }
    else
    {
      NSString *rPath = [bundle resourcePath];
      path = [rPath UTF8String] + std::string("/");
    }
  }
  
  return path;
}
  
  
} // namespace