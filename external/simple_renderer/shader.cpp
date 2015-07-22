
#include "shader.hpp"
#include "texture.hpp"
#include "frame_buffer.hpp"
#include <fstream>
#include <streambuf>
#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>


namespace
{
  std::string prase_shader(const std::string &combined_shader, const std::string &tag_name)
  {
    const std::string open_tag  = "[" + tag_name + "]";
    const std::string close_tag = "[/" + tag_name + "]";

    const std::size_t shader_start = combined_shader.find(open_tag);
    const std::size_t shader_end   = combined_shader.find(close_tag);

    if((shader_start != std::string::npos) && (shader_end != std::string::npos))
    {
      const std::size_t start = shader_start + open_tag.size();
      const std::size_t size = shader_end - start;

      const std::string shader = combined_shader.substr(start, size);

      return shader;
    }

    return "";
  }

  enum class constant_type
  {
	  FLOAT,
	  VEC2,
	  VEC3,
	  VEC4,
	  INT,
	  IVEC2,
	  IVEC3,
	  IVEC4,
	  BOOL,
	  BVEC2,
	  BVEC3,
	  BVEC4,
	  MAT2,
	  MAT3,
	  MAT4,
	
	  size,
  };

	constant_type get_gl_constant_type(GLenum type)
	{
			switch (type){
				case GL_FLOAT:      return constant_type::FLOAT;
				case GL_FLOAT_VEC2: return constant_type::VEC2;
				case GL_FLOAT_VEC3: return constant_type::VEC3;
				case GL_FLOAT_VEC4: return constant_type::VEC4;
				case GL_INT:        return constant_type::INT;
				case GL_INT_VEC2:   return constant_type::IVEC2;
				case GL_INT_VEC3:   return constant_type::IVEC3;
				case GL_INT_VEC4:   return constant_type::IVEC4;
				case GL_BOOL:       return constant_type::BOOL;
				case GL_BOOL_VEC2:  return constant_type::BVEC2;
				case GL_BOOL_VEC3:  return constant_type::BVEC3;
				case GL_BOOL_VEC4:  return constant_type::BVEC4;
				case GL_FLOAT_MAT2: return constant_type::MAT2;
				case GL_FLOAT_MAT3: return constant_type::MAT3;
				case GL_FLOAT_MAT4: return constant_type::MAT4;
			}

		return (constant_type) -1;
	}


} // anon namespace


