#pragma once
#include "Component.h"
#include <glm/glm.hpp>
class Pathfinder;
struct Path;
class PathfindingAgentComponent :public Component
{
public:
  PathfindingAgentComponent( GameObject* a_go, EComponentTypes a_type );
  ~PathfindingAgentComponent();

  void OnCosntruct() override;
  void Render( Window* a_window );

  bool HasPathBeenRequested() { return m_pathRequested; }
  Path* GetPath() { return m_path; }

  void RequestPath( glm::vec2 a_start , glm::vec2 a_end );
  void ClearPath();

private:
  void SetPath( Path* a_path ) { m_path = a_path; }
  void PathCallback( Path* a_path );
  bool m_pathRequested;
  Pathfinder* m_pathfinder;
  Path* m_path;
};

