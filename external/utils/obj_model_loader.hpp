#ifndef OBJ_MODEL_LOADER_INCLUDE_71435C61_A8DE_4D1B_BF0C_F5DD45775AD8
#define OBJ_MODEL_LOADER_INCLUDE_71435C61_A8DE_4D1B_BF0C_F5DD45775AD8


#include <string>
#include <vector>
#include <stdint.h>


namespace util {


struct obj_mesh
{
  std::string             name;       //!< Name of the mesh if available.
  std::vector<float>      positions;  //!< Positions in x,y,z form.
  std::vector<float>      uvs;        //!< Texture coords in u,v form.
  std::vector<float>      normals;    //!< Normals in x,y,z form.
  std::vector<uint32_t>   index;      //!< Index into data. index is [position, uv, normal].
};

  
struct obj_model
{
  std::vector<obj_mesh>   meshes;     //!< A model can contain multiple meshes.
};


//! Loads Wavefront obj file into mesh.
obj_model load_obj(const std::string &filename);


struct gl_mesh
{
  std::vector<float> mesh_data;
  //std::vector<uint32_t> index;
};

//! Converts mesh into a combined, position,normal,uv format for gl.
gl_mesh convert_to_open_gl_mesh(obj_mesh);

  
struct bounds
{
  float width, height;
};


} // namespace


#endif // include guard