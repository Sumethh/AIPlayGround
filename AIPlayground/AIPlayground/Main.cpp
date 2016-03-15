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

int32 frames;
float currentFPS;

Timer frameTimer , deltaTime , renderTimer , updateTimer , preRenderTimer;
int fpsTimerIndex , dtTimerIndex , renderTimerIndex , updateTimerIndex , preRenderTimerIndex;
Game game;

int main()
{
  std::srand( std::time(0) );
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
  while( !mainWindow.IsCloseRequested() )
  {
    float dt = (float)deltaTime.IntervalMS() / 1000.0f;
    DebugOnScreenTimer::SetTimerValue( dtTimerIndex , dt );
    deltaTime.Reset();


    mainWindow.Clear( sf::Color::Blue );
    mainWindow.Update();

    updateTimer.Start();
    game.Update( dt );
    DebugOnScreenTimer::SetTimerValue( updateTimerIndex , (float)updateTimer.IntervalMS() );

    preRenderTimer.Start();
    game.PreRender();
    DebugOnScreenTimer::SetTimerValue( preRenderTimerIndex, (float)preRenderTimer.IntervalMS() );

    renderTimer.Start();
    game.Render( &mainWindow );
    DebugOnScreenTimer::SetTimerValue( renderTimerIndex , (float)renderTimer.IntervalMS() );



    if( frameTimer.IntervalMS() >= 100.0f )
    {
      currentFPS = (float)( frames / ( frameTimer.IntervalMS() / 1000.0f ) );
      frames = 0;
      DebugOnScreenTimer::SetTimerValue( fpsTimerIndex , currentFPS );
      frameTimer.Reset();
    }
    frames++;


    DebugOnScreenTimer::DrawTimers( &mainWindow );
    if( Input::GetKey( sf::Keyboard::Key::Escape ) )
      mainWindow.Close();


    mainWindow.Swap();
    game.PostFrame();
    Input::Reset();
  }

  return 0;
}