workspace "navgator"
   architecture "x86_64"
   configurations { "Debug", "Release"}

project "navgator"
   kind "ConsoleApp"
   language "C++"
   cppdialect "gnu++17"
   targetdir "bin"
   includedirs { "../src", "../imgui" }
   files { "**.h", "**.cpp", "**.hpp" }
   
   links { "glfw", "GL", "GLEW" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

