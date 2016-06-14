#include "WindowManager.h"
#include "log.h"

WindowManager* WindowManager::s_instance;

WindowManager::WindowManager() :
  m_windows()
{
}

WindowManager::~WindowManager()
{
  delete s_instance;
  m_windows.clear();
}

std::shared_ptr<Window> WindowManager::GetWindow( std::string a_windowName )
{
  return m_windows[ a_windowName ];
}

void WindowManager::AddWindow( Window* a_newWindow , std::string a_newWindowName )
{
  if( m_windows[ a_newWindowName ] )
  {
    LOGI( "There is alread a window with the name of %s" , a_newWindowName.c_str() );
    return;
  }

  m_windows[ a_newWindowName ] = std::shared_ptr<Window>( a_newWindow );
}

void WindowManager::AddWindow( std::string a_windowTitle , const int a_width , const int a_height , std::string a_key )
{
  if( a_key.size() > 0 )
  {
    if( m_windows[ a_key ] )
    {
      LOGI( "There is alread a window with the name of %s" , a_key.c_str() );
      return;
    }
    else
    {
      m_windows[ a_key ] = std::shared_ptr<Window>( new Window( a_width , a_height , a_windowTitle.c_str() ) );
    }
  }
  else
  {
    if( m_windows[ a_windowTitle ] )
    {
      LOGI( "There is alread a window with the name of %s" , a_windowTitle.c_str() );
      return;
    }
    else
    {
      m_windows[ a_windowTitle ] = std::shared_ptr<Window>( new Window( a_width , a_height , a_windowTitle.c_str() ) );
    }
  }
}

void WindowManager::SetMainWindow( std::string a_key )
{
  m_mainWindow = m_windows[ a_key ];
}

std::shared_ptr<Window> WindowManager::GetMainWindow()
{
  return m_mainWindow;
}

void WindowManager::SwapAllWindowBuffers()
{
  for( auto it = m_windows.begin(); it != m_windows.end(); it++ )
  {
    it->second->Swap();
  }
}

void WindowManager::ClearAllWindows()
{
  for( auto it = m_windows.begin(); it != m_windows.end(); it++ )
    it->second->Clear( sf::Color::Black );
}

void WindowManager::UpdateAllWindows()
{
  for( auto it = m_windows.begin(); it != m_windows.end(); it++ )
    it->second->Update();
}

WindowManager* WindowManager::GI()
{
  if( !s_instance )
    s_instance = new WindowManager();
  return s_instance;
}