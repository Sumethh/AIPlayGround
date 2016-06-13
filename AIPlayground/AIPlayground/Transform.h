#pragma once
#define GLM_FORCE_SSE2
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
struct Transform
{
  glm::vec2 position;
  glm::vec2 scale;
  //radians
  float rotation;
  glm::mat4 transformationMatrix;
  glm::mat4 MakeMatrix(glm::vec2 a_sizeInPixels = glm::vec2(1, 1))
  {
    glm::mat4 myTrans;
    myTrans = glm::translate(glm::mat4(), glm::vec3(position, 0.0f));
    myTrans = glm::rotate(myTrans, rotation, glm::vec3(0, 0, 1));
    myTrans = glm::scale(myTrans, glm::vec3(scale * a_sizeInPixels, 1.0f));
    return myTrans;
  };
  glm::vec2 forward;
  glm::vec2 right;
};
