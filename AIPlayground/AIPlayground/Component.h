#pragma once
class GameObject;
class Component
{
public:
  Component(GameObject* a_gameObject);
  virtual ~Component();

  virtual void OnCosntruct();
  virtual void OnDestroy();
  virtual void BeginPlay();
  virtual void Update();
  virtual void PreRender();
  virtual void Render();
  virtual void PostFrame();


  bool HasBegunPlay() { return m_hasBegunPlay; }
private:
  GameObject* m_parent;

  bool m_hasBegunPlay;
};

