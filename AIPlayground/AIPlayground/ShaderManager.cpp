#include "ShaderManager.h"

ShaderManager* ShaderManager::m_instance;

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::Init()
{
  m_shaders[ EShaderID::SpriteBatch ].LoadFromFile( "../Assets/Shaders/SpriteShader.vs" , "../Assets/Shaders/SpriteShader.fs" , "../Assets/Shaders/SpriteGeometryShader.gs" );

  m_shaders[ EShaderID::LineRender ].LoadFromFile( "../Assets/Shaders/LineVertexShader.vs" , "../Assets/Shaders/LineFragmentShader.fs" );

  m_shaders[ EShaderID::BasicRender ].LoadFromFile( "../Assets/Shaders/BasicVertexShader.vs" , "../Assets/Shaders/BasicFragmentShader.fs" );
}