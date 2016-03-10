#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Common/timer.h>
#include <functional>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
class Node;
class Grid;

struct Path
{
  std::vector<glm::vec2> nodes;
};
struct PathJob
{
  std::function<void( Path* )> callback;
  Node* endNode , *startNode;

};

class Pathfinder
{
public:
  Pathfinder( Grid* const a_grid );
  ~Pathfinder();

  void AddPathfindingJob( std::function<void( Path* )> a_callback , glm::vec2 a_startPos , glm::vec2 a_endPos );
  void AddPathfindingJob( std::function<void( Path* )> a_callback , Node* a_startNode , Node* a_endNode );
  //a_start and a_end are absolute positions not tile indicies

private:

  void ThreadMainFunc();
  Path* GetPath( glm::vec2 a_start , glm::vec2 a_end );
  Path* RetracePath( Node* a_start , Node* a_end );
  Grid* const m_grid;
  Timer m_timer;


  std::atomic_flag m_threadRunningFlag;
  std::mutex m_queueMutex;
  std::queue <PathJob> m_pathJobs;
  std::thread m_pathFindingThread;
};

