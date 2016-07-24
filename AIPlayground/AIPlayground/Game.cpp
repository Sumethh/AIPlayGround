#include "Game.h"
#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Common/Input.h"
#include "TextureManager.h"
#include "ShaderManager.h"
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

#if 1
void Game::Init()
{
  TextureManager::GI()->Init();
  ShaderManager::GI()->Init();
  m_world->OnConstruct();
  m_world->BeginPlay();
  glm::mat4 projection = glm::ortho( 0.f , 1280.0f , 720.0f , 0.0f , 10.0f, -1.0f);
  Renderer2D::SetProjectionMatrix( projection );
  m_renderer.Init();
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

void Game::Render()
{
  m_renderer.Begin();
  m_world->Render( &m_renderer );
  m_renderer.End();
  m_renderer.Flush();
}

void Game::PostFrame()
{
  m_world->PostFrame();
}
#endif
#if 0

#include "SpriteBatchRenderer.h"
#include "Renderer2D.h"
#include "LineRenderer.h"
#include "StaticRenderer.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Basic2DRenderer.h"
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
Basic2DRenderer basicRenderer;
unsigned int shaderHandle;
Shader shader;

void Game::Init()
{
  ShaderManager::GI()->Init();
  TextureManager::GI()->Init();
  renderer.Init();
  lineRenderer.Init();
  staticRenderer.Init();
  basicRenderer.Init();
  GLenum err = glGetError();

  xCount /= 32;
  yCount /= 32;

  projection = glm::ortho( 0.f , 1280.0f , 720.0f , 0.0f );
  Renderer2D::SetProjectionMatrix( projection );
  myTrans.position = glm::vec2( 400.0f , 400.0f );
  myTrans.scale = glm::vec2( 32 , 32 );
  myTrans.rotation = 0.0f;

  myTrans.transformationMatrix = glm::translate( glm::mat4() , glm::vec3( myTrans.position , 0.0f ) );
  myTrans.transformationMatrix = glm::rotate( myTrans.transformationMatrix , myTrans.rotation , glm::vec3( 0 , 0 , 1 ) );
  myTrans.transformationMatrix = glm::scale( myTrans.transformationMatrix , glm::vec3( myTrans.scale , 1.0f ) );
  shaderHandle = shader.GetShaderHandle();
  for( int y = 0; y < yCount * 8; y++ )
  {
    for( int x = 0; x < xCount * 8; x++ )
    {
      Transform trans;
      trans.position = glm::vec2( x * 32 + 32 / 2 , y * 32 + 32 / 2 );
      trans.scale.x = 32;
      trans.scale.y = 32;
      trans.rotation = 0.0f;
      trans.transformationMatrix = glm::translate( glm::mat4() , glm::vec3( trans.position , 0.0f ) );
      trans.transformationMatrix = glm::rotate( trans.transformationMatrix , trans.rotation , glm::vec3( 0 , 0 , 1 ) );
      trans.transformationMatrix = glm::scale( trans.transformationMatrix , glm::vec3( trans.scale , 1.0f ) );
      Models.push_back( trans );
    }
  }

  uint reg = staticRenderer.Register();
  staticRenderer.UpdateTexCoords( reg , glm::vec4( 0 , 0 , 1 / 12.0f , 0 ) , glm::vec4( 0 , 1 , 1 / 12.0f , 1 ) );

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
  //lineRenderer.Begin();
  renderer.Begin();
  for( auto it : Models )
  {
    //it.transformationMatrix = glm::translate( glm::mat4() , glm::vec3( it.position , 0.0f ) );
    //it.transformationMatrix = glm::rotate( it.transformationMatrix , Angle , glm::vec3( 0 , 0 , 1 ) );
    //it.transformationMatrix = glm::scale( it.transformationMatrix , glm::vec3( it.scale , 1.0f ) );
    //renderer.Submit( glm::vec3( it.position , 1.0f ) , it.scale , Angle , glm::vec4( 0 , 0 , 1 , 0 ) , glm::vec4( 0 , 1 , 1 , 1 ) );
  }
  //
 // glm::vec2 mouseLoc = Input::GetMousePosition();
 // glm::vec2 prevLoc( 0 , 0 );
 // for( auto it : Models )
 // {
 //  lineRenderer.Submit( prevLoc , it.position , glm::vec4( 1.0f , 0.0f , 0.0f , 1.0f ) );
 //   prevLoc = it.position;
 // }
 // lineRenderer.End();
  renderer.End();
  RenderInfo info;
  info.mat = Models[ 0 ].transformationMatrix;
  info.shader = ShaderManager::GI()->GetShader( EShaderID::BasicRender );
  std::pair<uint , Texture*> texture;
  texture.first = 0;
  texture.second = TextureManager::GI()->GetTexture( ETextureID::DynamicSpriteSheet );
  info.textures.push_back( texture );
  basicRenderer.Submit( info );
}

void Game::Render()
{
  //renderer.Flush();
  staticRenderer.Flush();
  //lineRenderer.Flush();
  //basicRenderer.Flush();
}

void Game::PostFrame()
{
}
#endif // 1