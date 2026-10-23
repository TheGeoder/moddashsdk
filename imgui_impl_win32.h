#pragma once
#include "imgui.h"
#include <windows.h>

IMGUI_IMPL_API bool     ImGui_ImplWin32_Init(void* hwnd);
IMGUI_IMPL_API void     ImGui_ImplWin32_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplWin32_NewFrame();

// Handler function used directly inside your main.cpp WndProc function
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
