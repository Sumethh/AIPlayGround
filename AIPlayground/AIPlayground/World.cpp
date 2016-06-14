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
#include "PlayerController.h"
#include "PhysicsSystem.h"
#include "Pathfinder.h"
#include "Camera.h"
#include "Grid.h"

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
  m_playerController->SetWorld(this);
  AddGoDescriptors();
  m_worldInfo.topLeft = g_gridOrigin;
  m_worldInfo.bottomRight = g_gridOrigin + glm::vec2( g_tileSizeX * g_tileCountX , g_tileSizeY * g_tileCountY );
  m_worldInfo.tileSize = glm::vec2(g_tileSizeX, g_tileSizeY);
}

World::~World()
{
  delete m_grid;
  delete m_camera;
  delete m_playerController;
  delete m_pathfinder;
  delete m_physicsSystem;
  for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); )
  {
    delete *it;
    it = m_gameObjects.erase(it);
  }
}

RendererComponent* comp;
#include <fstream>
void World::OnConstruct()
{
  m_begunPlay = false;
  if (m_physicsSystem)
  {
    m_physicsSystem->SetWorld(this);
    m_physicsSystem->Construct();
  }
  m_grid->Init();
  GameObject* go = CreateGameObject(EGameObjectType::GOT_Unit);
  go->SetPosition(glm::vec2(400, 400));

  json savedData;
  go->OnSave(savedData);

  std::string data = savedData.dump(2);
  std::ofstream outFile("../Assets/Saves/Test.json");
  outFile << data;
  outFile.close();
}

void World::OnDestroyed()
{
}

void World::BeginPlay()
{
  m_playerController->Init();
  for( auto gameObject : m_gameObjects )
    gameObject->BeginPlay();
  m_begunPlay = true;
}
#include "Common/imgui.h"
#include <string>
void World::Update( float a_dt )
{
  ImGui::Begin("World Info");
  ImGui::Text("GO count");
  ImGui::Text(std::to_string(m_gameObjects.size()).c_str());
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
  ImGui::End();
}

void World::FixedUpdate( float a_dt )
{
  m_physicsSystem->Update( a_dt );
}

void World::PreRender()
{
  m_grid->PreRender(m_camera->GetPos());
  for (auto gameObject : m_gameObjects)
    gameObject->PreRender();
}

void World::Render(Renderer2D* a_renderer)
{
  //

   m_grid->Render(a_renderer);
  m_playerController->Render(a_renderer);
   m_physicsSystem->Render( a_renderer );
  for( auto gameObject : m_gameObjects )
    gameObject->Render(a_renderer);
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
    if (m_begunPlay)
      newGameObject->BeginPlay();
  }
  return newGameObject;
}


std::vector<GameObject*> World::GetCollidingGameObjects(glm::vec2 a_point)
{
  return m_physicsSystem->GetCollidiongGameObjects(a_point);
}

std::vector<GameObject*> World::GetCollidingGameObjects(TestableCollider* a_testableCollider)
{
  return m_physicsSystem->GetCollidiongGameObjects(a_testableCollider);
}

void World::SetGameObjectWorld(GameObject* a_gameobject)
{
  if( a_gameobject )
    a_gameobject->SetWorld( this );
}

void World::AddGoDescriptors()
{
  for (unsigned short i = 0; i < (unsigned int)EGameObjectType::GOT_Max; ++i)
    m_gameObjectDescriptors[(EGameObjectType)i] = {};

  GameObjectConstructionDescriptor& unitDescriptor = m_gameObjectDescriptors[EGameObjectType::GOT_Unit];
  unitDescriptor.listOfComps.push_back(EComponentTypes::CT_RenderComponent);
  unitDescriptor.listOfComps.push_back(EComponentTypes::CT_PathfindingAgentComponent);
  unitDescriptor.listOfComps.push_back(EComponentTypes::CT_WanderingComponent);
  unitDescriptor.listOfComps.push_back(EComponentTypes::CT_ColliderComponent);
  unitDescriptor.listOfComps.push_back(EComponentTypes::CT_SelectableGameObjectComponent);

  GameObjectConstructionDescriptor& physicsDescriptor = m_gameObjectDescriptors[EGameObjectType::GOT_PhysicsTest];
  physicsDescriptor.listOfComps.push_back(EComponentTypes::CT_RenderComponent);
  physicsDescriptor.listOfComps.push_back(EComponentTypes::CT_RigidbodyComponent);
  physicsDescriptor.listOfComps.push_back(EComponentTypes::CT_ColliderComponent);

}
