#include "Texture.h"
#include <gl/glew.h>
#include "Common/log.h"
Texture::Texture() :
  m_textureID( 0 ) ,
  m_init( false )
{
}

Texture::~Texture()
{
}

void Texture::Bind( uint a_slot )
{
  if( !m_init )
    return;
  switch( a_slot )
  {
  case 1:
  {
    glActiveTexture( GL_TEXTURE1 );
    break;
  }
  case 2:
  {
    glActiveTexture( GL_TEXTURE2 );
    break;
  }
  default:
  {
    glActiveTexture( GL_TEXTURE0 );
    break;
  }
  }
  glBindTexture( GL_TEXTURE_2D , m_textureID );
}

void Texture::UnBind( uint a_slot )
{
  if( !m_init )
    return;
  switch( a_slot )
  {
  case 1:
  {
    glActiveTexture( GL_TEXTURE1 );
    break;
  }
  case 2:
  {
    glActiveTexture( GL_TEXTURE2 );
    break;
  }
  default:
  {
    glActiveTexture( GL_TEXTURE0 );
    break;
  }
  }
  glBindTexture( GL_TEXTURE_2D , 0 );
}

void Texture::LoadFromFile( std::string a_file )
{
  if( m_init )
  {
    glDeleteTextures( 1 , &m_textureID );
  }
  LOGI( "Loading Texture %s" , a_file.c_str() );
  sf::Image image;
  image.loadFromFile( a_file );
  const uint8* data = image.getPixelsPtr();
  sf::Vector2u size = image.getSize();
  if( data )
  {
    glGenTextures( 1 , &m_textureID );
    glBindTexture( GL_TEXTURE_2D , m_textureID );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_BORDER );
    glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , size.x , size.y , 0 , GL_RGBA , GL_UNSIGNED_BYTE , data );
    glGenerateMipmap( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D , 0 );
    m_init = true;
    m_size.x = image.getSize().x;
    m_size.y = image.getSize().y;
  }
}

void Texture::LoadFromImage( sf::Image& a_image )
{
  LOGI( "Loading Texture from Image" );
  if( m_init )
  {
    glDeleteTextures( 1, &m_textureID );
    m_textureID = 0;
    m_init = false;
  }
  const uint8* data = a_image.getPixelsPtr();
  sf::Vector2u size = a_image.getSize();
  if( data )
  {
    glGenTextures( 1 , &m_textureID );
    glBindTexture( GL_TEXTURE_2D , m_textureID );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_BORDER );
    glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , size.x , size.y , 0 , GL_RGBA , GL_UNSIGNED_BYTE , data );
    glGenerateMipmap( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D , 0 );
    m_init = true;
    m_size.x = a_image.getSize().x;
    m_size.y = a_image.getSize().y;
  }
}
