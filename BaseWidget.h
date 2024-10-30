#pragma once
#include <stdexcept>
#include "Main.h"

class NotImplementedException : public std::logic_error {
public:
	NotImplementedException() : std::logic_error("Method is not implemented") {}
};

class BaseWidget
{
public:
	virtual void Draw();
	CHAR_INFO* Screen_Buffer;
	CONSOLE_SCREEN_BUFFER_INFO Screen_Buffer_Info;
	unsigned short Pos_X = 0, Pos_Y = 0;
	unsigned short Height, Width;
	WORD GraphicAttributes = 0xf0;
};

