#include "Component.h"

Component::Component( GameObject::SharedPtr  a_gameObject , EComponentTypes a_componentType ) :
  m_parent( a_gameObject ) ,
  m_componentType( a_componentType )
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

void Component::BeginPlay()
{
  m_hasBegunPlay = true;
}

void Component::Update( float a_dt )
{
}

void Component::FixedUpdate( float a_dt )
{
}

void Component::PreRender()
{
}

void Component::Render( Window* a_window )
{
}

void Component::PostFrame()
{
}

void Component::OnCollisionEnter( Collision& a_collision )
{
}

void Component::OnCollisionLeave( Collision& a_collision )
{
}

void Component::OnCollisionStay( Collision& a_collision )
{
}