namespace renderer {
namespace shader_utils {


shader_code
get_shader_code_from_tagged_file(const std::string &filename)
{
  const std::string code(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());
  return get_shader_code_from_tagged_string(code);
}


shader_code
get_shader_code_from_tagged_string(const std::string &code)
{
  auto parse_file = [](const std::string &combined_shader, const std::string &tag_name) -> std::string
  {
    const std::string open_tag  = "[" + tag_name + "]";
    const std::string close_tag = "[/" + tag_name + "]";

    const std::size_t shader_start = combined_shader.find(open_tag);
    const std::size_t shader_end = combined_shader.find(close_tag);

    if((shader_start != std::string::npos) && (shader_end != std::string::npos))
    {
      const std::size_t start = shader_start + open_tag.size();
      const std::size_t size = shader_end - start;

      const std::string shader = combined_shader.substr(start, size);

      return shader;
    }

    return "";
  };

  const auto vs = parse_file(code, "VERTEX");
  const auto gs = parse_file(code, "GEOMETRY");
  const auto ps = parse_file(code, "PIXEL");
  const auto cs = parse_file(code, "COMPUTE");

  return cs.empty() ? build_shader_code_vs_gs_ps(vs, gs, ps) : build_shader_code_cs(cs);
}


shader_code
build_shader_code_vs_gs_ps(const std::string &vs, const std::string &gs, const std::string &ps)
{
  return shader_code{vs, gs, ps, ""};
}


shader_code
build_shader_code_cs(const std::string &cs)
{
  return shader_code{"", "", "", cs};
}


} // namespace


shader::shader(const shader_code &code)
{
  load_shader(code);
}

  
shader::shader(const std::string &vs_code, const std::string &gs_code, const std::string &ps_code, const std::string &cs_code)
{
  
  load_shader(shader_code{vs_code, gs_code, ps_code, cs_code});
}


void
shader::_destroy()
{
  if(m_program_id)
  {
  	glDeleteProgram(m_program_id);
    m_program_id = 0;
    check_and_log_error("shader::_destroy - destroying program.");
  }

  if(m_vertex_id)
  {
    glDeleteShader(m_vertex_id);
    m_vertex_id = 0;
    check_and_log_error("shader::_destroy - destroying vertex shader.");
  }

  if(m_geometry_id)
  {
    glDeleteShader(m_geometry_id);
    m_geometry_id = 0;
    check_and_log_error("shader::_destroy - destroying geometry shader.");
  }

  if(m_fragment_id)
  {
    glDeleteShader(m_fragment_id);
    m_fragment_id = 0;
    check_and_log_error("shader::_destroy - destroying fragment shader.");
  }
}
  

shader::~shader()
{
  _destroy();
}


void
shader::_move(shader &this_one, shader &other_one)
{
  _destroy();

  this_one.m_constants            = std::move(other_one.m_constants);
  this_one.m_samplers             = std::move(other_one.m_samplers);
  this_one.m_pending_upload       = std::move(other_one.m_pending_upload);
  this_one.m_pending_upload_data  = std::move(other_one.m_pending_upload_data);
  this_one.m_pending_texture_data = std::move(other_one.m_pending_texture_data);

  this_one.m_program_id   = other_one.m_program_id;
  this_one.m_vertex_id    = other_one.m_vertex_id;
  this_one.m_geometry_id  = other_one.m_geometry_id;
  this_one.m_fragment_id  = other_one.m_fragment_id;

  other_one.m_program_id  = 0;
  other_one.m_vertex_id   = 0;
  other_one.m_geometry_id = 0;
  other_one.m_fragment_id = 0;
}


shader::shader(shader&& other)
{
  _move(*this, other);
}


shader&
shader::operator=(shader&& other)
{
  _move(*this, other);

  return *this;
}


bool
shader::load_shader(const shader_code &code)
{
  if(is_valid())
  {
    log_error(GL_NO_ERROR, "shader::load_shader - a shader program already exists, destroying old one.");
    _destroy();
  }

 auto compile_shader = [](const std::string &shader_code, const GLenum shader_type, const std::string &shader_name) -> GLuint
  {
    if(shader_code.empty())
    {
      return 0;
    }

    GLuint shader_id = glCreateShader(shader_type);

		const char * vert_source = shader_code.c_str();
		glShaderSource(shader_id, 1, &vert_source, NULL);
		glCompileShader(shader_id);
		
		// Log
		GLint log_length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
		
		std::vector<GLchar> log;
		log.resize(log_length);
		glGetShaderInfoLog(shader_id, log_length, 0, &log[0]);

		if(log_length > 1)
		{
      std::string log_str;
      log_str.reserve(log.size());
      log_str.append(&log[0]);

		  // Did it compile
		  GLint is_compiled;
		  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);

      auto find_line = [](const std::string &log, const std::string &shader, const std::string &log_tag) -> std::string
      {
        // This needs to work much better.
        return "";
      };

      if(is_compiled == GL_FALSE)
      {
        log_error(GL_NO_ERROR, "shader::load_shader - errors compiling shader.");
        log_error(GL_NO_ERROR, log_str);
        return 0;
      }
      else
      {
        log_error(GL_NO_ERROR, "shader::load_shader - warnings compiling shader.");
        log_error(GL_NO_ERROR, log_str);
      }
		}

    return shader_id;
  };

  m_vertex_id   = compile_shader(code.vs_code, GL_VERTEX_SHADER, "vertex");
  m_geometry_id = compile_shader(code.gs_code, GL_GEOMETRY_SHADER, "geometry");
  m_fragment_id = compile_shader(code.ps_code, GL_FRAGMENT_SHADER, "fragment");

  // Link the shader
  if(m_vertex_id && m_fragment_id)
  {
    m_program_id = glCreateProgram();
    
    glAttachShader(m_program_id, m_vertex_id);
    if(m_geometry_id) { glAttachShader(m_program_id, m_geometry_id); }
    glAttachShader(m_program_id, m_fragment_id);
    glLinkProgram(m_program_id);

    // Check the log
    GLint log_length;
		glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &log_length);
		
		std::vector<GLchar> log;
		log.resize(log_length);
		glGetProgramInfoLog(m_program_id, log_length, 0, &log[0]);

    // Did it link
    GLint is_linked;
		glGetProgramiv(m_program_id, GL_LINK_STATUS, &is_linked);

