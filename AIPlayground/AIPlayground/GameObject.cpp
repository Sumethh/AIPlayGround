#include "GameObject.h"
#include "Component.h"

typedef std::vector<Component*>::iterator ComponentItr;


GameObject::GameObject( EGameObjectType a_type ) :
  m_goType( a_type ) ,
  m_hasBegunPlay( false ) ,
  m_toBeDestroyed( false ) ,
  m_world( nullptr ) ,
  m_thisSharedPtr( this )
{
}


GameObject::~GameObject()
{
}

void GameObject::OnCosntruct( GameObjectConstructionDescriptor* a_constructionDescriptor )
{
  if( a_constructionDescriptor )
  {
    for( auto i : a_constructionDescriptor->listOfComps )
      AddComponent( i );
  }
  PostFrame();
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnCosntruct();
}

void GameObject::OnDestroy()
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnDestroy();
}

void GameObject::BeginPlay()
{
  m_hasBegunPlay = true;
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->BeginPlay();
}

void GameObject::Update( float a_dt )
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->Update( a_dt );
}

void GameObject::PreRender()
{

  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->PreRender();
}

void GameObject::Render( Window* a_window )
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->Render( a_window );
}

void GameObject::PostFrame()
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->PostFrame();

  for( size_t i = 0; i < m_componentsToAdd.size(); ++i )
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
  Component* newComp = ComponentFactory::GI()->MakeComponent( a_componentType , m_thisSharedPtr );
  if( !nullptr )
    m_componentsToAdd.push_back( newComp );

}


Component* GameObject::GetComponentOfType( EComponentTypes a_type )
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    if( ( *itr )->IsComponentOfType( a_type ) )
      return ( *itr );
  return nullptr;
}