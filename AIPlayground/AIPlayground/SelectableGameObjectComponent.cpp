#include "SelectableGameObjectComponent.h"
#include "RendererComponent.h"


SelectableGameObjectComponent::SelectableGameObjectComponent(GameObject* a_parent, EComponentTypes a_compType) :
  Component(a_parent, a_compType)
{
}


SelectableGameObjectComponent::~SelectableGameObjectComponent()
{
}


void SelectableGameObjectComponent::BeginPlay()
{
  Component::BeginPlay();
  m_renderComponent = (RendererComponent*)GetParent()->AddComponent(EComponentTypes::CT_RenderComponent);
  {
    if (m_renderComponent)
    {
      m_renderComponent->SetActive(false);
      m_renderComponent->ChangeRenderType(RenderType::Dynamic);
      m_renderComponent->ChangeTextureID(1);
      m_renderComponent->SetRenderRotated(false);
    }
  }
}

void SelectableGameObjectComponent::Select()
{
  m_isSelected = true;
  if (m_renderComponent)
  {
    m_renderComponent->SetActive(m_isSelected);
  }
}

void SelectableGameObjectComponent::Deselect()
{
  m_isSelected = false;
  if (m_renderComponent)
  {
    m_renderComponent->SetActive(m_isSelected);
  }
}
