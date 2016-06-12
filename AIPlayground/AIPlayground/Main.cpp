#include <SFML/Graphics.hpp>
#include "Common/Window.h"
#include "Common/Input.h"
#include "Common/Types.h"
#include "Common/timer.h"
#include "Common/log.h"
#include <string.h>
#include "Game.h"
#include "PhysicsSystem.h"
#include "Common/DebugOnScreenTimer.h"
#include <cstdlib>
#include <ctime>
#include "Common/TimedFunctionCallManager.h"
#include "Common/InvokableEvent.h"
#include "Common/JobSystem.h"
#include "Common/JobSystemDebugInfo.h"
#include "TimeConsts.h"
#include <GL/glew.h>
#include <Common/imgui.h>
#include "ImGuiImplementation.h"

int32 frames;
float currentFPS;
Timer frameTimer, deltaTime, renderTimer, updateTimer, preRenderTimer;
int fpsTimerIndex, dtTimerIndex, renderTimerIndex, updateTimerIndex, preRenderTimerIndex, physicsTimerIndex;
Game game;
int main()
{
  LOGI("Application starting");
  std::srand((uint)std::time(0));
  Window mainWindow(1280, 720, "AiPlayground");
  mainWindow.GetWindow()->setActive();
  mainWindow.GetWindow()->setPosition(sf::Vector2i(600, 0));
  if (glewInit() != GLEW_OK)
  {
    LOGE("Glew failed to initiate");
  }

  glViewport(0, 0, 1280, 720);
  ImGui_Init(&mainWindow);
  //ImGui_ImplGlfwGL3_CreateDeviceObjects();
  std::string baseString("FPS: ");
  fpsTimerIndex = DebugOnScreenTimer::AddNewTimer(baseString);

  baseString = "DT: ";
  dtTimerIndex = DebugOnScreenTimer::AddNewTimer(baseString);
  DebugOnScreenTimer::SetTimerTrailingText(dtTimerIndex, " s");

  baseString = "Game::Update: ";
  updateTimerIndex = DebugOnScreenTimer::AddNewTimer(baseString);
  DebugOnScreenTimer::SetTimerTrailingText(updateTimerIndex, " ms");

  baseString = "Game::PhysicsUpdate: ";
  physicsTimerIndex = DebugOnScreenTimer::AddNewTimer(baseString);
  DebugOnScreenTimer::SetTimerTrailingText(updateTimerIndex, " ms");

  baseString = "Game::PrRender: ";
  preRenderTimerIndex = DebugOnScreenTimer::AddNewTimer(baseString);
  DebugOnScreenTimer::SetTimerTrailingText(preRenderTimerIndex, " ms");

  baseString = "Game::Render: ";
  renderTimerIndex = DebugOnScreenTimer::AddNewTimer(baseString);
  DebugOnScreenTimer::SetTimerTrailingText(renderTimerIndex, " ms");

  Timer initTimer;
  initTimer.Start();
  game.Init();
  LOGI("Game Init Took: %fms", initTimer.IntervalMS());
  frameTimer.Start();
  deltaTime.Start();

  JobSystem::Init(4);
  float fixedUpdateAccum = 0.0f;
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  float fpsOverTime[60] = { 0 };
  bool debugGraphOpen = true;
  int currentFpsIndex = 0;
  while (!mainWindow.IsCloseRequested())
  {
    float dt = (float)deltaTime.IntervalS();
    if (dt > 1.0f)
      dt = 1.0f;
    fixedUpdateAccum += dt;
    TimeConsts::DeltaTime = dt;
    DebugOnScreenTimer::SetTimerValue(dtTimerIndex, dt);
    deltaTime.Reset();

    glClear(GL_COLOR_BUFFER_BIT);
    mainWindow.Update();

    if (fixedUpdateAccum >= TimeConsts::fixedUpdateTimeStep)
    {
      fixedUpdateAccum -= TimeConsts::fixedUpdateTimeStep;
      game.FixedUpdate(TimeConsts::fixedUpdateTimeStep);
      mainWindow.Update();
    }
    ImGui_NewFrame(dt);
    updateTimer.Start();
    if (!debugGraphOpen)
      ImGui::SetNextWindowCollapsed(true);
    debugGraphOpen = ImGui::Begin("Debug Graphs", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PlotHistogram("Fps last 60 seconds", fpsOverTime, 60, 0, nullptr, 0.0f, 1000.0f, ImVec2(0, 100));
    ImGui::End();
    game.Update(dt);
    TimedFunctionCallManager::GI()->Update();
    DebugOnScreenTimer::SetTimerValue(updateTimerIndex, (float)updateTimer.IntervalMS());
    DebugOnScreenTimer::SetTimerValue(physicsTimerIndex, PhysicsSystem::time);
    preRenderTimer.Start();
    game.PreRender();
    DebugOnScreenTimer::SetTimerValue(preRenderTimerIndex, (float)preRenderTimer.IntervalMS());
    renderTimer.Start();
    game.Render();
    DebugOnScreenTimer::SetTimerValue(renderTimerIndex, (float)renderTimer.IntervalMS());
    JobSystemDebugInfo::GI()->Render();
    if (frameTimer.IntervalMS() >= 1000.0f)
    {
      currentFPS = (float)(frames / (frameTimer.IntervalMS() / 1000.0f));
      frames = 0;
      DebugOnScreenTimer::SetTimerValue(fpsTimerIndex, currentFPS);
      fpsOverTime[currentFpsIndex++] = currentFPS;
      if (currentFpsIndex == 60)
        currentFpsIndex = 0;
      frameTimer.Reset();
    }
    frames++;
    DebugOnScreenTimer::DrawTimers();
    ImGui::Render();

    if (Input::GetKey(sf::Keyboard::Key::Escape))
      mainWindow.Close();
    mainWindow.Swap();
    game.PostFrame();
    Input::Reset();
  }

  JobSystem::UnInit();

  return 0;
}