#pragma once
#include "Main.h"
#include <string>

class BaseButton
{
public:
	BaseButton();
	virtual ~BaseButton();
	virtual void Draw(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);
	void ChangePosition(unsigned short new_x, unsigned short new_y);
	void ChangeLimit(unsigned short new_limit);
	virtual void Mouse_Enter(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);
	virtual void Mouse_Leave(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);

protected:
	std::wstring Name;
	unsigned short Max_Length;
	LabelInfo Label_Info = {0, 0, 0, 0x00};
	WORD Default_Attributes, Hover_Attributes, Selected_Attributes;
};

