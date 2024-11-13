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
	void Draw();
	void DrawDrives() const;
	void ExitWithError(LPCSTR error_message);
	/*HANDLE* Std_Input_Handle = nullptr;
	HANDLE* Screen_Buffer_Handle = nullptr;
	INPUT_RECORD* Input_Record_Buffer = nullptr;
	int* Buffer_Size = nullptr;
	DWORD* Input_Records_Number = nullptr;*/
	bool Can_Run = false;
	CHAR_INFO* Screen_Buffer;
	CONSOLE_SCREEN_BUFFER_INFO* Screen_Buffer_Info;
	WORD Background_Attributes;
	std::wstring* Drive_Name_Value = nullptr;
	std::vector<YButton*> Drives;
};

