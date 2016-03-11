#include "Game.h"
#include "Common/Window.h"
#include "Common/Input.h"
const int g_tileSizeX = 32; //Tile Size X in pixels
const int g_tileSizeY = 32; //Tile Size Y in pixels

const int g_tileCountX = 100;
const int g_tileCountY = 100;

const glm::vec2 g_gridOrigin( 0 , 0 );

Game::Game() :
  m_grid( g_gridOrigin , g_tileCountX , g_tileCountY , g_tileSizeX , g_tileSizeY ) ,
  m_playerController( &m_grid , &m_camera ) , m_pathfinder( &m_grid )
{
}


Game::~Game()
{
}
Path* p = nullptr;
void Test( Path* a_p )
{
  p = a_p;
}

void Game::Init()
{
  m_grid.Init();
  m_pathfinder.AddPathfindingJob( std::bind( &Test , std::placeholders::_1 ) , glm::vec2( 0 , 0 ) , glm::vec2( 99 * 32 , 99 * 32 ) );
}

void Game::Update( float a_dt )
{
  m_playerController.Update( a_dt );
  if( Input::GetMouseButton( sf::Mouse::Right ) )
    m_pathfinder.AddPathfindingJob( std::bind( &Test , std::placeholders::_1 ) , glm::vec2( 0 , 0 ) , Input::GetMousePosition() );

}
void Game::PreRender()
{
  m_grid.PreRender( m_camera.GetPos() );
}

void Game::Render( Window* const a_window )
{
  m_grid.Render( a_window );
  m_playerController.Render( a_window );
  if( p )
  {
    for( int i = 1; i < p->nodes.size(); i++ )
    {
      sf::Vertex line[] =
      {
        sf::Vertex( sf::Vector2f( p->nodes[ i - 1 ].x + g_tileSizeX / 2, p->nodes[ i - 1 ].y + g_tileSizeY / 2 ) ),
        sf::Vertex( sf::Vector2f( p->nodes[ i ].x + g_tileSizeX / 2, p->nodes[ i ].y + g_tileSizeY / 2 ) )
      };
      a_window->GetWindow()->draw( line , 2 , sf::Lines );
    }
  }
}

void Game::PostFrame()
{

}
