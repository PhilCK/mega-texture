#ifndef TO_STRING_INCLUDED_AA5F04E4_BDD9_4385_99F2_2C9E7210FDD6
#define TO_STRING_INCLUDED_AA5F04E4_BDD9_4385_99F2_2C9E7210FDD6


#include "../vector/vector_types.hpp"
#include "../vector/vector2.hpp"
#include "../vector/vector3.hpp"
#include "../vector/vector4.hpp"
#include "../matrix/matrix_types.hpp"
#include "../matrix/matrix44.hpp"
#include <string>


namespace caffeine {
namespace math {


inline std::string to_string(const vector2 vec);
inline std::string to_string(const vector3 vec);
inline std::string to_string(const vector4 vec);
inline std::string to_string(const matrix44 mat, const bool line_breaks = false);


std::string
to_string(const vector2 vec)
{
	const std::string x = std::to_string(vector2_get_x(vec));
	const std::string y = std::to_string(vector2_get_y(vec));

	return std::string("Vec2: " + x + ", " + y);
}


std::string
to_string(const vector3 vec)
{
	const std::string x = std::to_string(vector3_get_x(vec));
	const std::string y = std::to_string(vector3_get_y(vec));
	const std::string z = std::to_string(vector3_get_z(vec));

	return std::string("Vec3: " + x + ", " + y + ", " + z);
}

std::string
to_string(const vector4 vec)
{
	const std::string x = std::to_string(vector4_get_x(vec));
	const std::string y = std::to_string(vector4_get_y(vec));
	const std::string z = std::to_string(vector4_get_z(vec));
	const std::string w = std::to_string(vector4_get_w(vec));

	return std::string("Vec4: " + x + ", " + y + ", " + z + ", " + w);
}


std::string
to_string(const matrix44 mat, const bool line_breaks)
{
	const std::string row_break = line_breaks ? "\n" : ", ";

	const std::string row_1 = std::to_string(matrix44_get(mat, 0, 0)) + ", " + std::to_string(matrix44_get(mat, 0, 1)) + ", " + std::to_string(matrix44_get(mat, 0, 2)) + ", " + std::to_string(matrix44_get(mat, 0, 3)) + row_break;
	const std::string row_2 = std::to_string(matrix44_get(mat, 1, 0)) + ", " + std::to_string(matrix44_get(mat, 1, 1)) + ", " + std::to_string(matrix44_get(mat, 1, 2)) + ", " + std::to_string(matrix44_get(mat, 1, 3)) + row_break;
	const std::string row_3 = std::to_string(matrix44_get(mat, 2, 0)) + ", " + std::to_string(matrix44_get(mat, 2, 1)) + ", " + std::to_string(matrix44_get(mat, 2, 2)) + ", " + std::to_string(matrix44_get(mat, 2, 3)) + row_break;
	const std::string row_4 = std::to_string(matrix44_get(mat, 3, 0)) + ", " + std::to_string(matrix44_get(mat, 3, 1)) + ", " + std::to_string(matrix44_get(mat, 3, 2)) + ", " + std::to_string(matrix44_get(mat, 3, 3));

	const std::string title_break = line_breaks ? "\n" : " ";

	return std::string("Mat44:" + title_break + row_1 + row_2 + row_3 + row_4);
}


} // namespace
} // namespace


#endif // include guard