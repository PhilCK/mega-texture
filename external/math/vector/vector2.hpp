#ifndef VECTOR2_INCLUDED_077ABD34_E392_49BA_8472_D6541EF2744E
#define VECTOR2_INCLUDED_077ABD34_E392_49BA_8472_D6541EF2744E


/*
  Vector2
  2D vector interface and impl.
*/


#include "vector_types.hpp"
#include "../general/general.hpp"
#include <array>
#include <assert.h>


namespace caffeine {
namespace math {


// ** Interface ** //

// ** Constants ** //
inline vector2                vector2_zero();
inline vector2                vector2_one();
inline vector2                vector2_zero_one();

// ** Initialize new vectors ** //
inline vector2                vector2_init(const float val);
inline vector2                vector2_init(const float x, const float y);
inline vector2                vector2_init_with_array(const float *arr);
inline vector2                vector2_init_with_array(const std::array<float, 2> &vec);

// ** Get elements ** //
inline float                  vector2_get_x(const vector2 vec);
inline float                  vector2_get_y(const vector2 vec);
inline void                   vector2_to_array(const vector2 a, float *out_array);
inline std::array<float, 2>   vector2_to_std_array(const vector2 a);

// ** Arithmetic ** //
inline vector2                vector2_add(const vector2 a, const vector2 b);
inline vector2                vector2_subtract(const vector2 a, const vector2 b);
inline vector2                vector2_multiply(const vector2 a, const vector2 b);
inline vector2                vector2_divide(const vector2 a, const vector2 b);

// ** Special Operations ** //
inline vector2                vector2_lerp(const vector2 start, const vector2 end, const float dt);
//inline vector2                vector2_slerp(); // not impl
inline vector2                vector2_scale(const vector2 a, const float scale);
inline vector2                vector2_normalize(const vector2 a);
inline float                  vector2_length(const vector2 a);
inline float 	                vector2_cross(const vector2 a, const vector2 b);
inline float                  vector2_dot(const vector2 a, const vector2 b);


// ** Implimentation ** //

// This is not part of public interface, keep walking :)
// Using anything in the detail namespace is undefined-behaviour.
namespace detail
{
  struct internal_vec2
  {
    float x, y;
  };
}


// Constants

vector2
vector2_zero()
{
  return vector2_init(0.f);
}


vector2
vector2_one()
{
  return vector2_init(1.f);
}


vector2
vector2_zero_one()
{
  return vector2_init(0.f, 1.f);
}

// Initialize

vector2
vector2_init(const float val)
{
  return vector2_init(val, val);
}


vector2
vector2_init(const float x, const float y)
{
  vector2 return_vec;
  detail::internal_vec2 *internal_vec = reinterpret_cast<detail::internal_vec2*>(&return_vec);

  internal_vec->x = x;
  internal_vec->y = y;

  return return_vec;
}


vector2
vector2_init_with_array(const float *arr)
{
  return vector2_init(arr[0], arr[1]);
}


vector2
vector2_init_with_array(const std::array<float, 2> &arr)
{
  return vector2_init(arr.at(0), arr.at(1));
}


// Get components.

float
vector2_get_x(const vector2 vec)
{
  const detail::internal_vec2 *internal_vec = reinterpret_cast<const detail::internal_vec2*>(&vec);
  return internal_vec->x;
}


float
vector2_get_y(const vector2 vec)
{
  const detail::internal_vec2 *internal_vec = reinterpret_cast<const detail::internal_vec2*>(&vec);
  return internal_vec->y;
}


void
vector2_to_array(const vector2 a, float *out_array)
{
  out_array[0] = vector2_get_x(a);
  out_array[1] = vector2_get_y(a);
}


std::array<float, 2>
vector2_to_std_array(const vector2 a)
{
  std::array<float, 2> return_array = {
    vector2_get_x(a),
    vector2_get_y(a),
  };

  return return_array;
}



vector2
vector2_add(const vector2 a, const vector2 b)
{
  const detail::internal_vec2 *vec_a = reinterpret_cast<const detail::internal_vec2*>(&a);
  const detail::internal_vec2 *vec_b = reinterpret_cast<const detail::internal_vec2*>(&b);

  return vector2_init(vec_a->x + vec_b->x, vec_a->y + vec_b->y);
}


vector2
vector2_subtract(const vector2 a, const vector2 b)
{
  const detail::internal_vec2 *vec_a = reinterpret_cast<const detail::internal_vec2*>(&a);
  const detail::internal_vec2 *vec_b = reinterpret_cast<const detail::internal_vec2*>(&b);

  return vector2_init(vec_a->x - vec_b->x, vec_a->y - vec_b->y);
}


vector2
vector2_multiply(const vector2 a, const vector2 b)
{
  const detail::internal_vec2 *vec_a = reinterpret_cast<const detail::internal_vec2*>(&a);
  const detail::internal_vec2 *vec_b = reinterpret_cast<const detail::internal_vec2*>(&b);

  return vector2_init(vec_a->x * vec_b->x, vec_a->y * vec_b->y);
}


vector2
vector2_divide(const vector2 a, const vector2 b)
{
  const detail::internal_vec2 *vec_a = reinterpret_cast<const detail::internal_vec2*>(&a);
  const detail::internal_vec2 *vec_b = reinterpret_cast<const detail::internal_vec2*>(&b);

  // Divide by zero check.
  assert(vec_b->x != 0 && vec_b->y != 0);

  return vector2_init(vec_a->x / vec_b->x, vec_a->y / vec_b->y);
}


vector2
vector2_lerp(const vector2 start, const vector2 end, const float dt)
{
  const vector2 difference = vector2_subtract(end, start);
  const vector2 scaled     = vector2_scale(difference, dt);
  const vector2 position   = vector2_add(start, scaled);

  return position;
}


vector2
vector2_scale(const vector2 a, const float scale)
{
  const vector2 scale_vec = vector2_init(scale);
  return vector2_multiply(a, scale_vec);
}


vector2
vector2_normalize(const vector2 a)
{
  const float length = vector2_length(a);

  assert(length != 0); // Don't pass zero vectors. (0,0);

  return vector2_scale(a, (1.f / length));
}


float
vector2_length(const vector2 a)
{
  const detail::internal_vec2 *vec_a = reinterpret_cast<const detail::internal_vec2*>(&a);
  const float x = vec_a->x * vec_a->x + vec_a->y * vec_a->y;

  return sqrt(x);
}


float
vector2_cross(const vector2 a, const vector2 b)
{
  return (vector2_get_x(a) * vector2_get_y(b)) -
         (vector2_get_y(a) * vector2_get_x(b));
}


float
vector2_dot(const vector2 a, const vector2 b)
{
  return (vector2_get_x(a) * vector2_get_x(b)) +
         (vector2_get_y(a) * vector2_get_y(b));
}


} // namespace
} // namespace


#endif // include guard