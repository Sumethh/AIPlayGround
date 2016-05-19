#include "SpriteBatchRenderer.h"
#include "Renderer2D.h"
#include "Common/log.h"
#include <glm/gtc/type_ptr.hpp>

SpriteBatchRenderer::SpriteBatchRenderer() :
  m_vao( 0 )
{
}


SpriteBatchRenderer::~SpriteBatchRenderer()
{
  glDeleteBuffers( 1 , &m_vbo );
  glDeleteVertexArrays( 1 , &m_vao );
}

void SpriteBatchRenderer::Init()
{



  m_shader.LoadFromFile( "../Assets/Shaders/SpriteShader.vs" , "../Assets/Shaders/SpriteShader.fs" , "../Assets/Shaders/SpriteGeometryShader.gs" );
  glGenBuffers( 1 , &m_vbo );
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  glBufferData( GL_ARRAY_BUFFER , sizeof( SpriteInfo ) * MAX_SPRITE_RENDERS , NULL , GL_STATIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER , 0 );

  glGenVertexArrays( 1 , &m_vao );
  glBindVertexArray( m_vao );

  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );

  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0 , 4 , GL_FLOAT , GL_FALSE , sizeof( SpriteInfo ) , 0 );

  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1 , 4 , GL_FLOAT , GL_FALSE , sizeof( SpriteInfo ) , (GLvoid*)offsetof( SpriteInfo , Bottom ) );

  glEnableVertexAttribArray( 2 );
  glVertexAttribPointer( 2 , 4 , GL_FLOAT , GL_FALSE , sizeof( SpriteInfo ) , (GLvoid*)offsetof( SpriteInfo , Model ) );

  glEnableVertexAttribArray( 3 );
  glVertexAttribPointer( 3 , 4 , GL_FLOAT , GL_FALSE , sizeof( SpriteInfo ) , (GLvoid*)( offsetof( SpriteInfo , Model ) + sizeof( glm::vec4 ) ) );

  glEnableVertexAttribArray( 4 );
  glVertexAttribPointer( 4 , 4 , GL_FLOAT , GL_FALSE , sizeof( SpriteInfo ) , (GLvoid*)( offsetof( SpriteInfo , Model ) + sizeof( glm::vec4 ) * 2 ) );

  glEnableVertexAttribArray( 5 );
  glVertexAttribPointer( 5 , 4 , GL_FLOAT , GL_FALSE , sizeof( SpriteInfo ) , (GLvoid*)( offsetof( SpriteInfo , Model ) + sizeof( glm::vec4 ) * 3 ) );

  glBindVertexArray( 0 );


  GLuint shaderHandle = m_shader.GetShaderHandle();
  m_projUniformLoc = glGetUniformLocation( shaderHandle , "Proj" );

}
void SpriteBatchRenderer::Begin()
{
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  m_data = (SpriteInfo*)glMapBuffer( GL_ARRAY_BUFFER , GL_WRITE_ONLY );
  glBindBuffer( GL_ARRAY_BUFFER , 0 );

}

void SpriteBatchRenderer::Submit( glm::mat4& a_mat , glm::vec4 a_top , glm::vec4 a_bottom )
{
  if( m_spriteCount < MAX_SPRITE_RENDERS )
  {
    m_data->Bottom = a_bottom;
    m_data->Top = a_top;
    m_data->Model = a_mat;
    m_data++;
    m_spriteCount++;
  }
  else
  {
    End();
    Flush();
    Begin();
    Submit( a_mat , a_top , a_bottom );
  }
}

void SpriteBatchRenderer::End()
{
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  glUnmapBuffer( GL_ARRAY_BUFFER );
  glBindBuffer( GL_ARRAY_BUFFER , 0 );
  m_data = nullptr;
}

void SpriteBatchRenderer::Flush()
{
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
  m_shader.Bind();
  glUniformMatrix4fv( m_projUniformLoc , 1 , GL_FALSE , glm::value_ptr( Renderer2D::GetProjection() ) );
  glBindVertexArray( m_vao );
  glDrawArrays( GL_POINTS , 0 , m_spriteCount );
  m_shader.UnBind();
  m_spriteCount = 0;
  glBindVertexArray( 0 );
  glUseProgram( 0 );
}
