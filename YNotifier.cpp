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

void YNotifier::NotifyMouseEvent(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, const MOUSE_EVENT_RECORD& mouse_event) const
{
	for (YObserver* observer : this->Observers)
	{
		observer->MouseEventHandler(screen_buffer, screen_buffer_info, mouse_event);
	}
}

void YNotifier::NotifyMouseEvent(const MOUSE_EVENT_RECORD& mouse_event)
{
	throw NotImplementedException();
}

void YNotifier::NotifyKeyEvent(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, const KEY_EVENT_RECORD& key_event) const
{
	for (YObserver* observer : this->Observers)
	{
		observer->KeyEventHandler(screen_buffer, screen_buffer_info, key_event);
	}
}

void YNotifier::NotifyKeyEvent(const KEY_EVENT_RECORD& key_event)
{
	throw NotImplementedException();
}

void YNotifier::SetInputData(HANDLE* std_input_handle, INPUT_RECORD* input_record_buffer, int* buffer_size, DWORD* input_records_number, HANDLE* screen_buffer_handle)
{
	this->Std_Input_Handle = std_input_handle;
	this->Screen_Buffer_Handle = screen_buffer_handle;
	this->Input_Record_Buffer = input_record_buffer;
	this->Buffer_Size = buffer_size;
	this->Input_Records_Number = input_records_number;
}

void YNotifier::Run()
{
	throw new NotImplementedException();
}

void YNotifier::ExitWithError(LPCSTR error_message)
{
	printf("(%s) - (%lu)\n", error_message, GetLastError());
	this->Can_Run = false;
	ExitProcess(0);
}

