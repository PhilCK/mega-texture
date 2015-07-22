#ifndef GENERAL_INCLUDED_BA1581F2_35C3_4026_BCDA_6E9EC3CF253E
#define GENERAL_INCLUDED_BA1581F2_35C3_4026_BCDA_6E9EC3CF253E


/*
  These are general math functions and constants.
  Alot of them are simple stubs to stl functions.
*/


#include <cmath>
#include <algorithm>


namespace caffeine {
namespace math {


// ** INTERFACE ** //


// ** Constants ** //

inline float              pi()          { return 3.14159265359f;  }
inline float              two_pi()      { return 2.f * pi();      }
inline float              half_pi()     { return pi() * 0.5f;     }
inline float              tau()         { return two_pi();        }
inline float              half_tau()    { return pi();            }
inline float              quart_tau()   { return half_pi();       }
inline float              g_ratio()     { return 1.61803398875f;  }
inline float              root_two()    { return 1.41421356237f;  }
inline float              root_three()  { return 1.73205080757f;  }


// ** Trig ** //

inline float              tan(const float x);
inline float              a_tan2(const float x, const float y);
inline float              cos(const float radians);
inline float              a_cos(const float radians);
inline float              sin(const float radians);
inline float              a_sin(const float radians);


// ** Degs/Rads ** //

inline float              radians_to_degrees(const float radians); // TODO:
inline float              degrees_to_radians(const float degrees); // TODO:


// ** Other general ** //

inline float              sqrt(const float x);
inline float              abs(const float x);
inline float              max(const float a, const float b); // TODO:
inline float              min(const float a, const float b); // TODO:
inline float              clamp(const float x, const float between_a, const float between_b);
inline bool               is_between(const float to_check, const float a, const float b); // TODO:
inline bool               is_near(const float actual, const float target, const float error_margin); // TODO: 
inline bool               is_pow_two(const uint32_t i); // TODO: 
inline bool               fmod(const float x, const float max); // TODO;



// ** IMPL ** //


float
sqrt(const float x)
{
  return std::sqrt(x);
}


float
clamp(const float x, const float between_a, const float between_b)
{
  const float highest = between_a >= between_b ? between_a : between_b;
  const float lowest  = between_a != highest ? between_a : between_b;

  if(x >= highest)
  {
    return highest;
  }
  else if(x <= lowest)
  {
    return lowest;
  }

  return x;
}


float
abs(const float x)
{
  return std::abs(x);
}


float
tan(const float x)
{
  return std::tan(x);
}


float
a_tan2(const float x, const float y)
{
  return std::atan2(x, y);
}


float
cos(const float radians)
{
  return std::cos(radians);
}


float
a_cos(const float radians)
{
  return std::acos(radians);
}


float
sin(const float radians)
{
  return std::sin(radians);
}


float
a_sin(const float radians)
{
  return std::asin(radians);
}


} // namespace
} // namespace


#endif // include guard