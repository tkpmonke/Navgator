#pragma once


#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Navgator is made so you can you whatever backend you want!
// GLFW & ImGui was the main use-target for navgator so this is the
// main example of a backend. If you would like to make a new
// backend (eg. sfml, sdl, your own engine ) 

// constant string telling what backend is being used
#define WINDOW_BACKEND "glfw"

// these parameters will be passed into the init function
#define WINDOW_BACKEND_INIT_PARAMS GLFWwindow* window

// what will be called when initilized
#define WINDOW_BACKEND_INIT                                    \
            ImGui_ImplGlfw_InitForOpenGL(window, true);        \
            ImGui_ImplOpenGL3_Init();

// This is called before rendering the frame, after glfwPollEvents()
#define WINDOW_BACKEND_BEGIN_FRAME                             \
            ImGui_ImplOpenGL3_NewFrame();                      \
            ImGui_ImplGlfw_NewFrame();                         \
            ImGui::NewFrame();

#define WINDOW_BACKEND_RENDER                                  \
            ImGui::Render();                                   \
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


#define WINDOW_BACKEND_SHUTDOWN                 \
            ImGui_ImplOpenGL3_Shutdown();       \
            ImGui_ImplGlfw_Shutdown();        

#define WINDOW_BACKEND_SWAP_BUFFERS(window) glfwSwapBuffers(window)

#define WINDOW_BACKEND_POLL_EVENTS glfwPollEvents()

#define WINDOW_BACKEND_WINDOW_OBJECT GLFWwindow*
