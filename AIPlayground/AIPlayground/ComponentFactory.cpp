#include "ComponentFactory.h"
#include "Component.h"
#include "RendererComponent.h"
#include "PathfindingAgentComponent.h"
#include "WanderingComponent.h"
#include <SFML/Graphics/Text.hpp>
ComponentFactory* ComponentFactory::m_instance;
sf::Texture t;


Component* MakeRenderComponent( std::weak_ptr<GameObject> a_go )
{
  RendererComponent* comp = new RendererComponent( a_go , EComponentTypes::CT_RenderComponent );
  if( comp )
    comp->SetTexture( &t );
  return comp;
}

Component* MakePathfindingAgentComponent( std::weak_ptr<GameObject> a_go )
{
  return new PathfindingAgentComponent( a_go , EComponentTypes::CT_PathfindingAgentComponent );
}

Component* MakeWanderingComponent( std::weak_ptr<GameObject>  a_go )
{
  return new WanderingComponent( a_go , EComponentTypes::CT_WanderingComponent );
}


ComponentFactory::ComponentFactory()
{
  t.loadFromFile( "../assets/art/unit1.png" );

  m_functionMap[ EComponentTypes::CT_RenderComponent ] = &MakeRenderComponent;
  m_functionMap[ EComponentTypes::CT_PathfindingAgentComponent ] = &MakePathfindingAgentComponent;
  m_functionMap[ EComponentTypes::CT_WanderingComponent ] = &MakeWanderingComponent;


}


ComponentFactory::~ComponentFactory()
{
}

Component* ComponentFactory::MakeComponent( EComponentTypes a_component , std::weak_ptr<GameObject> a_gameObject )
{
  Component* newComponent = nullptr;
  if( m_instance->m_functionMap[ a_component ] )
    newComponent = m_functionMap[ a_component ]( a_gameObject );
  return newComponent;
}
