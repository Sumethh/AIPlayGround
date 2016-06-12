#pragma once
#include "Texture.h"
#include <Common/Types.h>
#include <map>
#include <glm/glm.hpp>
enum class ETextureID : uint
{
  DynamicSpriteSheet ,
  StaticSpriteSheet
};
struct TextureCoordInfo
{
  glm::vec2 topLeft;
  glm::vec2 topRight;
  glm::vec2 bottomLeft;
  glm::vec2 bottomRight;
  glm::vec2 sizeInPixels;
};
class TextureManager
{
public:
  TextureManager();
  ~TextureManager();

  void Init();

  static TextureManager* TextureManager::GI()
  {
    if( !m_instance )
      m_instance = new TextureManager;
    return m_instance;
  }

  inline TextureCoordInfo& GetTextureCoordInfo( ETextureID a_textID , uint8 a_id ) { return m_textureCoordInfo[ a_textID ][ a_id ]; }
  inline void AddTextureCoordInfo( ETextureID a_textID , uint8 a_id , TextureCoordInfo a_info ) { m_textureCoordInfo[ a_textID ][ a_id ] = a_info; }
  inline Texture* GetTexture( ETextureID a_id ) { return &m_textures[ a_id ]; }

private:
  void LoadTextureCoordInfo(ETextureID a_textureId, const char* a_fileName);

  std::map<ETextureID , Texture> m_textures;
  std::map<ETextureID , std::map<uint8 , TextureCoordInfo>> m_textureCoordInfo;
  static TextureManager* m_instance;
};
