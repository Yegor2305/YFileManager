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
	void SetInputData(HANDLE* std_input_handle, INPUT_RECORD* input_record_buffer, int* buffer_size, DWORD* input_records_number, HANDLE* screen_buffer_handle);
	virtual void Run();
protected:
	std::vector<YObserver*> Observers;
	HANDLE* Std_Input_Handle = nullptr;
	HANDLE* Screen_Buffer_Handle = nullptr;
	INPUT_RECORD* Input_Record_Buffer = nullptr;
	int* Buffer_Size = nullptr;
	DWORD* Input_Records_Number = nullptr;
};

