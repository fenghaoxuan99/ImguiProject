#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "imgui/imgui.h"
#include <GLFW/glfw3.h>
#include <opencv4/opencv2/opencv.hpp>

class DisplayImage {
public:
    DisplayImage(std::string ID, ImVec2 Position,std::string ImagePath) :_ID(ID), _Position(Position), _ImagePath(ImagePath) {}
    void GetImage(cv::Mat& image);
    void ImageUpdate(std::string);
    void DynamicRangeAdjust(const cv::Mat& src, cv::Mat& dst, double low_percent, double high_percent);
    void PreProcess(cv::Mat& img16, cv::Mat& img8);
    void PutTextOnImage(cv::Mat& Image);
    GLuint MatToTexture(const cv::Mat& mat);
    
    void RenderImage();

private:
     const std::string _ID;
     const ImVec2 _Position;
     std::string _ImagePath;
     std::vector<std::string> _PathVector;
};