#include "GameObject.h"
#include "Component.h"

typedef std::vector<Component*>::iterator ComponentItr;


GameObject::GameObject() :
  m_hasBegunPlay( false ) ,
  m_toBeDestroyed( false ) ,
  m_world( nullptr )
{
}


GameObject::~GameObject()
{
}
void GameObject::OnCosntruct()
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnCosntruct();
}

void GameObject::OnDestroy()
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnDestroy();

  m_toBeDestroyed = true;
}

void GameObject::BeginPlay()
{
  m_hasBegunPlay = true;
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->BeginPlay();
}

void GameObject::Update()
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->Update();
}

void GameObject::PreRender()
{

  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->PreRender();
}

void GameObject::Render()
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->Render();
}

void GameObject::PostFrame()
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->PostFrame();

  for( int i = 0; i < m_componentsToAdd.size(); ++i )
  {
    Component* newComp = m_componentsToAdd[ i ];
    m_components.push_back( newComp );
    if( HasBegunPlay() && !newComp->HasBegunPlay() )
      newComp->BeginPlay();
  }
  m_componentsToAdd.clear();
}

void GameObject::AddComponent( EComponentTypes a_componentType )
{
  Component* newComp = ComponentFactory::GI()->MakeComponent( a_componentType , this );
  if( !nullptr )
    m_componentsToAdd.push_back( newComp );


}
