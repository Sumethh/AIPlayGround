#include "TextureManager.h"
#include "common/json.hpp"
#include "Common/log.h"

#include <fstream>
#include <string>

TextureManager* TextureManager::m_instance;
using json = nlohmann::json;
TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::Init()
{
  m_textures[ETextureID::DynamicSpriteSheet].LoadFromFile("../Assets/art/SpriteSHeetsGenerated/DynamicSpriteSheet.png");
  m_textures[ETextureID::StaticSpriteSheet].LoadFromFile("../Assets/art/SpriteSHeetsGenerated/StaticSpriteSheet.png");

  LoadTextureCoordInfo(ETextureID::StaticSpriteSheet, "../Assets/art/SpriteSheetsGenerated/StaticSpriteSheetData.json");
  LoadTextureCoordInfo(ETextureID::DynamicSpriteSheet, "../Assets/art/SpriteSheetsGenerated/DynamicSpriteSheetData.json");
  int t = 0;
}
void TextureManager::LoadTextureCoordInfo(ETextureID a_textureId, const char* a_fileName)
{
  uint spriteID = 0;

  json myJson;
  std::string fileData;
  std::ifstream VertexShaderStream(a_fileName, std::ios::in);
  if (VertexShaderStream.is_open()) {
    std::string Line = "";
    while (getline(VertexShaderStream, Line))
      fileData += "\n" + Line;
    VertexShaderStream.close();
  }
  myJson = json::parse(fileData);
  auto size = myJson.size();
  auto framesData = myJson["frames"];
  auto metaData = myJson["meta"];

  glm::vec2 imageSize;
  imageSize.x = metaData["size"]["w"];
  imageSize.y = metaData["size"]["h"];

  for (int i = 0; i < framesData.size(); i++)
  {
    TextureCoordInfo newCoordInfo;
    auto newSpriteData = framesData[i];
    auto frameSizeInfo = newSpriteData["frame"];
    newCoordInfo.sizeInPixels.x = frameSizeInfo["w"];
    newCoordInfo.sizeInPixels.y = frameSizeInfo["h"];
    glm::vec2 topLeftCornerLoc;
    topLeftCornerLoc.x = frameSizeInfo["x"];
    topLeftCornerLoc.y = frameSizeInfo["y"];

    newCoordInfo.topLeft = topLeftCornerLoc / imageSize;
    newCoordInfo.topRight = (topLeftCornerLoc + glm::vec2(newCoordInfo.sizeInPixels.x, 0)) / imageSize;
    newCoordInfo.bottomLeft = (topLeftCornerLoc + glm::vec2(0, newCoordInfo.sizeInPixels.y)) / imageSize;
    newCoordInfo.bottomRight = (topLeftCornerLoc + newCoordInfo.sizeInPixels) / imageSize;

    m_textureCoordInfo[a_textureId][spriteID] = newCoordInfo;
    spriteID++;
  }
}