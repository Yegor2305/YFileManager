#include "YFile.h"

YFile::YFile(unsigned short x_pos, unsigned short y_pos, const char* name, WORD attributes, WORD hover_attributes):
		Label_Info(x_pos, y_pos, 0, attributes)
{
	this->Label_Info.X_Pos = x_pos;
	this->Name = name;
	this->Default_Attributes = attributes;
	this->Hover_Attributes = hover_attributes;
}

auto YFile::Draw(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	if (this->Label_Info.Screen_Width == 0)
		this->Label_Info.Screen_Width = screen_buffer_info.dwSize.X;

	//DrawLabel(screen_buffer, this->Label_Info, this->Name);
	DrawLimitedLabel(screen_buffer, this->Label_Info, this->Name, 4);
}

void YFile::Mouse_Enter(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	this->Label_Info.Attributes = this->Hover_Attributes;
	this->Draw(screen_buffer, screen_buffer_info);
}

void YFile::Mouse_Leave(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	this->Label_Info.Attributes = this->Default_Attributes;
	this->Draw(screen_buffer, screen_buffer_info);
}
