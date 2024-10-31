#include "Panel.h"

Panel::Panel(unsigned short width, unsigned short height)
{
	this->Width = width;
	this->Height = height;
}

void Panel::Draw(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	OutputPos pos(this->Pos_X, this->Pos_Y, screen_buffer_info.dwSize.X, this->Width / 2);

	CHAR_INFO symbol_info{};
	symbol_info.Char.UnicodeChar = L' ';
	symbol_info.Attributes = 0xf0;

	for (int i = this->Pos_Y; i < this->Height; i++) {
		DrawLineHorizontal(screen_buffer, pos, symbol_info);
		pos.Y_Pos++;
	}
}


