#pragma once
#include "Component.h"
class PathfindingAgentComponent;
class WanderingComponent :public Component
{
public:
  WanderingComponent( GameObject* a_go , EComponentTypes a_type );
  ~WanderingComponent();
  void BeginPlay() override;
  void Update( float a_dt ) override;
private:
  PathfindingAgentComponent* m_pathfindingComp;

};

