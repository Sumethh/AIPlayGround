#include "PhysicsSystem.h"
#include "Common/log.h"


PhysicsSystem::PhysicsSystem() :
  m_timeStep( 1.0f / 50.0f ) ,
  m_timeStepMS( m_timeStep * 1000 )
{
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Update( float a_dt )
{
  for( auto it = m_physicsObjects.begin(); it != m_physicsObjects.end(); it++ )
  {
    ( *it )->PhysicsUpdate( m_timeStep );
  }
}

void PhysicsSystem::RegisterRigidbody( std::shared_ptr<PhysicsComponentBase> a_ptr )
{
  m_physicsObjects.push_back( a_ptr );
}
