#pragma  once
#include <windows.h>
#include <cstdio>
#include <string.h>

static HANDLE ___hstdOut = 0;
static CONSOLE_SCREEN_BUFFER_INFO ___defaultInfo;

enum ___TextColors
{
  ___RED = 0x0004 | 0x0008 ,
  ___GREEN = 0x002 | 0x008 ,
  ___BLUE = 0x001 | 0x008 ,
  ___YELLOW = ( ___RED | ___GREEN ) ,
  ___PINK = ( ___RED | ___BLUE ) ,
  ___LIGHTBLUE = ( ___GREEN | ___BLUE )
};

inline void ___ChangeColor( ___TextColors a_color )
{
  if( ___hstdOut == 0 )
  {
    ___hstdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    GetConsoleScreenBufferInfo( ___hstdOut , &___defaultInfo );
  }

  SetConsoleTextAttribute( ___hstdOut , (WORD)a_color );
}
inline void ___ResetColor()
{
  SetConsoleTextAttribute( ___hstdOut , ___defaultInfo.wAttributes );
}
#  define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#  define LOGLN() printf("\n");
#  define LOGI(f, ...) printf("%s [%d]: ", __FILENAME__, __LINE__), printf("[Info] "    ## f "\n", __VA_ARGS__)
#  define LOGW(f, ...) printf("%s [%d]: ", __FILENAME__, __LINE__), ___ChangeColor(___YELLOW), printf("[Warning] " ## f "\n", __VA_ARGS__), ___ResetColor()
#  define LOGE(f, ...) printf("%s [%d]: ", __FILENAME__, __LINE__), ___ChangeColor(___RED),  printf("[Error] "   ## f "\n", __VA_ARGS__), ___ResetColor()
#  define LOGS(f, ...) ___ChangeColor(___GREEN) , printf("[Success] "    ## f "\n", __VA_ARGS__), ___ResetColor()
#  define LOGf4(string, f) printf("[Float 4] " ##string " x: %f y:%f z: %f w: %f \n", f.x, f.y, f.z, f.w)
#  define LOGF3(string ,f) printf("[Vector3] " ##string " x: %f y: %f z: %f \n", f.x, f.y, f.z)
#  define LOGF(string, f) printf("[Float] "  ##string " %f \n", f )