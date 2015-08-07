#include <sdl_wrapper/sdl_lazy_include.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <math/math.hpp>
#include <utils/directory.hpp>
#include <utils/obj_model_loader.hpp>
#include <SOIL/SOIL.h>
#include <assert.h>
#include <iostream>


// In an effort to keep this demo consince I've just dumped the varables here.
// Needless to say you don't want todo that for bigger projects.
namespace
{
  // Some constants
  const uint32_t screen_width           = 1024;
  const uint32_t screen_height          = 600;
  const uint32_t number_of_mips         = 6;
  const float camera_move_speed         = 3.3f;
  const float mouse_move_speed          = 0.002f;
  
  // World axis.
  const caff_math::vector3 x_axis = caff_math::vector3_init(1, 0, 0);
  const caff_math::vector3 y_axis = caff_math::vector3_init(0, 1, 0);
  const caff_math::vector3 z_axis = caff_math::vector3_init(0, 0, 1);
  
  // Window/Input etc.
  sdl::window             window(screen_width, screen_height, false, "Mega Texture");
  sdl::input              input;
  const sdl::ogl_context  ogl_context(window);
  
  // Graphics things.
  renderer::shader                              simple_shader;
  renderer::vertex_format                       vert_fmt;
  std::array<renderer::texture, number_of_mips> dynamic_mips;
  renderer::vertex_buffer                       obj_plane;
  
  // Math things
  caff_math::transform      camera_transform; // position of the camera.
  
  const caff_math::matrix44 proj  = caff_math::matrix44_projection(screen_width, screen_height, 0.1f, 200000.f, caff_math::pi() * 0.25f);
  caff_math::matrix44       world = caff_math::matrix44_id(); // kill this when transform working better.
  caff_math::matrix44       view  = caff_math::matrix44_lookat(camera_transform.position, z_axis, y_axis);
}


void camera_control();

#include <fstream>
char* get_data(const uint32_t mip_level, const float center_x, const float center_y)
{
  const uint32_t bmp_bytes_header_offset  = 54;    // I don't think the data needs to be immediatly under the header.
  const uint32_t number_of_components     = 4;     // This could be 3 to save some space.
  const uint32_t width_of_mega_texture    = 16384;
  
  static char data[512 * 512 * 4];
 
  auto what_file = [](const uint32_t mip)
  {
    switch(mip)
    {
      case(5):
        return util::get_resource_path() + "assets/textures/mega_texture_5.bmp";
      case(4):
        return util::get_resource_path() + "assets/textures/mega_texture_4.bmp";
      case(3):
        return util::get_resource_path() + "assets/textures/mega_texture_3.bmp";
      case(2):
        return util::get_resource_path() + "assets/textures/mega_texture_2.bmp";
      case(1):
        return util::get_resource_path() + "assets/textures/mega_texture_1.bmp";
      case(0):
        return util::get_resource_path() + "assets/textures/mega_texture_0.bmp";
      default:
        assert(false);
    }
  };
  
  const std::string get_file = what_file(mip_level);
  
  std::ifstream fin(get_file, std::ios::binary | std::ios::in);
  
  uint32_t size_of_texture = width_of_mega_texture >> (mip_level);
  
  const uint32_t start_x = (size_of_texture / 2) - (512 / 2);
  const uint32_t start_y = (size_of_texture / 2) - (512 / 2);
  
  if(fin.good())
  {
    uint32_t d = 0;
    for(int i = 0; i < (512*512); ++i)
    {
      const uint32_t col = (i % 512) + start_x;
      const uint32_t row = ((i / 512) + start_y) * size_of_texture;
    
      const uint32_t index = col + row;
    
      fin.seekg((index * number_of_components) + bmp_bytes_header_offset);
      fin.read(&data[d], (number_of_components));
      d += number_of_components;
    }
  }
  
  return &data[0];
}


// Game loop renders, and calls update etc.
void game_loop()
{
  camera_control();
  
  renderer::reset();
  renderer::clear();
  
  simple_shader.set_raw_data("worldMat", caff_math::matrix44_get_data(world), sizeof(caff_math::matrix44));
  simple_shader.set_raw_data("viewMat",  caff_math::matrix44_get_data(view),  sizeof(caff_math::matrix44));
  simple_shader.set_raw_data("projMat",  caff_math::matrix44_get_data(proj),  sizeof(caff_math::matrix44));

  simple_shader.set_texture("mip0", dynamic_mips.at(0));
  simple_shader.set_texture("mip1", dynamic_mips.at(1));
  simple_shader.set_texture("mip2", dynamic_mips.at(2));
  simple_shader.set_texture("mip3", dynamic_mips.at(3));
  simple_shader.set_texture("mip4", dynamic_mips.at(4));
  simple_shader.set_texture("mip5", dynamic_mips.at(5));
  
  renderer::draw(simple_shader, vert_fmt, obj_plane);
  
  // SDL Things
  window.flip_buffer();
  
  window.think();
  input.think();
  sdl::message_pump();
}

