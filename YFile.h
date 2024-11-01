#pragma once
#include "Main.h"

class YFile
{
public:
	YFile(unsigned short x_pos, unsigned short y_pos, const char* name, WORD attributes = 0xf0, WORD hover_attributes = 0x0f);
	void Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);
	void Mouse_Enter(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);
	void Mouse_Leave(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);

private:
	const char* Name;
	LabelInfo Label_Info;
	WORD Default_Attributes, Hover_Attributes;
};

