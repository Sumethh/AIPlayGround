#include "World.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include <Common/log.h>
#include "glm/gtx/rotate_vector.hpp"
#include "Common/HelperFunctions.h"
#include "RigidbodyComponent.h"
#include "Common/Input.h"
#include "DebugValues.h"
#include "ColliderComponent.h"
const int g_tileSizeX = 32; //Tile Size X in pixels
const int g_tileSizeY = 32; //Tile Size Y in pixels

const int g_tileCountX = 100;
const int g_tileCountY = 100;

const glm::vec2 g_gridOrigin( 0 , 0 );

World::World() :
  m_grid( new Grid( g_gridOrigin , g_tileCountX , g_tileCountY , g_tileSizeX , g_tileSizeY ) ) ,
  m_camera( new Camera() ) ,
  m_playerController( new PlayerController( m_grid , m_camera ) ) ,
  m_pathfinder( new Pathfinder( m_grid ) ) ,
  m_physicsSystem( new PhysicsSystem() )
{
  for( unsigned short i = 0; i < (unsigned int)EGameObjectType::GOT_Max; ++i )
    m_gameObjectDescriptors[ (EGameObjectType)i ] = {};


  GameObjectConstructionDescriptor& descriptor = m_gameObjectDescriptors[ EGameObjectType::GOT_Unit ];
  descriptor.listOfComps.push_back( EComponentTypes::CT_RenderComponent );
  descriptor.listOfComps.push_back( EComponentTypes::CT_PathfindingAgentComponent );
  descriptor.listOfComps.push_back( EComponentTypes::CT_WanderingComponent );
  descriptor.listOfComps.push_back( EComponentTypes::CT_ColliderComponent );


  GameObjectConstructionDescriptor& physicsDescriptor = m_gameObjectDescriptors[ EGameObjectType::GOT_PhysicsTest ];
  physicsDescriptor.listOfComps.push_back( EComponentTypes::CT_RenderComponent );
  physicsDescriptor.listOfComps.push_back( EComponentTypes::CT_RigidbodyComponent );
  physicsDescriptor.listOfComps.push_back( EComponentTypes::CT_ColliderComponent );


  m_worldLimits.topLeft = g_gridOrigin;
  m_worldLimits.bottomRight = g_gridOrigin + glm::vec2( g_tileSizeX * g_tileCountX , g_tileSizeY * g_tileCountY );


}

World::~World()
{
}

RendererComponent* comp;
void World::OnConstruct()
{
  if( m_physicsSystem )
  {
    m_physicsSystem->SetWorld( this );
    m_physicsSystem->Construct();
  }
  m_grid->Init();
  glm::vec2 worldSize = m_worldLimits.bottomRight - m_worldLimits.topLeft;
#if 0
  for( int i = 0; i < 2; i++ )
  {
    GameObject* newGO = CreateGameObject( EGameObjectType::GOT_PhysicsTest );
    Transform transform;
    if( i == 0 )
    {
      transform = newGO->GetTransform();
      transform.position = glm::vec2( 400 , 400 );
      transform.rotation = 0.0f;


      auto t = (RigidbodyComponent*)m_gameObjects[ 0 ]->GetComponentOfType( EComponentTypes::CT_RigidbodyComponent );

      t->AddForce( glm::vec2( 10.0f , 0.0f ) );

      auto collider = (ColliderComponent*)newGO->GetComponentOfType( EComponentTypes::CT_ColliderComponent );
      Collider col;
      col.radius = 32.0f / 2.0f;
      collider->SetCollider( col );
      collider->SetColliderType( EColliderType::Sphere );
    }
    else if( i == 1 )
    {
      transform = newGO->GetTransform();
      transform.position = glm::vec2( 500 , 400 );
      transform.rotation = 45.0f;

      auto collider = (ColliderComponent*)newGO->GetComponentOfType( EComponentTypes::CT_ColliderComponent );
      collider->SetColliderType( EColliderType::Box );
      Collider col;
      //col.radius = 32.0f / 2.0f;
      col.extents = glm::vec2( 32.0f , 32.0f );
      collider->SetCollider( col );

    }
    newGO->SetTransfrom( transform );
  }
#endif
#if 1
  for( int i = 0; i < 5; i++ )
  {
    GameObject* newGO = CreateGameObject( EGameObjectType::GOT_Unit );
    Transform transform;
    transform = newGO->GetTransform();
    transform.position = glm::vec2( 384 , 384 );
    transform.rotation = 0.0f;
    newGO->SetTransfrom( transform );
    auto t = (ColliderComponent*)newGO->GetComponentOfType( EComponentTypes::CT_ColliderComponent );
    auto collider = t->GetCollider();
    collider.extents = glm::vec2( 32.0f , 32.0f );
    t->SetCollider( collider );
    t->SetColliderType( EColliderType::Box );
  }
#endif
}

void World::OnDestroyed()
{

}

void World::BeginPlay()
{
  for( auto gameObject : m_gameObjects )
    gameObject->BeginPlay();
}

void World::Update( float a_dt )
{
  m_playerController->Update( a_dt );
  for( auto gameObject : m_gameObjects )
  {
    if( !gameObject->IsDestroyed() )
      gameObject->Update( a_dt );
  }
  Timer t;
  t.Start();
  m_pathfinder->ScheduleJobs();
  if( Input::GetKey( sf::Keyboard::Key::P ) )
  {
    DebugValues::GI()->RenderGrid = !DebugValues::GI()->RenderGrid;
  }

}

void World::FixedUpdate( float a_dt )
{
  m_physicsSystem->Update( a_dt );

}

void World::PreRender()
{
  m_grid->PreRender( m_camera->GetPos() );
  for( auto gameObject : m_gameObjects )
    gameObject->PreRender();
}

glm::vec2 RotatePoint( glm::vec2 vec , float radians )
{
  glm::vec2 newVec;
  float Cos = glm::cos( radians );
  float Sin = glm::sin( radians );
  newVec.x = vec.x * Cos - vec.y * Sin;
  newVec.y = vec.x * Sin + vec.y * Cos;

  return newVec;
}

void World::Render( Window* const a_window )
{

  //

  m_grid->Render( a_window );
  m_playerController->Render( a_window );
 // m_physicsSystem->Render( a_window );
  for( auto gameObject : m_gameObjects )
    gameObject->Render( a_window );
}

void World::PostFrame()
{
  for( auto gameObject : m_gameObjects )
    gameObject->PostFrame();

  for( auto gameObject = m_gameObjects.begin(); gameObject != m_gameObjects.end(); )
  {
    if( ( *gameObject )->IsDestroyed() )
    {
      ( *gameObject )->OnDestroy();

      GameObject* deletingGameObject = *gameObject;
      gameObject = m_gameObjects.erase( gameObject );
      delete deletingGameObject;
    }
    else
      ++gameObject;

  }
}

GameObject* World::CreateGameObject( EGameObjectType a_type )
{
  GameObject* newGameObject = new GameObject( a_type );
  if( newGameObject )
  {
    m_gameObjects.push_back( newGameObject );
    newGameObject->SetWorld( this );
    newGameObject->OnCosntruct( &m_gameObjectDescriptors[ a_type ] );
  }
  return newGameObject;
}

void World::SetGameObjectWorld( GameObject* a_gameobject )
{
  if( a_gameobject )
    a_gameobject->SetWorld( this );
}