//#ifndef SDL_MIXER_INCLUDED_F0E6D002_0008_439E_8989_F59789BD232F
//#define SDL_MIXER_INCLUDED_F0E6D002_0008_439E_8989_F59789BD232F
//
//
//#include "sdl_common.hpp"
////#include <SDL_mixer.h>
//#include <vector>
//#include <mutex>
//#include <array>
//
//
//namespace sdl {
//
//
///*
//  Wrap around for sdl_mixer.
//  --
//  There is only supposed to be one active instances of this class. Having two instances is
//  'undefined' as when an instances goes out of scope it will call SDL Mixer's quit function.
//  
//  Although this is move friendly.
//  
//  Issue.
//  Needs better support for playing the same file multiple times.
//*/
//class mixer
//{
//
//                  mixer(const mixer &) = delete;
//                  mixer operator=(const mixer &) = delete;
//
//public:
//
//  explicit        mixer();
//                  ~mixer();
//  
//                  mixer(mixer &&other);
//  mixer&          operator=(mixer &&other);
//  
//  // ** Samples ** //
//  uint32_t        sample_add(const std::string &filename);
//  bool            sample_remove(const uint32_t id);
//  bool            sample_play(const uint32_t id, const int32_t number_of_loops = 0);
//  bool            sample_is_playing(const uint32_t id) const;
//  void            sample_set_volume(const uint32_t id, const float vol);
//  float           sample_get_volume(const uint32_t id) const;
//  void            sample_set_position(const uint32_t id, const std::array<float, 3>  &position);
//  
//  // ** Music ** //
//  uint32_t        music_add(const std::string &filename);
//  bool            music_remove(const uint32_t id);
//  bool            music_play(const uint32_t id, const int32_t number_of_loops = -1);
//  bool            music_is_playing() const;
//  void            music_set_volume(const float vol);
//  float           music_get_volume() const;
//  void            music_set_balance(const uint32_t id, const float bal);
//  float           music_get_balance(const uint32_t id);
//  
//  // ** Everything else ** //
//  void            set_virtual_ear_look_at(const std::array<float, 3> &position,
//                                          const std::array<float, 3> &direction,
//                                          const std::array<float, 3> &up);
//  void            update_3d_positions();
//  void            reset();
//  void            clear_samples();
//  void            clear_music();
//  bool            is_valid() const;
//  
//private:
//
//  struct audio_sample
//  {
//    Mix_Chunk *chunk  = nullptr;
//    uint32_t channel  = 0;
//    float x           = 0.f;
//    float y           = 0.f;
//    float z           = 0.f;
//  };
//
//  struct audio_music
//  {
//    Mix_Music *chunk  = nullptr;
//  };
//
//  std::vector<audio_sample>     m_samples;
//  std::vector<audio_music>      m_music;
//  mutable std::mutex            m_lock;
//  std::array<float, 3>          m_position  = {{0.f, 0.f, 0.f}};
//  std::array<float, 3>          m_direction = {{0.f, 0.f, 1.f}};
//  std::array<float, 3>          m_up        = {{0.f, 1.f, 0.f}};
//  bool                          m_dirty_pos = false;
//
//}; // class
//
//
//} // namespace
//
//
//#endif // include guard