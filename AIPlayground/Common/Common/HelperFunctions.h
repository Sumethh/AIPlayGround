#pragma once
#include <glm/glm.hpp>
#include <sfml/System/Vector2.hpp>

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