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

#include <common/log.h>
void RendererComponent::PreRender()
{
  Component::PreRender();
  if (GetParent()->GetRenderStateDirty())
  {
    if (m_registered && m_renderType == RenderType::Static)
    {
      StaticRenderer& renderer = Renderer2D::GI()->GetStaticRenderer();
      glm::mat4 mat = GetParent()->GetTransform().MakeMatrix(m_textureCoordInfo.sizeInPixels);
        float z = (uint8)GetParent()->GetLayer() / 10.0f;
        mat[ 3 ][ 2 ] = z;
      renderer.UpdatePosition(m_registeredID, mat);
    }
    GetParent()->ResetRenderStateDirtyFlag();
  }
}

void RendererComponent::Render(Renderer2D* a_renderer)
{
  if (!m_active || m_renderType == RenderType::Static)  
    return;
  SpriteBatchRenderer& spriteRender = a_renderer->GetSpriteBatchRenderer();
  World* world = GetParent()->GetWorld();
  Camera* camera = world->GetCamera();
  Transform trans = GetParent()->GetTransform();
  ELayerID layer = GetParent()->GetLayer();
  float z = (uint8)layer / 10.0f;
  spriteRender.Submit(glm::vec3(trans.position - camera->GetPos(), z), trans.scale * m_textureCoordInfo.sizeInPixels, trans.rotation * (int)m_renderRotated,
    glm::vec4(m_textureCoordInfo.topLeft, m_textureCoordInfo.topRight), glm::vec4(m_textureCoordInfo.bottomLeft, m_textureCoordInfo.bottomRight));

}
void RendererComponent::ChangeTextureID(const uint a_newTextureID)
{
  m_textureID = a_newTextureID;
  switch (m_renderType)
  {
  case RenderType::Dynamic:
    m_textureCoordInfo = TextureManager::GI()->GetTextureCoordInfo(ETextureID::DynamicSpriteSheet, (uint8)a_newTextureID);   
    break;
  case RenderType::Static:
    m_textureCoordInfo = TextureManager::GI()->GetTextureCoordInfo(ETextureID::StaticSpriteSheet, (uint8)a_newTextureID);
    if (m_registered)
    {
      StaticRenderer& renderer = Renderer2D::GI()->GetStaticRenderer();
      renderer.UpdateTexCoords(m_registeredID, glm::vec4(m_textureCoordInfo.topLeft, m_textureCoordInfo.topRight), glm::vec4(m_textureCoordInfo.bottomLeft, m_textureCoordInfo.bottomRight));
    }
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
    m_textureCoordInfo = TextureManager::GI()->GetTextureCoordInfo(ETextureID::StaticSpriteSheet, m_textureID);
    renderer.UpdatePosition(m_registeredID, GetParent()->GetTransform().MakeMatrix());
    renderer.UpdateTexCoords(m_registeredID, glm::vec4(m_textureCoordInfo.topLeft, m_textureCoordInfo.topRight), glm::vec4(m_textureCoordInfo.bottomLeft, m_textureCoordInfo.bottomRight));
    GetParent()->SetLayer( ELayerID::StaticObject );
  }
}
