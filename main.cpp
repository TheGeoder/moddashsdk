#include <windows.h>
#include <d3d11.h>
#include <commdlg.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Link essential native 64-bit Windows graphics libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Global Device and State Pointers
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;
static ImTextureID              g_ModDashLogoTexture = nullptr;

static char gdPath[MAX_PATH] = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Geometry Dash\\GeometryDash.exe";
static bool closeOnLaunch = true;

// Forward declarations of Win32 setup helpers
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 🖼️ Function to load moddashlogo.png safely into Win64 DX11 Texture Memory
ImTextureID LoadModDashLogoDX11(const char* filename, ID3D11Device* d3d_device) {
    int image_width = 0, image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (!image_data) return (ImTextureID)nullptr;

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;

    ID3D11Texture2D* pTexture = NULL;
    d3d_device->CreateTexture2D(&desc, &subResource, &pTexture);

    ID3D11ShaderResourceView* out_srv = NULL;
    if (pTexture) {
        d3d_device->CreateShaderResourceView(pTexture, NULL, &out_srv);
        pTexture->Release();
    }

    stbi_image_free(image_data);
    return (ImTextureID)out_srv;
}

void BrowseForGDPath() {
    OPENFILENAMEA ofn;
    char szFile[MAX_PATH] = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Executables (*.exe)\0*.exe\0";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        strcpy_s(gdPath, sizeof(gdPath), ofn.lpstrFile);
    }
}

void LaunchModDash(const char* path) {
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcessA(path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        // --- Native 64-bit DLL Injection logic can be safely wired here ---
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        if (closeOnLaunch) ExitProcess(0);
    } else {
        MessageBoxA(NULL, "Failed to launch game. Verify path.", "Error", MB_ICONERROR);
    }
}

// 🎨 Render Interface layout matching your specifications perfectly
void RenderLauncherUI() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(540, 320));
    ImGui::Begin("ModDash", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    // 🔼 UPPER SIDE: BRANDING, LOGO & ICON
    ImGui::BeginChild("UpperSidePanel", ImVec2(520, 80), true);
    if (g_ModDashLogoTexture) {
        ImGui::Image(g_ModDashLogoTexture, ImVec2(48, 48));
        ImGui::SameLine();
        ImGui::SetCursorPosY(22);
    } else {
        ImGui::SetCursorPos(ImVec2(20, 22));
    }
    ImGui::SetWindowFontScale(1.6f);
    ImGui::TextColored(ImVec4(0.0f, 0.6f, 1.0f, 1.0f), "ModDash Launcher");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::EndChild();

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    // 🔽 LOWER SIDE: CONTROLS, SETTINGS & LAUNCH
    ImGui::BeginChild("SettingsControlBlock", ImVec2(320, 180), true);
    ImGui::Text("Launcher Configuration");
    ImGui::Separator(); ImGui::Spacing();
    ImGui::Text("Geometry Dash Location:");
    ImGui::InputText("##PathBox", gdPath, IM_ARRAYSIZE(gdPath));
    if (ImGui::Button("Browse Files...")) BrowseForGDPath();
    ImGui::Spacing();
    ImGui::Checkbox("Auto-close ModDash on launch", &closeOnLaunch);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.45f, 0.85f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.55f, 1.0f, 1.0f));
    if (ImGui::Button("LAUNCH\n\nGAME", ImVec2(190, 180))) {
        LaunchModDash(gdPath);
    }
    ImGui::PopStyleColor(2);
    ImGui::End();
}

// 💻 Core Win64 Desktop Entrypoint
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASSEXA wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, hInstance, NULL, NULL, NULL, NULL, "ModDashClass", NULL };
    RegisterClassExA(&wc);
    HWND hwnd = CreateWindowA(wc.lpszClassName, "ModDash", WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, 100, 100, 556, 359, NULL, NULL, hInstance, NULL);

    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        UnregisterClassA(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load logo file immediately after frame framework initialization
    g_ModDashLogoTexture = LoadModDashLogoDX11("moddashlogo.png", g_pd3dDevice);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessageA(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
            continue;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        RenderLauncherUI();

        ImGui::Render();
        const float clear_color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(1, 0); // Cap frame execution loop cleanly to active monitor refresh rate
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    DestroyWindow(hwnd);
    UnregisterClassA(wc.lpszClassName, wc.hInstance);
    return 0;
}

// DX11 Pipeline Initialization Helpers
bool CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK) return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget() { if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; } }

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;
    switch (msg) {
        case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}
