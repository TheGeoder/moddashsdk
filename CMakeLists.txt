cmake_minimum_required(VERSION 3.22)
project(ModDashSDK LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# All files live directly in this root workspace directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ==========================================
# 🪟 WINDOWS 64-BIT COMPILATION TARGET
# ==========================================
if(WIN32)
    add_executable(ModDashLauncher
        main_win64.cpp                # 🟢 Links your Windows specific core
        imgui.cpp
        imgui_impl_win32.cpp
        imgui_impl_dx11.cpp
    )
    target_link_libraries(ModDashLauncher PRIVATE d3d11 d3dcompiler comdlg32)
    target_compile_definitions(ModDashLauncher PRIVATE _MBCS)

# ==========================================
# 📱 ANDROID OPENGL ES 3.0 COMPILATION TARGET
# ==========================================
elseif(ANDROID)
    add_library(ModDashAndroid SHARED
        main_android.cpp              # 🟢 Links your newly renamed Android core
        imgui.cpp
        imgui_impl_android.cpp
        imgui_impl_opengl3.cpp
    )
    find_library(LOG_LIB log)
    find_library(ANDROID_LIB android)
    find_library(GLES3_LIB GLESv3)
    target_link_libraries(ModDashAndroid PRIVATE ${LOG_LIB} ${ANDROID_LIB} ${GLES3_LIB})
endif()
