#include "Input.h"
#include <memory>

bool Input::m_keys[ sf::Keyboard::KeyCount ];
bool Input::m_heldKeys[ sf::Keyboard::KeyCount ];

bool Input::m_mouseButtons[ sf::Mouse::ButtonCount ];
bool Input::m_mouseButtonsHeld[ sf::Mouse::ButtonCount ];

bool Input::m_UiCaptureKeyboard;
bool Input::m_UiCaptureMouse;

glm::vec2 Input::m_mouseMovement;
glm::vec2 Input::m_mousePosition;

std::string Input::m_inputString;

void Input::AddToInputString( char a_c )
{
  m_inputString += a_c;
}

std::string Input::GetInputString()
{
  return m_inputString;
}

void Input::SetKeys( const int a_key )
{
  m_keys[ a_key ] = true;
  m_heldKeys[ a_key ] = true;
}

void Input::ResetKey( const int a_key )
{
  m_heldKeys[ a_key ] = false;
}

bool Input::GetKey( const int a_key )
{
  return m_keys[ a_key ];
}

bool Input::GetKeyDown( const int a_key )
{
  return m_heldKeys[ a_key ];
}

void Input::SetMousePosition( const int a_x , const int a_y )
{
  m_mouseMovement.x = m_mousePosition.x - a_x;
  m_mouseMovement.y = m_mousePosition.y - a_y;
  m_mousePosition.x = (float)a_x;
  m_mousePosition.y = (float)a_y;
}

glm::vec2 Input::GetMousePosition()
{
  return m_mousePosition;
}

bool Input::GetMouseButton( const int a_mouseButton )
{
  return m_mouseButtons[ a_mouseButton ];
}

bool Input::GetMouseButtonDown( const int a_mouseButton )
{
  return m_mouseButtonsHeld[ a_mouseButton ];
}

void Input::SetMouseButtons( const int a_mouseButton )
{
  m_mouseButtons[ a_mouseButton ] = true;
  m_mouseButtonsHeld[ a_mouseButton ] = true;
}

void Input::ResetMouseButton( const int a_mouseButton )
{
  m_mouseButtonsHeld[ a_mouseButton ] = false;
}

void Input::Reset()
{
  memset( m_keys , 0 , _countof( m_keys ) * sizeof( bool ) );
  memset( m_mouseButtons , 0 , _countof( m_mouseButtons ) * sizeof( bool ) );
  m_inputString.clear();
}

bool Input::IsMouseOverUI()
{
  return m_UiCaptureMouse;
}

bool Input::IsKeyboardCapturedByUI()
{
  return m_UiCaptureKeyboard;
}

void Input::SetMouseCapture( bool a_capt )
{
  m_UiCaptureMouse = a_capt;
}

void Input::SetKeyboardCapture( bool a_capt )
{
  m_UiCaptureKeyboard = a_capt;
}
