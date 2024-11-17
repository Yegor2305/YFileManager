#include "YInputBox.h"

YInputBox::YInputBox(unsigned short width, unsigned short pos_x, unsigned short pos_y, const std::wstring& initial_text,
	CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO* screen_buffer_info, HANDLE* std_input_handle,
	INPUT_RECORD* input_record_buffer, int* buffer_size, DWORD* input_records_number, HANDLE* screen_buffer_handle)
{
	this->Width = width;
	this->Height = 1;
	this->Pos_X = pos_x;
	this->Pos_Y = pos_y;
	this->Current_Text = initial_text;
	this->Screen_Buffer = screen_buffer;
	this->Screen_Buffer_Info = screen_buffer_info;
	this->Std_Input_Handle = std_input_handle;
	this->Screen_Buffer_Handle = screen_buffer_handle;
	this->Input_Record_Buffer = input_record_buffer;
	this->Buffer_Size = buffer_size;
	this->Input_Records_Number = input_records_number;

	this->First_Char_To_Draw_Index = initial_text.length() > width - this->Text_Offset ? initial_text.length() - width + this->Text_Offset : 0;

}

std::wstring YInputBox::GetUserInput()
{
	this->Run();

	SetConsoleCursorPosition(*this->Screen_Buffer_Handle, { 0, this->Screen_Buffer_Info->srWindow.Bottom });

	return this->Current_Text;
}

void YInputBox::Draw()
{

	// Drawing first char
	std::wstring text = this->GetTextWithSpaces();
	LabelInfo label_info(this->Pos_X, this->Pos_Y, this->Screen_Buffer_Info->dwSize.X, 0x8cf0);
	std::wstring first_char(1, text[this->First_Char_To_Draw_Index]);
	AsmFunctions::DrawLabel(this->Screen_Buffer, label_info, first_char.c_str());

	// Drawing medium chars
	label_info.X_Pos += 1;
	label_info.Attributes = 0x84f0;
	AsmFunctions::DrawLabel(this->Screen_Buffer, label_info, text.substr(this->First_Char_To_Draw_Index + 1, this->Width - this->Text_Offset).c_str());

	// Drawing last char
	label_info.X_Pos = this->Pos_X + this->Width - this->Text_Offset;
	label_info.Attributes = 0x94f0;
	std::wstring last_char(1, L' ');
	AsmFunctions::DrawLabel(this->Screen_Buffer, label_info, last_char.c_str());

	if (!WriteConsoleOutput(*this->Screen_Buffer_Handle, this->Screen_Buffer,
		this->Screen_Buffer_Info->dwSize, { 0, 0 }, &this->Screen_Buffer_Info->srWindow)) {
		this->ExitWithError("WriteConsoleOutput failed");
	}

}

void YInputBox::Run()
{
	this->Can_Run = true;
	this->Draw();
	this->SetCursorPosition(200);

	while (this->Can_Run) {

		if (!ReadConsoleInput(*this->Std_Input_Handle, this->Input_Record_Buffer, *this->Buffer_Size, this->Input_Records_Number)) {
			this->ExitWithError("ReadConsoleInput failed");
		}
		for (unsigned short i = 0; i < *this->Input_Records_Number; i++) {
			switch (this->Input_Record_Buffer[i].EventType)
			{
			case KEY_EVENT:
				this->KeyEventHandler(this->Input_Record_Buffer[i].Event.KeyEvent);
				break;
			case MOUSE_EVENT:
				if (this->Input_Record_Buffer[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					if (this->Input_Record_Buffer[i].Event.MouseEvent.dwMousePosition.X < this->Pos_X ||
						this->Input_Record_Buffer[i].Event.MouseEvent.dwMousePosition.X > this->Pos_X + this->Width &&
						this->Input_Record_Buffer[i].Event.MouseEvent.dwMousePosition.Y < this->Pos_Y ||
						this->Input_Record_Buffer[i].Event.MouseEvent.dwMousePosition.Y > this->Pos_Y + this->Height)
					{
						this->Can_Run = false;
					}else
					{
						this->SetCursorPosition(this->Input_Record_Buffer[i].Event.MouseEvent.dwMousePosition.X);
					}
				}
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
				break;
			case FOCUS_EVENT:
				break;
			case MENU_EVENT:
				break;
			default:
				break;
			}
		}


		Sleep(30);
	}
}

void YInputBox::SetCursorPosition(unsigned short new_pos)
{
	if (new_pos < this->Pos_X)
	{
		this->Cursor_X_Position = this->Pos_X;
	}else
	{
		unsigned short limit;
		if (this->Current_Text.length() > this->Width - this->Text_Offset)
			limit = this->Pos_X + this->Width - this->Text_Offset;
		else
			limit = this->Pos_X + this->Current_Text.length();

		this->Cursor_X_Position = min(new_pos, limit);
	}

	SetConsoleCursorPosition(*this->Screen_Buffer_Handle, { static_cast<short>(this->Cursor_X_Position), static_cast<short>(this->Pos_Y) });

}

void YInputBox::Move(bool left)
{
	unsigned short next = this->Cursor_X_Position + (left ? -1 : 1);
	this->SetCursorPosition(next);

	bool check_limit = left ? this->First_Char_To_Draw_Index > 0 :
		this->First_Char_To_Draw_Index < this->Current_Text.length() - this->Width + this->Text_Offset;

	if (this->Current_Text.length() > this->Width - this->Text_Offset && this->Cursor_X_Position != next && check_limit)
	{
		this->First_Char_To_Draw_Index += left ? -1 : 1;
		this->Draw();
	}
}

void YInputBox::KeyEventHandler(KEY_EVENT_RECORD key_event)
{
	if (!key_event.bKeyDown) return;

	switch (key_event.wVirtualKeyCode)
	{
	case VK_LEFT:
		this->Move(true);
		break;
	case VK_RIGHT:
		this->Move(false);
		break;
	case VK_BACK:
		if (this->Current_Text.empty()) return;if(this->Cursor_X_Position != this->Pos_X)
			this->Current_Text.erase(this->First_Char_To_Draw_Index + (this->Cursor_X_Position - 1 - this->Pos_X), 1);
		if (this->Current_Text.length() >= this->Width - this->Text_Offset)
		{
			this->First_Char_To_Draw_Index = this->Current_Text.length() - this->Width + this->Text_Offset;
			
		}else
		{
			this->First_Char_To_Draw_Index = 0;
			this->Move(true);
		}
		this->Draw();
		break;
	case VK_RETURN:
		this->Can_Run = false;
		return;
	default:
		break;
	}

	if (iswalpha(key_event.uChar.UnicodeChar) || iswdigit(key_event.uChar.UnicodeChar) ||
		iswpunct(key_event.uChar.UnicodeChar) || iswspace(key_event.uChar.UnicodeChar))
	{
		this->Current_Text.insert(this->First_Char_To_Draw_Index + (this->Cursor_X_Position - this->Pos_X), 1, key_event.uChar.UnicodeChar);
		this->Move(false);
		this->Draw();
	}
}

std::wstring YInputBox::GetTextWithSpaces()
{
	if (this->Current_Text.length() < this->Width)
	{
		std::wstring result = this->Current_Text;
		result.append(this->Width - this->Current_Text.length(), L' ');
		return result;
	}

	return this->Current_Text;
}