// Main pretty much just initializes the data and starts the game loop.
int main()
{
  // Tap into error messages (we won't get startup errors because sdl_wrappers are in anon namespace.
  {
    auto sdl_error_callback = [](const std::string &error_msg)
    {
      std::cout << error_msg << std::endl;
    };
    
    auto renderer_error_callback = [](const uint32_t id, const std::string &msg)
    {
      std::cout << id << " : " << msg << std::endl;
    };
    
    sdl::set_error_callback(sdl_error_callback);
    renderer::set_log_callback(renderer_error_callback);
  }
  
  // SDL load ok?
  {
    assert(window.has_valid_context());
    assert(ogl_context.has_valid_context());
  }
  
  // Init Render things.
  {
    renderer::initialize();
    
    const auto resource_path = util::get_resource_path();
    
    renderer::shader_code shd_code = renderer::shader_utils::get_shader_code_from_tagged_file(resource_path + "assets/shaders/basic_fullbright.ogl");
    simple_shader.load_shader(shd_code);
    assert(simple_shader.is_valid());
    
    std::vector<renderer::attr_format_desc> vert_desc = {
      {"inPosition",  renderer::attr_type::FLOAT3},
      {"inTexC",      renderer::attr_type::FLOAT2},
      {"inNormal",    renderer::attr_type::FLOAT3},
    };
    
    vert_fmt.load_format(vert_desc);
    assert(vert_fmt.is_valid());
    
    const std::string texture_filepath = util::get_resource_path() + "assets/textures/";

    // Load up mips, with no data.
    for(uint32_t m = 0; m < dynamic_mips.size(); ++m)
    {
      auto &mip = dynamic_mips.at(m);
      
      mip.load_data(get_data(m, 0.5f, 0.5f), 512, 512);
      assert(mip.is_valid());
    }
  }
  
  // Init math things
  {
    camera_transform.position = caff_math::vector3_init(1.f, 600.f, -800.f);
    camera_transform.rotation = caff_math::quaternion_init_with_axis_angle(0, 1, 0, caff_math::quart_tau());
    
    // Mats
    const float scale = 10000.f;
    world = caff_math::matrix44_scale(scale, 1, scale);
  }
  
  // Load a model
  {
    util::obj_model plane = util::load_obj(util::get_resource_path() + "assets/models/unit_plane.obj");
    
    util::gl_mesh gl_plane = util::convert_to_open_gl_mesh(plane.meshes.front());
    obj_plane.load_vertex_buffer(gl_plane.mesh_data);
  }
  
  // Game Loop
  {
    input.set_mouse_hold(true);
    
    while(!window.wants_to_quit())
    {
      game_loop();
    }
  }

}


void
camera_control()
{
  // Head Angle
  {
    static float yaw   = 0.f;
    static float pitch = 0.f;
    
    yaw   += -input.get_mouse_delta_x() * mouse_move_speed;
    pitch += +input.get_mouse_delta_y() * mouse_move_speed;
    
    const caff_math::quaternion rot_yaw   = caff_math::quaternion_init_with_axis_angle(0, 1, 0, yaw);
    const caff_math::quaternion rot_pitch = caff_math::quaternion_init_with_axis_angle(1, 0, 0, pitch);
    const caff_math::quaternion new_rot   = caff_math::quaternion_multiply(rot_yaw, rot_pitch);
    
    // Update rot.
    camera_transform.rotation = new_rot;
  }
  
  // delta movement.
  float fwd_distance = 0;
  float right_distance = 0;
  
  // WASD controls.
  {
    if(input.is_key_down(SDLK_w)) { fwd_distance += camera_move_speed; }
    if(input.is_key_down(SDLK_s)) { fwd_distance -= camera_move_speed; }
    if(input.is_key_down(SDLK_a)) { right_distance += camera_move_speed; }
    if(input.is_key_down(SDLK_d)) { right_distance -= camera_move_speed; }
  }
  
  // Calc new position
  {
    const caff_math::vector3 cam_fwd        = caff_math::quaternion_rotate_point(camera_transform.rotation, z_axis);
    const caff_math::vector3 cam_right      = caff_math::quaternion_rotate_point(camera_transform.rotation, x_axis);
    const caff_math::vector3 cam_move_fwd   = caff_math::vector3_scale(cam_fwd, fwd_distance);
    const caff_math::vector3 cam_move_right = caff_math::vector3_scale(cam_right, right_distance);
    
    // Update position
    camera_transform.position = caff_math::vector3_add(camera_transform.position, cam_move_fwd);
    camera_transform.position = caff_math::vector3_add(camera_transform.position, cam_move_right);
  }
  
  // Update view mat
  {
    const caff_math::vector3 cam_look_fwd  = caff_math::quaternion_rotate_point(camera_transform.rotation, z_axis);
    const caff_math::vector3 cam_look_up   = caff_math::quaternion_rotate_point(camera_transform.rotation, y_axis);
    const caff_math::vector3 cam_lookat    = caff_math::vector3_add(camera_transform.position, cam_look_fwd);
    
    view = caff_math::matrix44_lookat(camera_transform.position, cam_lookat, cam_look_up);
  }
}
