#ifndef VERTEX_BUFFER_INCLUDED_9A4FC9A6_E5FD_4C2F_B761_0A3CF0BFA9E0
#define VERTEX_BUFFER_INCLUDED_9A4FC9A6_E5FD_4C2F_B761_0A3CF0BFA9E0


#include "common.hpp"
#include "renderer_fwd.hpp"
#include <vector>
#include <stdint.h>


namespace renderer {


class vertex_buffer
{

                      vertex_buffer(const vertex_buffer &) = delete;
                      vertex_buffer operator=(const vertex_buffer &) = delete;

public:

  explicit            vertex_buffer() {}
  explicit            vertex_buffer(const std::vector<float> &vertex_data, const bool is_dynamic = false);
                      ~vertex_buffer();

                      vertex_buffer(vertex_buffer&& other);
  vertex_buffer&      operator=(vertex_buffer&& other);

  bool                load_vertex_buffer(const std::vector<float> &vertex_data, const bool is_dynamic = false);
  void                bind(const vertex_format &set_vert_fmt, const shader &set_shader) const;

  inline bool         is_valid() const { return m_vertex_buffer != 0; }
  inline uint32_t     get_number_entries() const { return m_number_entries; }

private:

  void                _move(vertex_buffer &this_one, vertex_buffer &other_one);
  void                _destroy();

private:

  GLuint              m_vertex_buffer  = 0;
  GLuint              m_buffer_size    = 0;
  uint32_t            m_number_entries = 0;

}; // class


} // namespace


#endif // include guard