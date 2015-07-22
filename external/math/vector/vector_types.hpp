#ifndef VECTOR_TYPES_INCLUDED_76A0D96A_8385_4283_9EA9_CF89D0D5A10C
#define VECTOR_TYPES_INCLUDED_76A0D96A_8385_4283_9EA9_CF89D0D5A10C


/*
  Vector Types.
  These are the vector types in caffeine math.
*/


namespace caffeine {
namespace math {


// To access the components you should go through the vector interface.

class vector4
{
  float data[4];
};

class vector3
{
  float data[3];
};

class vector2
{
  float data[2];
};

  
} // namespace
} // namespace


// aliases
namespace caffmath = ::caffeine::math;
namespace caff_math = ::caffeine::math;


#endif // include guard