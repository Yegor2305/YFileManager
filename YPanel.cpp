#include "YPanel.h"

YPanel::YPanel(unsigned short width, unsigned short height,
               BOOL border_left, BOOL border_right,
               BOOL border_top, BOOL border_bottom, bool double_border,
               WORD background_attributes, WORD border_attributes)
{
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
	}
	else
	{
		this->Corner_Top_Left = L'┌';
		this->Corner_Top_Right = L'┐';
		this->Corner_Bottom_Left = L'└';
		this->Corner_Bottom_Right = L'┘';
		this->Horizontal_Line_Char = L'─';
		this->Vertical_Line_Char = L'│';
	}

	this->Width = width;
	this->Height = height;

	this->Files_List.emplace_back(YFile(2, 1, "First"));
	this->Files_List.emplace_back(YFile(2, 2, "Second"));
}

void YPanel::Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	OutputPos pos(this->Pos_X, this->Pos_Y, screen_buffer_info.dwSize.X, this->Width / 2);
	LineInfo line_info{};
	LabelInfo label_info(3, 3, screen_buffer_info.dwSize.X, 0xf0);

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
		pos.X_Pos = this->Width / 2 - 1;

		DrawLineVertical(screen_buffer, pos, line_info);
	}

	// Drawing background

	pos.X_Pos = this->Pos_X + static_cast<unsigned short>(this->Border_Left);
	pos.Y_Pos = this->Pos_Y + static_cast<unsigned short>(this->Border_Top);
	pos.Length = this->Width / 2 - (this->Border_Left + this->Border_Right);

	line_info.FirstChar.UnicodeChar = L' ';
	line_info.MediumChar.UnicodeChar = L' ';
	line_info.LastChar.UnicodeChar = L' ';
	line_info.Attributes = this->Background_Attributes;

	for (int i = 0; i < this->Height - (this->Border_Top + this->Border_Bottom); i++) {
		DrawLineHorizontal(screen_buffer, pos, line_info);
		pos.Y_Pos++;
	}

	for (auto& file : this->Files_List)
	{
		file.Draw(screen_buffer, screen_buffer_info);
	}
}

void YPanel::MouseEventHandler(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event)
{
	return;
}


