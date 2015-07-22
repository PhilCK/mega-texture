#ifndef VECTOR4_INCLUDED_9C1AD544_35E1_456B_B882_DB0C9DAEBF09
#define VECTOR4_INCLUDED_9C1AD544_35E1_456B_B882_DB0C9DAEBF09


/*
  Vector4
  4D vector interface and impl.
*/


#include "vector_types.hpp"
#include "vector3.hpp"
#include "vector2.hpp"
#include "../general/general.hpp"
#include <array>
#include <assert.h>


namespace caffeine {
namespace math {


// ** Interface ** //


// Constants
inline vector4                vector4_zero();
inline vector4                vector4_one();
inline vector4                vector4_zero_zero_zero_one();

// Initialize vector.
inline vector4                vector4_init(const float val);
inline vector4                vector4_init(const float x, const float y, const float z, const float w);
inline vector4                vector4_init(const vector2 vec, const float z ,const float w);
inline vector4                vector4_init(const vector3 vec, const float w);
inline vector4                vector4_init_with_array(const float *arr);
inline vector4                vector4_init_with_array(const std::array<float, 4> &vec);

// Get components.
inline float                  vector4_get_x(const vector4 vec);
inline float                  vector4_get_y(const vector4 vec);
inline float                  vector4_get_z(const vector4 vec);
inline float                  vector4_get_w(const vector4 vec);
inline vector2                vector4_get_xy(const vector4 vec);
inline vector2                vector4_get_yz(const vector4 vec);
inline vector2                vector4_get_zw(const vector4 vec);
inline vector3                vector4_get_xyz(const vector4 vec);
inline vector3                vector4_get_yxw(const vector4 vec);
inline void                   vector4_to_array(const vector4 a, float *out_array);
inline std::array<float, 4>   vector4_to_std_array(const vector4 a);
inline vector3                vector4_to_vector3(const vector4 a); // not impl

// Component wise arithmetic.
inline vector4                vector4_add(const vector4 a, const vector4 b);
inline vector4                vector4_subtract(const vector4 a, const vector4 b);
inline vector4                vector4_multiply(const vector4 a, const vector4 b);
inline vector4                vector4_divide(const vector4 a, const vector4 b);

// Special operations.
inline vector4                vector4_lerp(const vector4 start, const vector4 end, const float dt);
//inline vector4                vector4_slerp(const vector4 start, const vector4 end, const float dt); // not impl
inline vector4                vector4_scale(const vector4 a, const float scale);
inline vector4                vector4_normalize(const vector4 a);
inline float                  vector4_length(const vector4 a);
inline float                  vector4_dot(const vector4 a, const vector4 b);


// ** Implimentation ** //

// This is not part of public interface, keep walking :)
// Using anything in the detail namespace is undefined-behaviour.
namespace detail
{
  struct internal_vec4
  {
    float x, y, z, w;
  };
}


// Constants
vector4
vector4_zero()
{
  return vector4_init(0.f);
}


vector4
vector4_one()
{
  return vector4_init(1.f);
}


vector4
vector4_zero_zero_zero_one()
{
  return vector4_init(0.f, 0.f, 0.f, 1.f);
}


// Initialize

vector4
vector4_init(const float val)
{
  return vector4_init(val, val, val, val);
}


vector4
vector4_init(const float x, const float y, const float z, const float w)
{
  vector4 return_vec;
  detail::internal_vec4 *internal_vec = reinterpret_cast<detail::internal_vec4*>(&return_vec);

  internal_vec->x = x;
  internal_vec->y = y;
  internal_vec->z = z;
  internal_vec->w = w;

  return return_vec;
}


vector4
vector4_init(const vector2 vec, const float z ,const float w)
{
  return vector4_init(vector2_get_x(vec), vector2_get_y(vec), z, w);
}


vector4
vector4_init(const vector3 vec, const float w)
{
  return vector4_init(vector3_get_x(vec), vector3_get_y(vec), vector3_get_z(vec), w);
}


vector4
vector4_init_with_array(const float *arr)
{
  return vector4_init(arr[0], arr[1], arr[2], arr[3]);
}


vector4
vector4_init_with_array(const std::array<float, 4> &arr)
{
  return vector4_init(arr.at(0), arr.at(1), arr.at(2), arr.at(3));
}


// Get components.

float
vector4_get_x(const vector4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return internal_vec->x;
}


float
vector4_get_y(const vector4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return internal_vec->y;
}


float
vector4_get_z(const vector4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return internal_vec->z;
}


float
vector4_get_w(const vector4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return internal_vec->w;
}


vector2
vector4_get_xy(const vector4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return vector2_init(internal_vec->x, internal_vec->y);
}


vector2
vector4_get_yz(const vector4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return vector2_init(internal_vec->y, internal_vec->z);
}


vector2
vector4_get_zw(const vector4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return vector2_init(internal_vec->z, internal_vec->w);
}


vector3
vector4_get_xyz(const vector4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return vector3_init(internal_vec->x, internal_vec->y, internal_vec->z);
}


vector3
vector4_get_yxw(const vector4 vec)
{
  const detail::internal_vec4 *internal_vec = reinterpret_cast<const detail::internal_vec4*>(&vec);
  return vector3_init(internal_vec->y, internal_vec->x, internal_vec->w);
}


void
vector4_to_array(const vector4 a, float *out_array)
{
  out_array[0] = vector4_get_x(a);
  out_array[1] = vector4_get_y(a);
  out_array[2] = vector4_get_z(a);
  out_array[3] = vector4_get_w(a);
}


std::array<float, 4>
vector4_to_std_array(const vector4 a)
{
  std::array<float, 4> return_array = {
    vector4_get_x(a),
    vector4_get_y(a),
    vector4_get_z(a),
    vector4_get_w(a),
  };

  return return_array;
}


vector4
vector4_add(const vector4 a, const vector4 b)
{
  const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
  const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

  return vector4_init(vec_a->x + vec_b->x, vec_a->y + vec_b->y, vec_a->z + vec_b->z, vec_a->w + vec_b->w);
}


vector4
vector4_subtract(const vector4 a, const vector4 b)
{
  const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
  const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

  return vector4_init(vec_a->x - vec_b->x, vec_a->y - vec_b->y, vec_a->z - vec_b->z, vec_a->w - vec_b->w);
}


vector4
vector4_multiply(const vector4 a, const vector4 b)
{
  const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
  const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

  return vector4_init(vec_a->x * vec_b->x, vec_a->y * vec_b->y, vec_a->z * vec_b->z, vec_a->w * vec_b->w);
}


vector4
vector4_divide(const vector4 a, const vector4 b)
{
  const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
  const detail::internal_vec4 *vec_b = reinterpret_cast<const detail::internal_vec4*>(&b);

  // Divide by zero check.
  assert(vec_b->x != 0 && vec_b->y != 0 && vec_b->z != 0 && vec_b->w != 0);

  return vector4_init(vec_a->x / vec_b->x, vec_a->y / vec_b->y, vec_a->z / vec_b->z, vec_a->w / vec_b->w);
}


vector4
vector4_lerp(const vector4 start, const vector4 end, const float dt)
{
  const vector4 difference = vector4_subtract(end, start);
  const vector4 scaled     = vector4_scale(difference, dt);
  const vector4 position   = vector4_add(start, scaled);

  return position;
}


// vector4
// vector4_slerp(const vector4 start, const vector4 end, const float dt)
// {

// }



vector4
vector4_scale(const vector4 a, const float scale)
{
  const vector4 scale_vec = vector4_init(scale);
  return vector4_multiply(a, scale_vec);
}


vector4
vector4_normalize(const vector4 a)
{
  const float length = vector4_length(a);

  assert(length != 0); // Don't pass zero vectors. (0,0,0,0);

  return vector4_scale(a, (1.f / length));
}


float
vector4_length(const vector4 a)
{
  const detail::internal_vec4 *vec_a = reinterpret_cast<const detail::internal_vec4*>(&a);
  const float x = vec_a->x * vec_a->x + vec_a->y * vec_a->y + vec_a->z * vec_a->z + vec_a->w * vec_a->w;

  return sqrt(x);
}


float
vector4_dot(const vector4 a, const vector4 b)
{
   return (vector4_get_x(a) * vector4_get_x(b)) +
          (vector4_get_y(a) * vector4_get_y(b)) +
          (vector4_get_z(a) * vector4_get_z(b)) + 
          (vector4_get_w(a) * vector4_get_w(b));
}


} // namespace
} // namespace


#endif // include guard