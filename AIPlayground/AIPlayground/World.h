#pragma once
#include <vector>
#include <map>

#include "GameObjectTypes.h"
#include "GameObject.h"
#include <memory.h>
#include "Common/TimedFunctionCall.h"

class GameObject;
class Window;
class Grid;
class Camera;
class Pathfinder;
class PhysicsSystem;
class PlayerController;
struct TestableCollider;

struct WorldInfo
{
  glm::vec2 topLeft , bottomRight;
  glm::vec2 tileSize;
};

class World
{
public:
  World();
  virtual ~World();

  typedef std::weak_ptr<World> WeakPtr;
  typedef std::shared_ptr<World> SharedPtr;

  virtual void OnConstruct();
  virtual void OnDestroyed();
  virtual void BeginPlay();
  virtual void Update( float a_dt );
  virtual void FixedUpdate( float a_dt );
  virtual void PreRender();
  virtual void Render( Renderer2D* renderer);
  virtual void PostFrame();

  GameObject* CreateGameObject( EGameObjectType a_type = EGameObjectType::GOT_Generic );
  void DestroyGameObject(GameObject* a_go);
  inline PlayerController* GetPlayerController()const { return m_playerController; }
  inline Pathfinder* GetPathfinder()const { return m_pathfinder; }
  inline Camera* GetCamera()const { return m_camera; }
  inline Grid* GetGrid() const { return m_grid; }
  inline WorldInfo GetWorldInfo()const { return m_worldInfo; }
  inline PhysicsSystem* GetPhysicsSystem()const { return m_physicsSystem; }

  World::SharedPtr GetWorldShared()const { return m_thisShared; }
  void SetWorldShared( World::SharedPtr a_ptr ) { m_thisShared = a_ptr; }

  std::vector<GameObject*> GetCollidingGameObjects(glm::vec2 point);
  std::vector<GameObject*> GetCollidingGameObjects(TestableCollider* a_testableCollider);


protected:
  void SetGameObjectWorld( GameObject* a_gameobject );
private:

  void AddGoDescriptors();
  std::vector<GameObject*> m_gameObjects;
  std::map<EGameObjectType , GameObjectConstructionDescriptor> m_gameObjectDescriptors;
  World::SharedPtr m_thisShared;
  WorldInfo m_worldInfo;
  Grid* m_grid;
  Camera* m_camera;
  PlayerController* m_playerController;
  Pathfinder* m_pathfinder;
  PhysicsSystem* m_physicsSystem;
  bool m_begunPlay;
};
