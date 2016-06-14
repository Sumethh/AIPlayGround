#pragma once
#include <glm/glm.hpp>
#include <sfml/System/Vector2.hpp>
#include <cmath>
inline glm::vec2 ConvertVec2( sf::Vector2f& a_inVec )
{
  return glm::vec2( a_inVec.x , a_inVec.y );
}

inline glm::vec2 ConvertVec2( sf::Vector2i& a_inVec )
{
  return glm::vec2( a_inVec.x , a_inVec.y );
}

inline glm::vec2 ConvertVec2( sf::Vector2u& a_inVec )
{
  return glm::vec2( a_inVec.x , a_inVec.y );
}

inline sf::Vector2f ConvertVec2( glm::vec2& a_inVec )
{
  return sf::Vector2f( a_inVec.x , a_inVec.y );
}

inline float GetLookAtAngle( glm::vec2 vec1 , glm::vec2 vec2 )
{
  glm::vec2 diff = glm::normalize(vec1 - vec2);  
  return -std::atan2( diff.y , diff.x );
}