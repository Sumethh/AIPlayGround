#include "Pathfinder.h"
#include <Common/log.h>
#include "Grid.h"

Pathfinder::Pathfinder( Grid* const a_grid ) :
  m_grid( a_grid )
{
}


Pathfinder::~Pathfinder()
{
}

Path* Pathfinder::GetPath( glm::vec2 a_start , glm::vec2 a_end )
{

  if( !m_grid )
  {
    LOGE( "I got a path request however i dont have a grid" );
    return nullptr;
  }

  if( a_start == a_end )
  {
    LOGW( "Got a path requested with the same start and end positions " );
    return nullptr;
  }

  Node* startNode = nullptr , *endNode = nullptr;

  startNode = m_grid->GetNode( a_start );
  endNode = m_grid->GetNode( a_end );

  return nullptr;
}
