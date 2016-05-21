#include "PhysicsGrid.h"
#include "World.h"
#include "Common/HelperFunctions.h"
#include "Common/Window.h"
#include "ColliderComponent.h"
#include "Common/log.h"
#include "DebugValues.h"

void GridCell::AddCollider( ColliderComponent* a_collider )
{
}

GridCell* GridCell::GetCellFromPosition( glm::vec2 a_pos )
{
  return nullptr;
}

PhysicsGrid::PhysicsGrid( PhysicsSystem::WeakPtr a_physicsSystem ) :
  m_physicsSystem( a_physicsSystem )
{
}

PhysicsGrid::~PhysicsGrid()
{
}

void PhysicsGrid::Generate()
{
  PhysicsSystem::SharedPtr physSys;
  physSys = m_physicsSystem.lock();
  if( physSys )
  {
    World::WeakPtr worldWeak = physSys->GetWorld();

    World::SharedPtr worldPtr;
    worldPtr = worldWeak.lock();

    WorldLimits worldLimits = worldPtr->GetWorldLimits();
    glm::vec2 worldSize = worldLimits.bottomRight - worldLimits.topLeft;
    assert( worldSize.x > 0 && worldSize.y > 0 );
    Grid::SharedPtr grid = worldPtr->GetGrid().lock();
    int tileSizeX = grid->GetTileSizeX();
    int tileSizeY = grid->GetTileSizeY();
    m_gridSizeX = tileSizeX * TILE_COUNT_X_PER_CELL;
    m_gridSizeY = tileSizeY * TILE_COUNT_Y_PER_CELL;

    m_gridCountX = (uint)( worldSize.x / m_gridSizeX );
    if( (int)worldSize.x % m_gridSizeX )
      m_gridCountX++;

    m_gridCountY = (uint)( worldSize.y / m_gridSizeY );
    if( (int)worldSize.y % m_gridSizeY )
      m_gridCountY++;
    int t = 0;
    void* data = std::malloc( m_gridCountX * m_gridCountY * sizeof( GridCell ) );
    uint offset = 0;
    glm::vec2 location( 0.0f , 0.0f );
    for( uint y = 0; y < m_gridCountY; y++ )
    {
      location.y = (float)( y * m_gridSizeY );
      for( uint x = 0; x < m_gridCountX; x++ )
      {
        location.x = (float)( x*m_gridSizeX );
        char* loc = (char*)data + offset;
        GridCell* gridcell = new( loc ) GridCell;
        offset += sizeof( GridCell );
        gridcell->size = glm::vec2( m_gridSizeX , m_gridSizeY );
        gridcell->center = location + gridcell->size / 2.0f;
        gridcell->index.x = (float)x;
        gridcell->index.y = (float)y;
        std::memset( gridcell->neighbors , 0 , sizeof( int ) * 8 );
      }
    }
    m_grid = (GridCell*)data;

    for( uint cell = 0; cell < m_gridCountY*m_gridCountX; cell++ )
    {
      GridCell& gridCell = m_grid[ cell ];
      int count = 0;
      for( int y = -1; y <= 1; y++ )
      {
        for( int x = -1; x <= 1; x++ )
        {
          int index = (int)( ( gridCell.index.y + y ) * m_gridCountX + gridCell.index.x + x );
          GridCell* newCell = GetCell( index );
          if( !newCell )
          {
            gridCell.neighbors[ count ] = nullptr;
            count++;
            continue;
          }
          int xDiff = (int)glm::abs( newCell->index.x - gridCell.index.x );
          int yDiff = (int)glm::abs( newCell->index.y - gridCell.index.y );
          if( ( xDiff == 1 || xDiff == 0 ) && ( yDiff == 1 || yDiff == 0 ) && xDiff + yDiff != 0 )
          {
            gridCell.neighbors[ count ] = newCell;
            count++;
          }
        }
      }
    }

    int tasda = 0;
  }
}

void PhysicsGrid::Update()
{
  UpdateRegisteredColliders();

  for( uint i = 0; i < m_gridCountX * m_gridCountY; i++ )
  {
    GridCell& cell = m_grid[ i ];
    UpdateCell( cell );
  }
}

void PhysicsGrid::AddCollider( ColliderComponent* a_collider )
{
  if( !a_collider )
    return;
  GameObject* colliderParent = a_collider->GetParent();
  Transform colliderTransform = colliderParent->GetTransform();

  //Register to Cells that it collides with
  AddColliderToCollidingCells( a_collider );
  m_registeredColliders.push_back( a_collider );
}

void SwapCollisionResults( Collision& a_collision , ColliderComponent* a_component )
{
  a_collision.othercollider = a_component;
  a_collision.otherGameObject = a_component->GetParent();
  a_collision.otherRigidBody = a_component->GetParent()->GetComponentOfType( EComponentTypes::CT_RigidbodyComponent );
  a_collision.collisionNormal *= -1;
}

