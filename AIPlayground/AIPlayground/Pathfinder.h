#pragma once
#include <vector>
#include <glm/glm.hpp>
class Grid;

struct Path
{
  std::vector<glm::vec2> nodes;
};

class Pathfinder
{
public:
  Pathfinder(Grid* const a_grid);
  ~Pathfinder();
  //a_start and a_end are absolute positions not tile indicies
  Path* GetPath( glm::vec2 a_start , glm::vec2 a_end );

private:
  Grid* const m_grid;
};

