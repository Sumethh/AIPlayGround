#include "ColliderComponent.h"


ColliderComponent::ColliderComponent( GameObject::SharedPtr a_go , EComponentTypes a_compType , EColliderType a_colliderType ):
  Component(a_go, a_compType)
{

}

ColliderComponent::~ColliderComponent()
{

}

