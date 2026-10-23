#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Official Android Log Priority levels
typedef enum android_LogPriority {
    ANDROID_LOG_UNKNOWN = 0,
    ANDROID_LOG_DEFAULT,
    ANDROID_LOG_VERBOSE,
    ANDROID_LOG_DEBUG,
    ANDROID_LOG_INFO,     // Used by ModDash for standard launcher logs
    ANDROID_LOG_WARN,
    ANDROID_LOG_ERROR,    // Used by ModDash to log errors or crashes
    ANDROID_LOG_FATAL
} android_LogPriority;

// The native Android system function that prints your text to the debugger
int __android_log_print(int prio, const char* tag, const char* fmt, ...);

#ifdef __cplusplus
}
#endif
