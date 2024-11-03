#include "YFilePanel.h"

YFilePanel::YFilePanel(unsigned short width, unsigned short height, LPCWSTR path,
						unsigned short pos_x, unsigned short pos_y,
						BOOL border_left, BOOL border_right,
						BOOL border_top, BOOL border_bottom, bool double_border,
						WORD background_attributes, WORD border_attributes)
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

	this->Title = path;

	size_t pos;

	if ((pos = this->Title.find(L"*.*")) != std::wstring::npos)
		this->Title.erase(pos, 3);

	this->Max_Files_In_Column = this->Height - (this->Content_Offset_Top + this->Content_Offset_Bottom);
	this->FillFiles(path);
}

YFilePanel::~YFilePanel()
{
	for (YFile* ptr: this->Files_List)
	{
		delete ptr;
	}

	this->Files_List.clear();
}

void YFilePanel::FillFiles(LPCWSTR path)
{
	
	WIN32_FIND_DATAW file_data;
	HANDLE first_file = FindFirstFile(path, &file_data);

	/*unsigned short file_y_pos = 2;
	unsigned short file_x_pos = this->Pos_X + 1;*/

	// Searching for directories
	while (FindNextFile(first_file, &file_data))
	{
		if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY || wcscmp(file_data.cFileName, L"..") == 0)
		{
			this->Files_List.push_back(new YFile(0, 0, file_data, 0, 0xf5));
		}

	}

	//Searching for files

	first_file = FindFirstFile(path, &file_data);
	
	while (FindNextFile(first_file, &file_data))
	{
		if (!(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (wcscmp(file_data.cFileName, L"..") != 0)
			{
				this->Files_List.push_back(new YFile(0, 0, file_data, 0));
			}
		}
	}

}

void YFilePanel::Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	OutputPos pos(this->Pos_X, this->Pos_Y, screen_buffer_info.dwSize.X, this->Width);
	LineInfo line_info{};
	LabelInfo label_info(this->Pos_X + this->Width / 4 - 2, this->Pos_Y + this->Border_Top, screen_buffer_info.dwSize.X, this->GraphicAttributes);
	//LabelInfo label_info(3, 3, screen_buffer_info.dwSize.X, 0xf0);

	// Horizontal borders drawing

	line_info.FirstChar.UnicodeChar = this->Corner_Top_Left;
	line_info.MediumChar.UnicodeChar = this->Horizontal_Line_Char;
	line_info.LastChar.UnicodeChar = this->Corner_Top_Right;
	line_info.Attributes = this->Border_Attributes;

	// Top border
	if (this->Border_Top)
	{
		DrawLineHorizontal(screen_buffer, pos, line_info);
	}

	// Bottom border

	if (this->Border_Bottom)
	{
		pos.Y_Pos = this->Height - 1;
		line_info.FirstChar.UnicodeChar = this->Corner_Bottom_Left;
		line_info.LastChar.UnicodeChar = this->Corner_Bottom_Right;

		DrawLineHorizontal(screen_buffer, pos, line_info);
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
		DrawLineVertical(screen_buffer, pos, line_info);
	}

	// Right border

	if (this->Border_Right)
	{
		pos.X_Pos = this->Pos_X + this->Width - 1;

		DrawLineVertical(screen_buffer, pos, line_info);
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
		DrawLineHorizontal(screen_buffer, pos, line_info);
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

	DrawLineHorizontal(screen_buffer, pos, line_info);

	// Drawing vertical separator

	pos.X_Pos = this->Pos_X + this->Width / 2;
	pos.Y_Pos = this->Pos_Y;
	pos.Length = this->Pos_Y + this->Height - 2;
	line_info.FirstChar.UnicodeChar = this->Vertical_Separator_First_Char;
	line_info.MediumChar.UnicodeChar = this->Vertical_Line_Char;
	line_info.LastChar.UnicodeChar = this->Vertical_Separator_Last_Char;

	DrawLineVertical(screen_buffer, pos, line_info);

	// Drawing headers

	DrawLabel(screen_buffer, label_info, L"Name");

	label_info.X_Pos = this->Pos_X + this->Width / 4 * 3 - 2;

	DrawLabel(screen_buffer, label_info, L"Name");

	// Drawing title

	label_info.Y_Pos = this->Pos_Y;
	label_info.X_Pos = this->Pos_X + this->Width / 2 - this->Title.length() / 2;
	DrawLabel(screen_buffer, label_info, this->Title.c_str());

	unsigned short file_pos_x = this->Pos_X + 1;
	unsigned short file_pos_y = 2;

	// Limit between start pos and vertical separator
	unsigned short length_limit = (this->Pos_X + this->Width / 2) - file_pos_x - 1;

	for (int i = 0; i < this->Files_List.size(); i++)
	{
		if (i == this->Max_Files_In_Column)
		{
			file_pos_y = 2;
			file_pos_x = this->Pos_X + this->Width / 2 + 1;
			length_limit = this->Pos_X + this->Width - file_pos_x - 2;
		}
		if (i >= this->Max_Files_In_Column * 2) break;

		this->Files_List[i]->ChangePosition(file_pos_x, file_pos_y);
		this->Files_List[i]->ChangeLimit(length_limit);
		this->Files_List[i]->Draw(screen_buffer, screen_buffer_info);

		file_pos_y++;
	}
}

void YFilePanel::MouseEventHandler(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event)
{
	if (mouse_event.dwMousePosition.X >= this->Pos_X && mouse_event.dwMousePosition.X <= this->Pos_X + this->Width &&
		mouse_event.dwMousePosition.Y >= this->Pos_Y + Content_Offset_Top && mouse_event.dwMousePosition.Y < this->Pos_Y + this->Height - Content_Offset_Bottom)
	{
		int second_column_offset = mouse_event.dwMousePosition.X < this->Pos_X + this->Width / 2 ? 0 : this->Max_Files_In_Column;
		int file_index = mouse_event.dwMousePosition.Y - this->Pos_Y - 2 + second_column_offset;
		if (this->Hovered_File_Index != -1 && file_index != this->Hovered_File_Index)
		{
			this->Files_List[Hovered_File_Index]->Mouse_Leave(screen_buffer, screen_buffer_info);
		}

		if (file_index < this->Files_List.size())
		{
			this->Files_List[file_index]->Mouse_Enter(screen_buffer, screen_buffer_info);
			this->Hovered_File_Index = file_index;
		}
	}
	else
	{
		if (this->Hovered_File_Index != -1)
		{
			this->Files_List[Hovered_File_Index]->Mouse_Leave(screen_buffer, screen_buffer_info);
			this->Hovered_File_Index = -1;
		}
		
	}
	
}
