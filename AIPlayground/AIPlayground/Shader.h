#pragma once
#include <SFML/Graphics/Shader.hpp>
#include "Common/Types.h"
#include <string.h>
#include <functional>
#include <glm/glm.hpp>
#include <vector>
#include <map>
enum class EUniformId :uint8
{
};
class Shader
{
public:

  ~Shader();
  void LoadFromFile( const std::string a_vs , const std::string a_fs );
  void LoadFromFile( const std::string a_vs , const std::string a_fs , const std::string a_gs );
  void Bind();
  void UnBind();
  uint GetShaderHandle() { return m_shaderHandle; }
  inline uint GetProjectionUniformLoc() { return m_projectionLocation; }
  inline uint GetCameraLocationUniformLocation(){ return m_cameraPosLocation; }
  inline uint GetModelUnifromLoc() { return m_modelLocation; }

  int ListenOnBind( std::function<void( Shader* )>a_func )
  {
    m_OnBindCallBacks.push_back( a_func );
    return m_OnBindCallBacks.size() - 1;
  }

  void UnSubToOnBind( int a_id )
  {
    auto removal = m_OnBindCallBacks.begin() + a_id;
    m_OnBindCallBacks.erase( removal );
  }

  void SetUniform( EUniformId a_id , float a_value );
  void SetUniform( EUniformId a_id , glm::vec2 a_value );
  void SetUniform( EUniformId a_id , glm::vec3 a_value );

private:
  void Finalize();
  uint m_shaderHandle;
  std::string m_fs , m_vs , m_gs;
  uint m_projectionLocation, m_cameraPosLocation;
  uint m_modelLocation;
  std::vector < std::function< void( Shader* a_shader )>> m_OnBindCallBacks;
  std::map<EUniformId , uint8> m_uniformLocations;
};
