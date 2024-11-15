#include "YConfirmModal.h"

YConfirmModal::YConfirmModal(unsigned short parent_x, unsigned short parent_y, unsigned short parent_width, unsigned short parent_height,
	std::wstring* text, CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO* screen_buffer_info, HANDLE* std_input_handle,
	INPUT_RECORD* input_record_buffer, int* buffer_size, DWORD* input_records_number, HANDLE* screen_buffer_handle,
	WORD background_attributes, WORD elements_attributes)
{
	this->Width = text->length() + 2;
	this->Height = 6;
	this->Pos_X = parent_x + parent_width / 2 - this->Width / 2;
	this->Pos_Y = parent_y + parent_height / 2 - this->Height / 2;
	this->Screen_Buffer = screen_buffer;
	this->Screen_Buffer_Info = screen_buffer_info;
	this->Background_Attributes = background_attributes;
	this->Elements_Attributes = elements_attributes;
	this->Std_Input_Handle = std_input_handle;
	this->Screen_Buffer_Handle = screen_buffer_handle;
	this->Input_Record_Buffer = input_record_buffer;
	this->Buffer_Size = buffer_size;
	this->Input_Records_Number = input_records_number;
	this->Text = *text;

	this->Ok_Button = new YButton(this->Pos_X + this->Width / 4 - 1, this->Pos_Y + this->Height - 2, L"Ok", 2, elements_attributes);

	this->Cancel_Button = new YButton(this->Pos_X + this->Width / 2 + this->Width / 4 - 3, this->Pos_Y + this->Height - 2, L"Cancel", 6, elements_attributes);

	this->Ok_Button->SetBoolOnClick(&this->Ok);
	this->Cancel_Button->SetBoolOnClick(&this->Cancel);
}

bool YConfirmModal::Confirm()
{
	this->Run();
	if (this->Ok)
		return true;

	return false;
}

void YConfirmModal::Run()
{
	this->Draw();
	this->Can_Run = true;

	while (this->Can_Run) {

		if (!ReadConsoleInput(*this->Std_Input_Handle, this->Input_Record_Buffer, *this->Buffer_Size, this->Input_Records_Number)) {
			this->ExitWithError("ReadConsoleInput failed");
		}
		for (unsigned short i = 0; i < *this->Input_Records_Number; i++) {
			switch (this->Input_Record_Buffer[i].EventType)
			{
			case KEY_EVENT:
				break;
			case MOUSE_EVENT:
				if (this->Input_Record_Buffer[i].Event.MouseEvent.dwMousePosition.X < this->Pos_X ||
					this->Input_Record_Buffer[i].Event.MouseEvent.dwMousePosition.X > this->Pos_X + this->Width &&
					this->Input_Record_Buffer[i].Event.MouseEvent.dwMousePosition.Y < this->Pos_Y ||
					this->Input_Record_Buffer[i].Event.MouseEvent.dwMousePosition.Y > this->Pos_Y + this->Height)
				{
					if (this->Input_Record_Buffer[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
						this->Can_Run = false;
					
				}
				else
				{
					this->NotifyMouseEvent(this->Input_Record_Buffer[i].Event.MouseEvent);
				}
				if (!WriteConsoleOutput(*this->Screen_Buffer_Handle, this->Screen_Buffer,
					this->Screen_Buffer_Info->dwSize, { 0, 0 }, &this->Screen_Buffer_Info->srWindow)) {
					this->ExitWithError("WriteConsoleOutput failed");
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

		if (this->Ok || this->Cancel)
			this->Can_Run = false;

		Sleep(30);
	}
}

void YConfirmModal::Draw()
{
	LineInfo line_info = { {L' '}, {L' '}, {L' '}, this->Background_Attributes };
	OutputPos pos(this->Pos_X, this->Pos_Y, this->Screen_Buffer_Info->dwSize.X, this->Width);

	for (int i = 0; i < this->Height; i++)
	{
		AsmFunctions::DrawLineHorizontal(this->Screen_Buffer, pos, line_info);
		pos.Y_Pos += 1;
	}

	LabelInfo label_info(this->Pos_X + 1, this->Pos_Y + 2, this->Screen_Buffer_Info->dwSize.X, this->Elements_Attributes);

	AsmFunctions::DrawLabel(this->Screen_Buffer, label_info, this->Text.c_str());

	this->Ok_Button->Draw(this->Screen_Buffer, *this->Screen_Buffer_Info);
	this->Cancel_Button->Draw(this->Screen_Buffer, *this->Screen_Buffer_Info);

	if (!WriteConsoleOutput(*this->Screen_Buffer_Handle, this->Screen_Buffer,
		this->Screen_Buffer_Info->dwSize, { 0, 0 }, &this->Screen_Buffer_Info->srWindow)) {
		this->ExitWithError("WriteConsoleOutput failed");
	}

}

void YConfirmModal::NotifyMouseEvent(const MOUSE_EVENT_RECORD& mouse_event)
{
	this->Ok_Button->MouseEventHandler(this->Screen_Buffer, *this->Screen_Buffer_Info, mouse_event);
	this->Cancel_Button->MouseEventHandler(this->Screen_Buffer, *this->Screen_Buffer_Info, mouse_event);
}
