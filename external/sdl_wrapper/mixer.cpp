//#include "mixer.hpp"
//#include "sdl_common.hpp"
//#include <assert.h>
//
//
//namespace
//{
//  // This is a bit rubbish, but its a way of trying to ensure that only one
//  // instance of sdl_mixer is active and can call quit.
//  sdl_mixer *owner = nullptr;
//  
//  inline uint32_t size_to_id(const std::size_t size)
//  {
//    return static_cast<uint32_t>(size);
//  }
//  
//  inline std::size_t id_to_size(const uint32_t id)
//  {
//    return static_cast<std::size_t>(id) - 1;
//  }
//  
//  // Helper method to make sure id is valid.
//  template<typename T>
//  inline bool is_valid_id(const uint32_t id, const T &container)
//  {
//    if(id == 0) { return false; }
//    if(id > container.size()) { return false; }
//    
//    return true;
//  }
//}
//
//
//namespace sdl {
//
//
//mixer::mixer()
//{
//  if(owner != nullptr)
//  {
//    sdl_common::log_debug_error("There is already an active instance of sdl_mixer.");
//    return;
//  }
//  else
//  {
//    owner = this;
//  }
//
//  Mix_Init(0);
//
//  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
//  {
//    sdl_common::log_debug_error_check();
//  }
//  
//  if(Mix_AllocateChannels(2048) < 0)
//  {
//    sdl_common::log_debug_error_check();
//  }
//  
//  m_samples.reserve(128);
//  m_music.reserve(32);
//}
//
//
//mixer::~mixer()
//{
//  std::lock_guard<std::mutex> locker(m_lock);
//
//  if(is_valid())
//  {
//    owner = nullptr;
//    Mix_Quit();
//    sdl_common::log_debug_error_check();
//  }
//}
//
//
//mixer::mixer(mixer&& other)
//{
//  std::lock_guard<std::mutex> locker_this(m_lock);
//  std::lock_guard<std::mutex> locker_other(other.m_lock);
//  
//  if(other.is_valid())
//  {
//    owner = this;
//  }
//
//  this->m_samples = std::move(other.m_samples);
//  this->m_music = std::move(other.m_music);
//}
//
//
//mixer&
//mixer::operator=(mixer&& other)
//{
//  std::lock_guard<std::mutex> locker_this(m_lock);
//  std::lock_guard<std::mutex> locker_other(other.m_lock);
//
//  if(other.is_valid())
//  {
//    owner = this;
//  }
//
//  this->m_samples = std::move(other.m_samples);
//  this->m_music = std::move(other.m_music);
//  
//  return *this;
//}
//
//
//uint32_t
//mixer::sample_add(const std::string &filename)
//{
//  std::lock_guard<std::mutex> locker(m_lock);
//  
//  // Valid checks
//  {
//    const bool is_valid_mixer = is_valid();
//
//    assert(is_valid_mixer);
//    
//    if(!is_valid_mixer)
//    {
//      sdl_common::log_debug_error("sample_add. sdl_mixer is not valid.");
//      return 0;
//    }
//  }
//
//  // Add sample
//  {
//    audio_sample sample;
//    sample.chunk = Mix_LoadWAV(filename.c_str());
//    
//    if(sample.chunk)
//    {
//      sample.channel =  static_cast<uint32_t>(m_samples.size()) + 1;
//      m_samples.emplace_back(sample);
//      
//      return size_to_id(m_samples.size());
//    }
//    else
//    {
//      sdl_common::log_debug_error_check();
//      sdl_common::log_debug_error("sample_add. does the file exist?");
//      return 0;
//    }
//  }
//}
//
//
//bool
//mixer::sample_remove(const uint32_t id)
//{
//  std::lock_guard<std::mutex> locker(m_lock);
//
//  // Valid checks
//  {
//    const bool has_valid_id = is_valid_id(id, m_samples);
//    const bool is_valid_mixer = is_valid();
//
//    assert(has_valid_id && is_valid_mixer);
//    
//    if(!has_valid_id || !is_valid_mixer)
//    {
//      sdl_common::log_debug_error("sample_remove. ID or sdl_mixer is not valid.");
//      return false;
//    }
//  }
//
//  // Remove sample
//  {
//    audio_sample &sample = m_samples.at(id);
//    
//    if(sample.chunk != nullptr)
//    {
//      Mix_FreeChunk(sample.chunk);
//      memset(&sample, 0, sizeof(sample));
//      return true;
//    }
//
//    return false;
//  }
//}
//
//
//bool
//mixer::sample_play(const uint32_t id, const int32_t number_of_loops)
//{
//  std::lock_guard<std::mutex> locker(m_lock);
//  
//  // Valid checks
//  {
//    const bool has_valid_id = is_valid_id(id, m_samples);
//    const bool is_valid_mixer = is_valid();
//
//    assert(has_valid_id && is_valid_mixer);
//    
//    if(!has_valid_id || !is_valid_mixer)
//    {
//      sdl_common::log_debug_error("sample_set_position. ID or sdl_mixer is not valid.");
//      return false;
//    }
//  }
//
//  // Sample play
//  {
//    const auto &sample = m_samples.at(id_to_size(id));
//    
//    if(Mix_PlayChannel(sample.channel, sample.chunk, number_of_loops) < 0)
//    {
//      sdl_common::log_debug_error_check();
//      return false;
//    }
//    
//    return true;
//  }
//}
//
//
//bool
//mixer::sample_is_playing(const uint32_t id) const
//{
//  std::lock_guard<std::mutex> locker(m_lock);
//
//  // Valid checks
//  {
//    const bool has_valid_id = is_valid_id(id, m_samples);
//    const bool is_valid_mixer = is_valid();
//
//    assert(has_valid_id && is_valid_mixer);
//    
//    if(!has_valid_id || !is_valid_mixer)
//    {
//      sdl_common::log_debug_error("sample_is_playing. ID or sdl_mixer is not valid.");
//      return false;
//    }
//  }
//  
//  // is playing
//  {
//    const audio_sample &sample = m_samples.at(id_to_size(id));
//    
//    // Couldn't not find sample
//    if(sample.chunk == nullptr)
//    {
//      sdl_common::log_debug_error("sample_is_playing. Sample is null, did it get remmoved?");
//      
//      return false;
//    }
//    
//    return Mix_Playing(sample.channel) > 0;
//  }
//}
//
//
//namespace
//{
//  uint8_t volume_normal_to_sdl_mixer(const float vol)
//  {
//    uint8_t mix_vol = vol * MIX_MAX_VOLUME;
//  
//    if(mix_vol > MIX_MAX_VOLUME)
//    {
//      return MIX_MAX_VOLUME;
//    }
//    else if(vol <= 0)
//    {
//      return 0;
//    }
//    
//    return static_cast<uint8_t>(vol * 128);
//  }
//  
//  float volume_sdl_mixer_to_normal(const uint8_t vol)
//  {
//    return vol / MIX_MAX_VOLUME;
//  }
//}
//
//
//void
//mixer::sample_set_volume(const uint32_t id, const float vol)
//{
//  std::lock_guard<std::mutex> locker(m_lock);
//
//  // Valid checks
//  {
//    const bool has_valid_id = is_valid_id(id, m_samples);
//    const bool is_valid_mixer = is_valid();
//
//    assert(has_valid_id && is_valid_mixer);
//    
//    if(!has_valid_id || !is_valid_mixer)
//    {
//      sdl_common::log_debug_error("sample_set_volume. ID or sdl_mixer is not valid.");
//      return;
//    }
//  }
//
//  // Set volume
//  {
//    audio_sample &sample = m_samples.at(id_to_size(id));
//    
//    if(sample.chunk == nullptr)
//    {
//      sdl_common::log_debug_error("sample_set_volume. Sample is null, did it get remmoved?");
//    }
//    
//    sample.chunk->volume = volume_normal_to_sdl_mixer(vol);
//  }
//}
//
//
//float
//mixer::sample_get_volume(const uint32_t id) const
//{
//  std::lock_guard<std::mutex> locker(m_lock);
//
//  // Valid checks
//  {
//    const bool has_valid_id = is_valid_id(id, m_samples);
//    const bool is_valid_mixer = is_valid();
//
//    assert(has_valid_id && is_valid_mixer);
//    
//    if(!has_valid_id || !is_valid_mixer)
//    {
//      sdl_common::log_debug_error("sample_get_volume. ID or sdl_mixer is not valid.");
//      return 0.f;
//    }
//  }
//
//  // Get volume
//  {
//    const audio_sample &sample = m_samples.at(id_to_size(id));
//    
//    if(sample.chunk == nullptr)
//    {
//      sdl_common::log_debug_error("sample_get_volume. Sample is null, did it get remmoved?");
//    }
//
//    return volume_sdl_mixer_to_normal(sample.chunk->volume);
//  }
//}
//
//
//void
//mixer::sample_set_position(const uint32_t id, const std::array<float, 3> &position)
//{
//  std::lock_guard<std::mutex> locker(m_lock);
//
//  // Valid checks
//  {
//    const bool has_valid_id = is_valid_id(id, m_samples);
//    const bool is_valid_mixer = is_valid();
//
//    assert(has_valid_id && is_valid_mixer);
//    
//    if(!has_valid_id || !is_valid_mixer)
//    {
//      sdl_common::log_debug_error("sample_set_position. ID or sdl_mixer is not valid.");
//      return;
//    }
//  }
//
//  // Set position
//  {
//    audio_sample &sample = m_samples.at(id);
//    
//    enum { X = 0, Y, Z };
//    
//    sample.x = position.at(X);
//    sample.y = position.at(Y);
//    sample.z = position.at(Z);
//  }
//}
//
//
//uint32_t
//mixer::music_add(const std::string &filename)
//{
//  assert(!filename.empty());
// 
//  // Load Muisc
//  {
//    Mix_Music *music = Mix_LoadMUS(filename.c_str());
//    
//    if(music)
//    {
//      audio_music new_music;
//      new_music.chunk = music;
//      
//      m_music.emplace_back(new_music);
//      
//      return size_to_id(m_music.size());
//    }
//    else
//    {
//      sdl_common::log_debug_error_check();
//      sdl_common::log_debug_error("music_add Failed: Check path of music file.");
//    }
//  }
//
//  return 0;
//}
//
//
//bool
//mixer::music_remove(const uint32_t id)
//{
//  // Valid checks
//  {
//    const bool has_valid_id = is_valid_id(id, m_music);
//    const bool is_valid_mixer = is_valid();
//
//    assert(has_valid_id && is_valid_mixer);
//    
//    if(!has_valid_id || !is_valid_mixer)
//    {
//      sdl_common::log_debug_error("music_remove. ID or sdl_mixer is not valid.");
//      return false;
//    }
//  }
//  
//  // Remove
//  {
//    audio_music &music = m_music.at(id_to_size(id));
//    
//    if(music.chunk)
//    {
//      Mix_FreeMusic(music.chunk);
//      music.chunk = nullptr;
//      
//      return true;
//    }
//    else
//    {
//      sdl_common::log_debug_error("music_remove: Music already empty.");
//      
//      return false;
//    }
//  }
//
//  return false;
//}
//
//
//bool
//mixer::music_play(const uint32_t id, const int32_t number_of_loops)
//{
//  // Valid checks
//  {
//    const bool has_valid_id = is_valid_id(id, m_music);
//    const bool is_valid_mixer = is_valid();
//
//    assert(has_valid_id && is_valid_mixer);
//    
//    if(!has_valid_id || !is_valid_mixer)
//    {
//      sdl_common::log_debug_error("music_remove. ID or sdl_mixer is not valid.");
//      return false;
//    }
//  }
//  
//  // Play music
//  {
//    audio_music &music = m_music.at(id_to_size(id));
//    
//    if(music.chunk)
//    {
//      if(Mix_PlayMusic(music.chunk, number_of_loops) < 0)
//      {
//        sdl_common::log_debug_error("Failed playing music.");
//        sdl_common::log_debug_error_check();
//        
//        return false;
//      }
//      
//      return true;
//    }
//    else
//    {
//      sdl_common::log_debug_error("music_play: Music track is empty, has it been removed.");
//      return false;
//    }
//  }
//
//  return false;
//}
//
//
//bool
//mixer::music_is_playing() const
//{
//  // Valid checks
//  {
//    const bool is_valid_mixer = is_valid();
//
//    assert(is_valid_mixer);
//    
//    if(!is_valid_mixer)
//    {
//      sdl_common::log_debug_error("music_is_playing. ID or sdl_mixer is not valid.");
//      return false;
//    }
//  }
//  
//  // Is Playing
//  {
//    return Mix_PlayingMusic() == 1 ? true : false;
//  }
//
//  return false;
//}
//
//
//void
//mixer::music_set_volume(const float vol)
//{
//  const bool is_valid_mixer = is_valid();
//  assert(is_valid_mixer);
//  
//  if(!is_valid_mixer)
//  {
//    sdl_common::log_debug_error("music_set_volume: sdl_mixer is not valid.");
//    return;
//  }
//
//  const uint8_t set_vol = volume_normal_to_sdl_mixer(vol);
//  Mix_VolumeMusic(set_vol);
//}
//
//
//float
//mixer::music_get_volume() const
//{
//  return 0.f;
//}
//
//
//void
//mixer::music_set_balance(const uint32_t id, const float bal)
//{
//  
//}
//
//
//float
//mixer::music_get_balance(const uint32_t id)
//{
//  return 0.f;
//}
//
//
//void
//mixer::update_3d_positions()
//{
//}
//
//
//void
//mixer::reset()
//{
//  m_samples.clear();
//  m_music.clear();
//}
//
//
//void
//mixer::clear_samples()
//{
//}
//
//
//void
//mixer::clear_music()
//{
//  
//}
//
//
//bool
//mixer::is_valid() const
//{
//  return owner == this;
//}
//
//
//} // namespace