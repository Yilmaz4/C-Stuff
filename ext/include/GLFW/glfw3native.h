/*************************************************************************
 * GLFW 3.3 - www.glfw.org
 * A library for OpenGL, window and input
 *------------------------------------------------------------------------
 * Copyright (c) 2002-2006 Marcus Geelnard
 * Copyright (c) 2006-2018 Camilla Löwy <elmindreda@glfw.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would
 *    be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 *************************************************************************/

#ifndef _glfw3_native_h_
#define _glfw3_native_h_

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(GLFW_NATIVE_INCLUDE_NONE)

 #if defined(GLFW_EXPOSE_NATIVE_WIN32) || defined(GLFW_EXPOSE_NATIVE_WGL)
  #if defined(GLFW_APIENTRY_DEFINED)
   #undef APIENTRY
   #undef GLFW_APIENTRY_DEFINED
  #endif
  #include <windows.h>
 #elif defined(GLFW_EXPOSE_NATIVE_COCOA) || defined(GLFW_EXPOSE_NATIVE_NSGL)
  #if defined(__OBJC__)
   #import <Cocoa/Cocoa.h>
  #else
   #include <ApplicationServices/ApplicationServices.h>
   #include <objc/objc.h>
  #endif
 #elif defined(GLFW_EXPOSE_NATIVE_X11) || defined(GLFW_EXPOSE_NATIVE_GLX)
  #include <X11/Xlib.h>
  #include <X11/extensions/Xrandr.h>
 #elif defined(GLFW_EXPOSE_NATIVE_WAYLAND)
  #include <wayland-client.h>
 #endif

 #if defined(GLFW_EXPOSE_NATIVE_WGL)
 #endif
 #if defined(GLFW_EXPOSE_NATIVE_NSGL)
 #endif
 #if defined(GLFW_EXPOSE_NATIVE_GLX)
  #if defined(GLFW_GLAPIENTRY_DEFINED)
   #undef GLAPIENTRY
   #undef GLFW_GLAPIENTRY_DEFINED
  #endif
  #include <GL/glx.h>
 #endif
 #if defined(GLFW_EXPOSE_NATIVE_EGL)
  #include <EGL/egl.h>
 #endif
 #if defined(GLFW_EXPOSE_NATIVE_OSMESA)
  #if defined(GLFW_GLAPIENTRY_DEFINED)
   #undef GLAPIENTRY
   #undef GLFW_GLAPIENTRY_DEFINED
  #endif
  #include <GL/osmesa.h>
 #endif

#endif

#if defined(GLFW_EXPOSE_NATIVE_WIN32)
GLFWAPI const char* glfwGetWin32Adapter(GLFWmonitor* monitor);
GLFWAPI const char* glfwGetWin32Monitor(GLFWmonitor* monitor);
GLFWAPI HWND glfwGetWin32Window(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_WGL)
GLFWAPI HGLRC glfwGetWGLContext(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_COCOA)
GLFWAPI CGDirectDisplayID glfwGetCocoaMonitor(GLFWmonitor* monitor);
GLFWAPI id glfwGetCocoaWindow(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_NSGL)
GLFWAPI id glfwGetNSGLContext(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_X11)
GLFWAPI Display* glfwGetX11Display(void);
GLFWAPI RRCrtc glfwGetX11Adapter(GLFWmonitor* monitor);
GLFWAPI RROutput glfwGetX11Monitor(GLFWmonitor* monitor);
GLFWAPI Window glfwGetX11Window(GLFWwindow* window);
GLFWAPI void glfwSetX11SelectionString(const char* string);
GLFWAPI const char* glfwGetX11SelectionString(void);
#endif

#if defined(GLFW_EXPOSE_NATIVE_GLX)
GLFWAPI GLXContext glfwGetGLXContext(GLFWwindow* window);
GLFWAPI GLXWindow glfwGetGLXWindow(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_WAYLAND)
GLFWAPI struct wl_display* glfwGetWaylandDisplay(void);
GLFWAPI struct wl_output* glfwGetWaylandMonitor(GLFWmonitor* monitor);
GLFWAPI struct wl_surface* glfwGetWaylandWindow(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_EGL)
GLFWAPI EGLDisplay glfwGetEGLDisplay(void);
GLFWAPI EGLContext glfwGetEGLContext(GLFWwindow* window);
GLFWAPI EGLSurface glfwGetEGLSurface(GLFWwindow* window);
#endif

#if defined(GLFW_EXPOSE_NATIVE_OSMESA)
GLFWAPI int glfwGetOSMesaColorBuffer(GLFWwindow* window, int* width, int* height, int* format, void** buffer);
GLFWAPI int glfwGetOSMesaDepthBuffer(GLFWwindow* window, int* width, int* height, int* bytesPerValue, void** buffer);
GLFWAPI OSMesaContext glfwGetOSMesaContext(GLFWwindow* window);
#endif

#ifdef __cplusplus
}
#endif

#endif

