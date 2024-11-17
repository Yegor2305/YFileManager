#pragma once
#include "BaseWidget.h"
#include "YNotifier.h"
#include <cctype>

class YInputBox : public BaseWidget, public YNotifier
{
public:
	YInputBox(
		unsigned short width, unsigned short pos_x, unsigned short pos_y, const std::wstring& initial_text,
		CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO* screen_buffer_info,
		HANDLE* std_input_handle, INPUT_RECORD* input_record_buffer, int* buffer_size,
		DWORD* input_records_number, HANDLE* screen_buffer_handle
		);
	std::wstring GetUserInput();
private:
	void Draw() override;
	void Run() override;
	void SetCursorPosition(unsigned short new_pos);
	void Move(bool left);
	void KeyEventHandler(KEY_EVENT_RECORD key_event);
	std::wstring GetTextWithSpaces();

	std::wstring Current_Text;
	unsigned short First_Char_To_Draw_Index;
	unsigned short Cursor_X_Position;
	unsigned short Text_Offset = 2;
};

