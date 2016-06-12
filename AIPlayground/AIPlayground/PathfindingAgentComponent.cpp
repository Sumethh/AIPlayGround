#include "PathfindingAgentComponent.h"
#include "Pathfinder.h"
#include "World.h"
#include <functional>
#include "Common/HelperFunctions.h"
#include <SFML/Graphics.hpp>
#include "Common/Window.h"
#include "DebugValues.h"
#include "Renderer2D.h"
#include "Camera.h"
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

void PathfindingAgentComponent::Render( Renderer2D* a_renderer )
{
  if( m_path )
  {
    World* world = GetParent()->GetWorld();
    Camera* cam = nullptr;
    if( world )
      cam = world->GetCamera();
    if( DebugValues::GI()->RenderGrid && cam)
    {
      LineRenderer& lineRender = a_renderer->GetLineRenderer();
      for( uint i = 1; i < m_path->nodes.size(); i++ )
      {
        lineRender.Submit( m_path->nodes[ i - 1 ] -cam->GetPos(), m_path->nodes[ i ] - cam->GetPos() , glm::vec4( 1.0f , 0.0f , 0.0f , 1.0f ) );
      }
    }
  }
}

Grid* PathfindingAgentComponent::GetGrid() const
{
  if( m_pathfinder )
    return m_pathfinder->GetGrid();

  return nullptr;
}

void PathfindingAgentComponent::RequestPath( glm::vec2 a_start , glm::vec2 a_end )
{
  if( m_pathfinder )
  {
    if (m_path)
      delete m_path;
    m_pathfinder->AddPathfindingJob(
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