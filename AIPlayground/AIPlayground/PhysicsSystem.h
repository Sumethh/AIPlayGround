#pragma once
#include "PhysicsComponentBase.h"
#include <memory>

class PhysicsGrid;
class World;
class Window;
class ColliderComponent;
struct TestableCollider;
class PhysicsSystem
{
public:
  typedef std::shared_ptr<PhysicsSystem> SharedPtr;
  typedef std::weak_ptr<PhysicsSystem> WeakPtr;
  PhysicsSystem();
  ~PhysicsSystem();

  void Construct();

  void Update( float a_dt );

  void RegisterRigidbody( PhysicsComponentBase* a_rigidbody );
  void RegisterCollider( ColliderComponent* a_collider );


  std::vector<GameObject*> GetCollidiongGameObjects(glm::vec2 point);
  std::vector<GameObject*> GetCollidiongGameObjects(TestableCollider* a_testableCollider);

  void SetWorld( World* a_world );
  std::weak_ptr<World> GetWorld() const { return m_world; }

  void Render( Renderer2D* a_renderer );
  static float time;;
private:
  std::vector<PhysicsComponentBase*> m_physicsObjects;
  std::weak_ptr<World> m_world;

  //inline bool BoxCollisionTest( ColliderComponent::SharedPtr  a_boxCollider, ColliderComponent::SharedPtr a_other , Collision& a_collision);
  //inline bool SphereCollisionTest( ColliderComponent::SharedPtr a_sphereCollider , ColliderComponent::SharedPtr a_other , Collision& a_collision);

  void PerformCollisionTests();

  PhysicsGrid* m_grid;
  const double m_timeStep;
  const double m_timeStepMS;
};
