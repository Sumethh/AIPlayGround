#pragma once
#include "Component.h"
#include "Common/Types.h"

enum class EColliderType : uint8
{
  Sphere,
  Box
};



class ColliderComponent : Component
{
public:
  ColliderComponent( GameObject::SharedPtr a_go , EComponentTypes a_compType , EColliderType a_colliderType );
  virtual ~ColliderComponent();
  inline EColliderType GetColliderType() { return m_colliderType; }

private:
  EColliderType m_colliderType;
};