#include "Grid.h"
#include "Common/Types.h"
#include "Common/Window.h"
#include "Common/Log.h"
#include "Renderer2D.h"
#include "ShaderManager.h"
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>

sf::IntRect tiles[] =
{
  sf::IntRect( 0,0, 32,32 ),
  sf::IntRect( 32,0, 32,32 ),
};

Grid::Grid( glm::vec2 a_gridOrigin , int a_tileCountX , int a_tileCountY , int a_tileSizeX , int a_tileSizeY ) :
  m_gridOrigin( a_gridOrigin ) ,
  m_tileCountX( a_tileCountX ) ,
  m_tileCountY( a_tileCountY ) ,
  m_tileSizeX( a_tileSizeX ) ,
  m_tileSizeY( a_tileSizeY )
{
  if( !m_spriteSheet.loadFromFile( "../Assets/Art/TileSheet.png" ) )
    LOGE( "Could not load Sprite Sheet" );
  m_gridTransform.position = a_gridOrigin;
  m_gridTransform.scale = glm::vec2( 1 , 1 );
}

Grid::~Grid()
{
}

void Grid::Init()
{
  CreateGrid();
  LoadFromDisk();
  CalculateNeighbors();
  UpdateImage();
}

void Grid::PreRender( const glm::vec2 a_cameraPos )
{
  if( m_bImageDirty )
  {
    UpdateImage();
    m_bImageDirty = false;
  }
  glm::vec2 newPos;
  newPos.x = -a_cameraPos.x  + m_gridImage.getSize().x/2;
  newPos.y = -a_cameraPos.y+ m_gridImage.getSize().x/2;

  m_gridTransform.position =  newPos;
}

void Grid::Render(Renderer2D* a_renderer )
{
  Basic2DRenderer& basicRender = a_renderer->GetBasicRenderer();
  RenderInfo renderInfo;
  renderInfo.shader = ShaderManager::GI()->GetShader( EShaderID::BasicRender );
  renderInfo.textures.push_back( std::pair<uint , Texture*>(0, &m_gridTexture) );
  glm::mat4 renderMat;
  renderMat = glm::translate( renderMat , glm::vec3( m_gridTransform.position , 0.99f));
  renderMat = glm::rotate( renderMat , m_gridTransform.rotation , glm::vec3( 0 , 0 , 1 ) );
  glm::vec3 scale = glm::vec3(m_gridTransform.scale,1.0f);
  scale.x *= m_gridImage.getSize().x;
  scale.y *= m_gridImage.getSize().y;
  renderMat = glm::scale( renderMat , scale );
  renderInfo.mat = renderMat;
  basicRender.Submit( renderInfo );
}

bool Grid::SaveToDisk()
{
  std::ofstream file( "../assets/Saves/Grid.txt" , std::ofstream::out | std::ofstream::trunc );
  if( !file.is_open() )
  {
    LOGE( "failed to open file" );
    return false;
  }
  file << m_tileCountX << " " << m_tileCountY << " " << m_tileSizeX << " " << m_tileSizeY << " ";
  for( auto& i : m_nodes )
  {
    file << i.tileIndex;
    file << " ";
  }

  file.close();
  return true;
}

void ReadInt( std::string& a_source , int& a_int )
{
  size_t offset = 0;
  std::string sub;

  offset = a_source.find_first_of( " " );
  sub = a_source.substr( 0 , offset );
  a_source.erase( 0 , offset + 1 );
  a_int = std::atoi( sub.c_str() );
}

bool Grid::LoadFromDisk()
{
  std::ifstream file( "../assets/Saves/Grid.txt" );
  if( !file.is_open() )
  {
    LOGE( "failed to open file" );
    return false;
  }
  std::string fileLines;

  std::getline( file , fileLines );

  int countX , countY , sizeX , sizeY;

  ReadInt( fileLines , countX );
  ReadInt( fileLines , countY );
  ReadInt( fileLines , sizeX );
  ReadInt( fileLines , sizeY );

  size_t offset = 0;
  std::string sub;
  size_t count = 0;
  while( !fileLines.empty() )
  {
    offset = fileLines.find_first_of( " " );
    sub = fileLines.substr( 0 , offset );
    fileLines.erase( 0 , offset + 1 );

    if( count < m_nodes.size() )
    {
      m_nodes[ count ].tileIndex = std::atoi( sub.c_str() );
      m_nodes[ count ].bwalkable = m_nodes[ count ].tileIndex > 0;
      if( m_nodes[ count ].tileIndex )
        m_dirtyTileIndiciesQueue.push( m_nodes[ count ].indexSingle );
    }
    ++count;
  }

  file.close();

  return true;
}

