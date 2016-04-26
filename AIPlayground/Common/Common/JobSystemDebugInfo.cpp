#include "JobSystemDebugInfo.h"
#include <SFML/Graphics/Text.hpp>
#include <string.h>
#include "Window.h"

JobSystemDebugInfo* JobSystemDebugInfo::m_instance;

void JobSystemDebugInfo::Init()
{
  m_font.loadFromFile( "../Assets/Fonts/KenPixel.ttf" );
  m_functionTimerHandle = TimedFunctionCall::Create( 1.0f , std::bind( &JobSystemDebugInfo::SetDebugInfo , m_instance , std::placeholders::_1 ) , TimeFunctionCallArgument() , true );
}

std::shared_ptr<DebugThreadInfo> JobSystemDebugInfo::registerThread()
{
  m_threadInfos.push_back( std::shared_ptr<DebugThreadInfo>( new DebugThreadInfo ) );
  std::memset( ( m_threadInfos.end() - 1 )->get() , 0 , sizeof( DebugThreadInfo ) - sizeof( std::mutex ) );
  return *( m_threadInfos.end() - 1 );
}

void JobSystemDebugInfo::Render( Window* a_window )
{
  if( !a_window )
    return;

  sf::Text threadIDText , jobTimeAvg , jobTotal;
  threadIDText.setFont( m_font );
  jobTimeAvg.setFont( m_font );
  jobTotal.setFont( m_font );
  threadIDText.setCharacterSize( m_fontSize );
  jobTimeAvg.setCharacterSize( m_fontSize );
  jobTotal.setCharacterSize( m_fontSize );


  sf::Vector2f renderPosition;
  renderPosition.x = (float)m_xSpacing;
  renderPosition.y = (float)m_ySpacing + (float)m_startingYPos;

  for( auto threadInfo : m_threadInfos )
  {
    //get the info and copy it over to reduce the amount of time that we have the lock acquired
    threadInfo->infoLock.lock();
    float time = threadInfo->jobTimeAverage;
    uint32 jobsCompleted = threadInfo->jobsCompletedLast5Seconds;
    uint32 threadID = threadInfo->threadID;
    threadInfo->infoLock.unlock();
    sf::FloatRect rect;

    SetTextStrings( threadIDText , jobTimeAvg , jobTotal , threadID , time , jobsCompleted );

    rect = threadIDText.getLocalBounds();
    threadIDText.setPosition( renderPosition.x , renderPosition.y );
    a_window->RenderDrawable( threadIDText );

    renderPosition.y += m_ySpacing;
    rect = jobTimeAvg.getLocalBounds();
    jobTimeAvg.setPosition( renderPosition.x , renderPosition.y );
    a_window->RenderDrawable( jobTimeAvg );


    renderPosition.y += m_ySpacing;
    rect = jobTotal.getLocalBounds();
    jobTotal.setPosition( renderPosition.x , renderPosition.y );
    a_window->RenderDrawable( jobTotal );

    renderPosition.y += m_ySpacing * 2;

  }
}

void JobSystemDebugInfo::SetTextStrings( sf::Text& a_threadID , sf::Text& a_jobTimeAvg , sf::Text& a_jobsCompletedText , uint32& a_id , float& a_time , uint32& a_jobsCompleted )
{
  std::string infoString;
  infoString = "Thread ID: ";
  infoString.append( std::to_string( a_id ) );
  a_threadID.setString( infoString );

  infoString = "Avg time on Job (ms): ";
  infoString.append( std::to_string( a_time ) );
  a_jobsCompletedText.setString( infoString );

  infoString = "Jobs Completed (p/s): ";
  infoString.append( std::to_string( a_jobsCompleted ) );
  a_jobTimeAvg.setString( infoString );
}

void JobSystemDebugInfo::SetDebugInfo( TimeFunctionCallArgument a_argument )
{
  for( auto threadInfo : m_threadInfos )
  {
    threadInfo->infoLock.lock();

    threadInfo->jobsCompletedLast5Seconds = threadInfo->jobsCompleted;
    threadInfo->jobsCompleted = 0;

    if( threadInfo->jobTimeSamplesCount )
      threadInfo->jobTimeAverage = (float)threadInfo->jobTimeTotal / threadInfo->jobTimeSamplesCount;
    else
      threadInfo->jobTimeAverage = 0.0f;
    threadInfo->jobTimeTotal = 0;
    threadInfo->jobTimeSamplesCount = 0;


    threadInfo->infoLock.unlock();
  }
}

