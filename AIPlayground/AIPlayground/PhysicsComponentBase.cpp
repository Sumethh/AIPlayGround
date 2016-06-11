#include "PhysicsComponentBase.h"

PhysicsComponentBase::PhysicsComponentBase( GameObject*  a_parent , EComponentTypes a_types ) :
  Component::Component( a_parent , a_types )
{
}

PhysicsComponentBase::~PhysicsComponentBase()
{
}

void PhysicsComponentBase::PhysicsUpdate( float a_fixedDt )
{
}