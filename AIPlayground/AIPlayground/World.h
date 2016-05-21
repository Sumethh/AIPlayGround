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

#include <memory.h>
class GameObject;
class Window;

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
  virtual void Render( Window* const a_window );
  virtual void PostFrame();

  GameObject* CreateGameObject( EGameObjectType a_type = EGameObjectType::GOT_Generic );
  inline std::weak_ptr<PlayerController> GetPlayerController()const { return m_playerController; }
  inline std::weak_ptr<Pathfinder> GetPathfinder()const { return m_pathfinder; }
  inline std::weak_ptr<Camera> GetCamera()const { return m_camera; }
  inline Grid::WeakPtr GetGrid() const { return m_grid; }
  inline WorldLimits GetWorldLimits()const { return m_worldLimits; }
  inline std::weak_ptr<PhysicsSystem> GetPhysicsSystem()const { return m_physicsSystem; }

  World::SharedPtr GetWorldShared()const { return m_thisShared; }
  void SetWorldShared( World::SharedPtr a_ptr ) { m_thisShared = a_ptr; }

protected:
  void SetGameObjectWorld( GameObject* a_gameobject );
private:
  std::vector<GameObject*> m_gameObjects;
  std::map<EGameObjectType , GameObjectConstructionDescriptor> m_gameObjectDescriptors;
  World::SharedPtr m_thisShared;
  WorldLimits m_worldLimits;
  std::shared_ptr<Grid> m_grid;
  std::shared_ptr<Camera> m_camera;
  std::shared_ptr<PlayerController> m_playerController;
  std::shared_ptr<Pathfinder> m_pathfinder;
  std::shared_ptr<PhysicsSystem> m_physicsSystem;
};
