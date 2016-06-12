#include "RendererComponent.h"
#include "Common/Window.h"
#include "Transform.h"
#include "Common/HelperFunctions.h"
#include "Renderer2D.h"
#include "world.h"
#include "Camera.h"

RendererComponent::RendererComponent(GameObject*   a_gameObject, EComponentTypes a_type) :
  Component(a_gameObject, a_type),
  m_renderType(RenderType::Dynamic),
  m_active(true),
  m_renderRotated(true)
{
}

RendererComponent::~RendererComponent()
{
}

void RendererComponent::OnCosntruct()
{
  ChangeTextureID(0);
}


void RendererComponent::PreRender()
{
  Component::PreRender();
  if (GetParent()->GetRenderStateDirty())
  {
    if (m_registered && m_renderType == RenderType::Static)
    {
      StaticRenderer& renderer = Renderer2D::GI()->GetStaticRenderer();
      renderer.UpdatePosition(m_registeredID, GetParent()->GetTransform().MakeMatrix());
      renderer.UpdateTexCoords(m_registeredID, glm::vec4(m_textureCoordInfo.topLeft, m_textureCoordInfo.topRight), glm::vec4(m_textureCoordInfo.bottomLeft, m_textureCoordInfo.bottomRight));
    }
  }
}

void RendererComponent::Render(Renderer2D* a_renderer)
{
  if (!m_active)
    return;
  SpriteBatchRenderer& spriteRender = a_renderer->GetSpriteBatchRenderer();
  World* world = GetParent()->GetWorld();
  Camera* camera = world->GetCamera();
  Transform trans = GetParent()->GetTransform();
  ELayerID layer = GetParent()->GetLayer();
  spriteRender.Submit(glm::vec3(trans.position - camera->GetPos(), (uint8)layer / 10.0f), trans.scale * m_textureCoordInfo.sizeInPixels, trans.rotation * (int)m_renderRotated,
    glm::vec4(m_textureCoordInfo.topLeft, m_textureCoordInfo.topRight), glm::vec4(m_textureCoordInfo.bottomLeft, m_textureCoordInfo.bottomRight));

}
void RendererComponent::ChangeTextureID(const uint a_newTextureID)
{
  m_textureID = a_newTextureID;
  switch (m_renderType)
  {
  case RenderType::Dynamic:
    m_textureCoordInfo = TextureManager::GI()->GetTextureCoordInfo(ETextureID::DynamicSpriteSheet, (uint8)a_newTextureID);
    if (m_registered)
    {
      StaticRenderer& renderer = Renderer2D::GI()->GetStaticRenderer();
      renderer.UpdateTexCoords(m_registeredID, glm::vec4(m_textureCoordInfo.topLeft, m_textureCoordInfo.topRight), glm::vec4(m_textureCoordInfo.bottomLeft, m_textureCoordInfo.bottomRight));
    }
    break;
  case RenderType::Static:
    m_textureCoordInfo = TextureManager::GI()->GetTextureCoordInfo(ETextureID::StaticSpriteSheet, (uint8)a_newTextureID);
    break;
  default:
    break;
  }
}

void RendererComponent::ChangeRenderType(RenderType a_type)
{
  if (m_renderType == RenderType::Static)
  {
    m_registered = false;
    StaticRenderer& renderer = Renderer2D::GI()->GetStaticRenderer();
    renderer.UnRegister(m_registeredID);
    m_registeredID = 0;
  }
  m_renderType = a_type;
  if (m_renderType == RenderType::Static)
  {
    StaticRenderer& renderer = Renderer2D::GI()->GetStaticRenderer();
    m_registered = true;
    m_registeredID = renderer.Register();
    renderer.UpdatePosition(m_registeredID, GetParent()->GetTransform().MakeMatrix());
    renderer.UpdateTexCoords(m_registeredID, glm::vec4(m_textureCoordInfo.topLeft, m_textureCoordInfo.topRight), glm::vec4(m_textureCoordInfo.bottomLeft, m_textureCoordInfo.bottomRight));
  }
}
