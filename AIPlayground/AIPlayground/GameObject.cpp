#include "GameObject.h"
#include "Component.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/rotate_vector.hpp"
typedef std::vector<std::shared_ptr<Component>>::iterator ComponentItr;

GameObject::GameObject( EGameObjectType a_type ) :
  m_goType( a_type ) ,
  m_hasBegunPlay( false ) ,
  m_toBeDestroyed( false ) ,
  m_world( nullptr ) ,
  m_rotationMatrixDirty( true ) ,
  m_currentLayer( ELayerID::DynamicObject )
{
  SetScale( glm::vec2( 1.0f , 1.0f ) );
  SetRotation( 0 );
  SetPosition( glm::vec2( 0.0f , 0.0f ) );
}

GameObject::~GameObject()
{
  for (auto itr = m_components.begin(); itr != m_components.end();)
  {
    delete* itr;
    itr = m_components.erase(itr);
  }
  for (auto itr = m_componentsToAdd.begin(); itr != m_componentsToAdd.end();)
  {
    delete* itr;
    itr = m_components.erase(itr);
  }
}

void GameObject::OnCosntruct( GameObjectConstructionDescriptor* a_constructionDescriptor )
{
  if( a_constructionDescriptor )
  {
    for( auto i : a_constructionDescriptor->listOfComps )
      AddComponent( i );
  }
  PostFrame();
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnCosntruct();
}

void GameObject::OnDestroy()
{
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnDestroy();
}

void GameObject::BeginPlay()
{
  m_hasBegunPlay = true;
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->BeginPlay();
}

void GameObject::Update( float a_dt )
{
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
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
  if( GetRotationFlagDirty() )
  {
    ResetRotationDityFlag();
    glm::vec2 right( 0 , 1 );
    glm::vec2 forward( 1 , 0 );

    float angle = -m_transform.rotation;
    float cos = glm::cos( angle );
    float sin = glm::sin( angle );

    glm::vec2 newVec;
    newVec.x = forward.x * cos - forward.y * sin;
    newVec.y = forward.x * sin + forward.y * cos;
    forward = newVec;
    newVec.x = right.x * cos - right.y * sin;
    newVec.y = right.x * sin + right.y * cos;
    right = newVec;

    m_transform.forward = glm::normalize(forward);
    m_transform.right = glm::normalize( right);
  }
}

void GameObject::FixedUpdate( float a_dt )
{
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->FixedUpdate( a_dt );
}

void GameObject::PreRender()
{
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->PreRender();
}
#include "Renderer2D.h"
#include "World.h"
void GameObject::Render( Renderer2D* a_renderer )
{
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->Render( a_renderer );
}

void GameObject::PostFrame()
{
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
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

void GameObject::OnCollisionEnter( Collision a_collision )
{
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnCollisionEnter( a_collision );
}

void GameObject::OnCollisionLeave( Collision a_collision )
{
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnCollisionLeave( a_collision );
}

void GameObject::OnCollisionStay( Collision a_collision )
{
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
    ( *itr )->OnCollisionStay( a_collision );
}

void GameObject::AddComponent( EComponentTypes a_componentType )
{
  Component* newComp( ComponentFactory::GI()->MakeComponent( a_componentType , this ) );
  if( !nullptr )
    m_componentsToAdd.push_back( newComp );
}

Component* GameObject::GetComponentOfType( EComponentTypes a_type )
{
  for( auto itr = m_components.begin(); itr != m_components.end(); ++itr )
    if( ( *itr )->IsComponentOfType( a_type ) )
      return ( *itr );
  return nullptr;
}