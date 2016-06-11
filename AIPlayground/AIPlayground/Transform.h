#pragma once
#define GLM_FORCE_SSE2
#include <glm/glm.hpp>

struct Transform
{
  glm::vec2 position;
  glm::vec2 scale;
  //radians
  float rotation;
  glm::mat4 transformationMatrix;

  glm::vec2 forward;
  glm::vec2 right;
};