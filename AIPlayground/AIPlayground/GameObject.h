#pragma once
#include <vector>
#include "Transform.h"
#include "ComponentFactory.h"
#include "World.h"
class Component;
class World;
class GameObject
{
  friend class World;
public:
  GameObject();
  virtual ~GameObject();

  virtual void OnCosntruct();
  virtual void OnDestroy();
  virtual void BeginPlay();
  virtual void Update();
  virtual void PreRender();
  virtual void Render();
  virtual void PostFrame();

  void AddComponent( EComponentTypes a_componentType );

  inline Transform GetTransform()const { return m_transform; }
  inline void SetTransfrom( const Transform a_newTransform ) { m_transform = a_newTransform; }

  inline bool HasBegunPlay() { return m_hasBegunPlay; }
  inline bool IsDestroyed() { return m_toBeDestroyed; }

  World* GetWorld() { return m_world; }

private:
  void SetWorld( World* a_newWorld ) { m_world = a_newWorld; };

  std::vector<Component*> m_components;
  std::vector<Component*> m_componentsToAdd;

  Transform m_transform;
  World* m_world;

  bool m_hasBegunPlay;
  bool m_toBeDestroyed;
};

