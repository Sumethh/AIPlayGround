#pragma once
#include "Component.h"
#include "Common/Types.h"

enum class EColliderType : uint8
{
  Sphere ,
  Box
};

struct GridCell;

struct Collider
{
  //Sphere Params
  float radius;
  //Cube
  glm::vec2 extents;
};

class ColliderComponent : public Component
{
public:
  ColliderComponent( GameObject::SharedPtr a_go , EComponentTypes a_compType , EColliderType a_colliderType );
  virtual ~ColliderComponent();

  typedef std::shared_ptr<ColliderComponent> SharedPtr;
  typedef std::weak_ptr<ColliderComponent> WeakPtr;


  void Update(float a_dt) override;

  bool TestCollision( ColliderComponent* a_other , Collision& a_collision );

  inline EColliderType GetColliderType() const { return m_colliderType; }
  inline Collider GetCollider() const { return m_collider; }
  inline void SetCollider( Collider a_collider ) { m_collider = a_collider; }
  inline void SetGridCell( GridCell* a_gridCell ) { m_currentCell = a_gridCell; }

  std::map<ColliderComponent* , bool>& GetTestedColliders() { return m_testedColliders; }
  inline void ResetTestedColliders() { m_testedColliders.clear(); }
private:

  inline bool CircleCircleColTest( ColliderComponent* a_collider1 , ColliderComponent* a_collider2 , Collision& a_collision );
  inline bool BoxBoxColTest( ColliderComponent* a_collider1 , ColliderComponent* a_collider2 , Collision& a_collision );

  inline bool BoxCircleColTest( ColliderComponent* a_box , ColliderComponent* a_circle , Collision& a_collision );

  std::map<ColliderComponent* , bool> m_testedColliders;

  GridCell* m_currentCell;
  EColliderType m_colliderType;
  Collider m_collider;
};