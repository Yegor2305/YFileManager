#pragma once
#include "Main.h"
#include <string>

class YFile
{
public:
	YFile(unsigned short x_pos, unsigned short y_pos, const WIN32_FIND_DATA& file_data, unsigned short max_length,
		WORD attributes = 0xf0, WORD hover_attributes = 0x0f, WORD selected_attributes = 0x0f);
	void Draw(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);
	void ChangePosition(unsigned short new_x, unsigned short new_y);
	void ChangeLimit(unsigned short new_limit);
	void Mouse_Enter(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);
	void Mouse_Leave(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);

private:
	std::wstring Name;
	unsigned short Max_Length;
	LabelInfo Label_Info;
	WORD Default_Attributes, Hover_Attributes, Selected_Attributes;
};

