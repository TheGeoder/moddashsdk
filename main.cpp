#include <android_log.h>
#include <android_native_app_glue.h>
#include <gl3.h>
#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"

#define LOG_TAG "ModDashAndroid"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))

static bool closeOnLaunch = true;
static char gdDataPath[256] = "/sdcard/Android/data/com.robtopx.geometryjump/";

void RenderAndroidLauncherUI() {
    // Lock window parameters to adapt seamlessly to mobile touchscreen devices
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("ModDash Android Canvas", nullptr, 
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    // 🔼 UPPER SIDE: BRANDING & TITLE
    ImGui::BeginChild("UpperSidePanel", ImVec2(-1, 120), true);
    ImGui::SetWindowFontScale(2.0f); // Scale font larger for mobile screens
    ImGui::TextColored(ImVec4(0.0f, 0.6f, 1.0f, 1.0f), "ModDash Android");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::EndChild();

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    // 🔽 LOWER SIDE: SETTINGS & ACTION BUTTON
    ImGui::BeginChild("SettingsControlBlock", ImVec2(-1, 300), true);
    ImGui::Text("Target Game Directory Path:");
    ImGui::InputText("##PathBox", gdDataPath, IM_ARRAYSIZE(gdDataPath));
    
    ImGui::Spacing();
    ImGui::Checkbox("Auto-close ModDash on launch", &closeOnLaunch);
    ImGui::EndChild();

    ImGui::Spacing();

    // Large tap targets are essential for touch screen interactions
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.45f, 0.85f, 1.0f));
    if (ImGui::Button("LAUNCH GEOMETRY DASH", ImVec2(-1, 150))) {
        LOGI("ModDash: Launching Android APK Intent...");
        // --- Custom .so injection / path patching logic goes here ---
    }
    ImGui::PopStyleColor(1);

    ImGui::End();
}

// 📱 Core Native Entrypoint for Android OS Loop handling
void android_main(struct android_app* state) {
    // App initialization loops
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Bind mobile input queues and OpenGL hooks directly
    ImGui_ImplAndroid_Init(state->window);
    ImGui_ImplOpenGL3_Init("#version 300 es"); // Target mobile OpenGL ES 3.0 specs

    bool running = true;
    while (running) {
        // Read native mobile inputs/events loop 
        int events;
        struct android_poll_source* source;
        while (ALooper_pollAll(0, nullptr, &events, (void**)&source) >= 0) {
            if (source != nullptr) source->process(state, source);
            if (state->destroyRequested != 0) running = false;
        }

        // Stepping frame loop
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplAndroid_NewFrame();
        ImGui::NewFrame();

        RenderAndroidLauncherUI();

        ImGui::Render();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // Cleanup resources cleanly before unloading process memory
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
}
