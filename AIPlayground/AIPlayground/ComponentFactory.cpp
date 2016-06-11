#include "ComponentFactory.h"
#include "Component.h"
#include "RendererComponent.h"
#include "PathfindingAgentComponent.h"
#include "WanderingComponent.h"
#include "RigidbodyComponent.h"
#include "ColliderComponent.h"
#include <SFML/Graphics/Text.hpp>
ComponentFactory* ComponentFactory::m_instance;
sf::Texture t;

Component* MakeRenderComponent(GameObject*   a_go )
{
  RendererComponent* comp = new RendererComponent( a_go , EComponentTypes::CT_RenderComponent );
  if( comp )
    comp->SetTexture( &t );
  return comp;
}

Component* MakePathfindingAgentComponent( GameObject* a_go )
{
  return new PathfindingAgentComponent( a_go , EComponentTypes::CT_PathfindingAgentComponent );
}

Component* MakeWanderingComponent(GameObject* a_go )
{
  return new WanderingComponent( a_go , EComponentTypes::CT_WanderingComponent );
}

Component* MakeRigidbodyComponent(GameObject* a_go )
{
  return new RigidbodyComponent( a_go , EComponentTypes::CT_RigidbodyComponent );
}

Component* MakeColliderComponent(GameObject* a_go )
{
  return new ColliderComponent( a_go , EComponentTypes::CT_ColliderComponent , EColliderType::Sphere );
}

ComponentFactory::ComponentFactory()
{
  t.loadFromFile( "../assets/art/unit1.png" );

  m_functionMap[ EComponentTypes::CT_RenderComponent ] = &MakeRenderComponent;
  m_functionMap[ EComponentTypes::CT_PathfindingAgentComponent ] = &MakePathfindingAgentComponent;
  m_functionMap[ EComponentTypes::CT_WanderingComponent ] = &MakeWanderingComponent;
  m_functionMap[ EComponentTypes::CT_RigidbodyComponent ] = &MakeRigidbodyComponent;
  m_functionMap[ EComponentTypes::CT_ColliderComponent ] = &MakeColliderComponent;
}

ComponentFactory::~ComponentFactory()
{
}

Component* ComponentFactory::MakeComponent( EComponentTypes a_component , GameObject* a_gameObject )
{
  Component* newComponent = nullptr;
  if( m_instance->m_functionMap[ a_component ] )
    newComponent = m_functionMap[ a_component ]( a_gameObject );
  return newComponent;
}