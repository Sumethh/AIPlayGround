#pragma once
#include <map>
#include "common/Types.h"
#include "Shader.h"
enum class EShaderID : uint8
{
  SpriteBatch ,
  LineRender ,
  BasicRender,
  StaticRender
};
class ShaderManager
{
public:
  ShaderManager();
  ~ShaderManager();

  void Init();

  inline static ShaderManager* GI()
  {
    if( !m_instance )
      m_instance = new ShaderManager;
    return m_instance;
  }
  inline static void UnInit()
  {
    if( m_instance )
      delete m_instance;
  }
  inline Shader* GetShader( EShaderID a_id ) { return &m_shaders[ a_id ]; }
private:
  std::map<EShaderID , Shader> m_shaders;
  static ShaderManager* m_instance;
};
