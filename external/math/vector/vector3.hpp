#ifndef VECTOR3_INCLUDED_DD26787F_2663_4F50_85D0_7188D23BA64B
#define VECTOR3_INCLUDED_DD26787F_2663_4F50_85D0_7188D23BA64B


/*
  Vector3
  3D vector interface and impl.
*/


#include "vector_types.hpp"
#include "vector2.hpp"
#include "../general/general.hpp"
#include <array>
#include <assert.h>


namespace caffeine {
namespace math {


// ** Interface ** //

// Constants
inline vector3                vector3_zero();
inline vector3                vector3_one();
inline vector3                vector3_zero_zero_one();

// Initialize vector.
inline vector3                vector3_init(const float val);
inline vector3                vector3_init(const float x, const float y, const float z);
inline vector3                vector3_init(const vector2 vec, const float z);
inline vector3                vector3_init_with_array(const float *arr);
inline vector3                vector3_init_with_array(const std::array<float, 3> &vec);

// Get components.
inline float                  vector3_get_x(const vector3 vec);
inline float                  vector3_get_y(const vector3 vec);
inline float                  vector3_get_z(const vector3 vec);
inline vector2                vector3_get_xy(const vector3 vec);
inline vector2                vector3_get_yz(const vector3 vec);
inline void                   vector3_to_array(const vector3 a, float *out_array);
inline std::array<float, 3>   vector3_to_std_array(const vector3 a);

// Component wise arithmetic.
inline vector3                vector3_add(const vector3 a, const vector3 b);
inline vector3                vector3_subtract(const vector3 a, const vector3 b);
inline vector3                vector3_multiply(const vector3 a, const vector3 b);
inline vector3                vector3_divide(const vector3 a, const vector3 b);

// Special operations.
inline vector3                vector3_lerp(const vector3 start, const vector3 end, const float dt);
//inline vector3                vector3_slerp(const vector3 start, const vector3 end, const float dt);
inline vector3                vector3_scale(const vector3 a, const float scale);
inline vector3                vector3_normalize(const vector3 a);
inline float                  vector3_length(const vector3 a);
inline vector3                vector3_cross(const vector3 a, const vector3 b); // not impl
inline float                  vector3_dot(const vector3 a, const vector3 b); // not impl


// ** Implimentation ** //

// This is not part of public interface, keep walking :)
// Using anything in the detail namespace is undefined-behaviour.
namespace detail
{
  struct internal_vec3
  {
    float x, y, z;
  };
}


// Constants
vector3
vector3_zero()
{
  return vector3_init(0.f);
}


vector3
vector3_one()
{
  return vector3_init(1.f);
}


vector3
vector3_zero_zero_one()
{
  return vector3_init(0.f, 0.f, 1.f);
}

// Initialize

vector3
vector3_init(const float val)
{
  return vector3_init(val, val, val);
}


vector3
vector3_init(const float x, const float y, const float z)
{
  vector3 return_vec;
  detail::internal_vec3 *internal_vec = reinterpret_cast<detail::internal_vec3*>(&return_vec);

  internal_vec->x = x;
  internal_vec->y = y;
  internal_vec->z = z;

  return return_vec;
}


vector3
vector3_init(const vector2 vec, const float z)
{
  return vector3_init(vector2_get_x(vec), vector2_get_y(vec), z);
}


vector3
vector3_init_with_array(const float *arr)
{
  return vector3_init(arr[0], arr[1], arr[2]);
}


vector3
vector3_init_with_array(const std::array<float, 3> &arr)
{
  return vector3_init(arr.at(0), arr.at(1), arr.at(2));
}


// Get components.

float
vector3_get_x(const vector3 vec)
{
  const detail::internal_vec3 *internal_vec = reinterpret_cast<const detail::internal_vec3*>(&vec);
  return internal_vec->x;
}


float
vector3_get_y(const vector3 vec)
{
  const detail::internal_vec3 *internal_vec = reinterpret_cast<const detail::internal_vec3*>(&vec);
  return internal_vec->y;
}


float
vector3_get_z(const vector3 vec)
{
  const detail::internal_vec3 *internal_vec = reinterpret_cast<const detail::internal_vec3*>(&vec);
  return internal_vec->z;
}


void
vector3_to_array(const vector3 a, float *out_array)
{
  out_array[0] = vector3_get_x(a);
  out_array[1] = vector3_get_y(a);
  out_array[2] = vector3_get_z(a);
}


vector2
vector3_get_xy(const vector3 vec)
{
  const detail::internal_vec3 *internal_vec = reinterpret_cast<const detail::internal_vec3*>(&vec);
  return vector2_init(internal_vec->x, internal_vec->y);
}


vector2
vector3_get_yz(const vector3 vec)
{
  const detail::internal_vec3 *internal_vec = reinterpret_cast<const detail::internal_vec3*>(&vec);
  return vector2_init(internal_vec->y, internal_vec->z);
}


std::array<float, 3>
vector3_to_std_array(const vector3 a)
{
  std::array<float, 3> return_array = {
    vector3_get_x(a),
    vector3_get_y(a),
    vector3_get_z(a),
  };

  return return_array;
}



vector3
vector3_add(const vector3 a, const vector3 b)
{
  const detail::internal_vec3 *vec_a = reinterpret_cast<const detail::internal_vec3*>(&a);
  const detail::internal_vec3 *vec_b = reinterpret_cast<const detail::internal_vec3*>(&b);
  
  const float x = vec_a->x + vec_b->x;
  const float y = vec_a->y + vec_b->y;
  const float z = vec_a->z + vec_b->z;

  return vector3_init(x, y, z);
}


vector3
vector3_subtract(const vector3 a, const vector3 b)
{
  const detail::internal_vec3 *vec_a = reinterpret_cast<const detail::internal_vec3*>(&a);
  const detail::internal_vec3 *vec_b = reinterpret_cast<const detail::internal_vec3*>(&b);

  return vector3_init(vec_a->x - vec_b->x, vec_a->y - vec_b->y, vec_a->z - vec_b->z);
}


vector3
vector3_multiply(const vector3 a, const vector3 b)
{
  const detail::internal_vec3 *vec_a = reinterpret_cast<const detail::internal_vec3*>(&a);
  const detail::internal_vec3 *vec_b = reinterpret_cast<const detail::internal_vec3*>(&b);

  return vector3_init(vec_a->x * vec_b->x, vec_a->y * vec_b->y, vec_a->z * vec_b->z);
}


vector3
vector3_divide(const vector3 a, const vector3 b)
{
  const detail::internal_vec3 *vec_a = reinterpret_cast<const detail::internal_vec3*>(&a);
  const detail::internal_vec3 *vec_b = reinterpret_cast<const detail::internal_vec3*>(&b);

  // Divide by zero check.
  assert(vec_b->x != 0 && vec_b->y != 0 && vec_b->z != 0);

  return vector3_init(vec_a->x / vec_b->x, vec_a->y / vec_b->y, vec_a->z / vec_b->z);
}


vector3
vector3_lerp(const vector3 start, const vector3 end, const float dt)
{
  const vector3 difference = vector3_subtract(end, start);
  const vector3 scaled     = vector3_scale(difference, dt);
  const vector3 position   = vector3_add(start, scaled);

  return position;
}


// vector3
// vector3_slerp(const vector3 start, const vector3 end, const float dt)
// {
//   const float dot                   = vector3_dot(start, end);
//   const float clamp                 = caffmath::clamp(dot, -1.f, 1.f);
//   const float theta                 = caffmath::a_cos(clamp) * dt;

//   const vector3 start_scale         = vector3_scale(start, dot);
//   const vector3 relative            = vector3_subtract(end, start_scale);
//   const vector3 normal              = vector3_normalize(relative);

//   const float cos_theta             = caffmath::cos(theta);
//   const float sin_theta             = caffmath::sin(theta);

//   const vector3 start_cos_scale     = vector3_scale(start, cos_theta);
//   const vector3 relative_sin_scale  = vector3_scale(normal, sin_theta);

//   return vector3_add(start_cos_scale, relative_sin_scale);
// }


vector3
vector3_scale(const vector3 a, const float scale)
{
  const vector3 scale_vec = vector3_init(scale);
  return vector3_multiply(a, scale_vec);
}


vector3
vector3_normalize(const vector3 a)
{
  const float length = vector3_length(a);

  assert(length != 0); // Don't pass zero vectors. (0,0,0);

  return vector3_scale(a, (1.f / length));
}


float
vector3_length(const vector3 a)
{
  const detail::internal_vec3 *vec_a = reinterpret_cast<const detail::internal_vec3*>(&a);
  const float x = vec_a->x * vec_a->x + vec_a->y * vec_a->y + vec_a->z * vec_a->z;

  return sqrt(x);
}


vector3
vector3_cross(const vector3 a, const vector3 b)
{
  const float x = (vector3_get_y(a) * vector3_get_z(b)) - (vector3_get_z(a) * vector3_get_y(b));
  const float y = (vector3_get_x(a) * vector3_get_z(b)) - (vector3_get_z(a) * vector3_get_x(b));
  const float z = (vector3_get_x(a) * vector3_get_y(b)) - (vector3_get_y(a) * vector3_get_x(b));

  return vector3_init(x, -y, z);
}


float
vector3_dot(const vector3 a, const vector3 b)
{
  return (vector3_get_x(a) * vector3_get_x(b)) +
         (vector3_get_y(a) * vector3_get_y(b)) +
         (vector3_get_z(a) * vector3_get_z(b));
}


} // namespace
} // namespace


#endif // include guard