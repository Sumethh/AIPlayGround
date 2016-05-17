#pragma once
#include <SFML/Graphics/Shader.hpp>
#include "Common/Types.h"

class Shader
{


  sf::Shader& GetShaderRef() { return m_shader; }
private:
  sf::Shader m_shader;
  uint m_shaderHandle;
};
