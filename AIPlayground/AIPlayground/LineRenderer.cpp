#include "LineRenderer.h"
#include "Renderer2D.h";
#include "ShaderManager.h"
#include <glm/gtc/type_ptr.hpp>

LineRenderer::LineRenderer()
{
}

LineRenderer::~LineRenderer()
{
}

void LineRenderer::Init()
{
  m_shader = ShaderManager::GI()->GetShader( EShaderID::LineRender );

  glGenBuffers( 1 , &m_vbo );
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  glBufferData( GL_ARRAY_BUFFER , sizeof( LineRenderData ) * MAX_LINE_RENDERS * 2 , nullptr , GL_DYNAMIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER , 0 );

  glGenVertexArrays( 1 , &m_vao );
  glBindVertexArray( m_vao );

  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  //Position
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0 , 2 , GL_FLOAT , GL_FALSE , sizeof( LineRenderData ) , 0 );
  //Color
  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1 , 4 , GL_FLOAT , GL_FALSE , sizeof( LineRenderData ) , ( GLvoid* )sizeof( glm::vec2 ) );

  glBindVertexArray( 0 );
}

void LineRenderer::Begin()
{
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  m_data = (LineRenderData*)glMapBuffer( GL_ARRAY_BUFFER , GL_WRITE_ONLY );
  glBindBuffer( GL_ARRAY_BUFFER , 0 );
}

void LineRenderer::Submit( glm::vec2 a_startPosition , glm::vec2 a_endPosition , glm::vec4 a_color )
{
  if( m_lineRenderCount < MAX_LINE_RENDERS * 2 )
  {
    m_data->position = a_startPosition;
    m_data->color = a_color;
    m_data++;
    m_data->position = a_endPosition;
    m_data->color = a_color;
    m_data++;

    m_lineRenderCount += 2;
  }
  else
  {
    End();
    Flush();
    Begin();
    Submit( a_startPosition , a_endPosition , a_color );
  }
}

void LineRenderer::End()
{
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  glUnmapBuffer( GL_ARRAY_BUFFER );
  glBindBuffer( GL_ARRAY_BUFFER , 0 );
  m_data = nullptr;
  GLenum err = glGetError();
}

void LineRenderer::Flush()
{
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
  m_shader->Bind();
  glUniformMatrix4fv( m_shader->GetProjectionUniformLoc() , 1 , GL_FALSE , glm::value_ptr( Renderer2D::GetProjection() ) );
  glBindVertexArray( m_vao );
  glDrawArrays( GL_LINES , 0 , m_lineRenderCount );
  m_lineRenderCount = 0;
  glBindVertexArray( 0 );
  m_shader->UnBind();
}