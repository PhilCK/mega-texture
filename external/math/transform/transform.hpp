#ifndef TRANSFORM_INCLUDED_E79342EA_D69A_4FEB_BEAD_BA21D7B8B8E6
#define TRANSFORM_INCLUDED_E79342EA_D69A_4FEB_BEAD_BA21D7B8B8E6


/*
  Transform
  --
  Transform is a position and rotation in 3D space.
*/


#include "../transform/transform_types.hpp"
#include "../quaternion/quaternion.hpp"
#include "../vector/vector3.hpp"
#include "../matrix/matrix44.hpp"


namespace caffeine {
namespace math {


// ** Interface ** //

inline transform  transform_init(const vector3 position, const vector3 scale, const quaternion &rotation);
inline matrix44   transform_get_world_matrix(const transform &transform);
inline void       transform_set_with_world_matrix(transform &transform, const matrix44 &matrix);
inline transform  transform_inherited(const transform &parent, const transform &child);


// ** Impl ** //


transform
transform_init(const vector3 position, const vector3 scale, const quaternion &rotation)
{
  transform return_transform;
  return_transform.position = position;
  return_transform.scale    = scale;
  return_transform.rotation = rotation;

  return return_transform;
}


transform
transform_inherited(const transform &parent, const transform &child)
{
  transform inherited;

  inherited.scale    = caffmath::vector3_multiply(parent.scale, child.scale);
  inherited.position = caffmath::vector3_subtract(parent.position, child.position);

  // TODO: Rotation.

  return inherited;
}


} // namespace
} // namespace


#endif // include guard