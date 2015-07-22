#ifndef VERTEX_FORMAT_INCLUDED_AFB37FCF_7EB8_480E_AF41_496E7BB2230F
#define VERTEX_FORMAT_INCLUDED_AFB37FCF_7EB8_480E_AF41_496E7BB2230F


#include "common.hpp"
#include <string>
#include <vector>


namespace renderer {


enum class attr_type { FLOAT, FLOAT2, FLOAT3, FLOAT4, INT, UINT, };


struct attr_format_desc
{
  std::string   name;
  attr_type     type;
};


struct attribute
{
  std::string   name;
  uint32_t      size;
  std::size_t   pointer;
  GLenum        type;
};



class vertex_format
{
public:

  explicit            vertex_format() {}
  explicit            vertex_format(const std::vector<attr_format_desc> &desc);

  bool                load_format(const std::vector<attr_format_desc> &desc);

  attribute           get_attribute(const uint32_t i) const { return m_format.at(i); }
  inline bool         is_valid() const { return m_stride > 0; }
  inline uint32_t     get_stride() const { return m_stride; }
  inline uint32_t     get_number_of_elements() const { return static_cast<uint32_t>(m_format.size()); }
  inline uint32_t     get_number_of_entires() const { return m_number_of_entires; }

private:

  std::vector<attribute>  m_format;
  uint32_t                m_stride = 0;
  uint32_t                m_number_of_entires = 0;

}; // class


} // namespace


#endif // include guard