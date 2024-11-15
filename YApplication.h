#pragma once

#include <vector>
#include <windows.h>
#include "YFilePanel.h"
#include "YNotifier.h"


class YApplication final : public YNotifier
{
public:
	YApplication(short width = 120, short height = 30);
	~YApplication() override;
	void Run() override;
	void PrintColorPalette(int seconds);
	void SetFPS(unsigned short fps_value);
private:
	void DrawChildren();
	void ClearBuffer();
	void NotifyMouseEvent(const MOUSE_EVENT_RECORD& mouse_event) override;
	void NotifyKeyEvent(const KEY_EVENT_RECORD& key_event) override;
	HANDLE Std_Output_Handle, Std_Input_Handle, Screen_Buffer_Handle;
	INPUT_RECORD* Input_Record_Buffer = nullptr;
	DWORD Old_Console_Mode, Input_Records_Number;
	CONSOLE_SCREEN_BUFFER_INFO Screen_Buffer_Info{};
	COORD Screen_Buffer_Coord{};
	YFilePanel* Left_Panel;
	YFilePanel* Right_Panel;
	unsigned short Width, Height, Max_Height=0, Max_Width=0;
	int Buffer_Size{};
	int Delay_Time = 30;
	std::wstring File_To_Copy_Paste;
	bool Cut = false;
};
