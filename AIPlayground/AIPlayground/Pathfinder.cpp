#include "Pathfinder.h"
#include "Grid.h"
#include <Common/log.h>

#include <map>

Pathfinder::Pathfinder( std::shared_ptr<Grid>& a_grid ) :
  m_grid( a_grid )
{
}


Pathfinder::~Pathfinder()
{
}

int GetDistance( Node* a_node , Node* a_otherNode )
{

  int dstX = (int)abs( a_node->pos.x - a_otherNode->pos.x );
  int dstY = (int)abs( a_node->pos.y - a_otherNode->pos.y );
  return ( dstX * 10 ) + dstY * 10;
  //if( dstX > dstY )
  //  return 14 * dstY + 10 * ( dstX - dstY );
  //else
  //  return 14 * dstX + 10 * ( dstY - dstX );
}

void Pathfinder::AddPathfindingJob( std::function<void( Path* )> a_callback , glm::vec2 a_startPos , glm::vec2 a_endPos )
{
  if( !m_grid.expired() )
  {
    std::shared_ptr<Grid> grid = m_grid.lock();
    Node* startNode = grid->GetNode( a_startPos );
    Node* endNode = grid->GetNode( a_endPos );
    AddPathfindingJob( a_callback , startNode , endNode );
  }
}

void Pathfinder::AddPathfindingJob( std::function<void( Path* )> a_callback , Node* a_startNode , Node* a_endNode )
{
  Job* newJob = new Job();
  PathParameters* params = new PathParameters();
  params->callback = a_callback;
  params->startNode = a_startNode;
  params->endNode = a_endNode;

  newJob->jobCondition = JobCondition::ONE_AT_A_TIME;
  newJob->taskFunction = std::bind( &Pathfinder::GetPath , this , std::placeholders::_1 );
  newJob->jobParams = params;
  newJob->threadCleanUpJob = true;
  newJob->typeHashCode = typeid( params ).hash_code();
  JobSystem::ScheduleJob( newJob );
}

void Pathfinder::GetPath( JobParametersBase* a_params )
{
  if( !a_params )
  {
    LOGE( "Did not get any parameters" );
    return;
  }
  PathParameters* params = reinterpret_cast<PathParameters*>( a_params );
  Node* startNode = nullptr , *endNode = nullptr;
  startNode = params->startNode;
  endNode = params->endNode;

  m_timer.Start();
  if( m_grid.expired() )
  {
    LOGE( "I got a path request however i dont have a grid" );
    params->callback( nullptr );
    return ;
  }

  if( startNode == endNode )
  {
    LOGW( "Got a path requested with the same start and end positions " );
    params->callback( nullptr );
    return;
  }
  std::shared_ptr<Grid> grid = m_grid.lock();

  if( endNode && !endNode->bwalkable )
  {
    LOGW( "Got a path requested with end node being unwalkable" );
    params->callback( nullptr );
    return;
  }

  if( !endNode || !startNode )
  {

    LOGW( "Got a path requested with end node or start node being nullptr" );
    params->callback( nullptr );
    return;
  }

  std::vector<Node*> openSet;
  std::map<Node* , bool> closedSet;

  openSet.push_back( startNode );
  Node* currentNode;
  while( openSet.size() > 0 )
  {
    currentNode = openSet[ 0 ];
    std::vector<Node*>::iterator storedIt = openSet.begin();
    for( auto i = openSet.begin(); i != openSet.end(); i++ )
    {
      Node* node = *i;
      if( node->fCost < currentNode->GetFCost() ||
        node->fCost == currentNode->GetFCost() &&
        node->hCost < currentNode->hCost )
      {
        currentNode = node;
        storedIt = i;
      }
    }

    openSet.erase( storedIt );
    closedSet[ currentNode ] = true;
    if( currentNode == endNode )
      break;

    Node** neighbours = currentNode->neighbors;
    Node* currentNeighbor = nullptr;
    for( int i = 0; i < Node::Neighbors::NeighborCount; i++ )
    {
      currentNeighbor = neighbours[ i ];
      if( !currentNeighbor )
        continue;

      if( closedSet.find( currentNeighbor ) != closedSet.end() || !currentNeighbor->bwalkable )
        continue;

      float newMoveCost = currentNode->gCost + GetDistance( currentNode , currentNeighbor);
      if( newMoveCost < currentNeighbor->gCost || ( std::find( openSet.begin() , openSet.end() , currentNeighbor ) == openSet.end() ) )
      {
        currentNeighbor->gCost = newMoveCost;
        currentNeighbor->hCost = (float)GetDistance( currentNeighbor , endNode );
        currentNeighbor->parent = currentNode;
        currentNeighbor->GetFCost();
        if( std::find( openSet.begin() , openSet.end() , neighbours[ i ] ) == openSet.end() )
          openSet.push_back( currentNeighbor );

      }
    }
  }

  params->callback( RetracePath( startNode , endNode ));
}

Path* Pathfinder::RetracePath( Node* a_start , Node* a_end )
{
  Path* newPath = new Path();

  Node* currentNode = a_end;
  while( currentNode->parent != a_start )
  {
    newPath->nodes.push_back( currentNode->center );
    currentNode = currentNode->parent;
  }
  newPath->nodes.push_back( currentNode->center );
  std::reverse( newPath->nodes.begin() , newPath->nodes.end() );

  //LOGI( "Time taken to generate path %f" , (float)m_timer.IntervalMS() );
  return newPath;
}
