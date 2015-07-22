#ifndef QUATERNION_INCLUDED_ED2C1105_82A1_477F_B62C_5093DE20C971
#define QUATERNION_INCLUDED_ED2C1105_82A1_477F_B62C_5093DE20C971

/*
  Quaternion
  Used for rotation math.
*/

// ** NOT COMPLETE ** //


#include "quaternion_types.hpp"
#include "../general/general.hpp"
#include "../matrix/matrix44.hpp"
#include "../matrix/matrix33.hpp"
#include "../vector/vector3.hpp"


namespace caffmath = ::caffeine::math;

namespace caffeine {
namespace math {


// Interface

inline quaternion       quaternion_init();
inline quaternion       quaternion_init(const float x, const float y, const float z, const float w);
inline quaternion       quaternion_init_with_axis_angle(const float x, const float y, const float z, const float theta_radians);
inline quaternion       quaternion_init_with_euler_angles(const float pitch_radians, const float yaw_radians, const float roll_radians);

inline quaternion       quaternion_conjugate(const quaternion to_conj);
inline quaternion       quaternion_multiply(const quaternion left, const quaternion &right);
inline quaternion       quaternion_normalize(const quaternion to_normalize);
inline float            quaternion_length(const quaternion to_length);
inline vector3          quaternion_rotate_point(const quaternion rotation, const vector3 point);

inline matrix33         quaternion_get_rotation_matrix();
inline vector3          quaternion_get_axis();
inline vector3          quaternion_get_euler_angles_in_radians();

inline float            quaternion_get_x(const quaternion quat);
inline float            quaternion_get_y(const quaternion quat);
inline float            quaternion_get_z(const quaternion quat);
inline float            quaternion_get_w(const quaternion quat);
inline float            quaternion_get(const quaternion quat, const uint32_t i);


// Impl

namespace detail
{
  struct internal_quat
  {
    float x, y, z, w;
  };
}


quaternion
quaternion_init()
{
  return quaternion_init(0,0,0,1);
}


quaternion
quaternion_init(const float x, const float y, const float z, const float w)
{
  quaternion return_quat;
  detail::internal_quat *internal_quat = reinterpret_cast<detail::internal_quat*>(&return_quat);

  internal_quat->x = x;
  internal_quat->y = y;
  internal_quat->z = z;
  internal_quat->w = w;

  return return_quat;
}


quaternion
quaternion_init_with_axis_angle(const float x, const float y, const float z, const float theta_radians)
{
  // TODO: Normalize Axis.

  const float half_angle = 0.5f * theta_radians;
  const float sin_angle  = caffmath::sin(half_angle);

  const float qx = sin_angle * x;
  const float qy = sin_angle * y;
  const float qz = sin_angle * z;
  const float qw = caffmath::cos(half_angle);  

  return quaternion_init(qx, qy, qz, qw);
}


quaternion
quaternion_init_with_euler_angles(const float pitch_radians, const float yaw_radians, const float roll_radians)
{
  const float half_roll  = roll_radians * 0.5f;
  const float half_yaw   = yaw_radians * 0.5f;
  const float half_pitch = pitch_radians * 0.5f;

  const float cz = caffmath::cos(half_roll);
  const float sz = caffmath::sin(half_roll);
  const float cy = caffmath::cos(half_yaw);
  const float sy = caffmath::sin(half_yaw);
  const float cx = caffmath::cos(half_pitch);
  const float sx = caffmath::sin(half_pitch);

  const float w = cz * cy * cx + sz * sy * sz;
  const float x = cz * cy * sx - sz * sy * cz;
  const float y = cz * sy * cx + sz * cy * sz;
  const float z = sz * cy * cx - cz * sy * sz;

  return quaternion_init(x, y, z, w);
}


quaternion
quaternion_conjugate(const quaternion to_conj)
{
  const detail::internal_quat *conj_quat = reinterpret_cast<const detail::internal_quat*>(&to_conj);
  return quaternion_init(-conj_quat->x, -conj_quat->y, -conj_quat->z, conj_quat->w);
}


quaternion
quaternion_multiply(const quaternion left, const quaternion &right)
{
  const detail::internal_quat *left_quat  = reinterpret_cast<const detail::internal_quat*>(&left);
  const detail::internal_quat *right_quat = reinterpret_cast<const detail::internal_quat*>(&right);

  const float w = (left_quat->w * right_quat->w) - (left_quat->x * right_quat->x) - (left_quat->y * right_quat->y) - (left_quat->z * right_quat->z);
  const float x = (left_quat->w * right_quat->x) + (left_quat->x * right_quat->w) + (left_quat->y * right_quat->z) - (left_quat->z * right_quat->y);
  const float y = (left_quat->w * right_quat->y) + (left_quat->y * right_quat->w) + (left_quat->z * right_quat->x) - (left_quat->x * right_quat->z);
  const float z = (left_quat->w * right_quat->z) + (left_quat->z * right_quat->w) + (left_quat->x * right_quat->y) - (left_quat->y * right_quat->x);

  return quaternion_init(x, y, z, w);
}


quaternion
quaternion_normalize(const quaternion to_normalize)
{
  const detail::internal_quat *internal_quat = reinterpret_cast<const detail::internal_quat*>(&to_normalize);

  const float length = quaternion_length(to_normalize);
  assert(length); // Can't have zero length.

  const float w = internal_quat->w / length;
  const float x = internal_quat->x / length;
  const float y = internal_quat->y / length;
  const float z = internal_quat->z / length;

  return quaternion_init(x,y,z,w);
}


float
quaternion_length(const quaternion to_length)
{
  const detail::internal_quat *length_quat = reinterpret_cast<const detail::internal_quat*>(&to_length);
  return caffmath::sqrt((length_quat->w * length_quat->w) + (length_quat->x * length_quat->x) + (length_quat->y * length_quat->y) + (length_quat->z * length_quat->z));
}


vector3
quaternion_rotate_point(const quaternion rotation, const vector3 point)
{
  const detail::internal_quat *rot_quat = reinterpret_cast<const detail::internal_quat*>(&rotation);

  const float x = rot_quat->x;
  const float y = rot_quat->y;
  const float z = rot_quat->z;
  const float w = rot_quat->w;

  const float p_x = w*w*vector3_get_x(point) + 2*y*w*vector3_get_z(point) - 2*z*w*vector3_get_y(point) + x*x* vector3_get_x(point) + 2*y*x*vector3_get_y(point) + 2*z*x*vector3_get_z(point) - z*z*vector3_get_x(point) - y*y*vector3_get_x(point);
  const float p_y = 2*x*y*vector3_get_x(point) + y*y*vector3_get_y(point) + 2*z*y*vector3_get_z(point) + 2*w*z* vector3_get_x(point) - z*z*vector3_get_y(point) + w*w*vector3_get_y(point) - 2*x*w*vector3_get_z(point) - x*x*vector3_get_y(point);
  const float p_z = 2*x*z*vector3_get_x(point) + 2*y*z*vector3_get_y(point) + z*z*vector3_get_z(point) - 2*w*y* vector3_get_x(point) - y*y*vector3_get_z(point) + 2*w*x*vector3_get_y(point) - x*x*vector3_get_z(point) + w*w*vector3_get_z(point);

  const vector3 rotated_point = vector3_init(p_x, p_y, p_z);

  return rotated_point;
}


// matrix33
// quaternion_get_rotation_matrix()
// {

// }


// vector3
// quaternion_get_axis()
// {

// }


// vector3
// quaternion_get_euler_angles_in_radians()
// {

// }


float
quaternion_get_x(const quaternion quat)
{
  const detail::internal_quat *internal_quat = reinterpret_cast<const detail::internal_quat*>(&quat);
  return internal_quat->x;
}


float
quaternion_get_y(const quaternion quat)
{
  const detail::internal_quat *internal_quat = reinterpret_cast<const detail::internal_quat*>(&quat);
  return internal_quat->y;
}


float
quaternion_get_z(const quaternion quat)
{
  const detail::internal_quat *internal_quat = reinterpret_cast<const detail::internal_quat*>(&quat);
  return internal_quat->z;
}


float
quaternion_get_w(const quaternion quat)
{
  const detail::internal_quat *internal_quat = reinterpret_cast<const detail::internal_quat*>(&quat);
  return internal_quat->w;
}


float
quaternion_get(const quaternion quat, const uint32_t i)
{
  assert(i < 4);

  switch(i)
  {
    case(0):
      return quaternion_get_x(quat);
      break;
    case(1):
      return quaternion_get_y(quat);
      break;
    case(2):
      return quaternion_get_z(quat);
      break;
    case(3):
      return quaternion_get_w(quat);
      break; 
    default:
      assert(false);
  }
}





} // namespace
} // namespace


#endif // include guard