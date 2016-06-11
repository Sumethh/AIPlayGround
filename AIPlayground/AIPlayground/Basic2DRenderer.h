#pragma once
#include <common/Types.h>
#include <vector>
#include <glm/glm.hpp>

class Texture;
class Shader;
struct RenderInfo
{
  std::vector<std::pair<uint , Texture*>> textures;
  Shader* shader;
  glm::mat4 mat;
};

class Basic2DRenderer
{
public:
  Basic2DRenderer();
  ~Basic2DRenderer();
  void Init();
  void Submit( RenderInfo a_info );
  void Flush();

private:
  uint m_vbo , m_vao;
  std::vector<RenderInfo> m_renderInfo;
};
