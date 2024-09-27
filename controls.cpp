
#include "controls.h"


void SliderInt::RenderSlider() {
	ImGui::SetCursorPos(_Position);
	ImGui::AlignTextToFramePadding();
	ImGui::Dummy(ImVec2(_spacing, 0));
	ImGui::SameLine();
	ImGui::Text("%s",_text.c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(_sliderwidth);
	ImGui::PushID(_ID.c_str());
	if (ImGui::SliderInt("", &_intChannel, 1, _channelNum)) {
	}
	ImGui::PopItemWidth();
	ImGui::PopID();
}


int SliderInt::GetChannel() const{
	return  _intChannel;
}


void DisplayText::RenderTextBox() {

	ImGui::SetCursorPos(_PosTextBox);
	ImGui::PushID(_ID.c_str());
	ImGui::BeginChild("Text Area", _SizeTextBox);

	for (std::string infor : _Information) {
    ImGui::TextWrapped("%s",infor.c_str());
	}

	ImGui::EndChild();
	ImGui::PopID();

 }

ImVec2 DisplayText::GetPosTextBox()const {
	return _PosTextBox;
}

std::vector<std::string> DisplayText::GetInformation()const {
	return _Information;
}

void  DisplayText::UpdateTexBox(std::string NewInformation) {
	_Information.emplace_back(NewInformation);
}