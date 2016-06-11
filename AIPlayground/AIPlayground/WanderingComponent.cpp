#include "WanderingComponent.h"
#include "PathfindingAgentComponent.h"
#include "World.h"
#include "Common/Input.h"
#include <Common/HelperFunctions.h>

#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
WanderingComponent::WanderingComponent( GameObject::SharedPtr a_go , EComponentTypes a_type ) :
  Component( a_go , a_type )
{
}

WanderingComponent::~WanderingComponent()
{
}

void WanderingComponent::BeginPlay()
{
  std::shared_ptr<GameObject> owner = GetParentShared();
  if( owner )
  {
    m_pathfindingComp = (PathfindingAgentComponent*)owner->GetComponentOfType( EComponentTypes::CT_PathfindingAgentComponent );
  }
}
void WanderingComponent::Update( float a_dt )
{
  if( m_pathfindingComp )
  {
    Path* path = m_pathfindingComp->GetPath();
    if( !path && !m_pathfindingComp->HasPathBeenRequested() )
    {
      World* world;
      world = GetParentShared()->GetWorld();
      if( world )
      {
        WorldLimits limits = world->GetWorldLimits();
        float destX , destY = 0.0f;
        destX = (float)( std::rand() % (int)limits.bottomRight.x ) + limits.topLeft.x;
        destY = (float)( std::rand() % (int)limits.bottomRight.y ) + limits.topLeft.y;
        glm::vec2 destination( destX , destY );
        Transform parentTransform = GetParentShared()->GetTransform();
        Grid* grid = m_pathfindingComp->GetGrid();
        if( grid )
        {          
          Node* node = grid->GetNode( destination );
          Node* currentNode = grid->GetNode( parentTransform.position );
          /*while( !node->bwalkable && node != currentNode );
          {
            destination.x = (float)( std::rand() % (int)limits.bottomRight.x ) + limits.topLeft.x;
            destination.y = (float)( std::rand() % (int)limits.bottomRight.y ) + limits.topLeft.y;
            node = grid->GetNode( destination );
          }*/
          m_pathfindingComp->RequestPath( parentTransform.position , destination );
        }
      }
    }
    else if( path )
    {
      if( path->nodes.size() > 0 )
      {
        glm::vec2 currentNode = path->nodes[ 0 ];
        Transform parentTransform = GetParentShared()->GetTransform();
        glm::vec2 vecBetween = currentNode - parentTransform.position;
        float length = glm::length( vecBetween );
        if( length < 2.0f )
        {
          path->nodes.erase( path->nodes.begin() );
          if( path->nodes.size() == 0 )
          {
            m_pathfindingComp->ClearPath();
            return;
          }
          else
            currentNode = path->nodes[ 0 ];
          vecBetween = currentNode - parentTransform.position;
        }
        glm::vec2 direction = glm::normalize( vecBetween );
        parentTransform.position += direction* 128.0f * a_dt;

        float angle = GetLookAtAngle( currentNode , parentTransform.position );
        GetParentShared()->SetPosition( parentTransform.position );
        GetParentShared()->SetRotation(angle);
      }
      else
        m_pathfindingComp->ClearPath();
    }
  }
}