#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Common/timer.h>
#include <Common/JobSystem.h>
#include <functional>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
struct Node;
class Grid;

struct Path
{
  std::vector<glm::vec2> nodes;
};
struct PathParameters : public JobParametersBase
{
  std::function<void( Path* )> callback;
  Node* endNode , *startNode;
};

class Pathfinder
{
public:
  Pathfinder( std::shared_ptr<Grid>& a_grid );
  ~Pathfinder();

  void AddPathfindingJob( std::function<void( Path* )> a_callback , glm::vec2 a_startPos , glm::vec2 a_endPos );
  void AddPathfindingJob( std::function<void( Path* )> a_callback , Node* a_startNode , Node* a_endNode );
  //a_start and a_end are absolute positions not tile indicies

  std::weak_ptr<Grid> GetGrid() { return m_grid; }

private:

  void GetPath( JobParametersBase* a_params );
  Path* RetracePath( Node* a_start , Node* a_end );
  std::weak_ptr<Grid> m_grid;
  Timer m_timer;

};

