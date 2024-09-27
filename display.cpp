

#include"display.h"

namespace fs = std::filesystem;
 void DisplayImage::GetImage(cv::Mat &image) {
      if(fs::exists(_ImagePath)){
      image = cv::imread(_ImagePath, cv::IMREAD_UNCHANGED);}
      if (image.empty()) {
          return;
      }
}

 void DisplayImage::ImageUpdate(std::string NewImagePath) {
     _ImagePath = NewImagePath;
 }

void DisplayImage::DynamicRangeAdjust(const cv::Mat& src, cv::Mat& dst, double low_percent = 0.0, double high_percent = 100.0) {
    int lowVal = 0, highVal = 65535;
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(src, &minVal, &maxVal, &minLoc, &maxLoc);

    int histSize = 65536; 
    float range[] = { 0, 65535 };
    const float* histRange = { range };
    cv::Mat hist;
    cv::calcHist(&src, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

    int totalPixels = src.rows * src.cols;
    int lowCount = (low_percent / 100.0) * totalPixels;
    int highCount = (high_percent / 100.0) * totalPixels;

    double cumulative = 0;
    for (int i = 0; i < histSize; i++) {
        cumulative += hist.at<float>(i);
        if (cumulative >= lowCount && lowVal == 0) {
            lowVal = i;
        }
        if (cumulative >= highCount) {
            highVal = i;
            break;
        }
    }

    if (lowVal > highVal) {
        std::swap(lowVal, highVal);
    }

    if (highVal - lowVal < 50) {
        lowVal = std::max(0, lowVal - 25);
        highVal = std::min(65535, highVal + 25);
    }

    src.convertTo(dst, CV_8U, 255.0 / (highVal - lowVal), -(lowVal - minVal) * 255.0 / (highVal - lowVal));
}

void DisplayImage::PreProcess(cv::Mat & Img16, cv::Mat& Img8) {
   
    if (Img16.empty()) {
        std::cout << "Error: Could not open or find the image!" << std::endl;
    }

    if (Img16.depth() != CV_16U) {
        std::cout << "Error: The image is not 16-bit!" << std::endl;
    }

    DisplayImage::DynamicRangeAdjust(Img16, Img8, 1.0, 99.0);
    cv::equalizeHist(Img8, Img8); //��ȷ��Ч���Ƿ���ǿ���о���ࡣ

}



GLuint DisplayImage::MatToTexture(const cv::Mat& mat) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // ������������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // ������������

    if (mat.channels() == 1) {


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, mat.cols, mat.rows, 0, GL_RED, GL_UNSIGNED_BYTE, mat.data);

        //���Ҷ�ͼ������ֵӳ�䵽������ɫͨ��
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
    }
    else {
        std::cerr << "ͼ���ǵ�ͨ���Ҷ�ͼ��" << std::endl;
        return 0;
    }
    return textureID;
}

void DisplayImage::PutTextOnImage(cv::Mat& Image) {

    // �����ı����ݺ�����
    std::string text = _ImagePath;
    int fontFace = cv::FONT_HERSHEY_COMPLEX_SMALL;
    double fontScale = 1.0;
    int thickness = 1;


    int baseline = 0;
    cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
    baseline += thickness;
    cv::Point textOrg((10), (textSize.height + 10));
    cv::Scalar TextColor = cv::Scalar(0, 0, 0);
    cv::putText(Image, text, textOrg, fontFace, fontScale, TextColor, thickness, cv::LINE_AA);
}


void DisplayImage::RenderImage() {
    cv::Mat  _Image16 , _Image8;
    GetImage (_Image16);
    if (_Image16.empty()) { return; }
    PreProcess(_Image16, _Image8);
    PutTextOnImage(_Image8);
    GLuint textureID  = MatToTexture(_Image8);
    ImGui::SetCursorPos(_Position);
    ImGui::PushID(_ID.c_str());
    ImGui::Image((void*)(intptr_t)textureID, ImVec2(_Image16.cols, _Image16.rows));
    ImGui::PopID();
}

