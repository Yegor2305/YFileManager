#pragma once
#include "YObserver.h"
#include <vector>

class YNotifier
{
public:
	~YNotifier();
	void AddObserver(YObserver* observer);
	void NotifyMouseEvent(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, const MOUSE_EVENT_RECORD& mouse_event) const;
	void NotifyKeyEvent(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, const KEY_EVENT_RECORD& key_event) const;

private:
	std::vector<YObserver*> Observers;
};

