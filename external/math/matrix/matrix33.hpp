#ifndef MATRIX33_INCLUDED_D6DCE389_FA7C_4C48_A303_C0EDC9A90C8B
#define MATRIX33_INCLUDED_D6DCE389_FA7C_4C48_A303_C0EDC9A90C8B


/*
  Matrix33
  Through out this library we consider a 3x3 matrix as a 
  rotation and scale matrix specifically.
*/


#include "matrix_types.hpp"
#include "../vector/vector3.hpp"
#include <array>
#include <cstring>


namespace caffeine {
namespace math {


// Constants
inline matrix33                   matrix33_id();
inline matrix33                   matrix33_zero();

// Init
inline matrix33                   matrix33_init(); // will return an id array
inline matrix33                   matrix33_init(const float x);
inline matrix33                   matrix33_init_with_array(const float arr[]);
inline matrix33                   matrix33_init_with_array(const std::array<float, 9> &array);

// Operations
inline matrix33                   matrix33_add(const matrix33 &lhs, const matrix33 &rhs);
inline matrix33                   matrix33_subtract(const matrix33 &lhs, const matrix33 &rhs);
inline matrix33                   matrix33_scale(const float scale, const matrix33 &b);
inline vector3                    matrix33_multiply(const vector3 vector, const matrix33 &b);
inline matrix33                   matrix33_multiply(const matrix33 &lhs, const matrix33 &rhs);
inline matrix33                   matrix33_rotate(const matrix33 &lhs, const vector3 euler);

// Transform matrices into other forms
inline matrix33                   matrix33_get_transpose(const matrix33 &a);
inline matrix33                   matrix33_get_inverse(const matrix33 &a);
inline matrix33                   matrix33_get_scale(const matrix33 &a, const vector3 scale);
inline void                       matrix33_to_array(const matrix33 &m, float *array);
inline std::array<float, 9>       matrix33_to_array(const matrix33 &m);

// Get/Set information
inline float                      matrix33_get(const matrix33 &mat, const uint32_t row, const uint32_t col);
inline float                      matrix33_get(const matrix33 &mat, const uint32_t i);
inline void                       matrix33_set(matrix33 &mat, const uint32_t row, const uint32_t col, const float set);
inline vector3                    matrix33_get_scale(const matrix33 &a);


// Impl

namespace detail
{
  struct internal_mat3
  {
    float data[9];
  };

}


matrix33
matrix33_id()
{
  std::array<float, 9> id_array = {
    1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 1.f,
  };

  return matrix33_init_with_array(&id_array[0]);
}


matrix33
matrix33_zero()
{
  std::array<float, 9> zero_array = {
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
  };

  return matrix33_init_with_array(&zero_array[0]);  
}


matrix33
matrix33_init()
{
  return matrix33_id();
}


matrix33
matrix33_init(const float x)
{
  std::array<float, 9> x_array = {
    x,x,x,
    x,x,x,
    x,x,x,
  };

  return matrix33_init_with_array(x_array);
}


matrix33
matrix33_init_with_array(const float array[])
{
  matrix33 return_mat;
  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&return_mat);

  memcpy(internal_mat->data, array, sizeof(internal_mat->data));
  
  return return_mat;
}


matrix33
matrix33_init_with_array(const std::array<float, 9> &array)
{
  return matrix33_init_with_array(&array[0]);
}


// Operations
matrix33
matrix33_add(const matrix33 &lhs, const matrix33 &rhs)
{
  const detail::internal_mat3 *left = reinterpret_cast<const detail::internal_mat3*>(&lhs);
  const detail::internal_mat3 *right = reinterpret_cast<const detail::internal_mat3*>(&rhs);

  matrix33 return_mat; 
  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&return_mat);

  for(int i = 0; i < 9; ++i)
  {
    internal_mat->data[i] = left->data[0] + right->data[0];
  }

  return return_mat;
}


matrix33
matrix33_subtract(const matrix33 &lhs, const matrix33 &rhs)
{
  const detail::internal_mat3 *left = reinterpret_cast<const detail::internal_mat3*>(&lhs);
  const detail::internal_mat3 *right = reinterpret_cast<const detail::internal_mat3*>(&rhs);

  matrix33 return_mat; 
  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&return_mat);

  for(int i = 0; i < 9; ++i)
  {
    internal_mat->data[i] = left->data[0] - right->data[0];
  }

  return return_mat;
}


matrix33
matrix33_scale(const float lhs, const matrix33 &rhs)
{
  const detail::internal_mat3 *right = reinterpret_cast<const detail::internal_mat3*>(&rhs);

  matrix33 return_mat; 
  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&return_mat);

  for(int i = 0; i < 9; ++i)
  {
    internal_mat->data[i] = lhs * right->data[0];
  }

  return return_mat;
}


vector3
matrix33_multiply(const vector3 lhs, const matrix33 &rhs)
{
  const detail::internal_mat3 *right = reinterpret_cast<const detail::internal_mat3*>(&rhs);
  std::array<float, 3> vec_data;

  for(int i = 0; i < 9; i += 4)
  {
    const vector3 dot_vector = vector3_init(right->data[i + 0], right->data[i + 3], right->data[i + 6]);

    vec_data.at(i / 3) = vector3_dot(lhs, dot_vector);
  }

  return vector3_init_with_array(vec_data);
}


matrix33
matrix33_multiply(const matrix33 &lhs, const matrix33 &rhs)
{
  const detail::internal_mat3 *left = reinterpret_cast<const detail::internal_mat3*>(&lhs);
  const detail::internal_mat3 *right = reinterpret_cast<const detail::internal_mat3*>(&rhs);

  matrix33 return_mat; 
  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&return_mat);

  for(uint32_t i = 0; i < 9; ++i)
  {
    //[0,1,2,3] x [0,4,8,12]
    const uint32_t row = i / 3;
    const uint32_t col = i % 3;

    const vector3 left_vec = vector3_init(left->data[row + 0], left->data[row + 1], left->data[row + 2]);
    const vector3 right_vec = vector3_init(right->data[col + 0], right->data[col + 3], right->data[col + 6]);

    internal_mat->data[i] = vector3_dot(left_vec, right_vec);
  }

  return return_mat;
}


matrix33
matrix33_rotate(const matrix33 &a, const vector3 euler)
{
  return matrix33();
}


float
matrix33_get(const matrix33 &mat, const uint32_t row, const uint32_t col)
{
  assert(row < 4 && col < 4);

  const uint32_t i = (row * 4) + col;
  return matrix33_get(mat, i);
}


float
matrix33_get(const matrix33 &mat, const uint32_t index)
{
  assert(index < 9);
  
  const detail::internal_mat3 *internal_mat = reinterpret_cast<const detail::internal_mat3*>(&mat);
  return internal_mat->data[index];
}


void
matrix33_set(matrix33 &mat, const uint32_t row, const uint32_t col, const float set)
{
  assert(row < 4 && col < 4);

  detail::internal_mat3 *internal_mat = reinterpret_cast<detail::internal_mat3*>(&mat);
  internal_mat->data[(row * 4) + col] = set;
}




} // namespace
} // namespace


#endif // include guard