#include "YFilePanel.h"

YFilePanel::YFilePanel(unsigned short width, unsigned short height, LPCWSTR path,
                       unsigned short pos_x, unsigned short pos_y,
                       BOOL border_left, BOOL border_right,
                       BOOL border_top, BOOL border_bottom, bool double_border,
                       WORD background_attributes, WORD border_attributes, WORD elements_attributes):
	Change_Drive_Button(pos_x + 1, pos_y + 1, L"Drives", 6)
{
	this->Pos_X = pos_x;
	this->Pos_Y = pos_y;
	this->Border_Left = border_left;
	this->Border_Right = border_right;
	this->Border_Top = border_top;
	this->Border_Bottom = border_bottom;
	this->Double_Border = double_border;
	this->Background_Attributes = background_attributes;
	this->Border_Attributes = border_attributes;
	this->Elements_Attributes = elements_attributes;

	if (this->Double_Border)
	{
		this->Corner_Top_Left = L'╔';
		this->Corner_Top_Right = L'╗';
		this->Corner_Bottom_Left = L'╚';
		this->Corner_Bottom_Right = L'╝';
		this->Horizontal_Line_Char = L'═';
		this->Vertical_Line_Char = L'║';
		this->Separator_Left_Char = L'╟';
		this->Separator_Right_Char = L'╢';
		this->Vertical_Separator_First_Char = L'╦';
		this->Vertical_Separator_Last_Char = L'╨';
	}
	else
	{
		this->Corner_Top_Left = L'┌';
		this->Corner_Top_Right = L'┐';
		this->Corner_Bottom_Left = L'└';
		this->Corner_Bottom_Right = L'┘';
		this->Horizontal_Line_Char = L'─';
		this->Vertical_Line_Char = L'│';
		this->Separator_Left_Char = L'├';
		this->Separator_Right_Char = L'┤';
		this->Vertical_Separator_First_Char = L'┬';
		this->Vertical_Separator_Last_Char = L'┴';
	}

	this->Separator_Char = L'─';
	this->Width = width;
	this->Height = height;

	this->Path = path;

	size_t pos;

	if ((pos = this->Path.find(L"*.*")) != std::wstring::npos)
		this->Path.erase(pos, 3);

	this->Max_Files_In_Column = this->Height - (this->Content_Offset_Top + this->Content_Offset_Bottom);
	this->FillFiles(path);

	this->Change_Drive_Button.SetBoolOnClick(&this->Drive_Button_Clicked);
}

YFilePanel::~YFilePanel()
{
	this->ClearFiles();
}

