#include "Window.h"
#include "Input.h"
#include "log.h"
#include "../ImGuiImplementation.h"
Window::Window( const int a_width , const int a_height , const char* a_windowName ) :
  m_width( a_width ) ,
  m_height( a_height ) ,
  m_closeRequested( false )
{
  settings.majorVersion = 3;
  settings.minorVersion = 3;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.antialiasingLevel = 0;
  m_window = new sf::RenderWindow(
    sf::VideoMode( a_width , a_height ) , a_windowName , sf::Style::Default , settings
  );
  //m_window->setFramerateLimit( 120 );
}

Window::~Window()
{
  delete m_window;
  m_width , m_height = 0;
}

void Window::Swap()
{
  m_window->display();
}

void Window::Close()
{
  m_closeRequested = true;
}

void Window::Update()
{
  sf::Event windowEvent;

  while( m_window->pollEvent( windowEvent ) )
  {
    if( windowEvent.type == sf::Event::Closed )
    {
      m_closeRequested = true;
    }
    if( windowEvent.type == sf::Event::Resized )
    {
      m_width = windowEvent.size.width;
      m_height = windowEvent.size.height;
    }

    if( windowEvent.type == sf::Event::MouseMoved )
    {
      Input::SetMousePosition( windowEvent.mouseMove.x , windowEvent.mouseMove.y );
    }

    if( windowEvent.type == sf::Event::MouseButtonPressed )
    {
      Input::SetMouseButtons( windowEvent.mouseButton.button );
      //ImGui_ImplGlfwGL3_MouseButtonCallback( this , windowEvent.mouseButton.button , windowEvent.type , 0 );
      MouseButtonCallback( this , windowEvent.mouseButton.button , windowEvent.type , 0 );
    }
    if( windowEvent.type == sf::Event::MouseButtonReleased )
    {
      Input::ResetMouseButton( windowEvent.mouseButton.button );
    }

    if( windowEvent.type == sf::Event::KeyPressed )
    {
      Input::SetKeys( windowEvent.key.code );
      ImGui_ImplGlfwGL3_KeyCallback( this , windowEvent.key.code , windowEvent.type , 0 );
    }
    if( windowEvent.type == sf::Event::KeyReleased )
    {
      Input::ResetKey( windowEvent.key.code );
    }
    if( windowEvent.type == sf::Event::TextEntered )
    {
      Input::AddToInputString( (char)windowEvent.text.unicode );
    }
  }

  if( Input::GetKey( sf::Keyboard::Escape ) )
    Close();
}

void Window::Clear( sf::Color a_color )
{
  m_window->clear( a_color );
}

void Window::SetWindowName( std::string a_newName )
{
  m_window->setTitle( a_newName );
}

sf::RenderWindow* Window::GetWindow()
{
  return m_window;
}