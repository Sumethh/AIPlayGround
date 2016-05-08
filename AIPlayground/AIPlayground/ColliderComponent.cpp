#include "ColliderComponent.h"
#include "World.h"
#include "PhysicsSystem.h"

ColliderComponent::ColliderComponent( GameObject::SharedPtr a_go , EComponentTypes a_compType , EColliderType a_colliderType ) :
  Component( a_go , a_compType ) ,
  m_colliderType( a_colliderType ) ,
  m_currentCell( nullptr )
{
  m_collider.radius = 16;
  GameObject* parent = GetParent();
  if( parent )
  {
    World* world = parent->GetWorld();
    if( world )
    {
      std::shared_ptr<PhysicsSystem> phySsystem = world->GetPhysicsSystem().lock();
      if( phySsystem )
      {
        phySsystem->RegisterCollider( this );
      }
    }
  }
}

ColliderComponent::~ColliderComponent()
{

}

void ColliderComponent::Update( float a_dt )
{
  Component::Update( a_dt );
  ResetTestedColliders();
}

bool ColliderComponent::TestCollision( ColliderComponent* a_other , Collision& a_collision )
{
  m_testedColliders[ a_other ] = true;
  switch( m_colliderType )
  {
  case EColliderType::Sphere:
  {
    switch( a_other->GetColliderType() )
    {
    case EColliderType::Sphere:
    {
      if( CircleCircleColTest( this , a_other , a_collision ) )
        return true;
      break;
    }
    case EColliderType::Box:
    {
      if( BoxCircleColTest( a_other , this , a_collision ) )
      {
        a_collision.collisionNormal *= -1;
        a_collision.othercollider = a_other;
        a_collision.otherGameObject = a_other->GetParent();
        a_collision.otherRigidBody = a_other->GetParent()->GetComponentOfType( EComponentTypes::CT_RigidbodyComponent );
        return true;
      }
      break;
    }
    default:
      break;
    }
    break;
  }
  case EColliderType::Box:
  {
    switch( a_other->GetColliderType() )
    {
    case EColliderType::Sphere:
    {
      if( BoxCircleColTest( this , a_other , a_collision ) )
        return true;
      break;
    }
    case EColliderType::Box:
    {
      if( BoxBoxColTest( this , a_other , a_collision ) )
        return true;
      break;
    }
    default:
      break;
    }
    break;
  }
  default:
    break;
  }
  return false;
}

bool ColliderComponent::CircleCircleColTest( ColliderComponent* a_collider1 , ColliderComponent* a_collider2 , Collision& a_collision )
{
  Transform transform1 = a_collider1->GetParent()->GetTransform();
  Transform transform2 = a_collider1->GetParent()->GetTransform();
  Collider collider1 = a_collider1->GetCollider();
  Collider collider2 = a_collider2->GetCollider();


  glm::vec2 diff = transform2.position - transform1.position;
  float radisqrd = collider1.radius + collider2.radius;
  radisqrd *= radisqrd;

  if( glm::dot( diff , diff ) <= radisqrd )
  {
    a_collision.othercollider = a_collider2;
    a_collision.otherGameObject = a_collider2->GetParent();
    a_collision.otherRigidBody = a_collider2->GetParent()->GetComponentOfType( EComponentTypes::CT_RigidbodyComponent );
    a_collision.collisionNormal = glm::normalize( diff );
    return true;
  }
  return false;

}

bool ColliderComponent::BoxBoxColTest( ColliderComponent* a_collider1 , ColliderComponent* a_collider2 , Collision& a_collision )
{
  return false;
}

bool ColliderComponent::BoxCircleColTest( ColliderComponent* a_box , ColliderComponent* a_circle , Collision& a_collision )
{
  return false;
}