int tests = 0;
void PhysicsGrid::PerformCollisionTests()
{
  tests = 0;
  for( uint i = 0; i < m_gridCountX * m_gridCountY; i++ )
  {
    GridCell& cell = m_grid[ i ];
    for( size_t collider = 0; collider < cell.colliders.size(); ++collider )
    {
      ColliderComponent* colliderPtr = cell.colliders[ collider ];
      std::map<ColliderComponent* , bool>& testedCol = colliderPtr->GetTestedColliders();
      for( size_t collider2 = collider + 1; collider2 < cell.colliders.size(); ++collider2 )
      {
        ColliderComponent* colliderPtr2 = cell.colliders[ collider2 ];
        Collision result;
        if( colliderPtr != colliderPtr2 )
        {
          tests++;
          if( colliderPtr->TestCollision( colliderPtr2 , result ) )
          {
            colliderPtr->GetParent()->OnCollisionEnter( result );
            SwapCollisionResults( result , colliderPtr );
            colliderPtr2->GetParent()->OnCollisionEnter( result );
          }
        }
      }
    }
  }
  //LOGI( "%d" , tests );
}

void PhysicsGrid::Draw( Window* a_window )
{
  if( DebugValues::GI()->RenderGrid )
  {
    sf::RectangleShape rect;
    rect.setFillColor( sf::Color::Transparent );
    rect.setOutlineThickness( 1.5 );
    rect.setOutlineColor( sf::Color::Red );
    glm::vec2 camLoc( 0 , 0 );
    auto cam = m_physicsSystem.lock()->GetWorld().lock()->GetCamera();
    if( !cam.expired() )
    {
      auto t = cam.lock();
      camLoc = t->GetPos();
    }
    for( uint i = 0; i < m_gridCountX * m_gridCountY; i++ )
    {
      GridCell& cell = m_grid[ i ];

      rect.setPosition( ConvertVec2( cell.center - camLoc ) );
      rect.setOrigin( ConvertVec2( cell.size / 2.0f ) );
      rect.setSize( ConvertVec2( cell.size ) );
      a_window->RenderDrawable( rect );

      if( cell.colliders.size() )
        for( size_t i = 0; i < cell.colliders.size(); i++ )
        {
          sf::Vertex line[] = {
            sf::Vertex( ConvertVec2( cell.center - camLoc ) ),
            sf::Vertex( ConvertVec2( cell.colliders[ i ]->GetParent()->GetTransform().position - camLoc ) )
          };
          a_window->GetWindow()->draw( line , 2 , sf::PrimitiveType::Lines );
        }
    }
  }
}

//void PhysicsGrid::SplitCell( GridCell* a_cell )
//{
//  void* data = std::malloc( sizeof( GridCell ) * 4 );
//  int index , offset = 0;
//  for( int y = 0; y < 2; y++ )
//  {
//    for( int x = 0; x < 2; x++ )
//    {
//      index = y * 2 + x;
//      char* loc = (char*)data + offset;
//      GridCell* cell = new( loc ) GridCell;
//      a_cell->children[ index ] = cell;
//      cell->size = a_cell->size / 2.0f;
//      cell->center = a_cell->center;
//      cell->index = glm::vec2( x , y );
//      cell->parent = a_cell;
//      if( x == 0 )
//        cell->center.x -= cell->size.x / 2.0f;
//      else
//        cell->center.x += cell->size.x / 2.0f;
//
//      if( y == 0 )
//        cell->center.y -= cell->size.y / 2.0f;
//      else
//        cell->center.y += cell->size.y / 2.0f;
//
//    }
//  }
//  for( auto it = a_cell->colliders.begin(); it != a_cell->colliders.end(); )
//  {
//    if( GetCellFromPosition( ( *it )->GetParent()->GetTransform().position ) != a_cell )
//    {
//      AddCollider( *it );
//      ( *it )->GetParent()->SetPhysicsFlagDirty();
//      it = a_cell->colliders.erase( it );
//    }
//    else
//    {
//      AddColliderToSplitCell( a_cell , *it );
//      it = a_cell->colliders.erase( it );
//    }
//  }
//}
//void PhysicsGrid::AddColliderToCell( GridCell* a_cell , ColliderComponent* a_component )
//{
//  //If the cell has not been split then add it to the cell
//  if( !a_cell->children[ 0 ] )
//  {
//    a_component->SetGridCell( a_cell );
//    a_cell->colliders.push_back( a_component );
//
//    //Check if we need to split the cell , We also only split once
//    if( a_cell->colliders.size() >= COLLIDERS_PER_CELL && !a_cell->parent )
//    {
//      //Split the cell
//      SplitCell( a_cell );
//      AddColliderToSplitCell( a_cell , a_component );
//    }
//  }
//  else
//  {
//    AddColliderToSplitCell( a_cell , a_component );
//  }
//}

