
#define GLEW_STATIC 
#define STB_IMAGE_IMPLEMENTATION

#pragma execution_character_set("utf-8") //解决中文显示编译报错问题
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/stb_image.h"
#include "getpath.h"
#include "display.h"
#include <opencv2/opencv.hpp>

#include "display.h"
#include "controls.h"
#include "layout.h"
#include "runpython.h"

GLFWwindow* Windows; 

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	Windows = glfwCreateWindow(1306,800, "多光谱光场信息编解码", NULL, NULL);
	glfwSetWindowAttrib(Windows, GLFW_RESIZABLE, GLFW_FALSE);
	glfwMakeContextCurrent(Windows);
	glfwSwapInterval(0);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext(NULL); 
	


	RunPython python("../DemoModule");     //模型路径
	python.LoadModule("DemoModule");    //模型名字 无后缀
	python.Function("add",3,5);         //函数名  参数
	py::object result = python.GetResults();
	std::cout << result.cast<int>() << std::endl;


	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("../Fonts/STZHONGS.TTF", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding   = 6;
	style.FrameBorderSize = 1;
	style.WindowPadding   = ImVec2(9, 10);
	style.FramePadding    = ImVec2(9, 10);

	ImGui_ImplGlfw_InitForOpenGL(Windows, true);
	ImGui_ImplOpenGL3_Init("#version 330"); 
    ImGui::StyleColorsLight(); 
   

    static bool ShowOriginFileBrowser       = false;
	static bool ShowDecompressedFileBrowser = false;
    static std::string SelectedOriginFilePath;
	static std::string SelectedDecompressedFilePath;

	static std::vector<std::string> pathHistory;
	GetAllDriveString(pathHistory);
	
	const int	 ImageWidth        = 640;
	const int	 ImageHeight       = 512;
	const int	 ImageStartPosX    = 9;
	const int	 ImageStartPosY    = 10;
	const int	 ImageSpacing      = 8; 
	const float  ImageBoxThickness = 2.5;
	const ImVec2 ImageOriginTopLeft(ImageStartPosX, ImageStartPosY);      
	const ImVec2 ImageOriginBottomRight(ImageStartPosX+ ImageWidth, ImageStartPosY+ImageHeight);
	const ImVec2 ImageDecompressedTopLeft(ImageStartPosX + ImageWidth+ ImageSpacing, ImageStartPosY);
	const ImVec2 ImageDecompressedBottomRight(ImageStartPosX + 2*ImageWidth + ImageSpacing, ImageStartPosY + ImageHeight);
	
	std::string  DefaultOriginalImagePath = "../DemoImage/Demo_Balloons/balloons_ms_01.png";
	std::string  DefaultDecompressedImagePath = "../DemoImage/Demo_Balloons/balloons_ms_01.png";
	DisplayImage OriginalImage("Origin_Image", ImageOriginTopLeft, DefaultOriginalImagePath);
	DisplayImage DecompressedImage("Decompressed_Image", ImageDecompressedTopLeft, DefaultDecompressedImagePath);

	const ImVec2 OriginSliderIntPos(9, 535);
	const ImVec2 DecompressedSliderIntPos(657, 535);
	SliderInt OriginalChannels("Origin_SliderInt", OriginSliderIntPos);
	SliderInt DecompressedChannels("Decompressed_SliderInt", DecompressedSliderIntPos);

	const ImVec2 ButtonSize = ImVec2(150, 40);

	const int    TextStartPosX   = 9;    
	const int    TextStartPosY   = 630;
	const int    TextBoxWidth    = 640;
	const int    TextBoxHeiht    = 150;
	const int    TextBoxSpacing  = 8;    
	const int    TextBoxPaddingX = 10;
	const int	 TextBoxPaddingY = 10;
	const float	 TextBoxThickness = 2.5;
	const ImVec2 TextBoxChildSize(TextBoxWidth - 2*TextBoxPaddingX, TextBoxHeiht- 1.5*TextBoxPaddingY);
	const ImVec2 TextBoxChildOriginPos(TextStartPosX+TextBoxPaddingX, TextStartPosY + TextBoxPaddingY);
	const ImVec2 TextBoxChildDecompressedPos(TextStartPosX + TextBoxSpacing + TextBoxWidth + TextBoxPaddingX, TextStartPosY + TextBoxPaddingY);
	const ImVec2 TextBoxOriginTopLeft(TextStartPosX, TextStartPosY);
	const ImVec2 TextBoxOriginBottomRight(TextStartPosX+ TextBoxWidth, TextStartPosY+ TextBoxHeiht);
	const ImVec2 TextBoxDecompressedTopLeft(TextStartPosX+ TextBoxSpacing+ TextBoxWidth, TextStartPosY);
	const ImVec2 TextBoxDecompressedBottomRight(TextStartPosX + TextBoxSpacing + 2*TextBoxWidth, TextStartPosY + TextBoxHeiht);

	DisplayText TextBoxOrigin("Origin_TextBox", TextBoxChildOriginPos, TextBoxChildSize);
	DisplayText TextBoxDecompressed("Decompressed_TextBox", TextBoxChildDecompressedPos, TextBoxChildSize);
	for (int i = 0; i < 30; i++) {
		TextBoxOrigin.UpdateTexBox("This is a test ! Attention the distance 中文显示测试!");
	}

	static std::vector<std::string> AllOriginImagePathVector;
	static std::vector<std::string> AllDecompressedImagePathVector;
	int Display_Width, Display_Height; //窗口尺寸

	while (!glfwWindowShouldClose(Windows))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		glfwGetFramebufferSize(Windows, &Display_Width, &Display_Height);
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(Display_Width, Display_Height));
		ImGui::Begin("Demo",NULL,ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_NoResize);
		
		if(AllOriginImagePathVector.size()){
		OriginalImage.ImageUpdate(AllOriginImagePathVector[OriginalChannels.GetChannel()-1]);}

		if (AllDecompressedImagePathVector.size()) {
			DecompressedImage.ImageUpdate(AllDecompressedImagePathVector[DecompressedChannels.GetChannel() - 1]);
		}
		
		OriginalImage.RenderImage(); 
		DecompressedImage.RenderImage();

		ImGui::GetWindowDrawList()->AddRect(ImageOriginTopLeft, ImageOriginBottomRight, IM_COL32(0, 0, 0, 255), 0.0f, 15, ImageBoxThickness);
		ImGui::GetWindowDrawList()->AddRect(ImageDecompressedTopLeft, ImageDecompressedBottomRight, IM_COL32(0, 0, 0, 255), 0.0f, 15, ImageBoxThickness);

		ImGui::GetWindowDrawList()->AddRect(TextBoxOriginTopLeft, TextBoxOriginBottomRight, IM_COL32(0, 0, 0, 255), 0.0f, 15, TextBoxThickness);
		ImGui::GetWindowDrawList()->AddRect(TextBoxDecompressedTopLeft, TextBoxDecompressedBottomRight, IM_COL32(0, 0, 0, 255), 0.0f, 15, TextBoxThickness);
		
		OriginalChannels.RenderSlider();
		DecompressedChannels.RenderSlider();
		

		ImGui::Dummy(ImVec2(10, 0)); 
		ImGui::SameLine();
	 
		if (ImGui::Button("打开文件", ButtonSize)) {
			ShowOriginFileBrowser = true;  
		}
		if (ShowOriginFileBrowser) {
			FileBrowserWindow(&ShowOriginFileBrowser, SelectedOriginFilePath, pathHistory, AllOriginImagePathVector);
		}

		ParallelLayout(20);
		if (ImGui::Button("开始压缩", ButtonSize)) {
			
		}
		
		ParallelLayout(20);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
		if (ImGui::Button("信道编码", ButtonSize)) {
		}
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("该功能有待完善 ！"); }
		ImGui::PopStyleColor();
		


		ParallelLayout(20);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
		if (ImGui::Button("信道噪声", ButtonSize)) {
		}
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("该功能有待完善 ！"); }
		ImGui::PopStyleColor();
		
		ParallelLayout(20);
		if (ImGui::Button("打开码流文件", ButtonSize)) {
			ShowDecompressedFileBrowser = true;
		}
		if (ShowDecompressedFileBrowser) {
			FileBrowserWindow(&ShowDecompressedFileBrowser, SelectedDecompressedFilePath, pathHistory, AllDecompressedImagePathVector);
		}

		
		ParallelLayout(20);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
		if (ImGui::Button("信道解码", ButtonSize)) {
			
		}
		if (ImGui::IsItemHovered()) {ImGui::SetTooltip("该功能有待完善 ！");}
		ImGui::PopStyleColor();



		ParallelLayout(20);
		if (ImGui::Button("解压缩", ButtonSize)) {
			
		}

		
		TextBoxOrigin.RenderTextBox();
		TextBoxDecompressed.RenderTextBox();



		ImGui::End();
		ImGui::Render();
		glViewport(0, 0, Display_Width, Display_Height);
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
		glfwSwapBuffers(Windows);
		glfwPollEvents(); 
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(Windows);
	glfwTerminate();
	return 0;
}