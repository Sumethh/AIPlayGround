#include "Game.h"
#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Common/Input.h"
Game::Game() :
  m_world( new World() )
{
  if( m_world )
    m_world->SetWorldShared( m_world );
}


Game::~Game()
{
  m_world->OnDestroyed();
}

#if 0
void Game::Init()
{
  m_world->OnConstruct();
  m_world->BeginPlay();
}

void Game::FixedUpdate( float a_dt )
{
  m_world->FixedUpdate( a_dt );
}

void Game::Update( float a_dt )
{
  m_world->Update( a_dt );
}
void Game::PreRender()
{
  m_world->PreRender();
}

void Game::Render( Window* const a_window )
{
  m_world->Render( a_window );
}

void Game::PostFrame()
{
  m_world->PostFrame();
}
#endif
#if 1

GLfloat vertices[] = {
  0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
  0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
  -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
  -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
};

GLuint indices[] = {
  0, 1, 3,
  1, 2, 3
};

GLuint VBO , VAO , EBO;
sf::Shader shader;

Transform myTrans;
glm::mat4 projection;
int projLoc , modelLoc;

uint textureLoc;

void Game::Init()
{


  glGenVertexArrays( 1 , &VAO );
  glGenBuffers( 1 , &VBO );
  glGenBuffers( 1 , &EBO );
  glBindBuffer( GL_ARRAY_BUFFER , VBO );
  glBufferData( GL_ARRAY_BUFFER , sizeof( vertices ) , vertices , GL_STATIC_DRAW );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , EBO );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER , sizeof( indices ) , indices , GL_STATIC_DRAW );

  glBindVertexArray( VAO );
  glBindBuffer( GL_ARRAY_BUFFER , VBO );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , EBO );
  glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , 8 * sizeof( GLfloat ) , (GLvoid*)0 );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 1 , 3 , GL_FLOAT , GL_FALSE , 8 * sizeof( GLfloat ) , (GLvoid*)( 3 * sizeof( GLfloat ) ) );
  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 2 , 2 , GL_FLOAT , GL_FALSE , 8 * sizeof( GLfloat ) , (GLvoid*)( 6 * sizeof( GLfloat ) ) );
  glEnableVertexAttribArray( 2 );
  glEnableVertexAttribArray( 0 );
  glBindVertexArray( 0 );

  glGenTextures( 1 , &textureLoc );
  glBindTexture( GL_TEXTURE_2D , textureLoc );
  glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_BORDER );
  glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_BORDER );
  sf::Image image;
  image.loadFromFile( "../Assets/Art/Unit1.png" );
  const sf::Uint8* pixels = image.getPixelsPtr();
  auto t = image.getSize();
  glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , t.x , t.y , 0 , GL_RGBA , GL_UNSIGNED_BYTE , pixels );
  glGenerateMipmap( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D , 0 );

  shader.loadFromFile( "../Assets/Shaders/SimpleVertexShader.vs" , "../Assets/Shaders/SimpleFragmentShader.fs" );
  projLoc = glGetUniformLocation( shader.getNativeHandle() , "Proj" );
  modelLoc = glGetUniformLocation( shader.getNativeHandle() , "Model" );

  projection = glm::ortho( 0.f , 1280.0f , 720.0f , 0.0f );

  myTrans.position = glm::vec2( 400.0f , 400.0f );
  myTrans.scale = glm::vec2( t.x , t.y);
  myTrans.rotation = 0.0f;


}

void Game::FixedUpdate( float a_dt )
{

}

void Game::Update( float a_dt )
{
  if( Input::GetKeyDown( sf::Keyboard::Key::A ) )
    myTrans.rotation -= 10.0f * a_dt;

  if( Input::GetKeyDown( sf::Keyboard::Key::D ) )
    myTrans.rotation += 10.0f * a_dt;



}
void Game::PreRender()
{
  myTrans.transformationMatrix = glm::translate( glm::mat4() , glm::vec3( myTrans.position , 0.0f ) );
  myTrans.transformationMatrix = glm::rotate( myTrans.transformationMatrix , myTrans.rotation , glm::vec3( 0 , 0 , 1 ) );
  myTrans.transformationMatrix = glm::scale( myTrans.transformationMatrix , glm::vec3( myTrans.scale , 1.0f ) );


}

void Game::Render( Window* const a_window )
{

  glUseProgram( shader.getNativeHandle() );
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
  glBindVertexArray( VAO );
  glBindTexture( GL_TEXTURE_2D , textureLoc );
  glUniformMatrix4fv( projLoc , 1 , GL_FALSE , glm::value_ptr( projection ) );
  glUniformMatrix4fv( modelLoc , 1 , GL_FALSE , glm::value_ptr( myTrans.transformationMatrix ) );
  glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_INT , 0 );
  glUseProgram( 0 );
  glBindVertexArray( 0 );
  glBindTexture( GL_TEXTURE_2D , 0 );

}

void Game::PostFrame()
{

}
#endif // 1