#pragma once
#include "BaseWidget.h"
#include "YNotifier.h"
#include "YButton.h"

class YChangeDriveModal : public BaseWidget, public YNotifier
{
public:
	YChangeDriveModal(unsigned short width, unsigned short height,
		unsigned short pos_x, unsigned short pos_y, CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO* screen_buffer_info,
		std::wstring* wstring_ptr, HANDLE* std_input_handle, INPUT_RECORD* input_record_buffer, int* buffer_size,
		DWORD* input_records_number, HANDLE* screen_buffer_handle,
		WORD background_attributes = 0xf0, WORD elements_attributes = 0xf0);
	void Run() override;
private:
	void Draw() override;
	void DrawDrives() const;
	std::wstring* Drive_Name_Value = nullptr;
	std::vector<YButton*> Drives;
};

