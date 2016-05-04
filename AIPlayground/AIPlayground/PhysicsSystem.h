#pragma once
#include "PhysicsComponentBase.h"
#include <Common/timer.h>

class PhysicsSystem
{
public:
  typedef std::shared_ptr<PhysicsSystem> SharedPtr;
  typedef std::weak_ptr<PhysicsSystem> WeakPtr;
  PhysicsSystem();
  ~PhysicsSystem();

  void Update( float a_dt );

  void RegisterRigidbody( std::shared_ptr<PhysicsComponentBase> a_ptr );

private:
  std::vector<std::shared_ptr<PhysicsComponentBase>> m_physicsObjects;

  const double m_timeStep;
  const double m_timeStepMS;
};

