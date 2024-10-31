#pragma once
#include "Main.h"
#include "BaseWidget.h"
#include "YApplication.h"

class Panel : public BaseWidget
{
public:
	Panel(unsigned short width, unsigned short height);
	void Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) override;
};

