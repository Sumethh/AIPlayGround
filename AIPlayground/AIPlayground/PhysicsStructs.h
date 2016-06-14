#pragma once

class GameObject;
class ColliderComponent;
class Component;
struct Collision
{
  GameObject* otherGameObject;
  Component* otherRigidBody;
  ColliderComponent*  othercollider;
  glm::vec2 collisionNormal;
};
