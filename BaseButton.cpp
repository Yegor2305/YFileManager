#include "BaseButton.h"

BaseButton::BaseButton()
{
	this->Label_Info.X_Pos = 0;
	this->Name = L"";
	this->Default_Attributes = 0x00;
	this->Hover_Attributes = 0x00;
	this->Selected_Attributes = 0x00;
	this->Max_Length = 0;
}

BaseButton::~BaseButton() = default;

void BaseButton::Draw(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	if (this->Label_Info.Screen_Width == 0)
		this->Label_Info.Screen_Width = screen_buffer_info.dwSize.X;

	DrawLimitedLabel(screen_buffer, this->Label_Info, this->Name.c_str(), this->Max_Length);
}

void BaseButton::ChangePosition(unsigned short new_x, unsigned short new_y)
{
	this->Label_Info.X_Pos = new_x;
	this->Label_Info.Y_Pos = new_y;
}

void BaseButton::ChangeLimit(unsigned short new_limit)
{
	this->Max_Length = new_limit;
}

void BaseButton::Mouse_Enter(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	this->Label_Info.Attributes = this->Hover_Attributes;
	this->Draw(screen_buffer, screen_buffer_info);
}

void BaseButton::Mouse_Leave(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	this->Label_Info.Attributes = this->Default_Attributes;
	this->Draw(screen_buffer, screen_buffer_info);
}

