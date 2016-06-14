#include "JobSystemDebugInfo.h"
#include <SFML/Graphics/Text.hpp>
#include <string.h>
#include "Window.h"
#include "imgui.h"
#include <sstream>
JobSystemDebugInfo* JobSystemDebugInfo::m_instance;

void JobSystemDebugInfo::Init()
{
  m_font.loadFromFile("../Assets/Fonts/KenPixel.ttf");
  m_functionTimerHandle = TimedFunctionCall::Create(1.0f, std::bind(&JobSystemDebugInfo::SetDebugInfo, m_instance, std::placeholders::_1), TimeFunctionCallArgument(), true);
}

std::shared_ptr<DebugThreadInfo> JobSystemDebugInfo::registerThread()
{
  m_threadInfos.push_back(std::shared_ptr<DebugThreadInfo>(new DebugThreadInfo));
  std::memset((m_threadInfos.end() - 1)->get(), 0, sizeof(DebugThreadInfo) - sizeof(std::mutex));
  return *(m_threadInfos.end() - 1);
}

void JobSystemDebugInfo::Render()
{
  if (!m_threadInfoWindowOpen)
    ImGui::SetNextWindowCollapsed(true);
  m_threadInfoWindowOpen = ImGui::Begin("Threads info", nullptr, ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
  std::string threadText;
  for (auto threadInfo : m_threadInfos)
  {
    threadText = "Thread ID: ";
    //get the info and copy it over to reduce the amount of time that we have the lock acquired
    threadInfo->infoLock.lock();
    threadText.append(std::to_string(threadInfo->threadID));
    ImGui::Text(threadText.c_str());

    threadText = "Average Job Time: ";
    threadText.append(std::to_string(threadInfo->jobTimeAverage));
    ImGui::Text(threadText.c_str());
    threadText = "Average jobs Completed: ";
    threadText.append(std::to_string(threadInfo->jobsCompletedLast5Seconds));
    ImGui::Text(threadText.c_str());

    ImGui::Spacing();

    threadInfo->infoLock.unlock();
  }
  ImGui::End();
}

void JobSystemDebugInfo::SetTextStrings(sf::Text& a_threadID, sf::Text& a_jobTimeAvg, sf::Text& a_jobsCompletedText, uint32& a_id, float& a_time, uint32& a_jobsCompleted)
{
  std::string infoString;
  infoString = "Thread ID: ";
  infoString.append(std::to_string(a_id));
  a_threadID.setString(infoString);

  infoString = "Avg time on Job (ms): ";
  infoString.append(std::to_string(a_time));
  a_jobsCompletedText.setString(infoString);

  infoString = "Jobs Completed (p/s): ";
  infoString.append(std::to_string(a_jobsCompleted));
  a_jobTimeAvg.setString(infoString);
}

void JobSystemDebugInfo::SetDebugInfo(TimeFunctionCallArgument a_argument)
{
  void* data = &a_argument;
  for (auto threadInfo : m_threadInfos)
  {
    threadInfo->infoLock.lock();
    if (data) {}
    threadInfo->jobsCompletedLast5Seconds = threadInfo->jobsCompleted;
    threadInfo->jobsCompleted = 0;

    if (threadInfo->jobTimeSamplesCount)
      threadInfo->jobTimeAverage = (float)threadInfo->jobTimeTotal / threadInfo->jobTimeSamplesCount;
    else
      threadInfo->jobTimeAverage = 0.0f;
    threadInfo->jobTimeTotal = 0;
    threadInfo->jobTimeSamplesCount = 0;

    threadInfo->infoLock.unlock();
  }
}