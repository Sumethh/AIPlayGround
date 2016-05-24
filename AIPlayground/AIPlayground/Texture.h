#pragma once
#include "Common/Types.h"
#include <SFML/Graphics/image.hpp>
#include "glm/glm.hpp"
class sf::Image;
class Texture
{
public:
  Texture();
  ~Texture();

  void Bind( uint a_slot = 0 );
  void UnBind( uint a_slot = 0 );
  void LoadFromFile( std::string a_file );
  void LoadFromIamge( sf::Image& a_image );
  glm::vec2 GetSize() { return m_size; }
private:
  uint m_textureID;
  bool m_init;
  glm::vec2 m_size;
};