#include "YFilePanel.h"

YFilePanel::YFilePanel(unsigned short width, unsigned short height, LPCWSTR path,
						unsigned short pos_x, unsigned short pos_y,
						BOOL border_left, BOOL border_right,
						BOOL border_top, BOOL border_bottom, bool double_border,
						WORD background_attributes, WORD border_attributes, WORD elements_attributes)
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
}

YFilePanel::~YFilePanel()
{
	this->ClearFiles();
}

void YFilePanel::FillFiles(LPCWSTR path)
{
	
	WIN32_FIND_DATAW file_data;
	HANDLE first_file = FindFirstFile(path, &file_data);

	do
	{
		if ((file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY || wcscmp(file_data.cFileName, L"..") == 0) && wcscmp(file_data.cFileName, L".") != 0)
		{
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
			}
		}
	}

	if (this->Files_List.size() > this->Max_Files_In_Column * 2) this->Scrollable = true;

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
		DrawLabel(screen_buffer, label_info, final_title.c_str());
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
			DrawLineHorizontal(screen_buffer, pos, line_info);
		}
		LabelInfo label_info(this->Pos_X + this->Width / 2 - this->Path.length() / 2, this->Pos_Y, screen_buffer_info.dwSize.X, this->Elements_Attributes);
		DrawLabel(screen_buffer, label_info, this->Path.c_str());
	}
}

void YFilePanel::ClearFiles()
{
	for (YFile* ptr : this->Files_List)
	{
		delete ptr;
	}

	this->Files_List.clear();
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

		DrawLineHorizontal(screen_buffer, output_pos, line_info);

		output_pos.Y_Pos += 1;
	}
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

	this->DrawTitle(screen_buffer, screen_buffer_info);

	this->DrawFiles(screen_buffer, screen_buffer_info);
}

void YFilePanel::MouseEventHandler(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event)
{

	if (mouse_event.dwMousePosition.X >= this->Pos_X && mouse_event.dwMousePosition.X <= this->Pos_X + this->Width &&
		mouse_event.dwMousePosition.Y >= this->Pos_Y + Content_Offset_Top && mouse_event.dwMousePosition.Y < this->Pos_Y + this->Height - Content_Offset_Bottom)
	{

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
			this->Files_List[Hovered_File_Index]->Mouse_Leave(screen_buffer, screen_buffer_info);
		}

		if (file_index < this->Files_List.size())
		{
			this->Files_List[file_index]->Mouse_Enter(screen_buffer, screen_buffer_info);
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

			this->ClearColumns(screen_buffer, screen_buffer_info);
			this->ClearFiles();
			this->DrawTitle(screen_buffer, screen_buffer_info);
			this->FillFiles((this->Path + L"*.*").c_str());
			this->First_File_Index_To_Draw = 0;
			this->DrawFiles(screen_buffer, screen_buffer_info);
			this->Hovered_File_Index = -1;
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

void YFilePanel::KeyEventHandler(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, KEY_EVENT_RECORD key_event)
{
}
