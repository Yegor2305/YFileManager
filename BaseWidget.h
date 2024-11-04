#pragma once
#include "NotImplementedException.h"
#include "Main.h"


class BaseWidget
{
public:
	virtual ~BaseWidget() = default;
	virtual void Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);
	unsigned short Pos_X = 0, Pos_Y = 0;
	unsigned short Height, Width;
	WORD Elements_Attributes = 0xf0;
};

