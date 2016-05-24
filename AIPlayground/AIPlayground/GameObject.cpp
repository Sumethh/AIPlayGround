#include "GameObject.h"
#include "Component.h"
#include <glm/gtc/matrix_transform.hpp>

typedef std::vector<std::shared_ptr<Component>>::iterator ComponentItr;

GameObject::GameObject( EGameObjectType a_type ) :
  m_goType( a_type ) ,
  m_hasBegunPlay( false ) ,
  m_toBeDestroyed( false ) ,
  m_world( nullptr ) ,
  m_thisSharedPtr( this ) ,
  m_rotationMatrixDirty( true )
{
  SetScale( glm::vec2( 1.0f , 1.0f ) );
  SetRotation( 0 );
  SetPosition( glm::vec2( 0.0f , 0.0f ) );
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
  /*if( m_rotationMatrixDirty )
  {
    glm::mat2 rMatrix = m_transform.rotationMatrix;
    float cosResult = glm::cos( glm::radians( m_transform.rotation ) );
    float sinResult = glm::sin( glm::radians( m_transform.rotation ) );
    rMatrix[ 0 ][ 0 ] = cosResult;
    rMatrix[ 0 ][ 1 ] = -sinResult;
    rMatrix[ 1 ][ 0 ] = sinResult;
    rMatrix[ 1 ][ 1 ] = cosResult;
  }*/
}

void GameObject::FixedUpdate( float a_dt )
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->FixedUpdate( a_dt );
}

void GameObject::PreRender()
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->PreRender();
}

void GameObject::Render( Renderer2D* a_window )
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
    std::shared_ptr<Component> newComp = m_componentsToAdd[ i ];
    m_components.push_back( newComp );
    if( HasBegunPlay() && !newComp->HasBegunPlay() )
      newComp->BeginPlay();
  }
  m_componentsToAdd.clear();
}

void GameObject::OnCollisionEnter( Collision a_collision )
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnCollisionEnter( a_collision );
}

void GameObject::OnCollisionLeave( Collision a_collision )
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnCollisionLeave( a_collision );
}

void GameObject::OnCollisionStay( Collision a_collision )
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnCollisionStay( a_collision );
}

void GameObject::AddComponent( EComponentTypes a_componentType )
{
  std::shared_ptr<Component> newComp( ComponentFactory::GI()->MakeComponent( a_componentType , m_thisSharedPtr ) );
  if( !nullptr )
    m_componentsToAdd.push_back( newComp );
}

Component* GameObject::GetComponentOfType( EComponentTypes a_type )
{
  for( ComponentItr itr = m_components.begin(); itr != m_components.end(); ++itr )
    if( ( *itr )->IsComponentOfType( a_type ) )
      return ( itr )->get();
  return nullptr;
}