void YFilePanel::FillFiles(LPCWSTR path)
{
	this->Current_Directory_Folders_Count = 0;
	this->Current_Directory_Files_Count = 0;

	WIN32_FIND_DATAW file_data;
	HANDLE first_file = FindFirstFile(path, &file_data);

	// Searching for folders

	do
	{
		if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && wcscmp(file_data.cFileName, L".") != 0 
			&& !(file_data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
		{
			if (wcscmp(file_data.cFileName, L"..") != 0) this->Current_Directory_Folders_Count += 1;
			this->Files_List.push_back(new YFile(0, 0, file_data, 0, 0xf5));
		}
	} while (FindNextFile(first_file, &file_data));

	//Searching for files

	first_file = FindFirstFile(path, &file_data);
	
	while (FindNextFile(first_file, &file_data))
	{
		if (!(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (wcscmp(file_data.cFileName, L"..") != 0)
			{
				this->Files_List.push_back(new YFile(0, 0, file_data, 0));
				this->Current_Directory_Files_Count += 1;
			}
		}
	}

	this->Scrollable = this->Files_List.size() > this->Max_Files_In_Column * 2;

}

void YFilePanel::DrawFiles(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const
{
	unsigned short file_pos_x = this->Pos_X + 1;
	unsigned short file_pos_y = this->Content_Offset_Top;

	// Limit between start pos and vertical separator
	unsigned short length_limit = (this->Pos_X + this->Width / 2) - file_pos_x;

	for (unsigned short i = this->First_File_Index_To_Draw; i < this->Files_List.size(); i++)
	{
		if (i - this->First_File_Index_To_Draw == this->Max_Files_In_Column)
		{
			file_pos_y = 2;
			file_pos_x = this->Pos_X + this->Width / 2 + 1;
			length_limit = this->Pos_X + this->Width - file_pos_x - 1;
		}
		if (i - this->First_File_Index_To_Draw >= this->Max_Files_In_Column * 2) break;

		this->Files_List[i]->ChangePosition(file_pos_x, file_pos_y);
		this->Files_List[i]->ChangeLimit(length_limit);
		this->Files_List[i]->Draw(screen_buffer, screen_buffer_info);

		file_pos_y++;
	}
}

void YFilePanel::DrawTitle(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const
{
	if (this->Path.length() > this->Width - 2)
	{
		std::wstring first_part = this->Path.substr(0, this->Path.find(L'/') + 1);
		first_part += L"...";
		std::wstring last_part = this->Path.substr(this->Path.length() - (this->Width - 2 - first_part.length()) -1);
		std::wstring final_title = first_part + last_part;
		LabelInfo label_info(
			this->Pos_X + this->Width / 2 - final_title.length() / 2 - 1,
			this->Pos_Y,
			screen_buffer_info.dwSize.X,
			this->Elements_Attributes);
		AsmFunctions::DrawLabel(screen_buffer, label_info, final_title.c_str());
	}
	else
	{
		if (this->Border_Top)
		{
			OutputPos pos(this->Pos_X + 1, this->Pos_Y, screen_buffer_info.dwSize.X, this->Width - 2);
			LineInfo line_info{};
			line_info.FirstChar.UnicodeChar = this->Horizontal_Line_Char;
			line_info.MediumChar.UnicodeChar = this->Horizontal_Line_Char;
			line_info.LastChar.UnicodeChar = this->Horizontal_Line_Char;
			line_info.Attributes = this->Border_Attributes;
			AsmFunctions::DrawLineHorizontal(screen_buffer, pos, line_info);
		}
		LabelInfo label_info(this->Pos_X + this->Width / 2 - this->Path.length() / 2, this->Pos_Y, screen_buffer_info.dwSize.X, this->Elements_Attributes);
		AsmFunctions::DrawLabel(screen_buffer, label_info, this->Path.c_str());
	}
}

void YFilePanel::DrawFileInfo(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, int file_index) const
{
	LabelInfo label_info(this->Pos_X + 1, this->Pos_Y + this->Height - 2, screen_buffer_info.dwSize.X, this->Elements_Attributes);
	OutputPos pos(this->Pos_X + 1, this->Pos_Y + this->Height - 2, screen_buffer_info.dwSize.X, this->Width - 2);
	LineInfo line_info {{L' '}, {L' '}, {L' '}, this->Background_Attributes};

	AsmFunctions::DrawLineHorizontal(screen_buffer, pos, line_info);
	if (file_index != -1 && Files_List[file_index]->GetName() != L"..")
	{
		std::wstring file_name = this->Files_List[file_index]->GetName();
		if (file_name.length() > this->Width / 2 - 1)
		{
			file_name = file_name.substr(file_name.length() - this->Width / 2 + 1);
		}
		AsmFunctions::DrawLabel(screen_buffer, label_info, file_name.c_str());

		std::wstring file_info = this->Files_List[file_index]->GetSizeToStr() + L' ' + this->Files_List[file_index]->GetLastWriteTimeToStr();
		label_info.X_Pos = this->Pos_X + this->Width - file_info.length() - 1;
		AsmFunctions::DrawLabel(screen_buffer, label_info, file_info.c_str());
	}
}

void YFilePanel::DrawCurrentDirectoryInfo(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const
{
	OutputPos pos(this->Pos_X + 1, this->Pos_Y + this->Height - 1, screen_buffer_info.dwSize.X, this->Width - 2);
	LineInfo line_info{ {this->Horizontal_Line_Char}, {this->Horizontal_Line_Char}, {this->Horizontal_Line_Char}, this->Border_Attributes };

	AsmFunctions::DrawLineHorizontal(screen_buffer, pos, line_info);

	std::wstring current_directory_info =
		L" Folders: " + std::to_wstring(this->Current_Directory_Folders_Count) +
		L", files: " + std::to_wstring(this->Current_Directory_Files_Count) + L' ';

	LabelInfo label_info(this->Pos_X + this->Width / 2 - current_directory_info.length() / 2,
		this->Pos_Y + this->Height - 1, screen_buffer_info.dwSize.X, this->Elements_Attributes);

	AsmFunctions::DrawLabel(screen_buffer, label_info, current_directory_info.c_str());
}

void YFilePanel::DrawVerticalSeparator(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const
{
	OutputPos pos(this->Pos_X + this->Width / 2, this->Pos_Y + 1, screen_buffer_info.dwSize.X, this->Pos_Y + this->Height - 2);
	LineInfo line_info;

	line_info.FirstChar.UnicodeChar = this->Vertical_Line_Char;
	line_info.MediumChar.UnicodeChar = this->Vertical_Line_Char;
	line_info.LastChar.UnicodeChar = this->Vertical_Separator_Last_Char;
	line_info.Attributes = this->Border_Attributes;

	AsmFunctions::DrawLineVertical(screen_buffer, pos, line_info);
}

void YFilePanel::ClearFiles()
{
	for (YFile* ptr : this->Files_List)
	{
		delete ptr;
	}

	this->Files_List.clear();
}

void YFilePanel::Refresh(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	this->File_To_Copy_Cut_Path->clear();
	this->ClearFiles();
	this->FillFiles((this->Path + L"*.*").c_str());
	this->DrawFiles(screen_buffer, screen_buffer_info);
}

void YFilePanel::ClearColumns(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) const
{
	LineInfo line_info = { {L' '}, {L' '}, {L' '}, this->Background_Attributes };
	OutputPos output_pos (this->Pos_X + 1, this->Content_Offset_Top, screen_buffer_info.dwSize.X, (this->Pos_X + this->Width / 2) - (this->Pos_X + 1));

	for (unsigned short i = 0; i < this->Max_Files_In_Column * 2; i++)
	{
		if (i == this->Max_Files_In_Column)
		{
			output_pos.Y_Pos = 2;
			output_pos.X_Pos = this->Pos_X + this->Width / 2 + 1;
			output_pos.Length = this->Pos_X + this->Width - output_pos.X_Pos - 1;
		}

		AsmFunctions::DrawLineHorizontal(screen_buffer, output_pos, line_info);

		output_pos.Y_Pos += 1;
	}
}

void YFilePanel::ChangeDirectory(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	this->ClearColumns(screen_buffer, screen_buffer_info);
	this->ClearFiles();
	this->DrawTitle(screen_buffer, screen_buffer_info);
	this->FillFiles((this->Path + L"*.*").c_str());
	this->First_File_Index_To_Draw = 0;
	this->DrawFiles(screen_buffer, screen_buffer_info);
	this->DrawCurrentDirectoryInfo(screen_buffer, screen_buffer_info);
	this->Hovered_File_Index = -1;
	this->Selected_File_Index = -1;
}

void YFilePanel::Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	OutputPos pos(this->Pos_X, this->Pos_Y, screen_buffer_info.dwSize.X, this->Width);
	LineInfo line_info{};
	LabelInfo label_info(this->Pos_X + this->Width / 4 - 2, this->Pos_Y + this->Border_Top, screen_buffer_info.dwSize.X, this->Elements_Attributes);

	// Horizontal borders drawing

	line_info.FirstChar.UnicodeChar = this->Corner_Top_Left;
	line_info.MediumChar.UnicodeChar = this->Horizontal_Line_Char;
	line_info.LastChar.UnicodeChar = this->Corner_Top_Right;
	line_info.Attributes = this->Border_Attributes;

	// Top border
	if (this->Border_Top)
	{
		AsmFunctions::DrawLineHorizontal(screen_buffer, pos, line_info);
	}

	// Bottom border

	if (this->Border_Bottom)
	{
		pos.Y_Pos = this->Height - 1;
		line_info.FirstChar.UnicodeChar = this->Corner_Bottom_Left;
		line_info.LastChar.UnicodeChar = this->Corner_Bottom_Right;

		AsmFunctions::DrawLineHorizontal(screen_buffer, pos, line_info);
	}

	// Vertical borders drawing

	pos.Y_Pos = this->Pos_Y + 1;
	pos.Length = this->Height - 2;
	line_info.FirstChar.UnicodeChar = this->Vertical_Line_Char;
	line_info.MediumChar.UnicodeChar = this->Vertical_Line_Char;
	line_info.LastChar.UnicodeChar = this->Vertical_Line_Char;

	// Left border
	
	if (this->Border_Left)
	{
		AsmFunctions::DrawLineVertical(screen_buffer, pos, line_info);
	}

	// Right border

	if (this->Border_Right)
	{
		pos.X_Pos = this->Pos_X + this->Width - 1;

		AsmFunctions::DrawLineVertical(screen_buffer, pos, line_info);
	}

	// Drawing background

	pos.X_Pos = this->Pos_X + static_cast<unsigned short>(this->Border_Left);
	pos.Y_Pos = this->Pos_Y + static_cast<unsigned short>(this->Border_Top);
	pos.Length = this->Width - (this->Border_Left + this->Border_Right);

	line_info.FirstChar.UnicodeChar = L' ';
	line_info.MediumChar.UnicodeChar = L' ';
	line_info.LastChar.UnicodeChar = L' ';
	line_info.Attributes = this->Background_Attributes;

	for (int i = 0; i < this->Height - (this->Border_Top + this->Border_Bottom); i++) {
		AsmFunctions::DrawLineHorizontal(screen_buffer, pos, line_info);
		pos.Y_Pos++;
	}

	//Drawing bottom separator

	pos.X_Pos = this->Pos_X;
	pos.Y_Pos = this->Pos_Y + this->Height - 3;
	pos.Length = this->Width;
	line_info.FirstChar.UnicodeChar = this->Separator_Left_Char;
	line_info.MediumChar.UnicodeChar = this->Separator_Char;
	line_info.LastChar.UnicodeChar = this->Separator_Right_Char;
	line_info.Attributes = this->Border_Attributes;

	AsmFunctions::DrawLineHorizontal(screen_buffer, pos, line_info);

	// Drawing vertical separator

	this->DrawVerticalSeparator(screen_buffer, screen_buffer_info);

	// Drawing headers

	AsmFunctions::DrawLabel(screen_buffer, label_info, L"Name");

	label_info.X_Pos = this->Pos_X + this->Width / 4 * 3 - 2;

	AsmFunctions::DrawLabel(screen_buffer, label_info, L"Name");

	// Drawing title

	this->DrawTitle(screen_buffer, screen_buffer_info);

	this->Change_Drive_Button.Draw(screen_buffer, screen_buffer_info);

	this->DrawFiles(screen_buffer, screen_buffer_info);

	this->DrawCurrentDirectoryInfo(screen_buffer, screen_buffer_info);
}

void YFilePanel::MouseEventHandler(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event)
{

	if (mouse_event.dwMousePosition.X >= this->Pos_X && mouse_event.dwMousePosition.X <= this->Pos_X + this->Width &&
		mouse_event.dwMousePosition.Y >= this->Pos_Y + Content_Offset_Top && mouse_event.dwMousePosition.Y < this->Pos_Y + this->Height - Content_Offset_Bottom)
	{
		this->In_Focus = true;
		if (mouse_event.dwEventFlags == MOUSE_WHEELED)
		{
			bool down = static_cast<std::make_signed_t<WORD>>(HIWORD(mouse_event.dwButtonState)) < 0;
			if (this->Scrollable)
			{
				unsigned short new_first_file_index = 0;

				if (down)
					new_first_file_index = min(this->First_File_Index_To_Draw + this->Scroll_Offset, this->Files_List.size() - this->Max_Files_In_Column * 2);
				else
					new_first_file_index = max(this->First_File_Index_To_Draw - this->Scroll_Offset, 0);
				

				if (this->First_File_Index_To_Draw == new_first_file_index) return;
				this->First_File_Index_To_Draw = new_first_file_index;
				this->DrawFiles(screen_buffer, screen_buffer_info);
			}
		}

		int second_column_offset = mouse_event.dwMousePosition.X < this->Pos_X + this->Width / 2 ? 0 : this->Max_Files_In_Column;
		int file_index = mouse_event.dwMousePosition.Y - this->Pos_Y - this->Content_Offset_Top + second_column_offset + this->First_File_Index_To_Draw;

		if (this->Hovered_File_Index != -1 && file_index != this->Hovered_File_Index)
		{
			this->Files_List[this->Hovered_File_Index]->MouseLeave(screen_buffer, screen_buffer_info);
		}

		if (file_index < this->Files_List.size())
		{
			this->Files_List[file_index]->MouseEnter(screen_buffer, screen_buffer_info);
			this->Hovered_File_Index = file_index;
		}

		if (mouse_event.dwEventFlags == DOUBLE_CLICK)
		{

			if (this->Files_List[file_index]->GetName() == L"..")
			{
				this->Path.erase(this->Path.rfind(L'/', this->Path.rfind(L'/') - 1) + 1);
			}
			else
			{
				if (this->Files_List[file_index]->IsDirectory())
				{
					this->Path += this->Files_List[file_index]->GetName() + L"/";
				}
			}

			this->ChangeDirectory(screen_buffer, screen_buffer_info);
		}

		if (mouse_event.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && file_index < this->Files_List.size())
		{
			this->Files_List[file_index]->Select(screen_buffer, screen_buffer_info);

			if (this->Selected_File_Index != -1 && file_index != this->Selected_File_Index)
			{
				bool need_draw = true;
				if (this->Selected_File_Index < this->First_File_Index_To_Draw)
					need_draw = false;
					
				this->Files_List[this->Selected_File_Index]->UnSelect(screen_buffer, screen_buffer_info, need_draw);
			}

			this->Selected_File_Index = file_index;
			this->DrawFileInfo(screen_buffer, screen_buffer_info, this->Selected_File_Index);

		}

	}
	else
	{
		this->In_Focus = false;
		if (this->Hovered_File_Index != -1)
		{
			this->Files_List[this->Hovered_File_Index]->MouseLeave(screen_buffer, screen_buffer_info);
			this->Hovered_File_Index = -1;
		}

		if (mouse_event.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && this->Selected_File_Index != -1)
		{
			this->Files_List[this->Selected_File_Index]->UnSelect(screen_buffer, screen_buffer_info);
			this->Selected_File_Index = -1;
			this->DrawFileInfo(screen_buffer, screen_buffer_info, this->Selected_File_Index);
		}
		
	}

	Change_Drive_Button.MouseEventHandler(screen_buffer, screen_buffer_info, mouse_event);

	if (this->Drive_Button_Clicked)
	{
		this->Drive_Button_Clicked = false;

		std::wstring drive;
		YChangeDriveModal modal(this->Width - 8, 8, this->Pos_X + 2, this->Pos_Y + this->Content_Offset_Top + 2, screen_buffer,
			&screen_buffer_info, &drive, this->Std_Input_Handle, this->Input_Record_Buffer,
			this->Buffer_Size, this->Input_Records_Number, this->Screen_Buffer_Handle, 0x70, 0x70);
		modal.Run();

		this->DrawVerticalSeparator(screen_buffer, screen_buffer_info);

		if (drive.empty() || this->Path == drive)
		{
			this->DrawFiles(screen_buffer, screen_buffer_info);
		}else
		{
			this->Path = drive;
			this->ChangeDirectory(screen_buffer, screen_buffer_info);
		}
	}
}

void YFilePanel::KeyEventHandler(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, KEY_EVENT_RECORD key_event)
{

	if (!this->In_Focus || !this->File_To_Copy_Cut_Path) return;

	bool left_alt_pressed = key_event.dwControlKeyState & LEFT_ALT_PRESSED;

	if (left_alt_pressed &&( key_event.wVirtualKeyCode == 'C' || key_event.wVirtualKeyCode == 'X'))
	{
		if (this->Selected_File_Index == -1) return;
		
		*this->File_To_Copy_Cut_Path = this->Path + this->Files_List[this->Selected_File_Index]->GetName();

		*this->Cut = key_event.wVirtualKeyCode == 'X';

	}

	if (left_alt_pressed && key_event.wVirtualKeyCode == 'V')
	{
		if (this->File_To_Copy_Cut_Path->empty()) return;

		std::wstring destination_path = this->Path + this->File_To_Copy_Cut_Path->substr(this->File_To_Copy_Cut_Path->find_last_of(L'/') + 1);

		if (!*this->Cut)
		{
			if (CopyFile(this->File_To_Copy_Cut_Path->c_str(), destination_path.c_str(), true))
			{
				this->Refresh(screen_buffer, screen_buffer_info);
			}
				
		}else
		{
			if (MoveFile(this->File_To_Copy_Cut_Path->c_str(), destination_path.c_str()))
			{
				this->Refresh(screen_buffer, screen_buffer_info);
			}
		}

	}
}
