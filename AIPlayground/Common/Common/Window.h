#pragma once
#include <SFML/Graphics.hpp>

class Window
{
public:
  Window( const int a_width , const int a_height , const char* a_windowName );
  ~Window();

  void Swap();
  void Update();
  void Close();
  inline void RenderDrawable( sf::Drawable& a_drawingObject )
  {
    m_window->draw( a_drawingObject );
  }

  void Clear( sf::Color a_color );

  void SetWindowName( std::string a_newName );

  sf::RenderWindow* GetWindow();

  inline int GetHeight() const { return m_height; }
  inline int GetWidth() const { return m_width; }

  bool IsCloseRequested() { return m_closeRequested; }


  void( *MouseButtonCallback )( Window* , int , int , int );
private:

  sf::RenderWindow* m_window;
  int m_height , m_width;
  bool m_closeRequested;
  sf::ContextSettings settings;
  Window( const Window& );
  Window& operator = ( const Window& );
};