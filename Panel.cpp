#include "Panel.h"

Panel::Panel(unsigned short width, unsigned short height)
{
	this->Width = width;
	this->Height = height;
}

void Panel::Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	OutputPos pos(this->Pos_X, this->Pos_Y, screen_buffer_info.dwSize.X, this->Width / 2);
	LineInfo line_info{};

	// Horizontal lines drawing
	// Top line	
	line_info.FirstChar.UnicodeChar = L'╔';
	line_info.MediumChar.UnicodeChar = L'═';
	line_info.LastChar.UnicodeChar = L'╗';
	line_info.Attributes = 0xf0;

	DrawLineHorizontal(screen_buffer, pos, line_info);

	// Bottom line
	pos.Y_Pos = this->Height - 1;
	line_info.FirstChar.UnicodeChar = L'╚';
	line_info.LastChar.UnicodeChar = L'╝';

	DrawLineHorizontal(screen_buffer, pos, line_info);

	// Vertical lines drawing
	// Left line
	pos.Y_Pos = 0;
	pos.Lenght = this->Height;
	line_info.FirstChar.UnicodeChar = L'╔';
	line_info.MediumChar.UnicodeChar = L'║';
	line_info.LastChar.UnicodeChar = L'╚';

	DrawLineVertical(screen_buffer, pos, line_info);

	// Right line
	pos.X_Pos = this->Width / 2 - 1;
	line_info.FirstChar.UnicodeChar = L'╗';
	line_info.LastChar.UnicodeChar = L'╝';

	DrawLineVertical(screen_buffer, pos, line_info);

	// Drawing background

	pos.X_Pos = 1;
	pos.Y_Pos = 1;
	pos.Lenght = this->Width / 2 - 2;

	line_info.FirstChar.UnicodeChar = L' ';
	line_info.MediumChar.UnicodeChar = L' ';
	line_info.LastChar.UnicodeChar = L' ';

	for (int i = this->Pos_Y + 1; i < this->Height - 1; i++) {
		DrawLineHorizontal(screen_buffer, pos, line_info);
		pos.Y_Pos++;
	}
}


