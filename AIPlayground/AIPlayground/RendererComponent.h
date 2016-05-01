#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
class RendererComponent : public Component
{
public:
  RendererComponent( std::weak_ptr<GameObject>  a_gameObject , EComponentTypes a_type );
  ~RendererComponent();


  void PreRender() override;
  void Render( Window* a_window ) override;

  void SetTexture( sf::Texture* a_texture );

private:
  sf::Sprite m_sprite;
  sf::Texture* m_texture;
  glm::vec2 m_dimensions;
};

