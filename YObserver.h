#pragma once
#include <Windows.h>
#include "NotImplementedException.h"

class YObserver
{
public:
	virtual ~YObserver() = default;
	virtual void MouseEventHandler(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event);
	virtual void KeyEventHandler(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, KEY_EVENT_RECORD key_event);
};

