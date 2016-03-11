#pragma once
#include "Component.h"
class RendererComponent : public Component
{
public:
  RendererComponent(GameObject* a_gameObject);
  ~RendererComponent();
};

