#include "RendererComponent.h"
#include "Common/Window.h"
#include "Transform.h"
#include "Common/HelperFunctions.h"

#include "world.h"

RendererComponent::RendererComponent( GameObject::SharedPtr  a_gameObject , EComponentTypes a_type ) :
  Component( a_gameObject , a_type )
{
}


RendererComponent::~RendererComponent()
{
}


void RendererComponent::PreRender()
{
  Component::PreRender();
  std::shared_ptr<GameObject> parent = GetParentShared();
  if( parent && parent->RenderStateDirty() )
  {
    Transform goTransform = GetParentShared()->GetTransform();
    World* world = GetParentShared()->GetWorld();
    std::weak_ptr<Camera> cam;
    if( world )
      cam = world->GetCamera();
    if( !cam.expired() )
    {
      std::shared_ptr<Camera> camera = cam.lock();
      m_sprite.setPosition( ConvertVec2( goTransform.position - camera->GetPos() ) );
      m_sprite.setScale( ConvertVec2( goTransform.scale ) );
      m_sprite.setRotation( goTransform.rotation );
      parent->ResetRenderStateDirtyFlag();
    }
  }
}

void RendererComponent::Render( Window* a_window )
{
  Component::Render( a_window );
  a_window->RenderDrawable( m_sprite );
}

void RendererComponent::SetTexture( sf::Texture* a_texture )
{
  m_texture = a_texture;
  if( m_texture )
    m_sprite.setTexture( *m_texture , true );
  sf::Vector2u scale = m_texture->getSize();
  m_dimensions = ConvertVec2( scale );
  m_sprite.setOrigin( m_dimensions.x / 2.0f , m_dimensions.y / 2.0f );
}
