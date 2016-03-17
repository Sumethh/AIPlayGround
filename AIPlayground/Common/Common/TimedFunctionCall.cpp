#include "TimedFunctionCall.h"
#include "TimedFunctionCallManager.h"


TimedFunctionCall::TimedFunctionCall( float a_timeToWaitFor ,
  std::function<void( TimeFunctionCallArgument )> a_funcToCall ,
  TimeFunctionCallArgument a_argument , bool a_looping ) :
  m_timeToWaitFor( a_timeToWaitFor ) ,
  m_funcToCall( a_funcToCall ) ,
  m_argument( a_argument ) ,
  m_looping( a_looping )
{
  m_timer.Start();
  TimedFunctionCallManager::GI()->AddTimer( this );
}


TimedFunctionCall::~TimedFunctionCall()
{
}

bool TimedFunctionCall::Update()
{
  if( m_timer.IntervalS() >= m_timeToWaitFor )
  {
    m_funcToCall( m_argument );
    if( !m_looping )
      return true;
    else
      m_timer.Reset();
  }
  else
    return false;
}

TimedFunctionCall* TimedFunctionCall::Create( float a_timeToWaitFor , std::function<void( TimeFunctionCallArgument )> a_funcToCall , TimeFunctionCallArgument a_argument , bool a_looping )
{
  return new TimedFunctionCall( a_timeToWaitFor , a_funcToCall , a_argument , a_looping );
}
