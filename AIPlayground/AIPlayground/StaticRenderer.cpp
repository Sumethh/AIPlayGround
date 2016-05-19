#include "StaticRenderer.h"



StaticRenderer::StaticRenderer()
{
}


StaticRenderer::~StaticRenderer()
{
}



void StaticRenderer::Init()
{
  m_freeIds.reserve( 100 );
  m_shader.LoadFromFile( "../Assets/Shaders/StaticVertexShader.vs" , "../Assets/Shaders/StatixFragmentShader.fs" );

  glGenBuffers( 1 , &m_vbo );
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );
  glBufferData( GL_ARRAY_BUFFER , sizeof( Vertex ) * 4 * MAX_STATIC_RENDERS , nullptr , GL_STATIC_DRAW );

  for( int i = 0; i < 4 * MAX_STATIC_RENDERS; i++ )
  {
    Vertex newVert;
    newVert.pos = glm::vec3(0,0,0);
    newVert.texCoord = glm::vec2(0,0);
    glBufferSubData( GL_ARRAY_BUFFER , i * sizeof( Vertex ) , sizeof( Vertex ) , &newVert );
  }
  glBindBuffer( GL_ARRAY_BUFFER , 0 );

  glGenVertexArrays( 1 , &m_vao );
  glBindVertexArray( m_vao );

  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );

  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , sizeof( Vertex ) , 0 );

  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , sizeof( Vertex ) , (GLvoid*)sizeof(glm::vec2) );

  glBindVertexArray( 0 );
}


uint StaticRenderer::Register()
{

}

void StaticRenderer::UpdateTexCoords( uint a_index , StaticTexCoordData a_texCoords )
{

}

void StaticRenderer::UpdateVertices( uint a_index , glm::vec2 m_vertices[ 4 ] )
{

}

void StaticRenderer::UnRegister( uint a_index )
{

}
