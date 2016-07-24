#pragma once
#include <vector>
#include "Transform.h"
#include "ComponentFactory.h"
#include "GameObjectTypes.h"
#include "PhysicsStructs.h"
#include "Common/Types.h"
#include <Common/json.hpp>
class Component;
class World;
class Window;
class Renderer2D;
using json =  nlohmann::json;
enum EGameOjbectFlags
{
  RenderState = 1 << 0 ,
  PhysicsDirty = 1 << 1 ,
  CollisionRotationDirty = 1 << 2 ,
  RotationDirtyFlag = 1 <<3
};


/*Layers for the gameobjects, these will determine the z order of the objects the background is drawn on level 10*/
enum class ELayerID : uint8
{
  DynamicObject = 3 ,
  StaticObject = 4,
  COUNT
};

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

  typedef std::shared_ptr<GameObject> SharedPtr;
  typedef std::weak_ptr<GameObject> WeakPtr;
  virtual void OnCosntruct( GameObjectConstructionDescriptor* a_constructionDescriptor );
  virtual void OnDestroy();
  virtual void BeginPlay();
  virtual void Update( float a_dt );
  virtual void FixedUpdate( float a_dt );
  virtual void PreRender();
  virtual void Render( Renderer2D* a_renderer );
  virtual void PostFrame();

  virtual void OnCollisionEnter( Collision a_collision );
  virtual void OnCollisionLeave( Collision a_collision );
  virtual void OnCollisionStay( Collision a_collision );

  virtual void OnSave(json& a_json);
  virtual void OnLoad(json& a_json);


  Component* AddComponent( EComponentTypes a_componentType );

  inline World* GetWorld() const { return m_world; }
  inline Transform GetTransform()const { return m_transform; }
  inline void SetTransfrom( const Transform a_newTransform )
  {
    m_transform = a_newTransform;
    SetPhysicsFlagDirty();
    SetCollisionFlagDirty();
    SetRenderStateDirty();
    SetRotationFlagDirty();
  }
  inline void SetPosition( glm::vec2 a_position )
  {
    m_transform.position = a_position;
    SetPhysicsFlagDirty();
    SetCollisionFlagDirty();
    SetRenderStateDirty();
  }
  inline void SetScale( glm::vec2 a_scale )
  {
    m_transform.scale = a_scale;
    SetPhysicsFlagDirty();
    SetRenderStateDirty();
  }
  inline void SetRotation( float a_angle )
  {
    m_transform.rotation = a_angle;
    SetPhysicsFlagDirty();
    SetCollisionFlagDirty();
    SetRenderStateDirty();
    SetRotationFlagDirty();
  }

  inline void Destroy() { m_toBeDestroyed = true; }
  inline bool HasBegunPlay() { return m_hasBegunPlay; }
  inline bool IsDestroyed() { return m_toBeDestroyed; }

  inline int GetRenderStateDirty()const { return m_gameObjectFlags & EGameOjbectFlags::RenderState; }
  inline void ResetRenderStateDirtyFlag() { m_gameObjectFlags &= ~EGameOjbectFlags::RenderState; }
  inline void SetRenderStateDirty() { m_gameObjectFlags |= EGameOjbectFlags::RenderState; }
  Component* GetComponentOfType( EComponentTypes a_type );
  template<class T>
  T* TGetComponentOfType(EComponentTypes a_type);

  World* GetWorld() { return m_world; }

  inline EGameObjectType GetGoType() const { return m_goType; }

  inline int GetPhysicsDirtyFlag() const { return m_gameObjectFlags & EGameOjbectFlags::PhysicsDirty; }
  inline void ResetPhysicsDirtyFlag() { m_gameObjectFlags &= ~EGameOjbectFlags::PhysicsDirty; }
  inline void SetPhysicsFlagDirty() { m_gameObjectFlags |= EGameOjbectFlags::PhysicsDirty; }

  inline int GetCollisionDirtyFlag()const { return m_gameObjectFlags & EGameOjbectFlags::CollisionRotationDirty; }
  inline void ResetCollisionDirtyFlag() { m_gameObjectFlags &= ~EGameOjbectFlags::CollisionRotationDirty; }
  inline void SetCollisionFlagDirty() { m_gameObjectFlags |= EGameOjbectFlags::CollisionRotationDirty; }

  ELayerID GetLayer() { return m_currentLayer; }
  void SetLayer( ELayerID a_layer) { m_currentLayer = a_layer; }

  inline void SetRotationFlagDirty() { m_gameObjectFlags |= EGameOjbectFlags::RotationDirtyFlag; }
  inline int GetRotationFlagDirty() { return m_gameObjectFlags & EGameOjbectFlags::RotationDirtyFlag; }
  inline void ResetRotationDityFlag() { m_gameObjectFlags &= ~EGameOjbectFlags::RotationDirtyFlag; }
private:
  void SetWorld( World* a_newWorld ) { m_world = a_newWorld; };
  ELayerID m_currentLayer;
  std::vector<Component*> m_components;
  std::vector<Component*> m_componentsToAdd;

  Transform m_transform;
  World* m_world;

  EGameObjectType m_goType;

  bool m_hasBegunPlay;
  bool m_toBeDestroyed;

  int m_gameObjectFlags;

  bool m_renderStateDirty;
  bool m_physicsDirty;
  bool m_rotationMatrixDirty;
};

template<class T>
T* GameObject::TGetComponentOfType(EComponentTypes a_type)
{
  for (auto itr = m_components.begin(); itr != m_components.end(); ++itr)
    if ((*itr)->IsComponentOfType(a_type))
      return (T*)(*itr);
  return nullptr;
}