		if(log_length > 1)
		{
      std::string log_str;
      log_str.reserve(log.size());
      log_str.append(&log[0]);

      if(is_linked == GL_FALSE)
      {
        log_error(GL_NO_ERROR, "shader::load_shader - errors linking program.");
        log_error(GL_NO_ERROR, log_str);
      }
      else
      {
        log_error(GL_NO_ERROR, "shader::load_shader - warnings linking program.");
        log_error(GL_NO_ERROR, log_str);
      }
		}
    
    // Give up there
    if(is_linked == GL_FALSE)
    {
      m_program_id = 0;
      return false;
    }
  } // If linked
  else
  {
    //return false;
  }

  // Get uniforms
  {
    glUseProgram(m_program_id);
    check_and_log_error("shader::load_shader - failed to use program.");

    // Get uniforms.
    GLint uniform_count, uniform_length;
    glGetProgramiv(m_program_id, GL_ACTIVE_UNIFORMS, &uniform_count);
    glGetProgramiv(m_program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_length);

    m_samplers.reserve(uniform_count);
    m_constants.reserve(uniform_count);

    for(GLint i = 0; i < uniform_count; ++i)
    {
      GLenum gl_type = 0;
      GLint length  = 0;
      GLint size    = 0;

      std::vector<GLchar> uni_name;
      uni_name.reserve(uniform_length);

      glGetActiveUniform(m_program_id, i, uniform_length, &length, &size, &gl_type, uni_name.data());
      const std::string uniform_name(uni_name.data());

      // Is sampler?
      if((gl_type >= GL_SAMPLER_1D) && (gl_type <= GL_SAMPLER_2D_RECT_SHADOW))
      {
        const GLint location = glGetUniformLocation(m_program_id, uniform_name.c_str());
        check_and_log_error("shader::load_shader - getting uniform location.");

        glUniform1i(location, m_samplers.size());
        
        m_samplers.emplace_back(sampler{uniform_name, static_cast<uint32_t>(m_samplers.size())});
      }
      // Then uniform
      else
      {
        const std::string prefix = "gl_";

        if(uniform_name.compare(0, prefix.length(), prefix) != 0)
        {
          const GLint index = glGetUniformLocation(m_program_id, uniform_name.c_str());
          const auto type = get_gl_constant_type(gl_type);

            m_constants.emplace_back(constant{uniform_name, static_cast<uint32_t>(index), static_cast<int32_t>(type), static_cast<int32_t>(size)});
        }
      }
    }

    m_samplers.shrink_to_fit();
    m_constants.shrink_to_fit();

    m_pending_upload_data.reserve(m_constants.size() * (4 * sizeof(float))); // Huristic to resever space for data. constants * vec4
    m_pending_texture_data.reserve(m_constants.size());
    m_pending_upload.reserve(m_constants.size());

  } // getting uniforms and samplers

  glUseProgram(0);

  return true;
}


