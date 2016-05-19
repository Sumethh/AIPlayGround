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

#include "SpriteBatchRenderer.h"
#include "Renderer2D.h"
#include "LineRenderer.h"
#include "StaticRenderer.h"
Transform myTrans;
glm::mat4 projection;
int projLoc , modelLoc;
GLuint VAO , VBO;
uint textureLoc;



int xCount = 1280;
int yCount = 1080;
std::vector<Transform> Models;
SpriteBatchRenderer renderer;
LineRenderer lineRenderer;
StaticRenderer staticRenderer;
unsigned int shaderHandle;
Shader shader;

void Game::Init()
{
  renderer.Init();
  lineRenderer.Init();
  staticRenderer.Init();
  GLenum err = glGetError();
  sf::Image image;
  image.loadFromFile( "../Assets/Art/Unit1.png" );
  const sf::Uint8* pixels = image.getPixelsPtr();
  auto t = image.getSize();
  xCount /= t.x;
  yCount /= t.y;
  glGenTextures( 1 , &textureLoc );
  glBindTexture( GL_TEXTURE_2D , textureLoc );
  glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_BORDER );
  glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_BORDER );
  glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , t.x , t.y , 0 , GL_RGBA , GL_UNSIGNED_BYTE , pixels );
  glGenerateMipmap( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D , 0 );

  projection = glm::ortho( 0.f , 1280.0f , 720.0f , 0.0f );
  Renderer2D::SetProjectionMatrix( projection );
  myTrans.position = glm::vec2( 400.0f , 400.0f );
  myTrans.scale = glm::vec2( t.x , t.y );
  myTrans.rotation = 0.0f;

  myTrans.transformationMatrix = glm::translate( glm::mat4() , glm::vec3( myTrans.position , 0.0f ) );
  myTrans.transformationMatrix = glm::rotate( myTrans.transformationMatrix , myTrans.rotation , glm::vec3( 0 , 0 , 1 ) );
  myTrans.transformationMatrix = glm::scale( myTrans.transformationMatrix , glm::vec3( myTrans.scale , 1.0f ) );
  shaderHandle = shader.GetShaderHandle();
  for( int y = 0; y < yCount * 4; y++ )
  {
    for( int x = 0; x < xCount * 4; x++ )
    {
      Transform trans;
      trans.position = glm::vec2( x * t.x + t.x / 2 , y * t.y + t.y / 2 );
      trans.scale.x = t.x;
      trans.scale.y = t.y;
      trans.rotation = 0.0f;
      trans.transformationMatrix = glm::translate( glm::mat4() , glm::vec3( trans.position , 0.0f ) );
      trans.transformationMatrix = glm::rotate( trans.transformationMatrix , trans.rotation , glm::vec3( 0 , 0 , 1 ) );
      trans.transformationMatrix = glm::scale( trans.transformationMatrix , glm::vec3( trans.scale , 1.0f ) );
      Models.push_back( trans );
    }
  }

  uint reg = staticRenderer.Register();
  staticRenderer.UpdateTexCoords( reg , glm::vec4( 0 , 0 , 1 , 0 ) , glm::vec4( 0 , 1 , 1 , 1 ) );

  staticRenderer.UpdatePosition( reg , myTrans.transformationMatrix );
}

void Game::FixedUpdate( float a_dt )
{

}
float Angle = 0.0f;
void Game::Update( float a_dt )
{
  if( Input::GetKeyDown( sf::Keyboard::Key::A ) )
    Angle -= 10.0f * a_dt;

  if( Input::GetKeyDown( sf::Keyboard::Key::D ) )
    Angle += 10.0f * a_dt;



}
void Game::PreRender()
{


  int t = 0;
  lineRenderer.Begin();
  renderer.Begin();
  for( auto it : Models )
  {
    it.transformationMatrix = glm::translate( glm::mat4() , glm::vec3( it.position , 0.0f ) );
    it.transformationMatrix = glm::rotate( it.transformationMatrix , Angle , glm::vec3( 0 , 0 , 1 ) );
    it.transformationMatrix = glm::scale( it.transformationMatrix , glm::vec3( it.scale , 1.0f ) );
    renderer.Submit( it.transformationMatrix , glm::vec4( 0 , 0 , 1 , 0 ) , glm::vec4( 0 , 1 , 1 , 1 ) );
  }
  //
  glm::vec2 mouseLoc = Input::GetMousePosition();
  glm::vec2 prevLoc( 0 , 0 );
  for( auto it : Models )
  {
   lineRenderer.Submit( prevLoc , it.position , glm::vec4( 1.0f , 0.0f , 0.0f , 1.0f ) );
    prevLoc = it.position;
  }
  lineRenderer.End();
  renderer.End();

}

void Game::Render( Window* const a_window )
{
  glBindTexture( GL_TEXTURE_2D , textureLoc );
  renderer.Flush();
  staticRenderer.Flush();
  glBindTexture( GL_TEXTURE_2D , 0 );

  lineRenderer.Flush();

}

void Game::PostFrame()
{

}
#endif // 1