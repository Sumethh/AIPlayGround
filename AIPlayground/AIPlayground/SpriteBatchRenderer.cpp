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
  //glDeleteBuffers( 1 , &m_instanceVbo);
  glDeleteVertexArrays( 1 , &m_vao );
}
//GLfloat vertices[] = {
//  -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
//  0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom Right
//  -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,  // Top Left 
//  0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right

void SpriteBatchRenderer::Init()
{



  m_shader.LoadFromFile( "../Assets/Shaders/SpriteShader.vs" , "../Assets/Shaders/SpriteShader.fs" );

  glGenBuffers( 1 , &m_vbo );
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  glBufferData( GL_ARRAY_BUFFER , sizeof( VertexData ) * 6 * MAX_SPRITE_RENDERS , NULL , GL_STATIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER , 0 );

  //glGenBuffers( 1 , &m_instanceVbo);
  //glBindBuffer( GL_ARRAY_BUFFER , m_instanceVbo);
  //glBufferData( GL_ARRAY_BUFFER , sizeof(SpriteRenderData)*MAX_SPRITE_RENDERS , nullptr, GL_DYNAMIC_DRAW);
  //glBindBuffer( GL_ARRAY_BUFFER , 0 );

  glGenVertexArrays( 1 , &m_vao );
  glBindVertexArray( m_vao );

  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , 5 * sizeof( GLfloat ) , (GLvoid*)0 );
  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , 5 * sizeof( GLfloat ) , (GLvoid*)( 3 * sizeof( GLfloat ) ) );;

  /*glBindBuffer( GL_ARRAY_BUFFER , m_instanceVbo );
  glEnableVertexAttribArray( 2 );
  glVertexAttribPointer( 2 , 4 , GL_FLOAT , GL_FALSE , sizeof( SpriteRenderData ) , 0 );
  glEnableVertexAttribArray( 3 );
  glVertexAttribPointer( 3 , 4 , GL_FLOAT , GL_FALSE , sizeof( SpriteRenderData ) , (GLvoid*)( sizeof( glm::vec4 ) ) );
  glEnableVertexAttribArray( 4 );
  glVertexAttribPointer( 4 , 4 , GL_FLOAT , GL_FALSE , sizeof( SpriteRenderData ) , (GLvoid*)( 2 * sizeof( glm::vec4 ) ) );
  glEnableVertexAttribArray( 5 );
  glVertexAttribPointer( 5 , 4 , GL_FLOAT , GL_FALSE , sizeof( SpriteRenderData ) , (GLvoid*)( 3 * sizeof( glm::vec4 ) ) );

  glVertexAttribDivisor( 2 , 1 );
  glVertexAttribDivisor( 3 , 1 );
  glVertexAttribDivisor( 4 , 1 );
  glVertexAttribDivisor( 5 , 1 );
  */
  glBindVertexArray( 0 );


  GLuint shaderHandle = m_shader.GetShaderHandle();
  m_projUniformLoc = glGetUniformLocation( shaderHandle , "Proj" );

}
void SpriteBatchRenderer::Begin()
{
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  m_data = (VertexData*)glMapBuffer( GL_ARRAY_BUFFER , GL_WRITE_ONLY );
  glBindBuffer( GL_ARRAY_BUFFER , 0 );
  GLenum err = glGetError();
  int t = 0;
}

glm::vec3 vertices[] = {
  // First triangle
  glm::vec3( 0.5f,  0.5f, 0.0f ),  // Top Right
  glm::vec3( 0.5f, -0.5f, 0.0f ),  // Bottom Right
  glm::vec3( -0.5f,  0.5f, 0.0f ),  // Top Left 
// Second triangle
  glm::vec3( 0.5f, -0.5f, 0.0f ),  // Bottom Right
  glm::vec3( -0.5f, -0.5f, 0.0f ),  // Bottom Left
  glm::vec3( -0.5f,  0.5f, 0.0f )  // Top Left
};

void SpriteBatchRenderer::Submit( glm::mat4& a_mat , StaticTexCoordData& a_texCoords )
{
  if( m_spriteCount < MAX_SPRITE_RENDERS * 6 )
  {
    for( int i = 0; i < 6; i++ )
    {
      m_data->pos = glm::vec3( a_mat * glm::vec4( vertices[ i ] , 1.0f ) );
      m_data->textCoord = a_texCoords.coords[ i ];
      m_data++;
      m_spriteCount++;
    }
  }
  else
  {
    End();
    Flush();
    Begin();
    Submit( a_mat , a_texCoords );
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
  glDrawArrays( GL_TRIANGLES , 0 , m_spriteCount );
  m_shader.UnBind();
  m_spriteCount = 0;
  glBindVertexArray( 0 );
  glUseProgram( 0 );
}
