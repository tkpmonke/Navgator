#include "navgator.hpp"
#include <stdlib.h>
#include <map>
#include <set>
#include <vector>

std::map<std::string, std::string> filetypes = {
   {"png", "PNG Image"},
   {"jpg", "JPEG Image"},
   {"jpeg", "JPEG Image"},
   {"txt", "Text Image"},
   {"wav", "Audio File"},
   {"mp3", "Audio File"},
   {"ogg", "Vortex Audio File"},
   {"exe", "Windows Executable"},
   {"bat", "Windows Batch File"},
   {"obj", "Object File"},
   {"fbx", "Autodesk Model File"},
   {"sh", "Unix Shell Script"},
   {"zip", "Compressed Zip File"},
   {"7z", "Compressed 7Zip File"},
   {"pdf", "Portable Document Format"},
   {"bin", "Binary File"},
   {"iso", "Disc Image File"},
   {"ttf", "True Type Font"},
   {"otf", "Open Type Font"},
   {"webp", "Goofy Image File"},
   {"c", "C File"},
   {"cpp", "C++ Script"},
   {"h", "C/C++ Header File"},
   {"hpp", "C++ Header File"},
   {"java", "Java Script"},
   {"py", "Python Script"},
   {"js", "JavaScript File"},
   {"html", "Hyper Text Markup Language"},
   {"css", "Cascading Style Sheet"},
   {"cs", "C# Script"},
   {"php", "PHP Script"},
   {"r", "R Script"},
   {"lua", "Lua Script"},
   {"go", "Go Script"},
   {"asm", "Assembly File"},
   {"rs", "Rust Script"},
   {"conf", "Config File"},
   {"config", "Config File"},
   {"gitignore", "Git Ignore File"}};

std::string to_lower(std::string s)
{
   std::locale loc;
   std::string str;

   for(auto elem : s)
      str += std::tolower(elem,loc);
   return str;
}

#define HEADER_PIXEL(data,pixel) {\
pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
data += 4; \
}
#include "images/file.h"
#include "images/folder.h"
#include "images/empty_folder.h"

int LoadTexture(char* data)
{
   std::vector<char> image;
   for (int i = 0; i < 64*64*3; i += 3)
   {
      char a[4];
      HEADER_PIXEL(data, a);
      image.push_back(-a[0]);
      image.push_back(-a[1]);
      image.push_back(-a[2]);
      if (a[0] == 0 && a[1] == 0)
         image.push_back(0);
      else
         image.push_back(254);
   }

   GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

    return image_texture;
}

int image_file;
int image_empty_folder;
int image_folder;

void LoadTextures()
{
   image_file = LoadTexture(file);
   image_empty_folder = LoadTexture(empty_folder);
   image_folder = LoadTexture(folder);
}

namespace Navgator {
#ifndef NAVGATOR_IMGUI_ALREADY 
   void Init(WINDOW_BACKEND_INIT_PARAMS)
   {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     
      WINDOW_BACKEND_INIT
   }

   void Shutdown()
   {
      WINDOW_BACKEND_SHUTDOWN;
      ImGui::DestroyContext(); 
   }

#endif
   std::string temp;
   Result Launch(char* fileTypes[], WINDOW_BACKEND_WINDOW_OBJECT window, std::string startingDir)
   {
      if (image_file == 0)
         LoadTextures();

      bool open = true;
      Result r;
      std::string path;
      if (temp == "") temp = startingDir;
      while (open)   {
         
         glClear(GL_COLOR_BUFFER_BIT);
         WINDOW_BACKEND_POLL_EVENTS;
         WINDOW_BACKEND_BEGIN_FRAME;

         ImGui::OpenPopup("Navgator");
         if (ImGui::BeginPopupModal("Navgator"))
         {
            if (ImGui::BeginChild("Files", ImVec2(0, 600)))
            {
               std::set<std::filesystem::path> sorted_list;
               for (const auto& entry : std::filesystem::directory_iterator(temp)) {
                     sorted_list.insert(entry.path());
               }
               for (const auto& entry : sorted_list)
               {
                  if (temp.find("/home") != -1) { //quick and dirty linux fix so apps don't have to run in sudo
                     if (std::filesystem::is_directory(entry) && !std::filesystem::is_empty(entry)) {
                        ImGui::Image((void*)(intptr_t)image_folder, ImVec2(20, 20));
                     } else if (std::filesystem::is_directory(entry) && std::filesystem::is_empty(entry)) {
                        ImGui::Image((void*)(intptr_t)image_empty_folder, ImVec2(20, 20));
                     } else {
                        ImGui::Image((void*)(intptr_t)image_file, ImVec2(20, 20));
                     }
                     ImGui::SameLine();
                  }

                  if (ImGui::Button(entry.filename().string().data(), ImVec2(0, 20)))
                  {
                     if (std::filesystem::is_directory(entry.string()))
                     {
                        temp = entry.string();
                        path = entry.string();
                     } else {
                        path = entry.string();
                     }
                  } 
               }
               ImGui::EndChild();
            }
            ImGui::Separator();

            if (ImGui::Button("Page Up", ImVec2(60, 20)) && temp.find_last_of("/") != 0) temp = std::string(temp.begin(), temp.begin() + temp.find_last_of("/"));

            ImGui::Separator();
            
            if (ImGui::InputText("File Path", path.data(), 512))
            {
               if (std::filesystem::is_directory(path))
               {
                  temp = path;
               }
            }
            ImGui::SameLine(0.f, ImGui::GetStyle().ItemInnerSpacing.x);
            
            if (ImGui::Button("Enter", ImVec2(100, 25))) {
               ImGui::CloseCurrentPopup(); 
               open = false;

               if (std::filesystem::exists(path) && !std::filesystem::is_directory(path))
               {
                  r.size = std::filesystem::file_size(path);
                  r.path = path;
                  r.succeded = true;
               } else {
                  r.succeded = false;
               }
            }
            ImGui::SameLine();

            if (ImGui::Button("X")) {
               ImGui::CloseCurrentPopup();
               open = false;
               r.succeded = false;
            }

            ImGui::Separator();
            
            if (std::filesystem::exists(path) && !std::filesystem::is_directory(path)) {
               std::string p = std::string(path);            
   
               std::string extension = p.substr(p.find_last_of(".") + 1);
   
               std::string type;
               if (extension == p) type = "No File Extension";
               else {
                  for (auto s : filetypes)
                  {
                     if (to_lower(extension) == s.first)
                     {
                        type = s.second;
                        break;
                     }
                  }

                  if (type == "")
                  {
                     type = extension + " File";
                  }
               }
               ImGui::Text("Type: %s", type.data());
               ImGui::Text("Size: %lu", std::filesystem::file_size(path));

               if (ImGui::CollapsingHeader("Contents"))
               {
                  std::ifstream file(p);
                  std::stringstream s;
                  s << file.rdbuf();
                  ImGui::Text("%s", s.str().data()); 
               }

            }


         }

         ImGui::EndPopup();
         WINDOW_BACKEND_RENDER;
         WINDOW_BACKEND_SWAP_BUFFERS(window);
      }

      return r;
   }
}
