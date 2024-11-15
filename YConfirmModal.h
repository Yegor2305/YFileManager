#pragma once
#include "BaseWidget.h"
#include "YButton.h"
#include "YNotifier.h"

class YConfirmModal : public BaseWidget, public YNotifier
{
public:
	YConfirmModal(unsigned short parent_x, unsigned short parent_y, unsigned short parent_width, unsigned short parent_height,
		std::wstring* text, CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO* screen_buffer_info,
		HANDLE* std_input_handle, INPUT_RECORD* input_record_buffer, int* buffer_size, DWORD* input_records_number,
		HANDLE* screen_buffer_handle, WORD background_attributes = 0xf0, WORD elements_attributes = 0xf0);
	bool Confirm();
private:
	void Run() override;
	void Draw() override;
	void NotifyMouseEvent(const MOUSE_EVENT_RECORD& mouse_event) override;
	std::wstring Text;
	bool Ok = false, Cancel = false;
	YButton* Ok_Button;
	YButton* Cancel_Button;
};

