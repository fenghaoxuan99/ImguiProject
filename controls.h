#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "imgui/imgui.h"
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>


class SliderInt {
public:
	SliderInt(std::string ID, ImVec2 Position ,std::string text= "选择通道：", int intChannel=1,int channelNum =20, int spacing=20,
		int sliderwidth=400) :_ID(ID),_Position(Position), _text(text), _intChannel(intChannel), _channelNum(channelNum), _spacing(spacing),
		_sliderwidth(sliderwidth){};
	
    void RenderSlider( );
	int  GetChannel() const;

private:
	ImVec2 _Position;
	std::string _ID;
	std::string _text;
	int _intChannel;
	int _channelNum;
	int _spacing;  
	int _sliderwidth;
};




class DisplayText {
public:
	DisplayText(std::string ID,ImVec2 PosTextBox, ImVec2 SizeTextBox) :_ID(ID),_PosTextBox(PosTextBox), _SizeTextBox(SizeTextBox){};
	void UpdateTexBox(std::string);
	void RenderTextBox();
	ImVec2 GetPosTextBox()const;
	std::vector<std::string> GetInformation()const;

private:
	std::string _ID;
	std::vector<std::string> _Information;
	ImVec2 _PosTextBox;
	ImVec2 _SizeTextBox;
};