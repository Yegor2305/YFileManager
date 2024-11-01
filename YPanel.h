#pragma once
#include "Main.h"
#include "BaseWidget.h"
#include "YFile.h"
#include "YApplication.h"

class YPanel : public BaseWidget
{
public:
	YPanel(unsigned short width, unsigned short height,
		BOOL border_left = 1, BOOL border_right = 1,
		BOOL border_top = 1, BOOL border_bottom = 1, bool double_border = true,
		WORD background_attributes = 0xf0, WORD border_attributes = 0xf0);
	void Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) override;
	void MouseEventHandler(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event);
private:
	BOOL Border_Left, Border_Right, Border_Top, Border_Bottom;
	bool Double_Border;
	WCHAR Corner_Top_Left, Corner_Top_Right, Corner_Bottom_Left, Corner_Bottom_Right, Horizontal_Line_Char, Vertical_Line_Char;
	WORD Background_Attributes, Border_Attributes;
	std::vector<YFile> Files_List{};
};

