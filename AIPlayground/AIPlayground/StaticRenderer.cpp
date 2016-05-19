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

  int vertCount = 0;
  for( int i = 0; i < 4 * MAX_STATIC_RENDERS; i++ )
  {
    if( vertCount == 4 )
      vertCount = 0;
    Vertex newVert;
    newVert.pos = glm::vec3(0,0,0);
    newVert.texCoord = glm::vec2(0,0);
    vertCount++;
    glBufferSubData( GL_ARRAY_BUFFER , i * sizeof( Vertex ) , sizeof( Vertex ) , &newVert );
  }
  glBindBuffer( GL_ARRAY_BUFFER , 0 );

  glGenVertexArrays( 1 , &m_vao );
  glBindVertexArray( m_vao );

  glBindBuffer( GL_ARRAY_BUFFER , m_vbo );

  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , sizeof( glm::vec3 ) , 0 );
  glBindVertexArray( 0 );
}


uint StaticRenderer::Register()
{
  return 0;
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
