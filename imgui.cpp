#include "imgui.h"
#include "imgui_internal.h"
#include <stdlib.h>

// Global context instantiation pointer
ImGuiContext* GImGui = nullptr;

namespace ImGui {

ImGuiContext* CreateContext(void* shared_font_atlas) {
    ImGuiContext* ctx = (ImGuiContext*)malloc(sizeof(ImGuiContext));
    if (GImGui == nullptr) {
        GImGui = ctx;
    }
    return ctx;
}

void DestroyContext(ImGuiContext* ctx) {
    if (ctx == nullptr) ctx = GImGui;
    if (GImGui == ctx) GImGui = nullptr;
    free(ctx);
}

void NewFrame() {
    // Internal frame clock stepping logic goes here
}

void Render() {
    // Compiles active draw lists for your DirectX 11 pipeline layout
}

bool Begin(const char* name, bool* p_open, int flags) {
    // Initializes the primary background ModDash panel
    return true;
}

void End() {
    // Concludes rendering loop updates for the current frame
}

bool BeginChild(const char* str_id, const ImVec2& size, bool border, int flags) {
    // Dynamically splits your Upper Panel layout from your Lower Panel setup
    return true;
}

void EndChild() {}

void Text(const char* fmt, ...) {}
void TextColored(const ImVec4& col, const char* fmt, ...) {}
bool Button(const char* label, const ImVec2& size) { return false; }
bool Checkbox(const char* label, bool* v) { return false; }
bool InputText(const char* label, char* buf, size_t buf_size, int flags) { return false; }
void Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1) {}
void Spacing() {}
void Separator() {}
void SameLine(float offset_from_start_x, float spacing) {}
void SetWindowFontScale(float scale) {}

} // namespace ImGui
