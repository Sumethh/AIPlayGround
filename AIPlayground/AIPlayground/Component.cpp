#include "Component.h"



Component::Component( GameObject* a_gameObject ) :
  m_parent( a_gameObject )
{
}


Component::~Component()
{
}

void Component::OnCosntruct()
{

}

void Component::OnDestroy()
{

}

void Component::Update()
{

}

void Component::PreRender()
{

}

void Component::Render()
{

}
