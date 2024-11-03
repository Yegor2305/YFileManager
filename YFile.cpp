#include "YFile.h"

YFile::YFile(unsigned short x_pos, unsigned short y_pos, const WIN32_FIND_DATA& file_data, unsigned short max_length,
	WORD attributes, WORD hover_attributes, WORD selected_attributes)
{
	this->Label_Info = {x_pos, y_pos, 0, attributes};
	this->Name = file_data.cFileName;
	this->Default_Attributes = attributes;
	this->Hover_Attributes = hover_attributes;
	this->Selected_Attributes = selected_attributes;
	this->Max_Length = max_length;
}

void YFile::Draw(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	if (this->Label_Info.Screen_Width == 0)
		this->Label_Info.Screen_Width = screen_buffer_info.dwSize.X;

	//DrawLabel(screen_buffer, this->Label_Info, this->Name);
	DrawLimitedLabel(screen_buffer, this->Label_Info, this->Name.c_str(), this->Max_Length);
}
