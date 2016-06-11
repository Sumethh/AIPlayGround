#pragma once
#include "Component.h"
#include <glm/glm.hpp>

#include <memory>
class Pathfinder;
class Grid;
struct Path;
class PathfindingAgentComponent :public Component
{
public:
  PathfindingAgentComponent( GameObject::SharedPtr a_gameObject , EComponentTypes a_type );
  ~PathfindingAgentComponent();

  void OnCosntruct() override;
  void Render( Renderer2D* a_renderer) override;

  bool HasPathBeenRequested() { return m_pathRequested; }
  Path* GetPath() { return m_path; }

  void RequestPath( glm::vec2 a_start , glm::vec2 a_end );
  void ClearPath();

  std::weak_ptr<Grid> GetGrid() const;
private:
  void SetPath( Path* a_path ) { m_path = a_path; }
  void PathCallback( Path* a_path );
  bool m_pathRequested;
  std::weak_ptr<Pathfinder> m_pathfinder;
  Path* m_path;
};
