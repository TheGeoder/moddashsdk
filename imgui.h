// ImGui Codebase - Core Definitions Context Layout
#pragma once

// Core structural layout definitions
#ifndef IMGUI_VERSION
#define IMGUI_VERSION "1.91.x" // Current stable build layout context
#endif

#include <stddef.h> // size_t
#include <string.h> // ImVector configurations

// Forward declarations of basic types used in ModDash
struct ImGuiContext;          // Global state engine
struct ImGuiWindow;           // Window configurations
struct ImDrawData;            // Render pass pipeline pipelines
typedef void* ImTextureID;    // 🖼️ Used for your moddashlogo.png mapping

// ImGui Context System Flags
enum ImGuiWindowFlags_ {
    ImGuiWindowFlags_None           = 0,
    ImGuiWindowFlags_NoTitleBar     = 1 << 0,   // Hides the default top bar
    ImGuiWindowFlags_NoResize       = 1 << 1,   // Locks window dimensions
    ImGuiWindowFlags_NoMove         = 1 << 2,   // Locks launcher window position
    ImGuiWindowFlags_AlwaysAutoResize=1 << 6,   // Fits layout instantly
};

// Math helpers for placing buttons, images, and panels
struct ImVec2 {
    float x, y;
    ImVec2() { x = y = 0.0f; }
    ImVec2(float _x, float _y) { x = _x; y = _y; }
};

struct ImVec4 {
    float x, y, z, w;
    ImVec4() { x = y = z = w = 0.0f; }
    ImVec4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
};

// ==========================================
// 🛠️ THE CORE IMGUI FUNCTION API 
// ==========================================
namespace ImGui {
    // Context initialization engine links
    ImGuiContext* CreateContext(void* shared_font_atlas = NULL);
    void          DestroyContext(ImGuiContext* ctx = NULL);
    
    // Application runtime execution loops
    void          NewFrame();
    void          Render();
    ImDrawData*   GetDrawData();

    // Window canvas structures (Used for ModDash Main Window)
    bool          Begin(const char* name, bool* p_open = NULL, int flags = 0);
    void          End();

    // Layout blocks (Used to separate Upper Side from Lower Side panels)
    bool          BeginChild(const char* str_id, const ImVec2& size = ImVec2(0,0), bool border = false, int flags = 0);
    void          EndChild();

    // Interactive elements
    void          Text(const char* fmt, ...);
    void          TextColored(const ImVec4& col, const char* fmt, ...);
    bool          Button(const char* label, const ImVec2& size = ImVec2(0,0));
    bool          Checkbox(const char* label, bool* v);
    bool          InputText(const char* label, char* buf, size_t buf_size, int flags = 0);

    // Image utilities (Renders moddashlogo.png)
    void          Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0,0), const ImVec2& uv1 = ImVec2(1,1));

    // Design adjustments
    void          Spacing();
    void          Separator();
    void          SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);
    void          SetWindowFontScale(float scale);
}
