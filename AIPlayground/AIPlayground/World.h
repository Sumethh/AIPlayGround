#pragma once
#include <vector>
#include <map>
#include "Grid.h"
#include "Pathfinder.h"
#include "Camera.h"
#include "PlayerController.h"
#include "GameObjectTypes.h"
#include "GameObject.h"
#include "PhysicsSystem.h"
#include "Renderer2D.h"
#include <memory.h>
#include "Common/TimedFunctionCall.h"

class GameObject;
class Window;
struct TestableCollider;

struct WorldLimits
{
  glm::vec2 topLeft , bottomRight;
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
  inline PlayerController* GetPlayerController()const { return m_playerController; }
  inline Pathfinder* GetPathfinder()const { return m_pathfinder; }
  inline Camera* GetCamera()const { return m_camera; }
  inline Grid* GetGrid() const { return m_grid; }
  inline WorldLimits GetWorldLimits()const { return m_worldLimits; }
  inline PhysicsSystem* GetPhysicsSystem()const { return m_physicsSystem; }

  World::SharedPtr GetWorldShared()const { return m_thisShared; }
  void SetWorldShared( World::SharedPtr a_ptr ) { m_thisShared = a_ptr; }

  std::vector<GameObject*> GetCollidiongGameObjects(glm::vec2 point);
  std::vector<GameObject*> GetCollidiongGameObjects(TestableCollider* a_testableCollider);


protected:
  void SetGameObjectWorld( GameObject* a_gameobject );
private:

  void AddGoDescriptors();

  std::vector<GameObject*> m_gameObjects;
  std::map<EGameObjectType , GameObjectConstructionDescriptor> m_gameObjectDescriptors;
  World::SharedPtr m_thisShared;
  WorldLimits m_worldLimits;
  Grid* m_grid;
  Camera* m_camera;
  PlayerController* m_playerController;
  Pathfinder* m_pathfinder;
  PhysicsSystem* m_physicsSystem;
  bool m_begunPlay;
};