//void PhysicsGrid::AddColliderToSplitCell( GridCell* a_grid , ColliderComponent* a_component )
//{
//  GameObject* parent = a_component->GetParent();
//  glm::vec2 gridTopLeft = a_grid->center - ( a_grid->size / 2.0f );
//  glm::vec2 relativeVec = a_grid->center - parent->GetTransform().position;
//  glm::vec2 splitGridSize = a_grid->children[ 0 ]->size;
//
//  uint xIndex , yIndex;
//  xIndex = relativeVec.x / splitGridSize.x;
//  yIndex = relativeVec.y / splitGridSize.y;
//  uint index = xIndex + yIndex;
//
//  a_grid->children[ index ]->colliders.push_back( a_component );
//
//}

void PhysicsGrid::UpdateCell( GridCell& a_cell )
{
  //GameObject* parent;
  //for( auto it = a_cell.colliders.begin(); it != a_cell.colliders.end(); )
  //{
  //  parent = ( *it )->GetParent();
  //  if( parent->GetPhysicsDirtyFlag() )
  //  {
  //    parent->ResetPhysicsDirtyFlag();
  //    GridCell* goCell = GetCellFromPosition( parent->GetTransform().position );
  //    int index = (int)( a_cell.index.y * m_gridCountX + a_cell.index.x );
  //    if( goCell == &m_grid[ index ] || !goCell )
  //    {
  //      it++;
  //      continue;
  //    }
  //    else
  //    {
  //      goCell->colliders.push_back( ( *it ) );
  //      //remove the collider form the old cell
  //      it = a_cell.colliders.erase( it );
  //      continue;
  //    }
  //  }

  //  it++;
  //}
}

GridCell* PhysicsGrid::GetCell( uint index )
{
  if( index < 0 || index >= m_gridCountX * m_gridCountY )
    return nullptr;

  return &m_grid[ index ];
}

GridCell* PhysicsGrid::GetCellFromPosition( glm::vec2 a_pos )
{
  int indexX = (int)( a_pos.x / m_gridSizeX );
  int indexY = (int)( a_pos.y / m_gridSizeY );

  int cellIndex = indexY * m_gridCountX + indexX;

  return GetCell( cellIndex );
}

void PhysicsGrid::AddColliderToCollidingCells( ColliderComponent* a_collider )
{
  std::vector<GridCell*> addedGridCells;

  Collider collider = a_collider->GetCollider();

  for( int i = 0; i < _countof( collider.testableVerts ); i++ )
  {
    GridCell* cell = GetCellFromPosition( collider.testableVerts[ i ] );
    if( cell )
      if( std::find( addedGridCells.begin() , addedGridCells.end() , cell ) != addedGridCells.end() )
      {
        addedGridCells.push_back( cell );
        cell->colliders.push_back( a_collider );
        a_collider->AddRegisteredCell( cell );
      }
  }
}

void PhysicsGrid::UpdateRegisteredColliders()
{
  std::vector<GridCell*> cells;
  cells.reserve( 4 );
  for( auto iter = m_registeredColliders.begin(); iter != m_registeredColliders.end(); iter++ )
  {
    if( !( *iter )->GetParent()->GetPhysicsDirtyFlag() )
      return;
    Collider& collider = ( *iter )->GetColliderRef();
    std::vector<GridCell*>& registeredCells = ( *iter )->GetRegisteredCells();
    GetCollidingCells( *iter , cells );

    for( auto registeredCell = registeredCells.begin(); registeredCell != registeredCells.end(); )
    {
      if( !( *registeredCell ) )
      {
        registeredCell++;
        continue;
      }
      auto it = std::find( cells.begin() , cells.end() , *registeredCell );
      if( it == std::end( cells ) )
      {
        auto colliderIter = std::find( ( *registeredCell )->colliders.begin() , ( *registeredCell )->colliders.end() , *iter );
        ( *registeredCell )->colliders.erase( colliderIter );
        registeredCell = registeredCells.erase( registeredCell );
      }
      else
      {
        cells.erase( it );
        registeredCell++;
      }
    }

    for( auto newCell = cells.begin(); newCell != cells.end(); )
    {
      ( *newCell )->colliders.push_back( *iter );
      ( *iter )->AddRegisteredCell( *newCell );
      newCell = cells.erase( newCell );
    }
    ( *iter )->GetParent()->ResetPhysicsDirtyFlag();
  }
}

void PhysicsGrid::GetCollidingCells( ColliderComponent* a_collider , std::vector<GridCell*>& a_vecOut )
{
  Collider& collider = a_collider->GetColliderRef();
  for( int i = 0; i < _countof( collider.testableVerts ); i++ )
  {
    GridCell* cell = GetCellFromPosition( collider.testableVerts[ i ] );
    if( std::find( a_vecOut.begin() , a_vecOut.end() , cell ) == a_vecOut.end() )
      a_vecOut.push_back( cell );
  }
}