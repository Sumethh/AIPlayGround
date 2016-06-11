#include "ColliderComponent.h"
#include "World.h"
#include "PhysicsSystem.h"
#include "Common/log.h"
#include "Common/HelperFunctions.h"
#include "Common/Window.h"
ColliderComponent::ColliderComponent(GameObject::SharedPtr a_go, EComponentTypes a_compType, EColliderType a_colliderType) :
  Component(a_go, a_compType),
  m_colliderType(a_colliderType),
  m_cellsRegistered(4)
{
  m_collider.radius = 16;
  GameObject* parent = GetParent();
  if (parent)
  {
    World* world = parent->GetWorld();
    if (world)
    {
      std::shared_ptr<PhysicsSystem> phySsystem = world->GetPhysicsSystem().lock();
      if (phySsystem)
      {
        phySsystem->RegisterCollider(this);
      }
    }
  }
}

ColliderComponent::~ColliderComponent()
{
}

void CalculateRotatedVerts(Collider& a_collider, Transform& a_transform)
{
  const float cosResult = glm::cos(a_transform.rotation);
  const float sinResult = glm::sin(a_transform.rotation);

  glm::vec2& position = a_transform.position;
  glm::vec2& halfExtents = a_collider.extents * 0.5f;

  // [0] TopLeft [1] BottomLeft [2] TopRight [2]BottomRight

  __m128 xPositions, yPositions, cosResultm128, sinResultm128;
  union
  {
    float xFinalPosition[4];
    __m128 finalPositionsX;
  };
  union
  {
    float yFinalPosition[4];
    __m128 finalPositionsY;
  };

  xPositions = _mm_set_ps(-halfExtents.x, -halfExtents.x, halfExtents.x, halfExtents.x);
  yPositions = _mm_set_ps(-halfExtents.y, halfExtents.y, -halfExtents.y, halfExtents.y);
  cosResultm128 = _mm_set_ps1(cosResult);
  sinResultm128 = _mm_set_ps1(sinResult);

  finalPositionsX = _mm_sub_ps(_mm_mul_ps(xPositions, cosResultm128), _mm_mul_ps(sinResultm128, yPositions));

  finalPositionsY = _mm_add_ps(_mm_mul_ps(xPositions, sinResultm128), _mm_mul_ps(yPositions, cosResultm128));

  a_collider.rotatedVertices[0] = glm::vec2(xFinalPosition[0] + position.x, yFinalPosition[0] + position.y);
  a_collider.rotatedVertices[1] = glm::vec2(xFinalPosition[2] + position.x, yFinalPosition[2] + position.y);
  a_collider.rotatedVertices[2] = glm::vec2(xFinalPosition[1] + position.x, yFinalPosition[1] + position.y);
  a_collider.rotatedVertices[3] = glm::vec2(xFinalPosition[3] + position.x, yFinalPosition[3] + position.y);
}

void ColliderComponent::CalculateTestingVerts(Collider& a_collider, Transform& a_transform)
{
  static glm::vec2 circleTests[4] = { glm::normalize(glm::vec2(-1.0f , 1.0f)) , glm::normalize(glm::vec2(1.0f , -1.0f)), glm::normalize(glm::vec2(1.0f , 1.0f)),glm::normalize(glm::vec2(-1.0f , -1.0f)) };

  switch (m_colliderType)
  {
  case EColliderType::Sphere:
  {
    a_collider.testableVerts[0] = a_transform.position + circleTests[0] * a_collider.radius;
    a_collider.testableVerts[1] = a_transform.position + glm::vec2(0.0f, -1.0f)*a_collider.radius;
    a_collider.testableVerts[2] = a_transform.position + circleTests[1] * a_collider.radius;
    a_collider.testableVerts[3] = a_transform.position + glm::vec2(1.0f, 0.0f)*a_collider.radius;
    a_collider.testableVerts[4] = a_transform.position + circleTests[2] * a_collider.radius;
    a_collider.testableVerts[5] = a_transform.position + glm::vec2(0.0f, 1.0f)*a_collider.radius;
    a_collider.testableVerts[6] = a_transform.position + circleTests[3] * a_collider.radius;
    a_collider.testableVerts[7] = a_transform.position + glm::vec2(-1.0f, 0.0f)*a_collider.radius;
    break;
  }
  case EColliderType::Box:
  {
    glm::vec2 halfExtents = a_collider.extents / 2.0f;
    a_collider.testableVerts[0] = a_collider.rotatedVertices[0];
    a_collider.testableVerts[1] = glm::normalize(a_collider.rotatedVertices[1] - a_collider.rotatedVertices[0]) * halfExtents + a_collider.rotatedVertices[0];
    a_collider.testableVerts[2] = a_collider.rotatedVertices[1];
    a_collider.testableVerts[3] = glm::normalize(a_collider.rotatedVertices[3] - a_collider.rotatedVertices[1]) * halfExtents + a_collider.rotatedVertices[1];
    a_collider.testableVerts[4] = a_collider.rotatedVertices[2];
    a_collider.testableVerts[5] = glm::normalize(a_collider.rotatedVertices[3] - a_collider.rotatedVertices[2]) * halfExtents + a_collider.rotatedVertices[2];
    a_collider.testableVerts[6] = a_collider.rotatedVertices[3];
    a_collider.testableVerts[7] = glm::normalize(a_collider.rotatedVertices[0] - a_collider.rotatedVertices[2]) * halfExtents + a_collider.rotatedVertices[2];
    break;
  }
  default:
    break;
  }
}

