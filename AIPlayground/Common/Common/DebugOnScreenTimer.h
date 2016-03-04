#pragma once
#include "Window.h"
#include "types.h"

#include <vector>
#include <string.h>
#include <mutex>

#include <SFML/Graphics.hpp>

#define FONT_SIZE 8

class DebugOnScreenTimer
{
public:

  struct OnScreenTimer
  {
    OnScreenTimer( const OnScreenTimer& a_copyingTimer ) :
      value( a_copyingTimer.value ) ,
      baseText( a_copyingTimer.baseText ) ,
      trailingText( a_copyingTimer.trailingText ),
      displayingText( a_copyingTimer.displayingText ),
      timerMutex() ,
      active( a_copyingTimer.active )
    {
      active = !active;
      active = !active;
    }
    OnScreenTimer()
    {
    }

    float value;
    std::string baseText;
    std::string trailingText;
    std::string displayingText;
    std::mutex timerMutex;
    bool active;
  };

  inline static void Init()
  {
    if( m_font.loadFromFile( "../Assets/Fonts/KenPixel.ttf" ) )
    {
      m_text.setFont( m_font );
      m_text.setCharacterSize( FONT_SIZE );
      
    }

  }

  inline static int32 AddNewTimer( std::string& a_baseText )
  {
    size_t returnValue = m_timers.size();
    for( int i = 0; i < m_timers.size(); i++ )
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
    if( a_timerIndex < m_timers.size() && m_timers[ a_timerIndex ].active )
    {
      m_timers[ a_timerIndex ].value = a_newValue;
      m_timers[ a_timerIndex ].displayingText = m_timers[ a_timerIndex ].baseText +
        std::to_string( a_newValue ) + m_timers[ a_timerIndex ].trailingText;
      
    }
  }

  inline static void SetTimerTrailingText( int32 a_timerIndex , std::string a_trailing )
  {
    if( a_timerIndex < m_timers.size() && m_timers[ a_timerIndex ].active )
    {
      m_timers[ a_timerIndex ].trailingText = a_trailing;
    }
  }

  inline static void DisableTimer( int32 a_timerIndex )
  {
    if( a_timerIndex < m_timers.size() )
      m_timers[ a_timerIndex ].active = false;

  }

  inline static void DrawTimers( Window* a_window )
  {
    if( m_text.getFont() != nullptr )
    {
      for( int i = 0; i < m_timers.size(); i++ )
      {
        if( m_timers[ i ].active == false )
          continue;
        float yPos = 5.0f + (FONT_SIZE + 2) * (float)i;
        m_text.setPosition( sf::Vector2f( 5.0f , yPos ) );
        m_text.setString( m_timers[ i ].displayingText );
        a_window->RenderDrawable( m_text );
      }
    }
  }

private:
  static sf::Text m_text;
  static sf::Font m_font;
  static std::vector<OnScreenTimer> m_timers;

};

sf::Text DebugOnScreenTimer::m_text;
sf::Font DebugOnScreenTimer::m_font;
std::vector<DebugOnScreenTimer::OnScreenTimer> DebugOnScreenTimer::m_timers;