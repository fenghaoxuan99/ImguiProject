#pragma once




int  GetAllDriveString(std::vector<std::string>& rootDirectories);

void ShowFileBrowser(const std::string& rootPath, std::string& selectedPath, const std::string& filterExt);

//void GetAllImagePath(std::string ImagePath, std::vector<std::string>& AllImagePathVector);
void FileBrowserWindow(bool* isOpen, std::string& selectedFile, std::vector<std::string> pathHistory, std::vector<std::string>& AllImagePathVector);