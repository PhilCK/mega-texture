#include "input.hpp"
#include "message_pump.hpp"


namespace sdl {


input::input()
: m_mouse_delta_x(0)
, m_mouse_delta_y(0)
{
  sdl::message_callback_register(m_message_callback);
}
  
  
input::~input()
{
  sdl::message_callback_unregister(m_message_callback);
}


void
input::think()
{
  // Reset
  m_mouse_delta_x = 0;
  m_mouse_delta_y = 0;
}


int32_t
input::get_mouse_x() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);
  
  return x;
}


int32_t
input::get_mouse_y() const
{
	int x, y;
	SDL_GetMouseState(&x, &y);
  
  return y;
}


bool
input::_process_message(const SDL_Event &event)
{
  auto update_key = [&](const uint32_t id, const key_state state)
  {
    m_key_states[id]       = state;
    //m_frame_key_states[id] = state;
  };

  switch (event.type)
  {
    case(SDL_KEYDOWN):
    {
      const SDL_Keycode key = event.key.keysym.sym;
      update_key(key, key_state::DOWN);

      if(key == SDLK_PASTE)
      {
        // Paste
      }

      break;
    }
    
    case(SDL_KEYUP):
    {
      const SDL_Keycode key = event.key.keysym.sym;
      update_key(key, key_state::UP);

      break;
    }

    case(SDL_TEXTINPUT):
    {
//      if (on_text_stream_event)
//      {
//        const std::string text = event.text.text;
//        on_text_stream_event(text);
//      }

      break;
    }

    case(SDLK_PASTE):
    case(SDL_TEXTINPUTEVENT_TEXT_SIZE):
    case(SDL_TEXTEDITING):
    case(SDL_SCANCODE_PASTE):
    {
      break;
    }

    case(SDL_MOUSEMOTION):
    {
      m_mouse_delta_x = static_cast<int32_t>(event.motion.xrel);
      m_mouse_delta_y = static_cast<int32_t>(event.motion.yrel);

//      if (on_mouse_delta_event)
//      {
//        on_mouse_delta_event(m_x_delta, m_y_delta);
//      }
//
//      if (on_mouse_move_position_event)
//      {
//        on_mouse_move_position_event(static_cast<float>(get_mouse_x()), static_cast<float>(get_mouse_y()));
//      }

      break;
    }

    case(SDL_MOUSEBUTTONDOWN) :
    {
      switch (event.button.button)
      {
      case(SDL_BUTTON_LEFT) :
        //update_key(key_id::MS_LEFT, key_state::DOWN);
        break;

      case(SDL_BUTTON_MIDDLE) :
        //update_key(key_id::MS_CENTER, key_state::DOWN);
        break;

      case(SDL_BUTTON_RIGHT) :
        //update_key(key_id::MS_RIGHT, key_state::DOWN);
        break;
      }

      break;
    }

    case(SDL_MOUSEBUTTONUP) :
    {
      switch (event.button.button)
      {
        case(SDL_BUTTON_LEFT):
          ///update_key(key_id::MS_LEFT, key_state::UP);
          break;

        case(SDL_BUTTON_MIDDLE):
          //update_key(key_id::MS_CENTER, key_state::UP);
          break;

        case(SDL_BUTTON_RIGHT):
          //update_key(key_id::MS_RIGHT, key_state::UP);
          break;
      }

      break;
    }
  }

  return false; // Don't need to swallow any messages.
}

  
bool
input::set_mouse_hold(const bool set)
{
  const SDL_bool set_mouse_hold = set ? SDL_TRUE : SDL_FALSE;

  if(SDL_GetRelativeMouseMode() == set_mouse_hold)
  {
    return false;
  }

  SDL_SetRelativeMouseMode(set_mouse_hold);
  return true;
}


bool
input::is_mouse_held() const
{
  const SDL_bool is_mouse_held = SDL_GetRelativeMouseMode();
  return is_mouse_held ? true : false;
}


} // namespace