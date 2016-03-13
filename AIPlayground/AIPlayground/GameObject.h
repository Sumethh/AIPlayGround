#pragma once
#include <vector>
#include "Transform.h"
#include "ComponentFactory.h"
#include "GameObjectTypes.h"
class Component;
class World;
class Window;

struct GameObjectConstructionDescriptor
{
  std::vector<EComponentTypes> listOfComps;
};


class GameObject
{
  friend class World;
public:
  GameObject( EGameObjectType a_type = EGameObjectType::GOT_Generic );
  virtual ~GameObject();

  virtual void OnCosntruct( GameObjectConstructionDescriptor* a_constructionDescriptor);
  virtual void OnDestroy();
  virtual void BeginPlay();
  virtual void Update( float a_dt );
  virtual void PreRender();
  virtual void Render( Window* a_windows );
  virtual void PostFrame();

  void AddComponent( EComponentTypes a_componentType );

  inline World* GetWorld() const { return m_world; }
  inline Transform GetTransform()const { return m_transform; }
  inline void SetTransfrom( const Transform a_newTransform )
  {
    m_transform = a_newTransform;
    m_renderStateDirty = true;
  }
  inline void SetPosition( glm::vec2 a_position )
  {
    m_transform.position = a_position;
    m_renderStateDirty = true;
  }
  inline void SetScale( glm::vec2 a_scale )
  {
    m_transform.scale = a_scale;
    m_renderStateDirty = true;
  }
  inline void SetRotation( float a_angle )
  {
    m_transform.rotation = a_angle;
    m_renderStateDirty = true;
  }

  inline void Destroy() { m_toBeDestroyed = true; }
  inline bool HasBegunPlay() { return m_hasBegunPlay; }
  inline bool IsDestroyed() { return m_toBeDestroyed; }
  inline void ResetRenderStateDirtyFlag() { m_renderStateDirty = false; }
  inline bool RenderStateDirty()const { return m_renderStateDirty; }

  Component* GetComponentOfType( EComponentTypes a_type );

  World* GetWorld() { return m_world; }

  inline EGameObjectType GetGoType() const { return m_goType; }

private:
  void SetWorld( World* a_newWorld ) { m_world = a_newWorld; };

  std::vector<Component*> m_components;
  std::vector<Component*> m_componentsToAdd;

  Transform m_transform;
  World* m_world;

  EGameObjectType m_goType;

  bool m_hasBegunPlay;
  bool m_toBeDestroyed;
  bool m_renderStateDirty;
};

