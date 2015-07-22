#ifndef TRANSFORM_TYPES_INCLUDED_8D47B2F6_5764_483F_98BA_CE06AEEFB50C
#define TRANSFORM_TYPES_INCLUDED_8D47B2F6_5764_483F_98BA_CE06AEEFB50C


#include "../quaternion/quaternion.hpp"
#include "../vector/vector3.hpp"


namespace caffeine {
namespace math {


struct transform
{
  transform() {}

  quaternion  rotation  = quaternion_init(0.f,0.f,0.f,1.f);
  vector3     scale     = vector3_init(1.f,1.f,1.f);
  vector3     position  = vector3_init(0.f,0.f,0.f);
}; // class


} // namespace
} // namespace


#endif // include guard