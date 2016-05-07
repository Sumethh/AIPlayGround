#include "PhysicsGrid.h"
#include "World.h"
#include "Common/HelperFunctions.h"
#include "Common/Window.h"
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

  if( !m_physicsSystem.expired() )
    physSys = m_physicsSystem.lock();
  if( physSys )
  {
    World::WeakPtr worldWeak = physSys->GetWorld();

    World::SharedPtr worldPtr;
    if( !worldWeak.expired() )
      worldPtr = worldWeak.lock();

    WorldLimits worldLimits = worldPtr->GetWorldLimits();
    glm::vec2 worldSize = worldLimits.bottomRight - worldLimits.topLeft;
    assert( worldSize.x > 0 && worldSize.y > 0 );
    Grid::SharedPtr grid = worldPtr->GetGrid().lock();
    int tileSizeX = grid->GetTileSizeX();
    int tileSizeY = grid->GetTileSizeY();
    m_gridSizeX = tileSizeX * TILE_COUNT_X_PER_CELL;
    m_gridSizeY = tileSizeY * TILE_COUNT_Y_PER_CELL;

    m_gridCountX = worldSize.x / m_gridSizeX;
    if( (int)worldSize.x % m_gridSizeX )
      m_gridCountX++;

    m_gridCountY = worldSize.y / m_gridSizeY;
    if( (int)worldSize.y % m_gridSizeY )
      m_gridCountY++;
    int t = 0;
    void* data = std::malloc( m_gridCountX * m_gridCountY * sizeof( GridCell ) );
    uint offset = 0;
    glm::vec2 location( 0.0f , 0.0f );
    for( int y = 0; y < m_gridCountY; y++ )
    {
      location.y = y * m_gridSizeY;
      for( int x = 0; x < m_gridCountX; x++ )
      {
        location.x = x*m_gridSizeX;
        char* loc = (char*)data + offset;
        GridCell* gridcell = new( loc ) GridCell;
        offset += sizeof( GridCell );
        gridcell->size = glm::vec2( m_gridSizeX , m_gridSizeY );
        gridcell->center = location + gridcell->size / 2.0f;
        gridcell->index.x = x;
        gridcell->index.y = y;
        std::memset( gridcell->neighbors , 0 , sizeof( int ) * 8 );
        std::memset( gridcell->children , 0 , sizeof( int ) * 4 );

      }
    }
    m_grid = (GridCell*)data;

    for( int cell = 0; cell < m_gridCountY*m_gridCountX; cell++ )
    {
      GridCell& gridCell = m_grid[ cell ];
      int count = 0;
      for( int y = -1; y <= 1; y++ )
      {
        for( int x = -1; x <= 1; x++ )
        {
          int index = ( gridCell.index.y + y ) * m_gridCountX + gridCell.index.x + x;
          GridCell* newCell = GetCell( index );
          if( !newCell )
          {
            gridCell.neighbors[ count ] = nullptr;
            count++;
            continue;
          }
          int xDiff = glm::abs( newCell->index.x - gridCell.index.x );
          int yDiff = glm::abs( newCell->index.y - gridCell.index.y );
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
  GameObject* parent;
  for( int i = 0; i < m_gridCountX * m_gridCountY; i++ )
  {
    GridCell& cell = m_grid[ i ];
    for( auto it = cell.colliders.begin(); it != cell.colliders.end(); )
    {
      parent = (*it)->GetParent();
      if( parent->GetPhysicsDirtyFlag() )
      {
        parent->ResetPhysicsDirtyFlag();
        GridCell* goCell = GetCellFromPosition( parent->GetTransform().position );
        if( goCell == &m_grid[ i ] || !goCell )
        {
          it++;
          continue;
        }
        else
        {
          (*it)->SetGridCell( goCell );
          goCell->colliders.push_back( *it );
          it = cell.colliders.erase( it );
          continue;;
        }
      }

      it++;
    }
  }
}

void PhysicsGrid::AddCollider( ColliderComponent* a_collider )
{
  if( !a_collider )
    return;
  GameObject* colliderParent = a_collider->GetParent();
  Transform colliderTransform = colliderParent->GetTransform();
  GridCell* cell = GetCellFromPosition( colliderTransform.position );
  if( cell )
  {
    cell->colliders.push_back( a_collider );
    a_collider->SetGridCell( cell );
  }
}

void PhysicsGrid::PerformCollisionTests()
{

}

void PhysicsGrid::Draw( Window* a_window )
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
  for( int i = 0; i < m_gridCountX * m_gridCountY; i++ )
  {
    GridCell& cell = m_grid[ i ];
    rect.setPosition( ConvertVec2( cell.center - camLoc ) );
    rect.setOrigin( ConvertVec2( cell.size / 2.0f ) );
    rect.setSize( ConvertVec2( cell.size ) );
    a_window->RenderDrawable( rect );

    if( cell.colliders.size() )
      for( int i = 0; i < cell.colliders.size(); i++ )
      {
        sf::Vertex line[] = {
          sf::Vertex( ConvertVec2( cell.center - camLoc ) ),
          sf::Vertex( ConvertVec2( cell.colliders[ i ]->GetParent()->GetTransform().position - camLoc ) )
        };
        a_window->GetWindow()->draw( line , 2 , sf::PrimitiveType::Lines );
      }

  }
}

void PhysicsGrid::SplitCell( GridCell* a_cell )
{

}

void PhysicsGrid::UpdateCell( GridCell& a_cell )
{

}

bool PhysicsGrid::TestColliderWithGridCell( GridCell& a_cell , ColliderComponent* a_collider )
{
  return true;
}

GridCell* PhysicsGrid::GetCell( int index )
{
  if( index < 0 || index >= m_gridCountX * m_gridCountY )
    return nullptr;

  return &m_grid[ index ];
}

GridCell* PhysicsGrid::GetCellFromPosition( glm::vec2 a_pos )
{
  int indexX = a_pos.x / m_gridSizeX;
  int indexY = a_pos.y / m_gridSizeY;

  int cellIndex = indexY * m_gridCountX + indexX;

  return GetCell( cellIndex );
}
