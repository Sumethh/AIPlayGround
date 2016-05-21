#include "Shader.h"
#include "gl/glew.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

Shader::~Shader()
{
  glDeleteProgram( m_shaderHandle );
}

void Shader::LoadFromFile( const std::string a_vs , const std::string a_fs )
{
  m_vs = a_vs;
  m_fs = a_fs;

  // Create the shaders
  GLuint VertexShaderID = glCreateShader( GL_VERTEX_SHADER );
  GLuint FragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream( a_vs , std::ios::in );
  if( VertexShaderStream.is_open() ) {
    std::string Line = "";
    while( getline( VertexShaderStream , Line ) )
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }
  else {
    printf( "Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n" , a_vs );
    getchar();
    m_shaderHandle = 0;
    return;
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream( a_fs , std::ios::in );
  if( FragmentShaderStream.is_open() ) {
    std::string Line = "";
    while( getline( FragmentShaderStream , Line ) )
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  printf( "Compiling shader : %s\n" , a_vs.c_str() );
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource( VertexShaderID , 1 , &VertexSourcePointer , NULL );
  glCompileShader( VertexShaderID );

  // Check Vertex Shader
  glGetShaderiv( VertexShaderID , GL_COMPILE_STATUS , &Result );
  glGetShaderiv( VertexShaderID , GL_INFO_LOG_LENGTH , &InfoLogLength );
  if( InfoLogLength > 0 ) {
    std::vector<char> VertexShaderErrorMessage( InfoLogLength + 1 );
    glGetShaderInfoLog( VertexShaderID , InfoLogLength , NULL , &VertexShaderErrorMessage[ 0 ] );
    printf( "%s\n" , &VertexShaderErrorMessage[ 0 ] );
  }

  // Compile Fragment Shader
  printf( "Compiling shader : %s\n" , a_fs.c_str() );
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource( FragmentShaderID , 1 , &FragmentSourcePointer , NULL );
  glCompileShader( FragmentShaderID );

  // Check Fragment Shader
  glGetShaderiv( FragmentShaderID , GL_COMPILE_STATUS , &Result );
  glGetShaderiv( FragmentShaderID , GL_INFO_LOG_LENGTH , &InfoLogLength );
  if( InfoLogLength > 0 ) {
    std::vector<char> FragmentShaderErrorMessage( InfoLogLength + 1 );
    glGetShaderInfoLog( FragmentShaderID , InfoLogLength , NULL , &FragmentShaderErrorMessage[ 0 ] );
    printf( "%s\n" , &FragmentShaderErrorMessage[ 0 ] );
  }

  // Link the program
  printf( "Linking program\n" );
  GLuint ProgramID = glCreateProgram();
  glAttachShader( ProgramID , VertexShaderID );
  glAttachShader( ProgramID , FragmentShaderID );
  glLinkProgram( ProgramID );

  // Check the program
  glGetProgramiv( ProgramID , GL_LINK_STATUS , &Result );
  glGetProgramiv( ProgramID , GL_INFO_LOG_LENGTH , &InfoLogLength );
  if( InfoLogLength > 0 ) {
    std::vector<char> ProgramErrorMessage( InfoLogLength + 1 );
    glGetProgramInfoLog( ProgramID , InfoLogLength , NULL , &ProgramErrorMessage[ 0 ] );
    printf( "%s\n" , &ProgramErrorMessage[ 0 ] );
  }

  glDetachShader( ProgramID , VertexShaderID );
  glDetachShader( ProgramID , FragmentShaderID );

  glDeleteShader( VertexShaderID );
  glDeleteShader( FragmentShaderID );
  m_shaderHandle = ProgramID;
  Finalize();
}

void Shader::LoadFromFile( const std::string a_vs , const std::string a_fs , const std::string a_gs )
{
  m_vs = a_vs;
  m_fs = a_fs;
  m_gs = a_gs;
  // Create the shaders
  GLuint VertexShaderID = glCreateShader( GL_VERTEX_SHADER );
  GLuint FragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
  GLuint GeometryShaderID = glCreateShader( GL_GEOMETRY_SHADER );

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream( a_vs , std::ios::in );
  if( VertexShaderStream.is_open() ) {
    std::string Line = "";
    while( getline( VertexShaderStream , Line ) )
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }
  else {
    printf( "Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n" , a_vs );
    getchar();
    m_shaderHandle = 0;
    return;
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream( a_fs , std::ios::in );
  if( FragmentShaderStream.is_open() ) {
    std::string Line = "";
    while( getline( FragmentShaderStream , Line ) )
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }

  std::string GeometryCode;
  std::ifstream GeometryShaderStream( a_gs , std::ios::in );
  if( GeometryShaderStream.is_open() ) {
    std::string Line = "";
    while( getline( GeometryShaderStream , Line ) )
      GeometryCode += "\n" + Line;
    GeometryShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  printf( "Compiling shader : %s\n" , a_vs.c_str() );
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource( VertexShaderID , 1 , &VertexSourcePointer , NULL );
  glCompileShader( VertexShaderID );

  // Check Vertex Shader
  glGetShaderiv( VertexShaderID , GL_COMPILE_STATUS , &Result );
  glGetShaderiv( VertexShaderID , GL_INFO_LOG_LENGTH , &InfoLogLength );
  if( InfoLogLength > 0 ) {
    std::vector<char> VertexShaderErrorMessage( InfoLogLength + 1 );
    glGetShaderInfoLog( VertexShaderID , InfoLogLength , NULL , &VertexShaderErrorMessage[ 0 ] );
    printf( "%s\n" , &VertexShaderErrorMessage[ 0 ] );
  }

  // Compile Fragment Shader
  printf( "Compiling shader : %s\n" , a_fs.c_str() );
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource( FragmentShaderID , 1 , &FragmentSourcePointer , NULL );
  glCompileShader( FragmentShaderID );

  // Check Fragment Shader
  glGetShaderiv( FragmentShaderID , GL_COMPILE_STATUS , &Result );
  glGetShaderiv( FragmentShaderID , GL_INFO_LOG_LENGTH , &InfoLogLength );
  if( InfoLogLength > 0 ) {
    std::vector<char> FragmentShaderErrorMessage( InfoLogLength + 1 );
    glGetShaderInfoLog( FragmentShaderID , InfoLogLength , NULL , &FragmentShaderErrorMessage[ 0 ] );
    printf( "%s\n" , &FragmentShaderErrorMessage[ 0 ] );
  }

  //Compile Geometry
  printf( "Compiling shader : %s\n" , a_gs.c_str() );
  char const * GeometrySourcePointer = GeometryCode.c_str();
  glShaderSource( GeometryShaderID , 1 , &GeometrySourcePointer , NULL );
  glCompileShader( GeometryShaderID );

  // Check Fragment Shader
  glGetShaderiv( GeometryShaderID , GL_COMPILE_STATUS , &Result );
  glGetShaderiv( GeometryShaderID , GL_INFO_LOG_LENGTH , &InfoLogLength );
  if( InfoLogLength > 0 ) {
    std::vector<char> GeometryShaderError( InfoLogLength + 1 );
    glGetShaderInfoLog( GeometryShaderID , InfoLogLength , NULL , &GeometryShaderError[ 0 ] );
    printf( "%s\n" , &GeometryShaderError[ 0 ] );
  }

  // Link the program
  printf( "Linking program\n" );
  GLuint ProgramID = glCreateProgram();
  glAttachShader( ProgramID , VertexShaderID );
  glAttachShader( ProgramID , FragmentShaderID );
  glAttachShader( ProgramID , GeometryShaderID );
  glLinkProgram( ProgramID );

  // Check the program
  glGetProgramiv( ProgramID , GL_LINK_STATUS , &Result );
  glGetProgramiv( ProgramID , GL_INFO_LOG_LENGTH , &InfoLogLength );
  if( InfoLogLength > 0 ) {
    std::vector<char> ProgramErrorMessage( InfoLogLength + 1 );
    glGetProgramInfoLog( ProgramID , InfoLogLength , NULL , &ProgramErrorMessage[ 0 ] );
    printf( "%s\n" , &ProgramErrorMessage[ 0 ] );
  }

  glDetachShader( ProgramID , VertexShaderID );
  glDetachShader( ProgramID , FragmentShaderID );

  glDeleteShader( VertexShaderID );
  glDeleteShader( FragmentShaderID );
  glDeleteShader( GeometryShaderID );

  m_shaderHandle = ProgramID;
  Finalize();
}

void Shader::Bind()
{
  glUseProgram( m_shaderHandle );
  for( auto i : m_OnBindCallBacks )
    i( this );
}

void Shader::UnBind()
{
  glUseProgram( 0 );
}

void Shader::SetUniform( EUniformId a_id , float a_value )
{
  glUniform1f( m_uniformLocations[ a_id ] , a_value );
}

void Shader::SetUniform( EUniformId a_id , glm::vec2 a_value )
{
  glUniform2f( m_uniformLocations[ a_id ] , a_value.x , a_value.y );
}

void Shader::SetUniform( EUniformId a_id , glm::vec3 a_value )
{
  glUniform3f( m_uniformLocations[ a_id ] , a_value.x , a_value.y , a_value.z );
}

void Shader::Finalize()
{
  m_projectionLocation = glGetUniformLocation( m_shaderHandle , "Proj" );
  m_modelLocation = glGetUniformLocation( m_shaderHandle , "Model" );
}