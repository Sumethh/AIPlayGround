#include "PlayerController.h"
#include "Common/Input.h"
#include "Common/Window.h"
#include "Common/log.h"
#include "Camera.h"
#include "Renderer2D.h"
#include <glm/gtc/matrix_transform.hpp>
PlayerController::PlayerController( std::shared_ptr<Grid>& a_grid , std::shared_ptr<Camera>& a_camera ) :
  m_grid( a_grid ) ,
  m_camera( a_camera )
{
  if( m_selectionTexture.loadFromFile( "../assets/art/SelectionImage.png" ) )
  {
    m_selectionSprite.setTexture( m_selectionTexture );
  }
}

PlayerController::~PlayerController()
{
}

void PlayerController::Init()
{
}

void PlayerController::Update( float a_dt )
{
  int tileSizeX = 0 , tileSizeY = 0;
  if( !m_grid.expired() && !m_camera.expired() )
  {
    std::shared_ptr<Grid> grid = m_grid.lock();
    std::shared_ptr<Camera> camera = m_camera.lock();

    tileSizeX = grid->GetTileSizeX();
    tileSizeY = grid->GetTileSizeY();

    glm::vec2 mousePos = Input::GetMousePosition();

    sf::Vector2f newSpritePos;
    glm::vec2 camPos = camera->GetPos();
    int indexX = (int)( mousePos.x + camPos.x ) / tileSizeX;
    int indexY = (int)( mousePos.y + camPos.y ) / tileSizeY;
    Node* node = grid->GetNode( indexX , indexY );
    if( node )
    {
      newSpritePos.x = node->pos.x - camPos.x;
      newSpritePos.y = node->pos.y - camPos.y;

      m_selectionSprite.setPosition( newSpritePos );
      if( Input::GetMouseButton( sf::Mouse::Left ) )
        grid->SetNodeTileIndex( indexX , indexY , 0 );

      if( Input::GetMouseButton( sf::Mouse::Right ) )
        grid->SetNodeTileIndex( indexX , indexY , 1 );
    }

    if( Input::GetKey( sf::Keyboard::F10 ) )
      grid->SaveToDisk();
  }

  if( !m_camera.expired() )
  {
    std::shared_ptr<Camera> camera = m_camera.lock();
    if( Input::GetKeyDown( sf::Keyboard::Key::A ) )
      camera->MoveX( -m_moveSpeed*a_dt );
    if( Input::GetKeyDown( sf::Keyboard::Key::D ) )
      camera->MoveX( m_moveSpeed*a_dt );

    if( Input::GetKeyDown( sf::Keyboard::Key::W ) )
      camera->MoveY( -m_moveSpeed*a_dt );

    if( Input::GetKeyDown( sf::Keyboard::Key::S ) )
      camera->MoveY( m_moveSpeed*a_dt );
  }
}

void PlayerController::PreRender()
{
  m_selectionTransform.transformationMatrix = glm::translate( m_selectionTransform.transformationMatrix , glm::vec3(m_selectionTransform.position ,0.0f));
  m_selectionTransform.transformationMatrix = glm::rotate( m_selectionTransform.transformationMatrix , 0.0f , glm::vec3( 0 , 0 , 1 ) );
  m_selectionTransform.transformationMatrix = glm::scale( m_selectionTransform.transformationMatrix , glm::vec3() );
}

void PlayerController::Render( Renderer2D* a_renderer )
{
}