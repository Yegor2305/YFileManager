#pragma once

#include "BaseWidget.h"
#include "YFile.h"
#include "YChangeDriveModal.h"
#include "YConfirmModal.h"
#include "YInputBox.h"
#include <vector>
#include <shlobj.h>
#include <algorithm>

class YFilePanel : public BaseWidget, public YObserver, public YNotifier
{
public:
	YFilePanel(unsigned short width, unsigned short height, LPCWSTR path,
		unsigned short pos_x = 0, unsigned short pos_y = 0,
		BOOL border_left = 1, BOOL border_right = 1,
		BOOL border_top = 1, BOOL border_bottom = 1, bool double_border = true,
		WORD background_attributes = 0xf0, WORD border_attributes = 0xf0, WORD elements_attributes = 0xf0);
	~YFilePanel() override;
	void Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) override;
	void MouseEventHandler(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event) override;
	void KeyEventHandler(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, KEY_EVENT_RECORD key_event) override;
	void SetCopyPasteData(std::wstring* file_to_copy_cut_path, bool* cut);
private:
	void FillFiles(LPCWSTR path);
	void DrawFiles(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const;
	void DrawTitle(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const;
	void DrawFileInfo(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, int file_index) const;
	void DrawCurrentDirectoryInfo(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const;
	void DrawVerticalSeparator(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const;
	void ClearFiles();
	void Refresh(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);
	void ClearColumns(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const;
	void ChangeDirectory(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);
	YButton Change_Drive_Button, Refresh_Button;
	bool Drive_Button_Clicked = false, Refresh_Button_Clicked = false;
	BOOL Border_Left, Border_Right, Border_Top, Border_Bottom;
	bool Double_Border;
	bool Scrollable = false;
	unsigned short First_File_Index_To_Draw = 0, Scroll_Offset = 2;
	WCHAR Corner_Top_Left, Corner_Top_Right, Corner_Bottom_Left, Corner_Bottom_Right,
	Horizontal_Line_Char, Vertical_Line_Char, Separator_Left_Char, Separator_Right_Char, Separator_Char,
	Vertical_Separator_First_Char, Vertical_Separator_Last_Char;
	WORD Background_Attributes, Border_Attributes;
	std::vector<YFile*> Files_List{};
	int Hovered_File_Index = -1, Selected_File_Index = - 1;
	unsigned short Content_Offset_Top = 2, Content_Offset_Bottom = 3, Max_Files_In_Column;
	unsigned short Current_Directory_Folders_Count = 0, Current_Directory_Files_Count = 0;
	std::wstring Path;
	std::wstring* File_To_Copy_Cut_Path = nullptr;
	bool* Cut = nullptr;
};

