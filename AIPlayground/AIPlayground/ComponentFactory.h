#pragma once
#include <map>
#include <functional>
#include "ComponentTypes.h"
#include <memory>
class Component;
class GameObject;
class ComponentFactory
{
public:
  ComponentFactory();
  ~ComponentFactory();

  Component* MakeComponent( EComponentTypes a_component , GameObject*  a_gameObject );

  static ComponentFactory* GI()
  {
    if( !m_instance )
      m_instance = new ComponentFactory;
    return m_instance;
  }

private:
  static ComponentFactory* m_instance;
  std::map<EComponentTypes , std::function<Component*(GameObject* a_gameObject )>> m_functionMap;
};
