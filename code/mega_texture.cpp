#include <sdl_wrapper/sdl_lazy_include.hpp>
#include <simple_renderer/lazy_include.hpp>
#include <math/math.hpp>
#include <utils/directory.hpp>
#include <utils/obj_model_loader.hpp>

#include <SOIL/SOIL.h>

#include <assert.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// In an effort to keep this demo consince I've just dumped the varables here.
// Needless to say you don't want todo that for bigger projects.
namespace
{
  // Some constants
  const uint32_t screen_width           = 1024;
  const uint32_t screen_height          = 600;
  const uint32_t mega_texture_size      = 32768;
  const uint32_t mega_texture_mip_size  = 512;
  const uint32_t number_of_mips         = 5;
  const uint32_t camera_move_speed      = 10.f;
  
  // World axis.
  const caff_math::vector3 x_axis = caff_math::vector3_init(1, 0, 0);
  const caff_math::vector3 y_axis = caff_math::vector3_init(0, 1, 0);
  const caff_math::vector3 z_axis = caff_math::vector3_init(0, 0, -1);
  
  // Window/Input etc.
  sdl::window             window(screen_width, screen_height, false, "Mega Texture");
  sdl::input              input;
  const sdl::ogl_context  ogl_context(window);
  
  // Graphics things.
  renderer::shader                              simple_shader;
  renderer::vertex_format                       vert_fmt;
  std::array<renderer::texture, number_of_mips> dynamic_mips;
  renderer::texture                             green_grid_texture;
  renderer::texture                             orange_grid_texture;
  renderer::vertex_buffer                       obj_plane;
  
  // Math things
  const caff_math::matrix44 proj  = caff_math::matrix44_projection(screen_width, screen_height, 0.1f, 100.f, caff_math::pi() * 0.25f);
  caff_math::matrix44       world = caff_math::matrix44_id(); // kill this when transform working better.
  caff_math::matrix44       view  = caff_math::matrix44_id(); // kill this when transform working better.
  
  caff_math::transform      camera_transform; // position of the camera.
}


// We build the mips on the threads.
void thread_task()
{
  
}


// Generate the tasks for the mip generation.
void generate_mips()
{
  
}


// Game loop renders, and calls update etc.
void game_loop()
{
  // Check input
  {
    const float yaw = input.get_mouse_delta_x();
    const float pitch = input.get_mouse_delta_y();
    
    const caff_math::quaternion rot_yaw = caff_math::quaternion_init_with_axis_angle(0, 1, 0, yaw);
    const caff_math::quaternion rot_pitch = caff_math::quaternion_init_with_axis_angle(1, 0, 0, pitch);
    const caff_math::quaternion delta_rot = caff_math::quaternion_multiply(rot_yaw, rot_pitch);
    camera_transform.rotation = caff_math::quaternion_multiply(camera_transform.rotation, delta_rot);
    
    const caff_math::vector3 fwd        = caff_math::vector3_init(0, 0, 1);
    const caff_math::vector3 cam_fwd    = caff_math::quaternion_rotate_point(camera_transform.rotation, fwd);
    const caff_math::vector3 cam_lookat = caff_math::vector3_add(camera_transform.position, cam_fwd);
    
    view = caff_math::matrix44_lookat(camera_transform.position, cam_lookat, caff_math::vector3_init(0.f,1.f,0.f));
  }
  
  renderer::reset();
  renderer::clear();
  
  simple_shader.set_raw_data("worldMat", caff_math::matrix44_get_data(world), sizeof(caff_math::matrix44));
  simple_shader.set_raw_data("viewMat",  caff_math::matrix44_get_data(view),  sizeof(caff_math::matrix44));
  simple_shader.set_raw_data("projMat",  caff_math::matrix44_get_data(proj),  sizeof(caff_math::matrix44));
  simple_shader.set_texture("diffuseTex", green_grid_texture);
  
  renderer::draw(simple_shader, vert_fmt, obj_plane);
  
  // SDL Things
  window.flip_buffer();
  
  window.pump_messages();
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
    input.set_mouse_hold(true);
    renderer::initialize();
    
    const auto resource_path = util::get_resource_path();
    
    renderer::shader_code shd_code = renderer::shader_utils::get_shader_code_from_tagged_file(resource_path + "assets/shaders/basic_fullbright.ogl");
    //renderer::shader simple_shader(shd_code);
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
    
    int32_t tex_height = 0;
    int32_t tex_width = 0;
    
    const std::string orange_tex = texture_filepath + "dev_grid_orange_512.png";
    uint8_t *or_image = SOIL_load_image(orange_tex.c_str(), &tex_width, &tex_height, 0, SOIL_LOAD_RGBA);
    orange_grid_texture.load_data(or_image, tex_width, tex_height);
    assert(orange_grid_texture.is_valid());
    SOIL_free_image_data(or_image);
    
    const std::string green_tex = texture_filepath + "dev_grid_green_512.png";
    uint8_t *gre_image = SOIL_load_image(green_tex.c_str(), &tex_width, &tex_height, 0, SOIL_LOAD_RGBA);
    green_grid_texture.load_data(gre_image, tex_width, tex_height);
    assert(green_grid_texture.is_valid());
    SOIL_free_image_data(gre_image);
    
    // Load up mips, with no data.
    for(auto &mip : dynamic_mips)
    {
      mip.load_data(nullptr, 512, 512);
      assert(mip.is_valid());
    }
  }
  
  // Init math things
  {
    camera_transform.position = caff_math::vector3_init(-4.f, 1.f, 1.f);
    camera_transform.rotation = caff_math::quaternion_init_with_axis_angle(0, 1, 0, caff_math::quart_tau());
    
    // Mats

    world = caff_math::matrix44_scale(10, 10, 10);
    
    auto rot = caff_math::matrix44_rotate_around_axis(y_axis, caff_math::pi() / 3.f);
    
    world = caff_math::matrix44_multiply(world, rot);
    
  }
  
  // Load a model
  {
    util::obj_model plane = util::load_obj(util::get_resource_path() + "assets/models/unit_plane.obj");
    
    util::gl_mesh gl_plane = util::convert_to_open_gl_mesh(plane.meshes.front());
    obj_plane.load_vertex_buffer(gl_plane.mesh_data);
  }
  
  // Game Loop
  {
    while(!window.wants_to_quit())
    {
      game_loop();
    }
  }

}