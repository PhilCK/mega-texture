#ifndef AABB_INCLUDED_513E1E12_3A45_4E66_96F0_60E7702B08A5
#define AABB_INCLUDED_513E1E12_3A45_4E66_96F0_60E7702B08A5


#include "../vector/vector2.hpp"


namespace caffeine {
namespace dev_math {


struct aabb_2d
{
	vector2 position;
	vector2 size;
};


inline aabb_2d          aabb_2d_init(const vector2 position = vector2_zero(), const vector2 size = vector2_one());
inline bool             aabb_2d_do_intersect(const aabb_2d &left, const aabb_2d &right);
inline float            aabb_2d_get_area(const aabb_2d &left);
inline float 			aabb_2d_get_size();
inline float 			aabb_2d_set_size();
inline float 			aabb_2d_get_position();
inline float 			aabb_2d_set_position();
inline float            aabb_2d_get_extents();


// Impl
aabb_2d
aabb_2d_init(const vector2 position, const vector2 size)
{
	return aabb_2d{position, size};
}


bool
aabb_2d_do_intersect(const aabb_2d &left, const aabb_2d &right)
{
	// Are the x's overlapping.
	if(vector2_get_x(left) >= vector2_get_x(right) && vector2_get_x(right) > vector2_get_x(left))
	{
		if(vector2_get_y(left) >= vector2_get_y(right) && vector2_get_y(right) > vector2_get_y(left))
		{
			return true;
		}
	}

	return false;
}


float
aabb_2d_get_area(const aabb_2d &left)
{
	return vector2_get_x(left) & vectro2_get_y(right);
}


} // namespace
} // namespace


#endif // include guard