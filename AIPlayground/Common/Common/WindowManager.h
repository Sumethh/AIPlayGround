#pragma once
#include "Window.h"
#include <map>
#include <string>
#include <memory>


class WindowManager
{
public:
  ~WindowManager();
  std::shared_ptr<Window> GetWindow( std::string a_windowName );
  
  
  void AddWindow( std::string a_windowTitle , const int a_width , const int a_height, std::string a_key = 0 );
  void AddWindow( Window* a_newWindow , std::string a_newWindowName);

  void SetMainWindow( std::string a_key );
  std::shared_ptr<Window> GetMainWindow();

  void SwapAllWindowBuffers();
  void ClearAllWindows();
  void UpdateAllWindows();

  static WindowManager* GI();
private:
  WindowManager();

  static WindowManager* s_instance;
  std::shared_ptr<Window> m_mainWindow;
  std::map<std::string , std::shared_ptr<Window>> m_windows;
};