#include "ColliderComponent.h"
#include "World.h"
#include "PhysicsSystem.h"
#include "Common/log.h"
#include "Common/HelperFunctions.h"
#include "Common/Window.h"
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

void ColliderComponent::Render( Window* a_window )
{
  if( m_colliderType == EColliderType::Sphere )
  {
    sf::CircleShape circle;
    circle.setFillColor( sf::Color::Transparent );
    circle.setOutlineColor( sf::Color::Blue );
    circle.setOutlineThickness( 5.0f );
    circle.setPosition( ConvertVec2( GetParent()->GetTransform().position ) );
    circle.setRadius( m_collider.radius );
    circle.setOrigin( ConvertVec2( glm::vec2( m_collider.radius , m_collider.radius ) ) );
    a_window->RenderDrawable( circle );
  }
}

void ColliderComponent::OnCollisionEnter( Collision& a_collision )
{
  Component::OnCollisionEnter( a_collision );
  //LOGI( "Collided" );
}

bool ColliderComponent::CircleCircleColTest( ColliderComponent* a_collider1 , ColliderComponent* a_collider2 , Collision& a_collision )
{
  Transform transform1 = a_collider1->GetParent()->GetTransform();
  Transform transform2 = a_collider2->GetParent()->GetTransform();
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
    //a_collision.collisionNormal = glm::normalize( diff );
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

  Transform boxTransform = a_box->GetParent()->GetTransform();
  Transform circleTransform = a_circle->GetParent()->GetTransform();
  Collider boxCollider = a_box->GetCollider();
  Collider circleCollider = a_circle->GetCollider();

  glm::vec2& boxPos = boxTransform.position;
  glm::vec2& circlePos = circleTransform.position;

  glm::vec2 unrotatedCirclePos;

  unrotatedCirclePos.x = glm::cos( boxTransform.rotation ) * ( circlePos.x - boxPos.x ) -
    glm::sin( boxTransform.rotation ) * ( circlePos.y - boxPos.y ) + boxPos.x;
  unrotatedCirclePos.y = glm::sin( boxTransform.rotation ) * ( circlePos.x - boxPos.x ) +
    glm::cos( boxTransform.rotation ) * ( circlePos.y - boxPos.y ) + boxPos.y;

  glm::vec2 closestPoints( 0.0f , 0.0f );
  glm::vec2 boxTopLeft = boxPos - boxCollider.extents / 2.0f;
  glm::vec2 boxBottomRight = boxPos + boxCollider.extents / 2.0f;

  if( circlePos.x < boxTopLeft.x )
    closestPoints.x = boxTopLeft.x;
  else if( circlePos.x > boxBottomRight.x )
    closestPoints.x = boxBottomRight.x;
  else
    closestPoints.x = circlePos.x;

  if( circlePos.y < boxTopLeft.y )
    closestPoints.y = boxTopLeft.y;
  else if( circlePos.y > boxBottomRight.y )
    closestPoints.y = boxBottomRight.y;
  else
    closestPoints.y = circlePos.y;

  glm::vec2 vecBetween = closestPoints - circlePos;
  float distanceSqrd = glm::dot( vecBetween , vecBetween );
  if( distanceSqrd < circleCollider.radius * circleCollider.radius )
  {
    LOGI( "BOX CIRCLE COLLISION" );
  }


  return false;
}
