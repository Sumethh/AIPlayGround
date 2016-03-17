#include "TimedFunctionCallManager.h"
TimedFunctionCallManager* TimedFunctionCallManager::m_instance;

TimedFunctionCallManager::TimedFunctionCallManager()
{
}


TimedFunctionCallManager::~TimedFunctionCallManager()
{
}
TimedFunctionCallManager* TimedFunctionCallManager::GI()
{

  {
    if( !m_instance )
      m_instance = new TimedFunctionCallManager();
    return m_instance;
  }
}

void TimedFunctionCallManager::Update()
{
  for( auto i : m_timers )
  {
    if( i->Update() )
      RemoveTimer( i );
  }
}

bool TimedFunctionCallManager::RemoveTimer( TimedFunctionCall* a_timer )
{
  auto iterator = std::find( m_timers.begin() , m_timers.end() , a_timer );
  if( iterator != m_timers.end() )
  {
    delete ( *iterator );
    m_timers.erase( iterator );
    return true;
  }
  else
    return false;

}

bool TimedFunctionCallManager::AddTimer( TimedFunctionCall* a_timer )
{
  m_timers.push_back( a_timer );
  return true;
}
