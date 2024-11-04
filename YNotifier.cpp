#include "YNotifier.h"

YNotifier::~YNotifier()
{
	for (YObserver* ptr : this->Observers)
	{
		delete ptr;
	}
	this->Observers.clear();
}

void YNotifier::AddObserver(YObserver* observer)
{
	this->Observers.push_back(observer);
}

void YNotifier::NotifyMouseEvent(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, const MOUSE_EVENT_RECORD& mouse_event) const
{
	for (YObserver* observer : this->Observers)
	{
		observer->MouseEventHandler(screen_buffer, screen_buffer_info, mouse_event);
	}
}

void YNotifier::NotifyKeyEvent(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, const KEY_EVENT_RECORD& key_event) const
{
	for (YObserver* observer : this->Observers)
	{
		observer->KeyEventHandler(screen_buffer, screen_buffer_info, key_event);
	}
}

