#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <glm/glm.hpp>
#include <string>

class Input
{
public:
  static void SetKeys( const int a_key );
  static void ResetKey( const int a_key );

  static bool GetKey( const int a_key );
  static bool GetKeyDown( const int a_key );

  static void AddToInputString( char a_c );
  static std::string GetInputString();

  static void SetMousePosition( const int a_x , const int a_y );
  static glm::vec2 GetMousePosition();

  static bool GetMouseButton( const int a_mouseButton );
  static bool GetMouseButtonDown( const int a_mouseButton );
  static void SetMouseButtons( const int a_mouseButton );
  static void ResetMouseButton( const int a_mouseButton );

  static void Reset();

private:
  static bool m_keys[ sf::Keyboard::KeyCount ];
  static bool m_heldKeys[ sf::Keyboard::KeyCount ];

  static bool m_mouseButtons[ sf::Mouse::ButtonCount ];
  static bool m_mouseButtonsHeld[ sf::Mouse::ButtonCount ];

  static glm::vec2 m_mouseMovement;
  static glm::vec2 m_mousePosition;

  static std::string m_inputString;
};