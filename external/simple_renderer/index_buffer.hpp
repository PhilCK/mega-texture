#ifndef INDEX_BUFFER_INCLUDED_1E4A158B_1FEC_4954_B768_3BC77C61DB07
#define INDEX_BUFFER_INCLUDED_1E4A158B_1FEC_4954_B768_3BC77C61DB07


#include "common.hpp"
#include <vector>
#include <stdint.h>


namespace renderer {


class index_buffer
{

                        index_buffer(const index_buffer &) = delete;
                        index_buffer operator=(const index_buffer &) = delete;

public:

  explicit              index_buffer();
  explicit              index_buffer(const std::vector<uint32_t> &index);
                        ~index_buffer();

                        index_buffer(index_buffer&& other);
  index_buffer&         operator=(index_buffer&& other);


  bool                  load_index_buffer(const std::vector<uint32_t> &index);
  void                  bind();

  inline bool           is_valid() const { return m_index_buffer != 0; }
  inline uint32_t       get_number_of_indices() const { return m_number_of_indices; }

private:

  void                  _move(index_buffer &other);
  void                  _destroy();

private:

  GLuint                m_index_buffer = 0;
  uint32_t              m_number_of_indices = 0;


}; // include guard


} // include guard


#endif // include guard