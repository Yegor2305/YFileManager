#include "YChangeDriveModal.h"

YChangeDriveModal::YChangeDriveModal(unsigned short width, unsigned short height, unsigned short pos_x,
	unsigned short pos_y, CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO* screen_buffer_info, std::wstring* wstring_ptr,
	HANDLE* std_input_handle, INPUT_RECORD* input_record_buffer, int* buffer_size, DWORD* input_records_number, HANDLE* screen_buffer_handle,
	WORD background_attributes, WORD elements_attributes)
{
	this->Width = width;
	this->Height = height;
	this->Pos_X = pos_x;
	this->Pos_Y = pos_y;
	this->Screen_Buffer = screen_buffer;
	this->Screen_Buffer_Info = screen_buffer_info;
	this->Drive_Name_Value = wstring_ptr;
	this->Background_Attributes = background_attributes;
	this->Elements_Attributes = elements_attributes;
	this->Std_Input_Handle = std_input_handle;
	this->Screen_Buffer_Handle = screen_buffer_handle;
	this->Input_Record_Buffer = input_record_buffer;
	this->Buffer_Size = buffer_size;
	this->Input_Records_Number = input_records_number;

	this->Drives.push_back(new YButton(0, 0, L"C:/", 3));
	this->Drives.push_back(new YButton(0, 0, L"D:/", 3));
	this->Drives.push_back(new YButton(0, 0, L"F:/", 3));

	for (auto drive : this->Drives)
	{
		drive->SetWStringOnClick(this->Drive_Name_Value);
		this->AddObserver(drive);
	}
}

void YChangeDriveModal::Run()
{
	this->Can_Run = true;
	this->Draw();

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
				this->NotifyMouseEvent(this->Screen_Buffer, *this->Screen_Buffer_Info, this->Input_Record_Buffer[i].Event.MouseEvent);
				if (!WriteConsoleOutput(*this->Screen_Buffer_Handle, this->Screen_Buffer,
					this->Screen_Buffer_Info->dwSize, {0, 0}, &this->Screen_Buffer_Info->srWindow)) {
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

		if (!this->Drive_Name_Value->empty())
			this->Can_Run = false;

		//Sleep(3000);
		Sleep(30);
	}
}

void YChangeDriveModal::Draw()
{
	LineInfo line_info = { {L' '}, {L' '}, {L' '}, this->Background_Attributes };
	OutputPos pos(this->Pos_X, this->Pos_Y, this->Screen_Buffer_Info->dwSize.X, this->Width);

	for (int i = 0; i < this->Height; i++)
	{
		AsmFunctions::DrawLineHorizontal(this->Screen_Buffer, pos, line_info);
		pos.Y_Pos += 1;
	}

	this->DrawDrives();

	if (!WriteConsoleOutput(*this->Screen_Buffer_Handle, this->Screen_Buffer,
		this->Screen_Buffer_Info->dwSize, { 0, 0 }, &this->Screen_Buffer_Info->srWindow)) 
	{
		this->ExitWithError("WriteConsoleOutput failed");
	}
}

void YChangeDriveModal::DrawDrives() const
{
	OutputPos pos(this->Pos_X + 1, this->Pos_Y + 1, 0, 0);

	for (auto drive : this->Drives)
	{
		drive->ChangePosition(pos.X_Pos, pos.Y_Pos);
		drive->Draw(this->Screen_Buffer, *this->Screen_Buffer_Info);
		pos.Y_Pos += 1;
	}
}

void YChangeDriveModal::ExitWithError(LPCSTR error_message)
{
	printf("(%s) - (%d)\n", error_message, GetLastError());
	this->Can_Run = false;
	ExitProcess(0);
}
