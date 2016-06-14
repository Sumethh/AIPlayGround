#include "PhysicsSystem.h"
#include "PhysicsGrid.h"
#include "Common/log.h"
#include "World.h"
#include "Common/timer.h"

float PhysicsSystem::time;

PhysicsSystem::PhysicsSystem() :
  m_timeStep( 1.0f / 50.0f ) ,
  m_timeStepMS( m_timeStep * 1000 )
{}

PhysicsSystem::~PhysicsSystem()
{
}

std::vector<GameObject*> PhysicsSystem::GetCollidiongGameObjects(glm::vec2 point)
{
  return m_grid->GetCollidiongGameObjects(point);
}

std::vector<GameObject*> PhysicsSystem::GetCollidiongGameObjects(TestableCollider* a_testableCollider)
{
  return m_grid->GetCollidiongGameObjects(a_testableCollider);
}

void PhysicsSystem::SetWorld(World* a_world)
{
  if( a_world )
  {
    m_world = a_world;
    m_grid = new PhysicsGrid( a_world->GetPhysicsSystem() );
  }
}

void PhysicsSystem::Construct()
{
  m_grid->Generate();
}
Timer myTime;
void PhysicsSystem::Update( float a_dt )
{
  myTime.Start();
  for( auto it = m_physicsObjects.begin(); it != m_physicsObjects.end(); it++ )
  {
    ( *it )->PhysicsUpdate( a_dt );
  }
  m_grid->Update();
  m_grid->PerformCollisionTests();
  time = (float)myTime.IntervalMS();
}

void PhysicsSystem::Render( Renderer2D* a_renderer )
{
  m_grid->Draw( a_renderer );
}

void PhysicsSystem::RegisterRigidbody( PhysicsComponentBase* a_rigidbody )
{
  m_physicsObjects.push_back( a_rigidbody );
}

void PhysicsSystem::RegisterCollider( ColliderComponent* a_collider )
{
  m_grid->AddCollider( a_collider );
}

//bool PhysicsSystem::BoxCollisionTest( ColliderComponent::SharedPtr  a_boxCollider , ColliderComponent::SharedPtr a_other , Collision& a_collision )
//{
//  return false;
//}
//
//
//bool PhysicsSystem::SphereCollisionTest( ColliderComponent::SharedPtr  a_sphereCollider , ColliderComponent::SharedPtr a_other , Collision& a_collision )
//{
//  Collision collision;
//  Collider sphereCol = a_sphereCollider->GetCollider();
//  GameObject::SharedPtr go1 = a_sphereCollider->GetParentShared();
//  Transform transform1 = go1->GetTransform();
//  switch( a_other->GetColliderType() )
//  {
//  case EColliderType::Sphere:
//  {
//    Collider otherCollider = a_other->GetCollider();
//    GameObject::SharedPtr go2 = a_other->GetParentShared();
//    Transform transform2 = go2->GetTransform();
//    glm::vec2 diff = transform2.position - transform1.position;
//    float radiSum = otherCollider.radius + sphereCol.radius;
//    if( dot( diff , diff ) <= radiSum*radiSum )
//    {
//      collision.collisionNormal = glm::normalize( diff );
//      return true;
//    }
//    return false;
//    break;
//  }
//  case EColliderType::Box:
//  {
//    return false;
//    break;
//  }
//  default:
//    break;
//  }
//  return false;
//}

void PhysicsSystem::PerformCollisionTests()
{
  /*for( auto object1 = m_physicsObjects.begin(); object1 != m_physicsObjects.end(); object1++ )
  {
    for( auto object2 = object1+1; object2 != m_physicsObjects.end(); object2++ )
    {
      GameObject::SharedPtr gameObject1 = ( *object1 )->GetParentShared();
      GameObject::SharedPtr gameObject2 = ( *object2 )->GetParentShared();

      Component::SharedPtr component1 = gameObject1->GetComponentOfType( EComponentTypes::CT_ColliderComponent );
      Component::SharedPtr component2 = gameObject2->GetComponentOfType( EComponentTypes::CT_ColliderComponent );
      if( component1 && component2 )
      {
        ColliderComponent::SharedPtr collider1 = std::static_pointer_cast<ColliderComponent>( component1 );
        ColliderComponent::SharedPtr collider2 = std::static_pointer_cast<ColliderComponent>( component2 );

        if( !collider1 || !collider2 )
          continue;
        Collision resultingCollision;
        switch( collider1->GetColliderType() )
        {
        case EColliderType::Box:
        {
          if( BoxCollisionTest( collider1 , collider2 , resultingCollision ) )
          {
          }
          break;
        }
        case EColliderType::Sphere:
        {
          if( SphereCollisionTest( collider1 , collider2 , resultingCollision ) )
          {
            resultingCollision.othercollider = collider2.get();
            resultingCollision.otherGameObject = gameObject2.get();
            resultingCollision.otherRigidBody = gameObject2->GetComponentOfType( EComponentTypes::CT_RigidbodyComponent ).get();
            gameObject1->OnCollisionEnter( resultingCollision );

            resultingCollision.othercollider = collider1.get();
            resultingCollision.otherGameObject = gameObject1.get();
            resultingCollision.otherRigidBody = gameObject1->GetComponentOfType( EComponentTypes::CT_RigidbodyComponent ).get();
            gameObject2->OnCollisionEnter( resultingCollision );
          }
          break;
        }
        default:
          break;
        }
      }
    }
  }*/
}