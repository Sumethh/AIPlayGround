#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include "TextureManager.h"
enum class RenderType:uint8
{
  Dynamic,
  Static
};
class RendererComponent : public Component
{
public:
  RendererComponent(GameObject*, EComponentTypes a_type );
  ~RendererComponent();

  void OnCosntruct() override;
  void PreRender() override;
  void Render( Renderer2D* a_window ) override;

  void SetActive(bool a_active) { m_active = a_active; }
  inline bool IsActive() { return m_active; }

  void ChangeTextureID(const uint a_newTextureID);
  inline uint GetTextureID() { return m_textureID; }

  void ChangeRenderType(RenderType a_type);
  RenderType GetRenderType() { return m_renderType; }

  void SetRenderRotated(bool a_bool) {m_renderRotated = a_bool;}
  bool GetRenderRotated() { return m_renderRotated; }
private:
  TextureCoordInfo m_textureCoordInfo;
  RenderType m_renderType;
  uint m_textureID;
  bool m_active;
  bool m_renderRotated;
  int m_registeredID;
  bool m_registered;
};
