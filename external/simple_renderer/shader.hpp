#ifndef SHADER_INCLUDED_D5C16627_9F15_4771_B757_E86136A1212D
#define SHADER_INCLUDED_D5C16627_9F15_4771_B757_E86136A1212D


#include "common.hpp"
#include "renderer_fwd.hpp"
#include <string>
#include <vector>


namespace renderer {


struct shader_code
{
  std::string vs_code;
  std::string gs_code;
  std::string ps_code;
  std::string cs_code;
};


/*
shader
--
Holds the program information for a given shader.
*/
class shader
{

                          shader(const shader &) = delete;
                          shader operator=(const shader &) = delete;

public:

  explicit                shader() {}
  explicit                shader(const shader_code &code);
  explicit                shader(const std::string &vs_code, const std::string &gs_code, const std::string &ps_code, const std::string &cs_code = "");
                          ~shader();

                          shader(shader&& other);
  shader&                 operator=(shader&& other);

  bool                    load_shader(const shader_code &code);
  void                    bind() const;

  inline GLuint           get_program_gl_id() const { return m_program_id; }
  inline bool             is_valid() const          { return m_program_id > 0; }

  void                    set_raw_data(const std::string &name, const void* data, const std::size_t size_of_data);
  void                    set_texture(const std::string &name, const texture &texture);
  void                    set_texture(const std::string &name, const frame_buffer &frame_buffer);

private:

  void                    _move(shader &this_one, shader &other_one);
  void                    _destroy();

private:
  
  struct constant
  {
  	std::string name;
    uint32_t    index;
    int32_t     type;
    int32_t     number_of_elements;
  };

  struct sampler
  {
    std::string name;
    uint32_t    index;
  };

  struct pending_texture
  {
    GLuint    texture_id;
    GLenum    target;
    uint32_t  index;
  };

  std::vector<constant> m_constants;
  std::vector<sampler>  m_samplers;

  enum { UPLOAD_INDEX, UPLOAD_START_INDEX, UPLOAD_NUM_ELEMENTS, UPLOAD_TYPE, UPLOAD_NUM_BYTES };
  using pending_upload = std::tuple<uint32_t, uint32_t, uint32_t, int32_t, uint32_t>;  

  mutable std::vector<pending_upload>  m_pending_upload;
  mutable std::vector<uint8_t>         m_pending_upload_data;
  mutable std::vector<pending_texture> m_pending_texture_data;	

  GLuint m_program_id  = 0;
  GLuint m_vertex_id   = 0;
  GLuint m_geometry_id = 0;
  GLuint m_fragment_id = 0;

}; // class


/*
Helper methods to build the shader_code type
*/
namespace shader_utils {

/*
Give it a file that contains the the shader code split by tags

[VERTEX]
// VS Code
[/VERTEX]
[GEOMETRY]
// GS Code
[/GEOMETRY]
[PIXEL]
// Pixel
[/PIXEL]

and it will parse it into the shader_code type.
*/
shader_code get_shader_code_from_tagged_file(const std::string &filename);
shader_code get_shader_code_from_tagged_string(const std::string &code);

/*
Helper method to build shader_code, pass empty string to gs if it doesn't exist.
*/
shader_code build_shader_code_vs_gs_ps(const std::string &vs, const std::string &gs, const std::string &ps);

/*
Helper to keep inline with _vs_gs_ps function.
*/
shader_code build_shader_code_cs(const std::string &cs);

}


} // namespace


#endif // include guard