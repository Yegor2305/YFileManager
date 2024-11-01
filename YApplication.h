#pragma once

#include <vector>
#include <windows.h>
#include "YPanel.h"


class YApplication
{
public:
	YApplication(short width = 120, short height = 30);
	~YApplication();
	void AddWidget(YPanel* widget);	
	void Run();
	void PrintColorPalete(int seconds);
	void SetFPS(unsigned short fps_value);
	
private:
	void DrawChildren();
	void ClearBuffer();
	void ReportChildrenMouseMovement(MOUSE_EVENT_RECORD mouse_event);
	void ExitWithError(LPCSTR error_message);
	HANDLE Std_Output_Handle, Std_Input_Handle, Screen_Buffer_Handle;
	CHAR_INFO* Screen_Buffer = nullptr;
	INPUT_RECORD* Input_Record_Buffer = nullptr;
	DWORD Old_Console_Mode, Number_Input_Records;
	CONSOLE_SCREEN_BUFFER_INFO Screen_Buffer_Info{};
	COORD Screen_Buffer_Coord{};
	std::vector<YPanel*> Children;
	unsigned short Width, Height, Max_Height=0, Max_Width=0;
	int Buffer_Size{};
	int Delay_Time = 30;
	bool Can_Run = false;
};
