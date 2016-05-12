#include <SFML/Graphics.hpp>
#include "Common/Window.h"
#include "Common/Input.h"
#include "Common/Types.h"
#include "Common/timer.h"
#include "Common/log.h"
#include <string.h>
#include "Game.h"
#include "Common/DebugOnScreenTimer.h"
#include <cstdlib>
#include <ctime>
#include "Common/TimedFunctionCallManager.h"
#include "Common/InvokableEvent.h"
#include "Common/JobSystem.h"
#include "Common/JobSystemDebugInfo.h"
#include "TimeConsts.h"

int32 frames;
float currentFPS;

Timer frameTimer , deltaTime , renderTimer , updateTimer , preRenderTimer;
int fpsTimerIndex , dtTimerIndex , renderTimerIndex , updateTimerIndex , preRenderTimerIndex , physicsTimerIndex;
Game game;
int main()
{
  LOGI( "Application starting" );
  std::srand( (uint)std::time( 0 ) );
  DebugOnScreenTimer::Init();
  Window mainWindow( 1280 , 720 , "AiPlayground" );
  std::string baseString( "FPS: " );
  fpsTimerIndex = DebugOnScreenTimer::AddNewTimer( baseString );

  baseString = "DT: ";
  dtTimerIndex = DebugOnScreenTimer::AddNewTimer( baseString );
  DebugOnScreenTimer::SetTimerTrailingText( dtTimerIndex , " s" );

  baseString = "Game::Update: ";
  updateTimerIndex = DebugOnScreenTimer::AddNewTimer( baseString );
  DebugOnScreenTimer::SetTimerTrailingText( updateTimerIndex , " ms" );

  baseString = "Game::PhysicsUpdate: ";
  physicsTimerIndex = DebugOnScreenTimer::AddNewTimer( baseString );
  DebugOnScreenTimer::SetTimerTrailingText( updateTimerIndex , " ms" );



  baseString = "Game::PrRender: ";
  preRenderTimerIndex = DebugOnScreenTimer::AddNewTimer( baseString );
  DebugOnScreenTimer::SetTimerTrailingText( preRenderTimerIndex , " ms" );

  baseString = "Game::Render: ";
  renderTimerIndex = DebugOnScreenTimer::AddNewTimer( baseString );
  DebugOnScreenTimer::SetTimerTrailingText( renderTimerIndex , " ms" );

  Timer initTimer;
  initTimer.Start();
  game.Init();
  LOGI( "Game Init Took: %fms" , initTimer.IntervalMS() );
  frameTimer.Start();
  deltaTime.Start();

  JobSystem::Init( 4 );
  float fixedUpdateAccum = 0.0f;
  while( !mainWindow.IsCloseRequested() )
  {
    float dt = (float)deltaTime.IntervalS();
    fixedUpdateAccum += dt;
    TimeConsts::DeltaTime = dt;
    DebugOnScreenTimer::SetTimerValue( dtTimerIndex , dt );
    deltaTime.Reset();


    mainWindow.Clear( sf::Color::Blue );
    mainWindow.Update();

    if( fixedUpdateAccum >= TimeConsts::fixedUpdateTimeStep )
    {
      fixedUpdateAccum -= TimeConsts::fixedUpdateTimeStep;
      game.FixedUpdate( TimeConsts::fixedUpdateTimeStep );
    }

    updateTimer.Start();
    game.Update( dt );
    TimedFunctionCallManager::GI()->Update();
    DebugOnScreenTimer::SetTimerValue( updateTimerIndex , (float)updateTimer.IntervalMS() );
    DebugOnScreenTimer::SetTimerValue( physicsTimerIndex , PhysicsSystem::time );


    preRenderTimer.Start();
    game.PreRender();
    DebugOnScreenTimer::SetTimerValue( preRenderTimerIndex , (float)preRenderTimer.IntervalMS() );

    renderTimer.Start();
    game.Render( &mainWindow );
    DebugOnScreenTimer::SetTimerValue( renderTimerIndex , (float)renderTimer.IntervalMS() );



    if( frameTimer.IntervalMS() >= 1000.0f )
    {
      currentFPS = (float)( frames / ( frameTimer.IntervalMS() / 1000.0f ) );
      frames = 0;
      DebugOnScreenTimer::SetTimerValue( fpsTimerIndex , currentFPS );
      frameTimer.Reset();
    }
    frames++;


    DebugOnScreenTimer::DrawTimers( &mainWindow );
    JobSystemDebugInfo::GI()->Render( &mainWindow );

    if( Input::GetKey( sf::Keyboard::Key::Escape ) )
      mainWindow.Close();


    mainWindow.Swap();
    game.PostFrame();
    Input::Reset();
  }

  JobSystem::UnInit();

  return 0;
}
