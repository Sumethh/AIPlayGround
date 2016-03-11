#include "ComponentFactory.h"
#include "Component.h"
ComponentFactory* ComponentFactory::m_instance;


ComponentFactory::ComponentFactory()
{
}


ComponentFactory::~ComponentFactory()
{
}

Component* ComponentFactory::MakeComponent( EComponentTypes a_component , GameObject* a_gameObject )
{
  Component* newComponent = nullptr;
  if( m_instance->m_functionMap[ a_component ] )
    newComponent = m_functionMap[ a_component ]( a_gameObject );
  if( newComponent )
    newComponent->OnCosntruct();
  return newComponent;
}
