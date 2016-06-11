#include "PathfindingAgentComponent.h"
#include "Pathfinder.h"
#include "World.h"
#include <functional>
#include "Common/HelperFunctions.h"
#include <SFML/Graphics.hpp>
#include "Common/Window.h"
#include "DebugValues.h"
PathfindingAgentComponent::PathfindingAgentComponent( GameObject::SharedPtr a_go , EComponentTypes a_type ) :
  Component( a_go , a_type )
{
}

PathfindingAgentComponent::~PathfindingAgentComponent()
{
}

void PathfindingAgentComponent::OnCosntruct()
{
  std::shared_ptr<GameObject> parent = GetParentShared();
  if( parent )
  {
    World* world = parent->GetWorld();
    if( world )
      m_pathfinder = world->GetPathfinder();
  }
}

void PathfindingAgentComponent::Render( Renderer2D* a_renderer )
{
  if( m_path )
  {
    World* world = GetParentShared()->GetWorld();
    std::weak_ptr<Camera> cam;
    if( world )
      cam = world->GetCamera();
    if( DebugValues::GI()->RenderGrid )
    {
      Camera* camera = cam.lock().get();
      LineRenderer& lineRender = a_renderer->GetLineRenderer();
      for( uint i = 1; i < m_path->nodes.size(); i++ )
      {
        lineRender.Submit( m_path->nodes[ i - 1 ] -camera->GetPos(), m_path->nodes[ i ] - camera->GetPos() , glm::vec4( 1.0f , 0.0f , 0.0f , 1.0f ) );
      }
    }
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
    if (m_path)
      delete m_path;
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