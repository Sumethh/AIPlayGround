#pragma once
#include "ComponentTypes.h"
#include "GameObject.h"
class Window;
class Component
{
public:
  Component( GameObject* a_gameObject, EComponentTypes a_type );
  virtual ~Component();

  virtual void OnCosntruct();
  virtual void OnDestroy();
  virtual void BeginPlay();
  virtual void Update( float a_dt );
  virtual void PreRender();
  virtual void Render( Window* a_window );
  virtual void PostFrame();

  inline bool IsComponentOfType( EComponentTypes a_type ) { return a_type == m_componentType; }
  inline GameObject* GetParent()const { return m_parent; }
  inline bool HasBegunPlay() const { return m_hasBegunPlay; }
private:
  GameObject* m_parent;
  EComponentTypes m_componentType;
  bool m_hasBegunPlay;
};

