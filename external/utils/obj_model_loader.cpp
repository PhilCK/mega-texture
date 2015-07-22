#include "obj_model_loader.hpp"
#include <fstream>
#include <sstream>


namespace util {

  
obj_model
load_obj(const std::string &filename)
{
  obj_model return_model;
  
  std::ifstream file(filename);

  const bool is_gl = true; // Not sure what todo with this. Is for UV placement.
  
  if(file.is_open())
  {
    // Mesh Details
    std::string name = "";
    std::vector<float>      verts;
    std::vector<float>      normals;
    std::vector<float>      uvs;
    std::vector<uint32_t>   index;
    
    std::string line;
    
    // Loop through contents of the file.
    while(getline(file, line))
    {
      // Split line into an array of strings
      std::istringstream ss(line);
      std::istream_iterator<std::string> begin(ss), end;
      std::vector<std::string>objTokens(begin, end);
      
      std::string type = objTokens[0];
      
      // In Reality the v, vn, vt, and f would happen in order so checking each line for all three
      // is uneccassary, however it makes it a little eaiser to look at, and gives some resiliants
      // to mal-formed .obj files.
      
      // New Object
      if(type == "o")
      {
        //If an additional mesh store previous one.
        if(!index.empty())
        {
          obj_mesh this_mesh;
          this_mesh.name = name;
          this_mesh.positions = verts;
          this_mesh.normals = normals;
          this_mesh.uvs = uvs;
          this_mesh.index = index;
          
          // Insert into models meshes.
          //
          return_model.meshes.push_back(this_mesh);
        }
        
        name = "";
        
        for(auto it = objTokens.begin() + 1; it != objTokens.end(); ++it)
        {
          name += (*it) + " ";
        }
        
        // Trim last space off.
        name = name.substr(0, name.size() - 1);
      }
      
      // Vertices
      else if(type == "v")
      {
        for(std::size_t i = 1; i < objTokens.size(); ++i)
        {
          const float value = std::stof((objTokens.at(i)));
          
          verts.push_back(value);
          
          //const uint32_t index = static_cast<uint32_t>(i - 1);
        }
        
        //for(auto it = objTokens.begin() + 1; it != objTokens.end(); ++it)
        //{
        //  verts.push_back(::atof((*it).c_str()));
        //}
      }
      
      // Normals
      else if(type == "vn")
      {
        for(auto it = objTokens.begin() + 1; it != objTokens.end(); ++it)
        {
          normals.push_back(std::stof((*it)));
        }
      }
      
      // Texture
      else if(type == "vt")
      {
        for(auto it = objTokens.begin() + 1; it != objTokens.end(); ++it)
        {
          uvs.push_back(std::stof((*it)));
          
          // Adjust the V Coord on OpenGL
          if(is_gl && !(uvs.size() % 2))
          {
            uvs.back() = 1.0f - uvs.back();
          }
        }
      }
      
      // Faces / Index
      else if(type == "f")
      {
        // Check to see if there are any empty lists, and add a dummy element to them.
        if(verts.empty()) { verts.push_back(0.0f);   verts.push_back(0.0f);   verts.push_back(0.0f);   }
        if(normals.empty()) { normals.push_back(0.0f); normals.push_back(0.0f); normals.push_back(0.0f); }
        if(uvs.empty())   { uvs.push_back(0.0f);     uvs.push_back(0.0f);                }
        
        
        for(auto it = objTokens.begin() + 1; it != objTokens.end(); ++it)
        {
          const std::string indexStr = (*it);
          std::size_t startLoc = 0;
          
          while(true)
          {
            // Find the next '/' or end of file
            const std::size_t endLoc = indexStr.find("/", startLoc);
            const std::size_t len  = (std::min(endLoc, indexStr.length()) - startLoc);
            const std::string iStr   = indexStr.substr(startLoc, len);
            
            // Did we get a result? len = 0 will lead to missing info
            if(!iStr.empty())
            {
              const unsigned int i = ::atoi(iStr.c_str()) - 1; // .obj starts at 1, array starts at 0.
              
              index.push_back(i);
            }
            // Missing info
            else
            {
              // If it seems we are missing information.
              // Check to see if it was required.
              // If its not insert some blank data.
              index.push_back(0); // This is the dummy element inserted.
            }
            
            
            // End of file.
            if(endLoc == std::string::npos) {
              break;
            }
            
            startLoc = endLoc + 1;
          }
          
        } // While loop
      } // for index loop
    } // while file
    
    
    // Add mesh to the list of meshes.
    if(!index.empty())
    {
      obj_mesh this_mesh;
      this_mesh.name = name;
      this_mesh.positions = verts;
      this_mesh.normals = normals;
      this_mesh.uvs = uvs;
      this_mesh.index = index;
      
      // Insert into models meshes.
      return_model.meshes.push_back(this_mesh);
    }
  }
  
  // Couldn't open the file
  return return_model;
} // load_obj(...)

  
gl_mesh
convert_to_open_gl_mesh(obj_mesh mesh)
{
  gl_mesh return_mesh;
  
  for(std::size_t i = 0; i < mesh.index.size(); ++i)
  {
    enum data_t { VERTEX = 0, UV, NORMAL, DATA_SIZE, };
    
    const uint32_t data = i % DATA_SIZE;
    const uint32_t index = mesh.index.at(i);
    
    switch(data)
    {
      case(VERTEX):
      {
        const uint32_t vert_index = index * 3;
          
        return_mesh.mesh_data.push_back(mesh.positions.at(vert_index + 0));
        return_mesh.mesh_data.push_back(mesh.positions.at(vert_index + 1));
        return_mesh.mesh_data.push_back(mesh.positions.at(vert_index + 2));
        break;
      }
      case(UV):
      {
        const uint32_t uv_index = index * 2;
          
        return_mesh.mesh_data.push_back(mesh.uvs.at(uv_index + 0));
        return_mesh.mesh_data.push_back(mesh.uvs.at(uv_index + 1));
        break;
      }
      case(NORMAL):
      {
        const uint32_t normal_index = index * 3;
          
        return_mesh.mesh_data.push_back(mesh.normals.at(normal_index + 0));
        return_mesh.mesh_data.push_back(mesh.normals.at(normal_index + 1));
        return_mesh.mesh_data.push_back(mesh.normals.at(normal_index + 2));
        break;
      }
    };
  }
  
  return return_mesh;
}
  

} // namespace