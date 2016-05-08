#pragma once
#include "GameObject.h"
#include "Grid.h"
#include "PhysicsSystem.h"


#define TILE_COUNT_X_PER_CELL 6;
#define TILE_COUNT_Y_PER_CELL 6;
#define COLLIDERS_PER_CELL 10
class Window;
class ColliderComponent;
struct GridCell
{
  glm::vec2 center;
  glm::vec2 size;
  glm::vec2 index;
  std::vector<ColliderComponent*> colliders;

  GridCell* neighbors[ 8 ];
};


class PhysicsGrid
{
public:
  PhysicsGrid(PhysicsSystem::WeakPtr a_physicsSystem);
  ~PhysicsGrid();
  
  void Generate();

  void Update();

  void AddCollider( ColliderComponent* a_collider );

  void PerformCollisionTests();

  void Draw(Window* a_window);
private:
   

  //void SplitCell(GridCell* a_cell);
  //void AddColliderToCell( GridCell* a_cell , ColliderComponent* a_component );
  //void AddColliderToSplitCell( GridCell* a_grid , ColliderComponent* a_component );

  inline void UpdateCell( GridCell& a_cell );

  inline GridCell* GetCell( uint index );
  inline GridCell* GetCellFromPosition( glm::vec2 a_pos );
  PhysicsSystem::WeakPtr m_physicsSystem;

  GridCell* m_grid;
  uint m_gridSizeX;
  uint m_gridSizeY;
  uint m_gridCountX;
  uint m_gridCountY;
};

