#pragma once
#include "YObserver.h"
#include "NotImplementedException.h"
#include <vector>

class YNotifier
{
public:
	virtual ~YNotifier();
	void AddObserver(YObserver* observer);
	virtual void NotifyMouseEvent(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, const MOUSE_EVENT_RECORD& mouse_event) const;
	virtual void NotifyMouseEvent(const MOUSE_EVENT_RECORD& mouse_event);
	virtual void NotifyKeyEvent(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, const KEY_EVENT_RECORD& key_event) const;
	virtual void NotifyKeyEvent(const KEY_EVENT_RECORD& key_event);
	virtual void Run() = 0;
protected:
	std::vector<YObserver*> Observers;
};

