#pragma once
class GameObject;
class Component
{
public:
  Component( GameObject* a_gameObject );
  virtual ~Component();

  virtual void OnCosntruct();
  virtual void OnDestroy();
  virtual void Update();
  virtual void PreRender();
  virtual void Render();
private:
  GameObject* m_parent;
};

