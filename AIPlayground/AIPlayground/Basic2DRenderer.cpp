#include "Basic2DRenderer.h"
#include <gl/glew.h>
#include "Texture.h"
#include "Shader.h"
#include "Renderer2D.h"
#include <glm/gtc/type_ptr.hpp>
#include "Common/Log.h"
Basic2DRenderer::Basic2DRenderer()
{
  m_renderInfo.reserve( 1000 );
}

Basic2DRenderer::~Basic2DRenderer()
{
}

void Basic2DRenderer::Submit( RenderInfo a_info )
{
  m_renderInfo.push_back( a_info );
}

void Basic2DRenderer::Init()
{
  static const GLfloat verticies[] = {
    -0.5f, -0.5f, 0.0f, 0 , 0 ,
    0.5f, -0.5f, 0.0f, 1,0,
    -0.5f,  0.5f, 0.0f, 0,1,
    0.5f,  0.5f, 0.0f, 1,1
  };
  glGenBuffers( 1 , &m_vbo );
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  glBufferData( GL_ARRAY_BUFFER , sizeof( verticies ) , verticies , GL_STATIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER , 0 );

  glGenVertexArrays( 1 , &m_vao );
  glBindVertexArray( m_vao );
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , 5 * sizeof( float ) , 0 );
  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , 5 * sizeof( float ) , (GLvoid*)( 3 * sizeof( float ) ) );

  glBindVertexArray( 0 );
}

void Basic2DRenderer::Flush()
{
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
  glBindVertexArray( m_vao );
  for( auto render : m_renderInfo )
  {
    render.shader->Bind();
    glUniformMatrix4fv( render.shader->GetProjectionUniformLoc() , 1 , GL_FALSE , glm::value_ptr( Renderer2D::GetProjection() ) );
    glUniformMatrix4fv( render.shader->GetModelUnifromLoc() , 1 , GL_FALSE , glm::value_ptr( render.mat ) );
    for( auto texture : render.textures )
      texture.second->Bind( texture.first );
    glDrawArrays( GL_TRIANGLE_STRIP , 0 , 4 );
    for( auto texture : render.textures )
      texture.second->UnBind();
    render.shader->UnBind();
  }
  m_renderInfo.clear();
}