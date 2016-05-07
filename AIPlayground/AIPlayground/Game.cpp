#include "Game.h"

Game::Game():
  m_world(new World())
{
  if( m_world )
    m_world->SetWorldShared( m_world );
}


Game::~Game()
{
  m_world->OnDestroyed();
}


void Game::Init()
{
  m_world->OnConstruct();
  m_world->BeginPlay();
}

void Game::FixedUpdate( float a_dt )
{
  m_world->FixedUpdate( a_dt );
}

void Game::Update( float a_dt )
{
  m_world->Update( a_dt );
}
void Game::PreRender()
{
  m_world->PreRender();
}

void Game::Render( Window* const a_window )
{
  m_world->Render( a_window );
}

void Game::PostFrame()
{
  m_world->PostFrame();
}
