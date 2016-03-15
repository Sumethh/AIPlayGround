#pragma once
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

class Window;

struct Node
{
  enum Neighbors
  {
    NW = 0 ,
    N = 1 ,
    NE = 2 ,
    W = 3 ,
    E = 4 ,
    SW = 5 ,
    S = 6 ,
    SE = 7 ,
    NeighborCount
  };
  glm::vec2 pos;
  glm::vec2 center;
  glm::vec2 index;
  int indexSingle;
  Node* neighbors[ 8 ];
  Node* parent;
  float gCost , hCost , fCost;
  bool bwalkable;
  int tileIndex;
  bool btoBeProcessed;
  inline float GetFCost()
  {
    fCost = gCost + hCost;
    return fCost;
  }
};

class Grid
{
public:
  Grid( glm::vec2 m_gridOrigin , int a_tileCountX , int a_tileCountY , int a_tileSizeX , int a_tileSizeY );
  ~Grid();

  void Init();
  void PreRender( const glm::vec2 a_cameraPo );
  void Render( Window* const a_windowToDrawTo );


  inline int GetTileSizeX() const { return m_tileSizeX; }
  inline int GetTileSizeY() const { return m_tileSizeY; }

  inline Node* GetNode( const glm::vec2 a_pos )
  {
    const int indexX = (int)a_pos.x / m_tileSizeX;
    const int indexY = (int)a_pos.y / m_tileSizeY;
    if( (size_t)( indexX + indexY * m_tileCountX ) < m_nodes.size() )
      return &m_nodes[ indexY* m_tileCountX + indexX ];
    return nullptr;


  }

  inline Node* GetNode( const int a_indexX , const int a_indexY )
  {
    if( (size_t)( a_indexX + a_indexY * m_tileCountX ) < m_nodes.size() )
      return &m_nodes[ a_indexY* m_tileCountX + a_indexX ];

    return nullptr;
  }

  inline void SetNodeWalkable( const int a_indexX , const int a_indexY , const bool a_walkable )
  {
    if( (size_t)( a_indexX + a_indexY * m_tileCountX ) < m_nodes.size() )
    {
      Node& node = m_nodes[ a_indexY * m_tileCountX + a_indexX ];
      if( node.bwalkable != a_walkable )
      {
        node.bwalkable = a_walkable;
        node.btoBeProcessed = true;

        m_dirtyTileIndiciesQueue.push( a_indexY * m_tileCountX + a_indexX );
        m_bImageDirty = true;
      }
    }
    return;
  }

  inline void SetNodeTileIndex( const int a_indexX , const int a_indexY , const int a_newTileIndex )
  {
    if( (size_t)( a_indexX + a_indexY * m_tileCountX ) < m_nodes.size() )
    {
      Node& node = m_nodes[ a_indexY * m_tileCountX + a_indexX ];
      if( node.tileIndex != a_newTileIndex )
      {
        node.tileIndex = a_newTileIndex;
        node.bwalkable = a_newTileIndex > 0;
        node.btoBeProcessed = true;
        m_dirtyTileIndiciesQueue.push( a_indexY * m_tileCountX + a_indexX );
        m_bImageDirty = true;
      }
    }
    return;
  }

  bool SaveToDisk();
  bool LoadFromDisk();

private:
  Grid( const Grid& ) = delete;
  Grid& operator = ( const Window& ) = delete;

  void CreateGrid();
  void CalculateNeighbors();

  void CalculateTileIndex( Node* a_node );

  void UpdateImage();

  sf::Sprite m_gridSprite;
  sf::Texture m_gridTexture;

  sf::Image m_spriteSheet;
  sf::Image m_gridImage;

  std::queue<int> m_dirtyTileIndiciesQueue;
  bool m_bImageDirty;

  int m_tileCountX , m_tileCountY;
  int m_tileSizeX , m_tileSizeY;
  glm::vec2 m_gridOrigin;
  std::vector<Node> m_nodes;
};

