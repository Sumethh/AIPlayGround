#pragma once
#include <SFML/Graphics/Shader.hpp>
#include "Common/Types.h"
#include <string.h>
class Shader
{
public:

  ~Shader();
  void LoadFromFile( const std::string a_vs , const std::string a_fs );
  void Bind();
  void UnBind();
  uint GetShaderHandle() { return m_shaderHandle; }
private:
  uint m_shaderHandle;
  std::string m_fs , m_vs;
};
