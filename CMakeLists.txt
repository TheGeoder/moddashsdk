cmake_minimum_required(VERSION 3.22.1)
project(ModDashAndroid)

# Locate native Android graphics and logging libraries automatically
find_library(log-lib log)
find_library(android-lib android)
find_library(gles3-lib GLESv3)

# Define your ModDash compilation targets
add_library(moddash_launcher SHARED
    main.cpp
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_impl_android.cpp
    imgui_impl_opengl3.cpp
)

# Include directories
target_include_directories(moddash_launcher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link essential mobile system modules cleanly
target_link_libraries(moddash_launcher
    native_app_glue
    ${gles3-lib}
    ${android-lib}
    ${log-lib}
)
