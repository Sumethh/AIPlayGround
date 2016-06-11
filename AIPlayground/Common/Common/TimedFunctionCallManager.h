#pragma once
#include "TimedFunctionCall.h"
#include <vector>
class TimedFunctionCallManager
{
public:
  TimedFunctionCallManager();
  ~TimedFunctionCallManager();

  static TimedFunctionCallManager* GI();

  void Update();
  bool RemoveTimer( TimedFunctionCall* a_timer );
  bool AddTimer( TimedFunctionCall* a_timer );
private:
  std::vector<TimedFunctionCall*> m_timers;
  static TimedFunctionCallManager* m_instance;
};
