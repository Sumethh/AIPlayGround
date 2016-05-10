#include "RigidbodyComponent.h"
#include "PhysicsSystem.h"
#include "World.h"
#include "Common/log.h"

RigidbodyComponent::RigidbodyComponent( GameObject::SharedPtr a_go , EComponentTypes a_compType ) :
  PhysicsComponentBase( a_go , a_compType ) ,
  m_velocity( 0.0f , 0.0f ) ,
  m_acceleration( 0.0f , 0.0f ) ,
  m_force( 0 , 0 ) ,
  m_forcesToApply( 0.0f , 0.0f ) ,
  m_drag( 1.0f ) ,
  m_mass( 1.f )
{

}

RigidbodyComponent::~RigidbodyComponent()
{
  m_velocity.x = 0.0f;
  m_velocity.y = 0.0f;
  m_mass = 0.0f;
}

void RigidbodyComponent::BeginPlay()
{
  GameObject::SharedPtr parent = GetParentShared();
  if( parent )
  {
    Component* comp = parent->GetComponentOfType( GetComponentType() );
    if( comp == this )
    {
      PhysicsSystem::WeakPtr physicsSys = parent->GetWorld()->GetPhysicsSystem();
      if( !physicsSys.expired() )
      {
        PhysicsSystem::SharedPtr physics = physicsSys.lock();
        physics->RegisterRigidbody( (PhysicsComponentBase*)comp );
      }
    }
  }
}

void RigidbodyComponent::PhysicsUpdate( float a_dt )
{
  PhysicsComponentBase::PhysicsUpdate( a_dt );
  if( GetParent() )
  {
    GameObject* parent = GetParent();
    Transform parentTransform = parent->GetTransform();
    parentTransform.position += m_velocity * a_dt;
    parent->SetTransfrom( parentTransform );
    m_force += m_forcesToApply;
    m_forcesToApply *= 0;
    // Calculate Drag
    float drag = ( 1.0f - a_dt*m_drag );
    // Calculate Acceleration
    m_acceleration = m_force / m_mass;
    // Apply Accleration and Drag
    m_velocity += m_acceleration;
    m_velocity *= drag;

    //Reset Force 
    m_force *= 0.0f;
  }
}
bool collided = false;
void RigidbodyComponent::OnCollisionEnter( Collision& a_collision )
{
  if( !collided )
  {
    m_velocity *= -1;
    collided = true;
  }
}
