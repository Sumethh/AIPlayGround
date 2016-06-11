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
  // [0] TopLeft [1] TopRight [2] BottomLeft [3] BottomRight
  glm::vec2 rotatedVertices[ 4 ];
  // Vertices to test against grids
  glm::vec2 testableVerts[ 8 ];
};

struct TestableCollider
{
  glm::vec2 position;
  float radius;
};

class ColliderComponent : public Component
{
public:
  ColliderComponent( GameObject::SharedPtr a_go , EComponentTypes a_compType , EColliderType a_colliderType );
  virtual ~ColliderComponent();

  typedef std::shared_ptr<ColliderComponent> SharedPtr;
  typedef std::weak_ptr<ColliderComponent> WeakPtr;

  void Update( float a_dt ) override;

  bool TestCollision( ColliderComponent* a_other , Collision& a_collision );

  inline EColliderType GetColliderType() const { return m_colliderType; }
  inline Collider GetCollider() const { return m_collider; }
  inline Collider& GetColliderRef() { return m_collider; }
  inline void SetCollider( Collider a_collider ) { m_collider = a_collider; }
  inline void AddRegisteredCell( GridCell* a_gridCell ) { m_cellsRegistered.push_back( a_gridCell ); }
  inline std::vector<GridCell*>& GetRegisteredCells() { return m_cellsRegistered; }

  inline void Render( Renderer2D* a_window )override ;
  inline void SetColliderType( EColliderType a_type ) { m_colliderType = a_type; }

  std::map<ColliderComponent* , bool>& GetTestedColliders() { return m_testedColliders; }
  inline void ResetTestedColliders() { m_testedColliders.clear(); }

  void OnCollisionEnter( Collision& a_collision ) override;

  bool CollidesWithPoint(glm::vec2 a_point);
  bool CollidesWithTestableCollider(TestableCollider a_collider);

private:

  inline bool CircleCircleColTest( ColliderComponent* a_collider1 , ColliderComponent* a_collider2 , Collision& a_collision );
  inline bool BoxBoxColTest( ColliderComponent* a_collider1 , ColliderComponent* a_collider2 , Collision& a_collision );

  inline bool BoxCircleColTest( ColliderComponent* a_box , ColliderComponent* a_circle , Collision& a_collision );

  void CalculateTestingVerts( Collider& a_collider , Transform& a_transform );

  std::map<ColliderComponent* , bool> m_testedColliders;

  std::vector<GridCell*> m_cellsRegistered;
  EColliderType m_colliderType;
  Collider m_collider;
};