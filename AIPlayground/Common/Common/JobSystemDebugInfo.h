#pragma once
#include "Types.h"
#include "TimedFunctionCall.h"

#include <memory.h>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <mutex>

struct DebugThreadInfo
{
  uint32 threadID;

  double jobTimeTotal;
  uint32 jobTimeSamplesCount;
  float jobTimeAverage;

  uint32 jobsCompleted;
  uint32 jobsCompletedLast5Seconds;

  std::mutex infoLock;
};
class Window;
class JobSystemDebugInfo
{
public:
  void Init();

  std::shared_ptr<DebugThreadInfo> registerThread();

  static inline JobSystemDebugInfo* GI()
  {
    if (!m_instance)
    {
      m_instance = new JobSystemDebugInfo();
      m_instance->Init();
    }
    return m_instance;
  }

  void Render();
private:
  JobSystemDebugInfo() {};

  std::vector<std::shared_ptr<DebugThreadInfo>> m_threadInfos;
  sf::Font m_font;

  static JobSystemDebugInfo* m_instance;

  void SetTextStrings(sf::Text& a_threadID, sf::Text& a_jobTimeAvg, sf::Text& a_jobsCompletedText, uint32& a_id, float& a_time, uint32& a_jobsCompleted);

  void SetDebugInfo(TimeFunctionCallArgument a_argument);

  TimedFunctionCall* m_functionTimerHandle;
  bool m_threadInfoWindowOpen;
};