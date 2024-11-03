#pragma once
#include <Windows.h>

class YObserver
{
public:
	virtual ~YObserver() = default;
	virtual void MouseEventHandler(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event) = 0;
	virtual void KeyEventHandler(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, KEY_EVENT_RECORD key_event) = 0;
};

