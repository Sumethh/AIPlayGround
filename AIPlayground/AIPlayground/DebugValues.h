#pragma once

class DebugValues
{
public:

  bool RenderGrid;

  static DebugValues* GI()
  {
    if( !m_instance )
      m_instance = new DebugValues;
    return m_instance;
  }

private:
  static DebugValues* m_instance;
};