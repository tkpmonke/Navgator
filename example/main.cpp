#define GLEW_NO_GLU
#include "GL/glew.h" // for glClear

#define NAVGATOR_USE_GLFW
#include "navgator.hpp"

int main()
{
   glfwInit();
   GLFWwindow* window = glfwCreateWindow(700, 700, "test window", NULL, NULL);
   glfwMakeContextCurrent(window);
   glewInit();
   
   // INIT + SHUTDOWN ONLY NEED TO BE CALLED IF IMGUI ISN'T ALREADY INITILIZED
   Navgator::Init(window);
   
   Navgator::Result file = Navgator::Launch((char**)".txt", window); 
   
   while (!glfwWindowShouldClose(window))
   {
      glClear(GL_COLOR_BUFFER_BIT);
      glfwPollEvents();
      WINDOW_BACKEND_BEGIN_FRAME;
      
      ImGui::Begin("Navgater Test App");
      if (file.succeded) { 
         ImGui::Text(file.path.data());
      } else {
         ImGui::Text("Navgator Failed >:(");
      }
      ImGui::End();

      WINDOW_BACKEND_RENDER;
      
      glfwSwapBuffers(window);
   }
   
   Navgator::Shutdown();

   return 0;
}
