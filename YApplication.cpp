#include "YApplication.h"

YApplication::YApplication()
{
	// Disable user resizing
	HWND hwnd = GetConsoleWindow();
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));

	this->Std_Output_Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	this->Screen_Buffer_Handle = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	this->Std_Input_Handle = GetStdHandle(STD_INPUT_HANDLE);
	if (this->Std_Input_Handle == INVALID_HANDLE_VALUE) {
		this->ExitWithError("GetStdHandle (input) failed");
	}
	
	if (this->Std_Output_Handle == INVALID_HANDLE_VALUE || this->Screen_Buffer_Handle == INVALID_HANDLE_VALUE) {
		this->ExitWithError("CreateConsoleScreenBuffer failed");
	}

	if (!GetConsoleMode(this->Std_Input_Handle, &this->Old_Console_Mode)) {
		this->ExitWithError("GetConsoleMode failed");
	}

	if (!SetConsoleMode(this->Std_Input_Handle, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS)) {
		this->ExitWithError("SetConsoleMode failed");
	}

	if (!SetConsoleActiveScreenBuffer(this->Screen_Buffer_Handle)) {
		this->ExitWithError("SetConsoleActiveScreenBuffer failed");
	}

	if (!GetConsoleScreenBufferInfo(this->Screen_Buffer_Handle, &this->Screen_Buffer_Info)) {
		this->ExitWithError("GetConsoleScreenBufferInfo failed");
	}

	this->Width = this->Screen_Buffer_Info.dwSize.X;
	this->Height = this->Screen_Buffer_Info.dwSize.Y;

	SetConsoleCursorPosition(this->Screen_Buffer_Handle, { 0, this->Screen_Buffer_Info.srWindow.Bottom });

	COORD max_size = GetLargestConsoleWindowSize(this->Screen_Buffer_Handle);
	this->Buffer_Size = max_size.X * max_size.Y;
	this->Max_Width = max_size.X;
	this->Max_Height = max_size.Y;

	this->Screen_Buffer = new CHAR_INFO[this->Buffer_Size];
	memset(this->Screen_Buffer, 0, this->Buffer_Size * sizeof(CHAR_INFO));
	this->Input_Record_Buffer = new INPUT_RECORD[this->Buffer_Size];
}

YApplication::~YApplication()
{
	delete[] this->Screen_Buffer;
	delete[] this->Input_Record_Buffer;
	SetConsoleActiveScreenBuffer(this->Std_Output_Handle);
	SetConsoleMode(this->Std_Input_Handle, this->Old_Console_Mode);
}

void YApplication::AddWidget(BaseWidget* widget)
{	
	if (widget->Height > this->Height)
		widget->Height = this->Height;
	if (widget->Width > this->Width)
		widget->Width = this->Width;

	this->Children.push_back(widget);
}

void YApplication::DrawChildren()
{
	
	for (auto& i : this->Children)
	{
		i->Width = this->Width;
		i->Height = this->Height;
		i->Draw(this->Screen_Buffer, this->Screen_Buffer_Info);
	}

	if (!WriteConsoleOutput(this->Screen_Buffer_Handle, this->Screen_Buffer,
		this->Screen_Buffer_Info.dwSize, this->Screen_Buffer_Coord, &this->Screen_Buffer_Info.srWindow)) {
		this->ExitWithError("WriteConsoleOutput failed");
	}

}

void YApplication::ClearBuffer()
{
	for (unsigned short i = 0; i < this->Buffer_Size; i++) {
		this->Screen_Buffer[i].Char.UnicodeChar = L' ';
		this->Screen_Buffer[i].Attributes = 0x0f;
	}
}

void YApplication::ReportChildrenMouseMovement(MOUSE_EVENT_RECORD mouse_event)
{
	for (auto& i : this->Children)
	{
		//i->MouseEventHandler(this->Screen_Buffer, this->Screen_Buffer_Info, mouse_event);
	}
}

void YApplication::ExitWithError(LPCSTR error_message)
{
	printf("(%s) - (%d)\n", error_message, GetLastError());
	this->Can_Run = false;
	ExitProcess(0);
}

void YApplication::Run()
{
	this->Can_Run = true;

	this->DrawChildren();

	while (this->Can_Run) {
		
		if (!ReadConsoleInput(this->Std_Input_Handle, this->Input_Record_Buffer, this->Buffer_Size, &this->Number_Input_Records)) {
			this->ExitWithError("ReadConsoleInput failed");
		}
		for (unsigned short i = 0; i < this->Number_Input_Records; i++) {
			switch (this->Input_Record_Buffer[i].EventType)
			{
			case KEY_EVENT:
				break;
			case MOUSE_EVENT:
				this->ReportChildrenMouseMovement(this->Input_Record_Buffer[i].Event.MouseEvent);
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
				break;
			case FOCUS_EVENT:
				break;
			case MENU_EVENT:
				break;
			default:
				this->ExitWithError("Unknown event type");
				break;
			}
		}

		//Sleep(3000);
		Sleep(this->Delay_Time);
	}
}

void YApplication::PrintColorPalete(int seconds)
{
	if (this->Can_Run) return;

	CHAR_INFO symbol_info{};
	symbol_info.Char.UnicodeChar = L'X';
	PrintColorPalette(this->Screen_Buffer, OutputPos(0, 0, this->Screen_Buffer_Info.dwSize.X,
		this->Screen_Buffer_Info.dwSize.X), symbol_info);

	if (!WriteConsoleOutput(this->Screen_Buffer_Handle, this->Screen_Buffer,
		this->Screen_Buffer_Info.dwSize, this->Screen_Buffer_Coord, &this->Screen_Buffer_Info.srWindow)) {
		this->ExitWithError("WriteConsoleOutput failed");
	}
	Sleep(seconds * 1000);
}

void YApplication::SetFPS(unsigned short fps_value)
{
	this->Delay_Time = 1000 / fps_value;
}
