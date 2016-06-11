#include "SelectableGameObjectComponent.h"



SelectableGameObjectComponent::SelectableGameObjectComponent(GameObject* a_parent) :
  Component(a_parent, EComponentTypes::CT_SelectableGameObjectComponent)
{
}


SelectableGameObjectComponent::~SelectableGameObjectComponent()
{
}
