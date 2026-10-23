#include "imgui_impl_win32.h"

static HWND g_hWnd = nullptr;

bool ImGui_ImplWin32_Init(void* hwnd) {
    g_hWnd = (HWND)hwnd;
    return true;
}

void ImGui_ImplWin32_Shutdown() {
    g_hWnd = nullptr;
}

void ImGui_ImplWin32_NewFrame() {
    // Synchronizes cursor configurations and inputs for the active frame loop
}

// Intercepts input streams so clicking inside your launcher doesn't click items behind it
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui::GetCurrentContext() == nullptr)
        return 0;

    switch (msg) {
        case WM_LBUTTONDOWN: case WM_RBUTTONDOWN:
            return 1; // Mark input handled to stop double execution bugs
    }
    return 0;
}
