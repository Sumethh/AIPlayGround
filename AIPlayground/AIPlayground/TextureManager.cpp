#include "TextureManager.h"

TextureManager* TextureManager::m_instance;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::Init()
{
  m_textures[ ETextureID::DynamicSpriteSheet ].LoadFromFile( "../Assets/art/unit1.png" );
  m_textures[ ETextureID::StaticSpriteSheet ].LoadFromFile( "../Assets/art/unit1.png" );
}