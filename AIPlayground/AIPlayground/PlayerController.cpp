#include "PlayerController.h"
#include "Common/Input.h"
#include "Common/Window.h"
#include "Common/log.h"
#include "Camera.h"
#include "Renderer2D.h"
#include <glm/gtc/matrix_transform.hpp>
PlayerController::PlayerController( Grid* a_grid , Camera* a_camera ) :
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