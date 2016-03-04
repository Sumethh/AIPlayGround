#include "PlayerController.h"
#include "Common/Input.h"
#include "Common/Window.h"
#include "Common/log.h"
#include "Camera.h"



PlayerController::PlayerController( Grid* const a_grid , Camera* const a_camera ) :
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
  if( m_grid && m_camera )
  {
    tileSizeX = m_grid->GetTileSizeX();
    tileSizeY = m_grid->GetTileSizeY();

    glm::vec2 mousePos = Input::GetMousePosition();

    sf::Vector2f newSpritePos;
    glm::vec2 camPos = m_camera->GetPos();
    int indexX = (int)( mousePos.x + camPos.x ) / tileSizeX;
    int indexY = (int)( mousePos.y + camPos.y ) / tileSizeY;
    Node* node = m_grid->GetNode( indexX , indexY );
    if( node )
    {
      newSpritePos.x = node->pos.x - camPos.x;
      newSpritePos.y = node->pos.y - camPos.y;

      m_selectionSprite.setPosition( newSpritePos );
      if( Input::GetMouseButton( sf::Mouse::Left ) )
        m_grid->SetNodeTileIndex( indexX , indexY , 0 );

      if( Input::GetMouseButton( sf::Mouse::Right ) )
        m_grid->SetNodeTileIndex( indexX , indexY , 1 );
    }

    if( Input::GetKey( sf::Keyboard::F10 ) )
      m_grid->SaveToDisk();
  }

  if( m_camera )
  {
    if( Input::GetKeyDown( sf::Keyboard::Key::A ) )
      m_camera->MoveX( -m_moveSpeed*a_dt );
    if( Input::GetKeyDown( sf::Keyboard::Key::D ) )
      m_camera->MoveX( m_moveSpeed*a_dt );

    if( Input::GetKeyDown( sf::Keyboard::Key::W ) )
      m_camera->MoveY( -m_moveSpeed*a_dt );

    if( Input::GetKeyDown( sf::Keyboard::Key::S ) )
      m_camera->MoveY( m_moveSpeed*a_dt );
  }
}

void PlayerController::PreRender()
{

}

void PlayerController::Render( Window* const a_window )
{
  a_window->RenderDrawable( m_selectionSprite );
}
