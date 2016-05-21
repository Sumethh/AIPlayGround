#pragma once
#include "PhysicsComponentBase.h"
#include <glm/vec2.hpp>

#define FORCE_MULTIPLIER 10.0f
// TODO: Think about a scaling force constant so that force applications make sense, right now small forces dont do much as positions are pixels and not world coordinates resulting in large forces needed to move objects far

class RigidbodyComponent : public PhysicsComponentBase
{
public:
  RigidbodyComponent( GameObject::SharedPtr  a_go , EComponentTypes a_compType );
  ~RigidbodyComponent();
  typedef std::shared_ptr<RigidbodyComponent> SharedPtr;
  typedef std::weak_ptr<RigidbodyComponent> WeakPtr;

  void BeginPlay() override;
  void PhysicsUpdate( float a_fixedDt ) override;

  void OnCollisionEnter( Collision& a_collision ) override;

  inline void SetDrag( const float a_drag ) { m_drag = a_drag; }
  inline void SetMass( const float a_mass ) { m_mass = a_mass; }
  inline void SetVelocity( const glm::vec2 a_velocity ) { m_velocity = a_velocity; }

  inline glm::vec2 GetVelocity()const { return m_velocity; }
  inline float GetMass()const { return m_mass; }
  inline float GetDrag()const { return m_drag; }

  inline void AddForce( glm::vec2& a_force )
  {
    m_forcesToApply += a_force * FORCE_MULTIPLIER;
  }
private:

  glm::vec2 m_velocity;
  glm::vec2 m_acceleration;
  glm::vec2 m_force;
  glm::vec2 m_forcesToApply;
  float m_drag;
  float m_mass;
};
