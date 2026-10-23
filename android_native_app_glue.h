#pragma once
#include <poll.h>
#include <pthread.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_window.h>

// Core structural state data wrapper for your ModDash execution loop
struct android_app {
    void* userData;
    void (*onAppCmd)(struct android_app* app, int32_t cmd);
    int32_t (*onInputEvent)(struct android_app* app, AInputEvent* event);

    ANativeActivity* activity;
    AConfiguration* config;
    ALooper* looper;

    ANativeWindow* window;
    int destroyRequested;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

// Android execution event identifiers
enum {
    APP_CMD_INPUT_CHANGED,
    APP_CMD_INIT_WINDOW,
    APP_CMD_TERM_WINDOW,
    APP_CMD_DESTROY
};

// Global lifecycle loop functions invoked natively inside your main script
void app_dummy();
