#pragma once

#include "BaseWidget.h"
#include "YFile.h"
#include "YObserver.h"
#include <vector>

class YFilePanel : public BaseWidget, public YObserver
{
public:
	YFilePanel(unsigned short width, unsigned short height, LPCWSTR path,
		unsigned short pos_x = 0, unsigned short pos_y = 0,
		BOOL border_left = 1, BOOL border_right = 1,
		BOOL border_top = 1, BOOL border_bottom = 1, bool double_border = true,
		WORD background_attributes = 0xf0, WORD border_attributes = 0xf0, WORD elements_attributes = 0xf0);
	~YFilePanel() override;
	void Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) override;
	void MouseEventHandler(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event) override;
	void KeyEventHandler(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, KEY_EVENT_RECORD key_event) override;
private:
	void FillFiles(LPCWSTR path);
	void DrawFiles(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const;
	void DrawTitle(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const;
	void ClearFiles();
	void ClearColumns(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const;
	BOOL Border_Left, Border_Right, Border_Top, Border_Bottom;
	bool Double_Border;
	bool Scrollable = false;
	unsigned short First_File_Index_To_Draw = 0, Scroll_Offset = 2;
	WCHAR Corner_Top_Left, Corner_Top_Right, Corner_Bottom_Left, Corner_Bottom_Right,
	Horizontal_Line_Char, Vertical_Line_Char, Separator_Left_Char, Separator_Right_Char, Separator_Char,
	Vertical_Separator_First_Char, Vertical_Separator_Last_Char;
	WORD Background_Attributes, Border_Attributes;
	std::vector<YFile*> Files_List{};
	int Hovered_File_Index = -1;
	unsigned short Content_Offset_Top = 2, Content_Offset_Bottom = 3, Max_Files_In_Column;
	std::wstring Path;
};

