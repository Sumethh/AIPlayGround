#pragma once
#include "Window.h"
#include "types.h"

#include <vector>
#include <string.h>
#include "imgui.h"
#define FONT_SIZE 8

class DebugOnScreenTimer
{
public:

  struct OnScreenTimer
  {
    OnScreenTimer( const OnScreenTimer& a_copyingTimer ) :
      value( a_copyingTimer.value ) ,
      baseText( a_copyingTimer.baseText ) ,
      trailingText( a_copyingTimer.trailingText ) ,
      displayingText( a_copyingTimer.displayingText ) ,
      active( a_copyingTimer.active )
    {}
    OnScreenTimer()
    {
    }

    float value;
    std::string baseText;
    std::string trailingText;
    std::string displayingText;
    
    bool active;
  };

  inline static int32 AddNewTimer( std::string& a_baseText )
  {
    size_t returnValue = m_timers.size();
    for( size_t i = 0; i < m_timers.size(); i++ )
    {
      if( m_timers[ i ].active == false )
      {
        returnValue = i;
        break;
      }
    }
    if( returnValue == m_timers.size() )
    {
      m_timers.push_back( OnScreenTimer() );
    }
    OnScreenTimer* timer = &m_timers[ returnValue ];
    timer->baseText = a_baseText;
    timer->active = true;
    return (int32)returnValue;
  }

  inline static void SetTimerValue( int32 a_timerIndex , float a_newValue )
  {
    if( (size_t)a_timerIndex < m_timers.size() && m_timers[ a_timerIndex ].active )
    {
      m_timers[ a_timerIndex ].value = a_newValue;
      m_timers[ a_timerIndex ].displayingText = m_timers[ a_timerIndex ].baseText +
        std::to_string( a_newValue ) + m_timers[ a_timerIndex ].trailingText;
    }
  }

  inline static void SetTimerTrailingText( int32 a_timerIndex , std::string a_trailing )
  {
    if( (size_t)a_timerIndex < m_timers.size() && m_timers[ a_timerIndex ].active )
    {
      m_timers[ a_timerIndex ].trailingText = a_trailing;
    }
  }

  inline static void DisableTimer( int32 a_timerIndex )
  {
    if( (size_t)a_timerIndex < m_timers.size() )
      m_timers[ a_timerIndex ].active = false;
  }

  inline static void DrawTimers()
  {
    ImGui::Begin("Debug Timers",nullptr, ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
    for( size_t i = 0; i < m_timers.size(); i++ )
      {
        if( m_timers[ i ].active == false )
          continue;
        ImGui::Text( m_timers[ i ].displayingText.c_str() );
      }
    ImGui::End();
  }

private:
  static std::vector<OnScreenTimer> m_timers;
};
std::vector<DebugOnScreenTimer::OnScreenTimer> DebugOnScreenTimer::m_timers;