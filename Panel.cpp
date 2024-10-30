#include "Panel.h"

Panel::Panel(unsigned short width, unsigned short height)
{
	this->Width = width;
	this->Height = height;
}

void Panel::Draw()
{
	OutputPos pos(this->Pos_X, this->Pos_Y, this->Screen_Buffer_Info.dwSize.X, this->Width);

	CHAR_INFO symbol_info{};
	symbol_info.Char.UnicodeChar = L' ';
	symbol_info.Attributes = 0xf0;

	for (int i = this->Pos_Y; i < this->Height; i++) {
		DrawLineHorizontal(this->Screen_Buffer, pos, symbol_info);
		pos.Y_Pos++;
	}
}


