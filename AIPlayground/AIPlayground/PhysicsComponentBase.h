#pragma once
#include "Component.h"
#include <Common/Types.h>


class PhysicsComponentBase : public Component
{

public:
  typedef std::shared_ptr<PhysicsComponentBase> SharedPtr;
  PhysicsComponentBase( GameObject::SharedPtr , EComponentTypes a_types);
  virtual ~PhysicsComponentBase();
  //this will be called 50 times a second
  virtual void PhysicsUpdate(float a_fixedDt);

  
private:

};
