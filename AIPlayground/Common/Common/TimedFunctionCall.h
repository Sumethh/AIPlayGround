#pragma once
#include "timer.h"
#include <functional>
struct TimeFunctionCallArgument
{};

class TimedFunctionCall
{
public:
  ~TimedFunctionCall();
  bool Update();

  static TimedFunctionCall* Create( float a_timeToWaitFor , std::function<void( TimeFunctionCallArgument )> m_funcToCall , TimeFunctionCallArgument a_argument = TimeFunctionCallArgument() , bool a_looping = false );
private:

  TimedFunctionCall( float a_timeToWaitFor , std::function<void( TimeFunctionCallArgument )> a_funcToCall , TimeFunctionCallArgument a_argument = TimeFunctionCallArgument() , bool a_looping = false );
  Timer m_timer;
  float m_timeToWaitFor;
  std::function<void( TimeFunctionCallArgument )> m_funcToCall;
  TimeFunctionCallArgument m_argument;
  bool m_looping;
};