void
shader::bind() const
{
  if(!is_valid())
  {
    log_error(GL_NO_ERROR, "vertex_buffer::bind - shader program is not valid.");
  }

  glUseProgram(m_program_id);
  check_and_log_error("shader::bind - failed to use program.");

  // Constants 
  {
    for(const auto &upload : m_pending_upload)
    {
      const constant_type type               = (constant_type)std::get<UPLOAD_TYPE>(upload);
      const uint32_t      index              = std::get<UPLOAD_INDEX>(upload);
      const uint32_t      number_of_elements = std::get<UPLOAD_NUM_ELEMENTS>(upload);
      const void          *data              = &(m_pending_upload_data.at(std::get<UPLOAD_START_INDEX>(upload)));

      switch(type)
      {
        case(constant_type::FLOAT):  glUniform1fv(index, number_of_elements, (GLfloat*)data);  break;
        case(constant_type::VEC2):   glUniform2fv(index, number_of_elements, (GLfloat*)data);  break;
        case(constant_type::VEC3):   glUniform3fv(index, number_of_elements, (GLfloat*)data);  break;
        case(constant_type::VEC4):   glUniform4fv(index, number_of_elements, (GLfloat*)data);  break;
        case(constant_type::INT):    glUniform1iv(index, number_of_elements, (GLint*)data);    break;
        case(constant_type::IVEC2):  glUniform2iv(index, number_of_elements, (GLint*)data);    break;
        case(constant_type::IVEC3):  glUniform3iv(index, number_of_elements, (GLint*)data);    break;
        case(constant_type::IVEC4):  glUniform4iv(index, number_of_elements, (GLint*)data);    break;
        case(constant_type::BOOL):   glUniform1iv(index, number_of_elements, (GLint*)data);    break;
        case(constant_type::BVEC2):  glUniform2iv(index, number_of_elements, (GLint*)data);    break;
        case(constant_type::BVEC3):  glUniform3iv(index, number_of_elements, (GLint*)data);    break;
        case(constant_type::BVEC4):  glUniform4iv(index, number_of_elements, (GLint*)data);    break;
        case(constant_type::MAT2):   glUniformMatrix2fv(index, number_of_elements, GL_FALSE, (GLfloat*)data); break;
        case(constant_type::MAT3):   glUniformMatrix3fv(index, number_of_elements, GL_FALSE, (GLfloat*)data); break;
        case(constant_type::MAT4):   glUniformMatrix4fv(index, number_of_elements, GL_FALSE, (GLfloat*)data); break;
          
        default:
          assert(false); // Why did you get here?
      };
    }
    
    m_pending_upload.clear();
    m_pending_upload_data.clear();
  } // Constants

  // Textures
  {
    const std::size_t number_of_textures = std::min<std::size_t>(m_pending_texture_data.size(), static_cast<std::size_t>(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS));

    for(std::size_t i = 0; i < number_of_textures; ++i)
    {
      const auto &tex = m_pending_texture_data.at(i);

      glActiveTexture(GL_TEXTURE0 + tex.index);
      check_and_log_error("shader::bind - applying active texture.");

      glBindTexture(tex.target, tex.texture_id);
      check_and_log_error("shader::bind - binding texture.");

  	  glTexParameteri(tex.target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		  glTexParameteri(tex.target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		  glTexParameteri(tex.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		  glTexParameteri(tex.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      check_and_log_error("shader::bind - setting texParaeteri.");  
    }

    m_pending_texture_data.clear();
  }
}


void
shader::set_raw_data(const std::string &name, const void* data, const std::size_t bytes_of_data)
{
  const auto it = std::find_if(std::begin(m_constants), std::end(m_constants), [name](const constant &con){ return con.name == name; });

  if(it != m_constants.end())
  {    
    // Check to see if data is already pending.    
    const uint32_t index = it->index;
    const auto data_it = std::find_if(std::begin(m_pending_upload), std::end(m_pending_upload), [index](const pending_upload &upload){ return std::get<UPLOAD_INDEX>(upload) == index; });
      
    // If already have data then overwrite the data.
    if(data_it != m_pending_upload.end())
    {
        memcpy(&m_pending_upload_data.at(std::get<UPLOAD_START_INDEX>(*data_it)), data, bytes_of_data);
    }
    // Else push back into pending data.
    else
    {
      m_pending_upload.emplace_back(pending_upload{it->index, m_pending_upload_data.size(), it->number_of_elements, it->type, bytes_of_data});

      // Check resize container.
      const std::size_t size_needed  = m_pending_upload_data.size() + bytes_of_data;
      const std::size_t insert_point = m_pending_upload_data.size();

      m_pending_upload_data.resize(size_needed);
      memcpy(&m_pending_upload_data.at(insert_point), data, bytes_of_data);
    }
  }
}


void
shader::set_texture(const std::string &name, const texture &set_texture)
{
  const auto it = std::find_if(std::begin(m_samplers), std::end(m_samplers), [name](const sampler &samp){ return samp.name == name; });

  if(it != m_samplers.end())
  {
    const GLenum target = set_texture.get_gl_dimention();
    m_pending_texture_data.emplace_back(pending_texture{set_texture.get_gl_id(), target, it->index});
  }
  else
  {
    log_error(GL_NO_ERROR, "shader::set_texture - couldn't find sampler '" + name + "' for texture.");
  }
}


void
shader::set_texture(const std::string &name, const frame_buffer &set_frame_buffer)
{
  const auto it = std::find_if(std::begin(m_samplers), std::end(m_samplers), [name](const sampler &samp){ return samp.name == name; });

  if(it != m_samplers.end())
  {
    const GLenum target = GL_TEXTURE_2D;
    m_pending_texture_data.emplace_back(pending_texture{set_frame_buffer.get_gl_id(), target, it->index});
  }
  else
  {
    log_error(GL_NO_ERROR, "shader::set_texture - couldn't find sampler '" + name + "' for framebuffer.");
  }
}


} // namespace