void Grid::CreateGrid()
{
  m_nodes.reserve( m_tileCountX * m_tileCountY );
  m_gridImage.create( m_tileSizeX * m_tileCountX , m_tileSizeY * m_tileCountY , sf::Color::Red );

  glm::vec2 nodeNewPos = m_gridOrigin;
  glm::vec2 nodeNewCenter = m_gridOrigin;

  uint32 yIndex = 0;

  for( int y = 0; y < m_tileCountY; ++y )
  {
    nodeNewPos.y = (float)( m_tileSizeY * y );
    nodeNewCenter.y = (float)( m_tileSizeY * y + m_tileSizeY / 2.0f );

    yIndex = m_tileCountX * y;
    for( int x = 0; x < m_tileCountX; ++x )
    {
      m_nodes.push_back( Node() );
      Node& currentNode = m_nodes[ yIndex + x ];
      nodeNewPos.x = (float)( m_tileSizeX * x );
      nodeNewCenter.x = (float)( m_tileSizeX * x + m_tileSizeX / 2.0f );
      currentNode.pos = nodeNewPos;
      currentNode.center = nodeNewCenter;
      currentNode.bwalkable = true;
      currentNode.tileIndex = 1;
      currentNode.indexSingle = yIndex + x;
      currentNode.parent = nullptr;
      currentNode.index.x = (float)x;
      currentNode.index.y = (float)y;
      m_dirtyTileIndiciesQueue.push( yIndex + x );
    }
    nodeNewPos.x = 0;
  }
}

void Grid::CalculateNeighbors()
{
  int nodeIndexX = 0;
  int nodeIndexY = 0;
  for( auto& i : m_nodes )
  {
    nodeIndexX = (int)i.index.x;
    nodeIndexY = (int)i.index.y;

    if( nodeIndexY - 1 >= 0 )
    {
      if( nodeIndexX - 1 >= 0 )
        i.neighbors[ Node::Neighbors::NW ] = GetNode( nodeIndexX - 1 , nodeIndexY - 1 );
      else
        i.neighbors[ Node::Neighbors::NW ] = nullptr;

      i.neighbors[ Node::Neighbors::N ] = GetNode( nodeIndexX , nodeIndexY - 1 );

      if( nodeIndexX + 1 < m_tileCountX )
        i.neighbors[ Node::Neighbors::NE ] = GetNode( nodeIndexX + 1 , nodeIndexY - 1 );
      else
        i.neighbors[ Node::Neighbors::NE ] = nullptr;
    }
    else
    {
      i.neighbors[ Node::Neighbors::NW ] = nullptr;
      i.neighbors[ Node::Neighbors::N ] = nullptr;
      i.neighbors[ Node::Neighbors::NE ] = nullptr;
    }

    if( nodeIndexX - 1 >= 0 )
      i.neighbors[ Node::Neighbors::W ] = GetNode( nodeIndexX - 1 , nodeIndexY );
    else
      i.neighbors[ Node::Neighbors::W ] = nullptr;

    if( nodeIndexX + 1 < m_tileCountX )
      i.neighbors[ Node::Neighbors::E ] = GetNode( nodeIndexX + 1 , nodeIndexY );
    else
      i.neighbors[ Node::Neighbors::E ] = nullptr;

    if( nodeIndexY + 1 < m_tileCountX )
    {
      if( nodeIndexX - 1 >= 0 )
        i.neighbors[ Node::Neighbors::SW ] = GetNode( nodeIndexX - 1 , nodeIndexY + 1 );
      else
        i.neighbors[ Node::Neighbors::SW ] = nullptr;

      i.neighbors[ Node::Neighbors::S ] = GetNode( nodeIndexX , nodeIndexY + 1 );

      if( nodeIndexX + 1 < m_tileCountX )
      {
        i.neighbors[ Node::Neighbors::SE ] = GetNode( nodeIndexX + 1 , nodeIndexY + 1 );
      }
      else
        i.neighbors[ Node::Neighbors::SE ] = nullptr;
    }
    else
    {
      i.neighbors[ Node::Neighbors::SW ] = nullptr;
      i.neighbors[ Node::Neighbors::S ] = nullptr;
      i.neighbors[ Node::Neighbors::SE ] = nullptr;
    }

    if( i.neighbors[ Node::Neighbors::S ] && !i.neighbors[ Node::Neighbors::S ]->bwalkable ||
      i.neighbors[ Node::Neighbors::E ] && !i.neighbors[ Node::Neighbors::E ]->bwalkable )
      i.neighbors[ Node::Neighbors::SE ] = nullptr;

    if( i.neighbors[ Node::Neighbors::N ] && !i.neighbors[ Node::Neighbors::N ]->bwalkable ||
      i.neighbors[ Node::Neighbors::E ] && !i.neighbors[ Node::Neighbors::E ]->bwalkable )
      i.neighbors[ Node::Neighbors::NE ] = nullptr;

    if( i.neighbors[ Node::Neighbors::N ] && !i.neighbors[ Node::Neighbors::N ]->bwalkable ||
      i.neighbors[ Node::Neighbors::W ] && !i.neighbors[ Node::Neighbors::W ]->bwalkable )
      i.neighbors[ Node::Neighbors::NW ] = nullptr;

    if( i.neighbors[ Node::Neighbors::S ] && !i.neighbors[ Node::Neighbors::S ]->bwalkable ||
      i.neighbors[ Node::Neighbors::W ] && !i.neighbors[ Node::Neighbors::W ]->bwalkable )
      i.neighbors[ Node::Neighbors::SW ] = nullptr;
  }
}

