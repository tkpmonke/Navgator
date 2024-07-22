#pragma once

#define NAVGATOR_USE_GLFW

#ifdef NAVGATOR_USE_GLFW
#include "backends/glfw_backend.h"
#else
#error A Backend Needs to be Declared
#endif

#ifdef DEBUG
#define NAVGATOR_PRINT_WARNINGS
#endif


#include <iostream>
#include <fstream>
#include <filesystem>

// Use this if you already are managing imgui
// (eg, creating imgui, creation of frames, shutting down)
//#define NAVGATOR_IMGUI_ALREADY

namespace Navgator {

   struct Result {
      std::string path;
      bool succeded;
      unsigned long size;
      std::string type;
   };

#ifndef NAVGATOR_IMGUI_ALREADY
   // Only Call if imgui isn't already initilized
   void Init(WINDOW_BACKEND_INIT_PARAMS);
    
   // Once again, only if you don't already shutdown imgui
   void Shutdown();
#endif

   // Launch File Explorer
   Result Launch(char* fileTypes[],
         WINDOW_BACKEND_WINDOW_OBJECT window,
         std::string startingDir = "/home");
}
