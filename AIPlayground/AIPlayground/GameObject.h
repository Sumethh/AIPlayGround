#pragma once
#include "Component.h"
#include <vector>
class GameObject
{
public:
  GameObject();
  virtual ~GameObject();

  virtual void OnCosntruct();
  virtual void OnDestroy();
  virtual void Update();
  virtual void PreRender();
  virtual void Render();
  void PostFrame();
  Component* AddComponent( Component* a_newComponent );

private:

  std::vector<Component*> m_components;
  std::vector<Component*> m_futureComponents;

};

