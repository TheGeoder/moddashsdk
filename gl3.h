#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Standard Khronos Graphics Types for Mobile Devices
typedef unsigned int  GLuint;
typedef unsigned int  GLenum;
typedef unsigned int  GLbitfield;
typedef int           GLint;
typedef int           GLsizei;
typedef float         GLfloat;
typedef char          GLchar;
typedef unsigned char GLubyte;

// Essential OpenGL ES 3.0 System Constants
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_VERSION                        0x1F02

// Native mobile graphics pipeline function hooks used inside main_android.cpp
void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void glClear(GLbitfield mask);
const GLubyte* glGetString(GLenum name);

#ifdef __cplusplus
}
#endif
