#include "GameObject.h"



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::OnCosntruct()
{

}

void GameObject::OnDestroy()
{

}

void GameObject::Update()
{

}

void GameObject::PreRender()
{

}

void GameObject::Render()
{

}

void GameObject::PostFrame()
{
  for( int i = 0; i < m_futureComponents.size(); ++i )
    m_components.push_back( m_futureComponents[ i ] );
  m_futureComponents.clear();

}

Component* GameObject::AddComponent( Component* a_newComponent )
{

}
