#include "YApplication.h"

#include "YNotifier.h"

YApplication::YApplication(short width, short height)
{
	// Disable user resizing
	HWND console = GetConsoleWindow();
	SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));

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

	// Setting console buffer size
	if (!SetConsoleScreenBufferSize(this->Screen_Buffer_Handle, { width, height }))
	{
		this->ExitWithError("SetConsoleScreenBufferSize failed");
	}

	// Setting console window size
	SMALL_RECT window_size = {0, 0, static_cast<short>(width - 1), static_cast<short>(height - 1) };
	if (!SetConsoleWindowInfo(this->Screen_Buffer_Handle, TRUE, &window_size))
	{
		this->ExitWithError("SetConsoleWindowInfo failed");
	}

	// Setting console active buffer
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

	this->Left_Panel = new YFilePanel(this->Width / 2, this->Height, L"C:/Users/Yegor/Desktop/*.*", 0, 0);
	this->Right_Panel = new YFilePanel(this->Width / 2, this->Height, L"C:/Users/Yegor/Desktop/*.*", this->Width / 2, 0);

	this->Left_Panel->SetInputData(&this->Std_Input_Handle, this->Input_Record_Buffer, &this->Buffer_Size, &this->Input_Records_Number, &this->Screen_Buffer_Handle);
	this->Right_Panel->SetInputData(&this->Std_Input_Handle, this->Input_Record_Buffer, &this->Buffer_Size, &this->Input_Records_Number, &this->Screen_Buffer_Handle);

	this->Left_Panel->SetCopyPasteData(&this->File_To_Copy_Paste, &this->Cut);
	this->Right_Panel->SetCopyPasteData(&this->File_To_Copy_Paste, &this->Cut);
}

YApplication::~YApplication()
{
	delete[] this->Screen_Buffer;
	delete[] this->Input_Record_Buffer;

	delete this->Left_Panel;
	delete this->Right_Panel;

	SetConsoleActiveScreenBuffer(this->Std_Output_Handle);
	SetConsoleMode(this->Std_Input_Handle, this->Old_Console_Mode);
}

void YApplication::DrawChildren()
{

	this->Left_Panel->Draw(this->Screen_Buffer, this->Screen_Buffer_Info);
	this->Right_Panel->Draw(this->Screen_Buffer, this->Screen_Buffer_Info);

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

void YApplication::NotifyMouseEvent(const MOUSE_EVENT_RECORD& mouse_event)
{
	this->Left_Panel->MouseEventHandler(this->Screen_Buffer, this->Screen_Buffer_Info, mouse_event);
	this->Right_Panel->MouseEventHandler(this->Screen_Buffer, this->Screen_Buffer_Info, mouse_event);

	if (!WriteConsoleOutput(this->Screen_Buffer_Handle, this->Screen_Buffer,
		this->Screen_Buffer_Info.dwSize, this->Screen_Buffer_Coord, &this->Screen_Buffer_Info.srWindow)) {
		this->ExitWithError("WriteConsoleOutput failed");
	}
}

void YApplication::NotifyKeyEvent(const KEY_EVENT_RECORD& key_event)
{
	this->Left_Panel->KeyEventHandler(this->Screen_Buffer, this->Screen_Buffer_Info, key_event);
	this->Right_Panel->KeyEventHandler(this->Screen_Buffer, this->Screen_Buffer_Info, key_event);

	if (!WriteConsoleOutput(this->Screen_Buffer_Handle, this->Screen_Buffer,
		this->Screen_Buffer_Info.dwSize, this->Screen_Buffer_Coord, &this->Screen_Buffer_Info.srWindow)) {
		this->ExitWithError("WriteConsoleOutput failed");
	}
}

void YApplication::Run()
{
	this->Can_Run = true;
	this->DrawChildren();

	while (this->Can_Run) {
		
		if (!ReadConsoleInput(this->Std_Input_Handle, this->Input_Record_Buffer, this->Buffer_Size, &this->Input_Records_Number)) {
			this->ExitWithError("ReadConsoleInput failed");
		}
		for (unsigned short i = 0; i < this->Input_Records_Number; i++) {
			switch (this->Input_Record_Buffer[i].EventType)
			{
			case KEY_EVENT:
				this->NotifyKeyEvent(this->Input_Record_Buffer[i].Event.KeyEvent);
				break;
			case MOUSE_EVENT:
				this->NotifyMouseEvent(this->Input_Record_Buffer[i].Event.MouseEvent);
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

void YApplication::PrintColorPalette(int seconds)
{
	if (this->Can_Run) return;

	CHAR_INFO symbol_info{};
	symbol_info.Char.UnicodeChar = L'X';
	AsmFunctions::PrintColorPalette(this->Screen_Buffer, OutputPos(0, 0, this->Screen_Buffer_Info.dwSize.X,
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
