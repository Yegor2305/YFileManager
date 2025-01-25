#pragma once

#include <vector>
#include <windows.h>
#include "YFilePanel.h"
#include "YNotifier.h"

struct ShortcutsInfo
{
	ShortcutsInfo(std::wstring shortcut_label, std::wstring meaning) :
	Shortcut_Label(std::move(shortcut_label)), Meaning(std::move(meaning)) {}

	std::wstring Shortcut_Label;
	std::wstring Meaning;
};

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
	void DrawBottomHelper();
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
	unsigned short Shortcuts_Count = 7;
	ShortcutsInfo Shortcuts[7] = {
		{L"Alt + X -", L"Cut"},
		{L"Alt + C -", L"Copy"},
		{L"Alt + V -", L"Paste"},
		{L"Alt + D -", L"New dir"},
		{L"Alt + F -", L"New file"},
		{L"R -", L"Rename"},
		{L"Del -", L"Delete"},
	};
};
