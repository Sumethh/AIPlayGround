#pragma once
#include "Component.h"
class RendererComponent;
class SelectableGameObjectComponent :  public Component
{
public:
  SelectableGameObjectComponent(GameObject* a_parent, EComponentTypes a_compType);
  ~SelectableGameObjectComponent();

  void BeginPlay()override;

  inline bool CanBeSelected() { return m_canBeSelected; }
  inline void SetCanBeSelected(bool a_bool) { m_canBeSelected = a_bool; }

  inline bool IsSelected() { return m_isSelected; }
  void Select();
  void Deselect();
private:
  
  bool m_isSelected;
  RendererComponent* m_renderComponent;
  bool m_canBeSelected;
};

