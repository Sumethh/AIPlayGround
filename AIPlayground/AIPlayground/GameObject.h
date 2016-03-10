#pragma once
class GameObject
{
public:
  GameObject();
  virtual ~GameObject();

  virtual void Cosntruct();
  virtual void Destroy();
  virtual void Update();
  virtual void PreRender();
  virtual void Render();

private:



};

