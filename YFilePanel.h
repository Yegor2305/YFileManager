#pragma once

#include "BaseWidget.h"
#include "YFile.h"
#include <vector>

class YFilePanel : public BaseWidget
{
public:
	YFilePanel(unsigned short width, unsigned short height, LPCWSTR path,
		unsigned short pos_x = 0, unsigned short pos_y = 0,
		BOOL border_left = 1, BOOL border_right = 1,
		BOOL border_top = 1, BOOL border_bottom = 1, bool double_border = true,
		WORD background_attributes = 0xf0, WORD border_attributes = 0xf0);
	void Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) override;
	void MouseEventHandler(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event);
private:
	void FillFiles(LPCWSTR path);
	BOOL Border_Left, Border_Right, Border_Top, Border_Bottom;
	bool Double_Border;
	WCHAR Corner_Top_Left, Corner_Top_Right, Corner_Bottom_Left, Corner_Bottom_Right,
	Horizontal_Line_Char, Vertical_Line_Char, Separator_Left_Char, Separator_Right_Char, Separator_Char,
	Vertical_Separator_First_Char, Vertical_Separator_Last_Char;
	WORD Background_Attributes, Border_Attributes;
	std::vector<YFile*> Files_List{};
	int Hovered_File_Index = -1;
	unsigned short Content_Offset_Top = 2, Content_Offset_Bottom = 3, Max_Files_In_Column;
	std::wstring Title;
};

