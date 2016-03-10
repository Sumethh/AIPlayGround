#include "Pathfinder.h"
#include "Grid.h"
#include <Common/log.h>

#include <map>

Pathfinder::Pathfinder( Grid* const a_grid ) :
  m_grid( a_grid ) ,
  m_threadRunningFlag() ,
  m_pathFindingThread( &Pathfinder::ThreadMainFunc , this )
{
}


Pathfinder::~Pathfinder()
{
  m_threadRunningFlag.clear();
  m_pathFindingThread.join();
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
  PathJob newJob;
  Node* startNode = m_grid->GetNode( a_startPos );
  Node* endNode = m_grid->GetNode( a_endPos );
  newJob.callback = a_callback;
  newJob.startNode = startNode;
  newJob.endNode = endNode;
  std::lock_guard<std::mutex> mutexLock( m_queueMutex );
  m_pathJobs.push( newJob );
}

void Pathfinder::AddPathfindingJob( std::function<void( Path* )> a_callback , Node* a_startNode , Node* a_endNode )
{
  PathJob newJob;
  newJob.callback = a_callback;
  newJob.startNode = a_startNode;
  newJob.endNode = a_endNode;
  std::lock_guard<std::mutex> mutexLock( m_queueMutex );
  m_pathJobs.push( newJob );
}

Path* Pathfinder::GetPath( glm::vec2 a_start , glm::vec2 a_end )
{
  m_timer.Start();
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

  if( endNode && !endNode->bwalkable )
  {
    LOGW( "Got a path requested with end node being unwalkable" );
    return nullptr;
  }

  if( !endNode || !startNode )
  {

    LOGW( "Got a path requested with end node or start node being nullptr" );
    return nullptr;
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

      int newMoveCost = currentNode->gCost + GetDistance( currentNode , currentNeighbor );
      if( newMoveCost < currentNeighbor->gCost || ( std::find( openSet.begin() , openSet.end() , currentNeighbor ) == openSet.end() ) )
      {
        currentNeighbor->gCost = newMoveCost;
        currentNeighbor->hCost = GetDistance( currentNeighbor , endNode );
        currentNeighbor->parent = currentNode;
        currentNeighbor->GetFCost();
        if( std::find( openSet.begin() , openSet.end() , neighbours[ i ] ) == openSet.end() )
          openSet.push_back( currentNeighbor );

      }
    }
  }

  return RetracePath( startNode , endNode );
}

void Pathfinder::ThreadMainFunc()
{
  m_threadRunningFlag.test_and_set();
  while( m_threadRunningFlag.test_and_set( std::memory_order::memory_order_relaxed ) )
  {
    if( m_pathJobs.size() == 0 )
      std::this_thread::yield();
    else
    {
      m_queueMutex.lock();
      PathJob job = m_pathJobs.front();
      m_pathJobs.pop();
      m_queueMutex.unlock();

      Path* path = nullptr;
      path = GetPath( job.startNode->pos , job.endNode->pos );
      job.callback( path );
    }
  }
}

Path* Pathfinder::RetracePath( Node* a_start , Node* a_end )
{
  Path* newPath = new Path();

  Node* currentNode = a_end;
  while( currentNode->parent )
  {
    newPath->nodes.push_back( currentNode->pos );
    currentNode = currentNode->parent;
  }
  newPath->nodes.push_back( currentNode->pos );
  std::reverse( newPath->nodes.begin() , newPath->nodes.end() );

  LOGI( "Time taken to generate path %f" , (float)m_timer.IntervalMS() );
  return newPath;
}
