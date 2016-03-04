#include "Game.h"
#include "Common/Window.h"

const int g_tileSizeX = 32; //Tile Size X in pixels
const int g_tileSizeY = 32; //Tile Size Y in pixels

const int g_tileCountX = 100;
const int g_tileCountY = 100;

const glm::vec2 g_gridOrigin( 0 , 0 );

Game::Game() :
  m_grid( g_gridOrigin , g_tileCountX , g_tileCountY , g_tileSizeX , g_tileSizeY ) ,
  m_playerController( &m_grid , &m_camera )
{
}


Game::~Game()
{
}

void Game::Init()
{
  m_grid.Init();
}

void Game::Update( float a_dt )
{
  m_playerController.Update( a_dt );
}

void Game::PreRender()
{
  m_grid.PreRender( m_camera.GetPos() );
}

void Game::Render( Window* const a_window )
{
  m_grid.Render( a_window );
  m_playerController.Render( a_window );
}
