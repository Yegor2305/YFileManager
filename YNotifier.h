#pragma once
#include "YObserver.h"
#include <vector>

class YNotifier
{
public:
	virtual ~YNotifier();
	void AddObserver(YObserver* observer);
	virtual void NotifyMouseEvent(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, const MOUSE_EVENT_RECORD& mouse_event) const;
	virtual void NotifyMouseEvent(const MOUSE_EVENT_RECORD& mouse_event) = 0;
	virtual void NotifyKeyEvent(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, const KEY_EVENT_RECORD& key_event) const;
	virtual void NotifyKeyEvent(const KEY_EVENT_RECORD& key_event) = 0;
	virtual void Run() = 0;
private:
	std::vector<YObserver*> Observers;
};

