#pragma once
class Window;
bool        ImGui_Init( Window* window);
 void        ImGui_Shutdown();
 void        ImGui_NewFrame(float a_dt);

// Use if you want to reset your rendering device without losing ImGui state.
 void        ImGui_InvalidateDeviceObjects();
 bool        ImGui_CreateDeviceObjects();

// GLFW callbacks (installed by default if you enable 'install_callbacks' during initialization)
// Provided here if you want to chain callbacks.
// You can also handle inputs yourself and use those as a reference.
void        ImGui_MouseButtonCallback( Window* window , int button , int action , int mods );
void        ImGui_ScrollCallback( Window* window , double xoffset , double yoffset );
void        ImGui_KeyCallback( Window* window , int key , int action , int mods );
void        ImGui_CharCallback( Window* window , unsigned int c );

void Imgui_HandleEvents( void* a_event );