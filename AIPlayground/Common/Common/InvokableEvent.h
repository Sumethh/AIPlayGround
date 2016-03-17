#pragma once
#include "Types.h"
#include <functional>
#include <vector>

template <typename T>
class InvokableEvent
{
public:
  InvokableEvent();
  ~InvokableEvent();

  void Invoke( T a_t );
  uint RegisterToEvent( std::function<void( T )> );
  void DeRegisterToEvent( uint a_index);

private:
  std::vector < std::function<void( T )>> m_callbacks;
};

template<class T>
InvokableEvent<T>::InvokableEvent()
{
}

template<class T>
InvokableEvent<T>::~InvokableEvent()
{
}

template<class T>
void InvokableEvent<T>::Invoke( T a_t )
{
  for( auto i : m_callbacks )
  {
    i( a_t );
  }
}

template <class T>
uint InvokableEvent<T>::RegisterToEvent( std::function<void( T )> a_func)
{
  m_callbacks.push_back( a_func );
  return m_callbacks.size() - 1;
}

template <class T>
void InvokableEvent<T>::DeRegisterToEvent( uint a_index )
{
  auto iterator = m_callbacks.begin() + a_index;
  m_callbacks.erase( iterator );
}