#include "PathfindingAgentComponent.h"
#include "Pathfinder.h"
#include "World.h"
#include <functional>
#include "Common/HelperFunctions.h"
#include <SFML/Graphics.hpp>
#include "Common/Window.h"
PathfindingAgentComponent::PathfindingAgentComponent( GameObject* a_go , EComponentTypes a_type ) :
  Component( a_go , a_type )
{
}


PathfindingAgentComponent::~PathfindingAgentComponent()
{
}

void PathfindingAgentComponent::OnCosntruct()
{
  GameObject* parent = GetParent();
  if( parent )
  {
    World* world = parent->GetWorld();
    if( world )
      m_pathfinder = world->GetPathfinder();
  }
}

void PathfindingAgentComponent::Render( Window* a_window )
{

  if( m_path )
  {
    World* world = GetParent()->GetWorld();
    std::weak_ptr<Camera> cam;
    if( world )
      cam = world->GetCamera();
#if 0
    if( !cam.expired() )
    {
      std::shared_ptr<Camera> camera = cam.lock();
      for( size_t i = 1; i < m_path->nodes.size(); ++i )
      {
        sf::Vertex line[] = {
          sf::Vertex( ConvertVec2( m_path->nodes[ i - 1 ] - camera->GetPos() ) ),
          sf::Vertex( ConvertVec2( m_path->nodes[ i ] - camera->GetPos() ) )
        };
        a_window->GetWindow()->draw( line , 2 , sf::PrimitiveType::Lines );
      }
    }
#endif
  }
}

std::weak_ptr<Grid> PathfindingAgentComponent::GetGrid() const
{
  if( !m_pathfinder.expired() )
    return m_pathfinder.lock()->GetGrid();

  return std::weak_ptr<Grid>();
}

void PathfindingAgentComponent::RequestPath( glm::vec2 a_start , glm::vec2 a_end )
{
  if( !m_pathfinder.expired() )
  {
    m_pathfinder.lock()->AddPathfindingJob(
      std::bind( &PathfindingAgentComponent::PathCallback , this , std::placeholders::_1 )
      , a_start , a_end );

    m_pathRequested = true;
  }
}

void PathfindingAgentComponent::ClearPath()
{
  if( m_path )
    delete m_path;
  m_path = nullptr;
}

void PathfindingAgentComponent::PathCallback( Path* a_path )
{
  m_path = a_path;
  m_pathRequested = false;
}
