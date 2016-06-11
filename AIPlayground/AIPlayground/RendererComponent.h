#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
class RendererComponent : public Component
{
public:
  RendererComponent(GameObject*, EComponentTypes a_type );
  ~RendererComponent();

  void PreRender() override;
  void Render( Renderer2D* a_window ) override;

  void SetTexture( sf::Texture* a_texture );

private:
  sf::Sprite m_sprite;
  sf::Texture* m_texture;
  glm::vec2 m_dimensions;
};