void ColliderComponent::Update(float a_dt)
{
  Component::Update(a_dt);
  GameObject* parent = GetParent();
  if (parent->GetCollisionDirtyFlag())
  {
    if (m_colliderType == EColliderType::Box)
    {
      //Calculate rotated vertices;
      CalculateRotatedVerts(m_collider, parent->GetTransform());
      CalculateTestingVerts(m_collider, parent->GetTransform());
      parent->ResetCollisionDirtyFlag();
    }
    else
    {
      CalculateTestingVerts(m_collider, parent->GetTransform());
      parent->ResetCollisionDirtyFlag();
    }
  }
  ResetTestedColliders();
}

bool ColliderComponent::TestCollision(ColliderComponent* a_other, Collision& a_collision)
{
  switch (m_colliderType)
  {
  case EColliderType::Sphere:
  {
    switch (a_other->GetColliderType())
    {
    case EColliderType::Sphere:
    {
      if (CircleCircleColTest(this, a_other, a_collision))
        return true;
      break;
    }
    case EColliderType::Box:
    {
      if (BoxCircleColTest(a_other, this, a_collision))
      {
        a_collision.collisionNormal *= -1;
        a_collision.othercollider = a_other;
        a_collision.otherGameObject = a_other->GetParent();
        a_collision.otherRigidBody = a_other->GetParent()->GetComponentOfType(EComponentTypes::CT_RigidbodyComponent);
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
    switch (a_other->GetColliderType())
    {
    case EColliderType::Sphere:
    {
      if (BoxCircleColTest(this, a_other, a_collision))
        return true;
      break;
    }
    case EColliderType::Box:
    {
      if (BoxBoxColTest(this, a_other, a_collision))
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

void ColliderComponent::Render(Window* a_window)
{
  return;
  Camera* cam = GetParent()->GetWorld()->GetCamera().lock().get();
  if (m_colliderType == EColliderType::Sphere)
  {
    sf::CircleShape circle;
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Blue);
    circle.setOutlineThickness(2.0f);
    circle.setPosition(ConvertVec2(GetParent()->GetTransform().position - cam->GetPos()));
    circle.setRadius(m_collider.radius);
    circle.setOrigin(ConvertVec2(glm::vec2(m_collider.radius, m_collider.radius)));
    //a_window->RenderDrawable( circle );
    circle.setRadius(2.0f);
    circle.setOrigin(2.0f, 2.0f);
    for (int i = 0; i < 8; i++)
    {
      circle.setPosition(ConvertVec2(m_collider.testableVerts[i]));
      a_window->RenderDrawable(circle);
    }
  }
  else if (m_colliderType == EColliderType::Box)
  {
    sf::RectangleShape rect;
    rect.setSize(ConvertVec2(m_collider.extents));
    rect.setOrigin(ConvertVec2(m_collider.extents / 2.0f));
    rect.setOutlineThickness(2.0f);
    rect.setRotation(GetParent()->GetTransform().rotation);
    rect.setPosition(ConvertVec2(GetParent()->GetTransform().position - cam->GetPos()));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Blue);
    //a_window->RenderDrawable( rect );
    rect.setSize(sf::Vector2f(2.0f, 2.0f));
    rect.setOrigin(1.0f, 1.0f);
    //for( int i = 0; i < 8; i++ )
    //{
    //  rect.setPosition( ConvertVec2( m_collider.testableVerts[ i ] ) );
    //  a_window->RenderDrawable( rect );
    //}
    for (int i = 0; i < 8; i++)
    {
      sf::CircleShape circle;
      circle.setRadius(2.0f);
      circle.setFillColor(sf::Color::Transparent);
      circle.setOutlineColor(sf::Color::Blue);
      circle.setOutlineThickness(1.0f);
      circle.setPosition(ConvertVec2(m_collider.testableVerts[i]));
      circle.setOrigin(ConvertVec2(glm::vec2(0.5f, 0.5f)));
      a_window->RenderDrawable(circle);
    }
  }
}

void ColliderComponent::OnCollisionEnter(Collision& a_collision)
{
  Component::OnCollisionEnter(a_collision);
  //LOGI( "Collided" );
}

bool ColliderComponent::CollidesWithPoint(glm::vec2 a_point)
{
  switch (m_colliderType)
  {
  case EColliderType::Sphere:
  {

    glm::vec2 diff = a_point - GetParent()->GetTransform().position;
    if (glm::dot(diff, diff) <= m_collider.radius*m_collider.radius)
      return true;
    break;
  }
  case EColliderType::Box:
  {
    break;
  }
  default:
    break;
  }
  return false;
}

bool ColliderComponent::CollidesWithTestableCollider(TestableCollider a_collider)
{
  switch (m_colliderType)
  {
  case EColliderType::Sphere:
  {
    glm::vec2 diff = a_collider.position - GetParent()->GetTransform().position;
    if (glm::dot(diff, diff) <= m_collider.radius * m_collider.radius)
      return true;
    break;
  }
  case EColliderType::Box:
    break;
  default:
    break;
  }
  return false;
}

bool ColliderComponent::CircleCircleColTest(ColliderComponent* a_collider1, ColliderComponent* a_collider2, Collision& a_collision)
{
  Transform transform1 = a_collider1->GetParent()->GetTransform();
  Transform transform2 = a_collider2->GetParent()->GetTransform();
  Collider collider1 = a_collider1->GetCollider();
  Collider collider2 = a_collider2->GetCollider();

  glm::vec2 diff = transform2.position - transform1.position;
  float radisqrd = collider1.radius + collider2.radius;
  radisqrd *= radisqrd;

  if (glm::dot(diff, diff) <= radisqrd)
  {
    a_collision.othercollider = a_collider2;
    a_collision.otherGameObject = a_collider2->GetParent();
    a_collision.otherRigidBody = a_collider2->GetParent()->GetComponentOfType(EComponentTypes::CT_RigidbodyComponent);
    //a_collision.collisionNormal = glm::normalize( diff );
    return true;
  }
  return false;
}

bool ColliderComponent::BoxBoxColTest(ColliderComponent* a_collider1, ColliderComponent* a_collider2, Collision& a_collision)
{
  return false;
}

bool ColliderComponent::BoxCircleColTest(ColliderComponent* a_box, ColliderComponent* a_circle, Collision& a_collision)
{
  Transform boxTransform = a_box->GetParent()->GetTransform();
  Transform circleTransform = a_circle->GetParent()->GetTransform();
  Collider boxCollider = a_box->GetCollider();
  Collider circleCollider = a_circle->GetCollider();

  glm::vec2& boxPos = boxTransform.position;
  glm::vec2& circlePos = circleTransform.position;

  glm::vec2 unrotatedCirclePos;

  unrotatedCirclePos.x = glm::cos(boxTransform.rotation) * (circlePos.x - boxPos.x) -
    glm::sin(boxTransform.rotation) * (circlePos.y - boxPos.y) + boxPos.x;
  unrotatedCirclePos.y = glm::sin(boxTransform.rotation) * (circlePos.x - boxPos.x) +
    glm::cos(boxTransform.rotation) * (circlePos.y - boxPos.y) + boxPos.y;

  glm::vec2 closestPoints(0.0f, 0.0f);
  glm::vec2 boxTopLeft = boxPos - boxCollider.extents / 2.0f;
  glm::vec2 boxBottomRight = boxPos + boxCollider.extents / 2.0f;

  if (circlePos.x < boxTopLeft.x)
    closestPoints.x = boxTopLeft.x;
  else if (circlePos.x > boxBottomRight.x)
    closestPoints.x = boxBottomRight.x;
  else
    closestPoints.x = circlePos.x;

  if (circlePos.y < boxTopLeft.y)
    closestPoints.y = boxTopLeft.y;
  else if (circlePos.y > boxBottomRight.y)
    closestPoints.y = boxBottomRight.y;
  else
    closestPoints.y = circlePos.y;

  glm::vec2 vecBetween = closestPoints - circlePos;
  float distanceSqrd = glm::dot(vecBetween, vecBetween);
  if (distanceSqrd < circleCollider.radius * circleCollider.radius)
  {
    return true;
  }

  return false;
}