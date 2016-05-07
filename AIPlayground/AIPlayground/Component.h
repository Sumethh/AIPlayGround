#pragma once
#include "ComponentTypes.h"
#include "GameObject.h"
#include <memory>
class Window;
class Component
{
public:
  typedef std::shared_ptr<Component> SharedPtr;
  Component( GameObject::SharedPtr a_gameObject , EComponentTypes a_type );
  virtual ~Component();

  virtual void OnCosntruct();
  virtual void OnDestroy();
  virtual void BeginPlay();
  virtual void Update( float a_dt );
  virtual void FixedUpdate( float a_dt );
  virtual void PreRender();
  virtual void Render( Window* a_window );
  virtual void PostFrame();

  virtual void OnCollisionEnter( Collision& a_collision );
  virtual void OnCollisionLeave( Collision& a_collision );
  virtual void OnCollisionStay( Collision& a_collision );





  inline bool IsComponentOfType( EComponentTypes a_type ) { return a_type == m_componentType; }
  inline GameObject* GetParent()const { return m_parent.get(); }
  inline std::shared_ptr<GameObject> GetParentShared()const { return m_parent; }
  inline bool HasBegunPlay() const { return m_hasBegunPlay; }
  inline EComponentTypes GetComponentType() { return m_componentType; }
private:
  GameObject::SharedPtr  m_parent;
  EComponentTypes m_componentType;
  bool m_hasBegunPlay;
};

