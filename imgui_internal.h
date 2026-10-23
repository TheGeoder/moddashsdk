#pragma once
#include "imgui.h"

// Internal structures used by imgui.cpp to manage ModDash canvas rendering
struct ImGuiWindow;
struct ImGuiContext;

namespace ImGui {
    // Internal layout tracking utilities
    ImGuiWindow* GetCurrentWindow();
    void         FocusWindow(ImGuiWindow* window);
    
    // Internal item storage system
    void         PushMultiItemsWidths(int components, float width_full);
}