void SetBoolsFromNeighborWalkable( bool& n , bool& w ,
  bool& e , bool& s , Node* a_node )
{
  if( a_node->neighbors[ Node::Neighbors::N ] )
    n = a_node->neighbors[ Node::Neighbors::N ]->bwalkable;

  if( a_node->neighbors[ Node::Neighbors::W ] )
    w = a_node->neighbors[ Node::Neighbors::W ]->bwalkable;

  if( a_node->neighbors[ Node::Neighbors::E ] )
    e = a_node->neighbors[ Node::Neighbors::E ]->bwalkable;

  if( a_node->neighbors[ Node::Neighbors::S ] )
    s = a_node->neighbors[ Node::Neighbors::S ]->bwalkable;
}

void Grid::CalculateTileIndex( Node* a_node )
{
  bool n = true , w = true , e = true , s = true;
  SetBoolsFromNeighborWalkable( n , w , e , s , a_node );
  if( n && !w && e  && s )
  {
    a_node->tileIndex = 4;
    if( !a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::W ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed = true;
    }
  }
  if( n && w && !e  && s )
  {
    a_node->tileIndex = 2;

    if( !a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::E ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed = true;
    }
  }
  if( n && !w && !e  && s )
  {
    a_node->tileIndex = 3;
    if( !a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::W ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::E ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed = true;
    }
  }

  if( !n && w && !e  && s )
  {
    a_node->tileIndex = 12;
    if( !a_node->neighbors[ Node::Neighbors::N ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::N ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::N ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::E ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed = true;
    }
  }

  if( !n && !w && e  && s )
  {
    a_node->tileIndex = 13;
    if( !a_node->neighbors[ Node::Neighbors::N ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::N ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::N ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::W ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed = true;
    }
  }

  if( n && w && e && !s )
  {
    a_node->tileIndex = 6;
    if( !a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::S ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed = true;
    }
  }

  if( !n && w && e  && s )
  {
    a_node->tileIndex = 16;
    if( !a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::N ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::N ]->btoBeProcessed = true;
    }
  }

  if( !n && !w && !e  && s )
  {
    a_node->tileIndex = 10;
    if( !a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::W ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::E ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed = true;
    }

    if( !a_node->neighbors[ Node::Neighbors::N ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::N ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::N ]->btoBeProcessed = true;
    }
  }

  if( n && !w && !e && !s )
  {
    a_node->tileIndex = 15;
    if( !a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::W ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::E ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::S ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed = true;
    }
  }

  if( n && w && e && !s )
  {
    a_node->tileIndex = 6;

    if( !a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::S ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed = true;
    }
  }

  if( !n && w && e && !s )
  {
    a_node->tileIndex = 11;
    if( !a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::S ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::N ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::N ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::N ]->btoBeProcessed = true;
    }
  }

  if( n && w && !e && !s )
  {
    a_node->tileIndex = 7;
    if( !a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::S ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::E ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed = true;
    }
  }

  if( n && !w && e && !s )
  {
    a_node->tileIndex = 8;
    if( !a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::S ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::W ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed = true;
    }
  }

  if( !n && !w && !e && !s )
  {
    a_node->tileIndex = 5;
    if( !a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::W ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::W ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::E ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::E ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::S ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::S ]->btoBeProcessed = true;
    }
    if( !a_node->neighbors[ Node::Neighbors::N ]->btoBeProcessed )
    {
      m_dirtyTileIndiciesQueue.push( a_node->neighbors[ Node::Neighbors::N ]->indexSingle );
      a_node->neighbors[ Node::Neighbors::N ]->btoBeProcessed = true;
    }
  }
}

void Grid::UpdateImage()
{
  int index;
  std::vector<Node*> nodesProcessed;
  while( !m_dirtyTileIndiciesQueue.empty() )
  {
    index = m_dirtyTileIndiciesQueue.front();
    m_dirtyTileIndiciesQueue.pop();
    int xPixelCoord , yPixelCoord;
    Node& currentNode = m_nodes[ index ];
    xPixelCoord = (int)( currentNode.index.x  * m_tileSizeX );
    yPixelCoord = (int)( currentNode.index.y  * m_tileSizeY );
    m_gridImage.copy( m_spriteSheet , xPixelCoord , yPixelCoord , tiles[ currentNode.tileIndex ] );
    nodesProcessed.push_back( &m_nodes[ index ] );
  }
  m_gridTexture.LoadFromImage( m_gridImage );
  for( auto i : nodesProcessed )
    i->btoBeProcessed = false;
}