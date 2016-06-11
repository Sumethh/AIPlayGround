#include "RendererComponent.h"
#include "Common/Window.h"
#include "Transform.h"
#include "Common/HelperFunctions.h"
#include "Renderer2D.h"
#include "world.h"

RendererComponent::RendererComponent( GameObject*   a_gameObject , EComponentTypes a_type ) :
  Component( a_gameObject , a_type )
{
}

RendererComponent::~RendererComponent()
{
}

void RendererComponent::PreRender()
{
  Component::PreRender();
  GameObject* parent = GetParent();
  if( parent && parent->GetRenderStateDirty() )
  {
    Transform goTransform = parent->GetTransform();
    World* world = parent->GetWorld();
    Camera* cam = nullptr;
    if( world )
      cam = world->GetCamera();
    if( cam )
    {
      m_sprite.setPosition( ConvertVec2( goTransform.position - cam->GetPos() ) );
      m_sprite.setScale( ConvertVec2( goTransform.scale ) );
      m_sprite.setRotation( goTransform.rotation );
      parent->ResetRenderStateDirtyFlag();
    }
  }
}

void RendererComponent::Render( Renderer2D* a_renderer)
{
  SpriteBatchRenderer& spriteRender = a_renderer->GetSpriteBatchRenderer();
  World* world = GetParent()->GetWorld();
  Camera* camera = world->GetCamera();
  Transform trans = GetParent()->GetTransform();
  ELayerID layer = GetParent()->GetLayer();
  spriteRender.Submit( glm::vec3(trans.position - camera->GetPos() , (uint8)layer / 10.0f) , trans.scale * 32.0f , trans.rotation , glm::vec4( 0 , 0 , 1 , 0 ) , glm::vec4( 0 , 1 , 1 , 1 ) );
  
}

void RendererComponent::SetTexture( sf::Texture* a_texture )
{
  m_texture = a_texture;
  if( m_texture )
  {
    m_sprite.setTexture( *m_texture , true );
    sf::Vector2u scale = m_texture->getSize();
    m_dimensions = ConvertVec2( scale );
    m_sprite.setOrigin( m_dimensions.x / 2.0f , m_dimensions.y / 2.0f );
  }
}