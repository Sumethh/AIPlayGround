#include "World.h"
#include "GameObject.h"


World::World()
{
}


World::~World()
{
}


void World::SetGameObjectWolrd( GameObject* a_gameobject )
{
  if( a_gameobject )
    a_gameobject->SetWorld( this );
}