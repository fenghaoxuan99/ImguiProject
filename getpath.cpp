#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cstdlib>
#include "imgui/imgui.h"

namespace fs = std::filesystem;

void GetAllDriveString(std::vector<std::string>& rootDirectories) {
  const char* rootpath = getenv("HOME");
  rootDirectories.emplace_back(rootpath);
}



void ShowFileBrowser(const std::string& rootPath, std::string& selectedPath, const std::string& filterExt) {
    for (const auto& entry : fs::directory_iterator(rootPath)) {
        std::string fileName = entry.path().filename().string();
       
        if (entry.is_directory()) {
            if (ImGui::TreeNode(fileName.c_str())) {
                ShowFileBrowser(entry.path().string(), selectedPath, filterExt);
                ImGui::TreePop();
            }
        }
        else {
            if (!filterExt.empty() && entry.path().extension() != filterExt) {
                continue;
            }
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); 
            if (ImGui::Selectable(fileName.c_str())) {
                selectedPath = entry.path().string();
               
       
            }
            ImGui::PopStyleColor();
        }
    }
}

void GetAllImagePath(std::string ImagePath, std::vector<std::string>& AllImagePathVector) {
    for (const auto& _entry : fs::directory_iterator(ImagePath)) {
        if (_entry.path().extension()==".png") {
            AllImagePathVector.emplace_back(_entry.path().string());
        }
    }
}


int extractNumber(const std::string& filename) {
    size_t pos = filename.find_last_of('_');
    if (pos == std::string::npos) return -1; 
    std::string numberStr = filename.substr(pos + 1,2);
    return std::stoi(numberStr);
}


void FileBrowserWindow(bool* isOpen, std::string& selectedFile, std::vector<std::string> RootPathVector, std::vector<std::string>& AllImagePathVector) {
    
    if (!(*isOpen)) {
        return;  
    }
    static bool tempbool = true;
    if(tempbool){
    ImGui::SetNextWindowSize(ImVec2(500, 400));
    tempbool = false;
    }

    ImGui::SetNextWindowFocus();
    ImGui::Begin("File Browser", isOpen, ImGuiWindowFlags_NoCollapse);
    
    static std::string rootPath = fs::current_path().parent_path().string();
    static std::string selectedPath;
   

 
    const std::vector<std::string> ExtensionVector = { ".png", ".jpg", ".tif", ".ppm", ".pgm"};
    static std::string Extension = ".png";
   
    ImGui::AlignTextToFramePadding();
    ImGui::Text("根目录:");
    ImGui::SameLine();
    for (size_t i = 0; i < RootPathVector.size(); i++) {
        if (i > 0) ImGui::SameLine();
        if (ImGui::Button(RootPathVector[i].c_str())) {
            rootPath = RootPathVector[i];
        }
    }

    ImGui::Separator();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("文件类型:");
    ImGui::SameLine();
    for (size_t i = 0; i < ExtensionVector.size(); i++) {
        if (i > 0) ImGui::SameLine();
        if (ImGui::Button(ExtensionVector[i].c_str())) {
            Extension = ExtensionVector[i];
        }
    }


    ShowFileBrowser(rootPath, selectedPath, Extension);
    fs::path temppath1 (selectedPath);
    std::string temppath= temppath1.parent_path().string();

    if (!selectedPath.empty()) {
        ImGui::Separator();
        ImGui::Text("Selected File: %s", selectedPath.c_str());

        if (ImGui::Button("Confirm")) {
            selectedFile = selectedPath;
            AllImagePathVector.clear();
            for (const auto& _entry : fs::directory_iterator(temppath)) {
                if (_entry.path().extension() == ".png") {
                    AllImagePathVector.emplace_back(_entry.path().string());
                }
            }
            std::sort(AllImagePathVector.begin(), AllImagePathVector.end(), [](const std::string& a, const std::string& b) {
                return extractNumber(a) < extractNumber(b);
                });
            *isOpen = false;  
        }
    }

    ImGui::End();
}

