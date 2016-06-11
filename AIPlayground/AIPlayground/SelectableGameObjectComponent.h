#pragma once
#include "Component.h"
class SelectableGameObjectComponent :  public Component
{
public:
  SelectableGameObjectComponent(GameObject* a_parent);
  ~SelectableGameObjectComponent();

  inline bool CanBeSelected() { return m_canBeSelected; }
  inline void SetCanBeSelected(bool a_bool) { m_canBeSelected = a_bool; }
private:

  bool m_canBeSelected;
};

