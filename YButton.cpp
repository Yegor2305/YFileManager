#include "YButton.h"

YButton::YButton(unsigned short x_pos, unsigned short y_pos, const std::wstring& name, unsigned short max_length, WORD attributes, WORD hover_attributes, WORD selected_attributes)
{
	this->Label_Info = { x_pos, y_pos, 0, attributes };
	this->Name = name;
	this->Max_Length = max_length;
	this->Default_Attributes = attributes;
	this->Hover_Attributes = hover_attributes;
	this->Selected_Attributes = selected_attributes;
}

void YButton::MouseEventHandler(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event)
{
	if (mouse_event.dwMousePosition.X >= this->Label_Info.X_Pos &&
		mouse_event.dwMousePosition.X <= this->Label_Info.X_Pos + this->Name.length() &&
		mouse_event.dwMousePosition.Y >= this->Label_Info.Y_Pos &&
		mouse_event.dwMousePosition.Y < this->Label_Info.Y_Pos + 1)
	{
		this->MouseEnter(screen_buffer, screen_buffer_info);

		if (mouse_event.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			this->ChangeValueOnClick();
		}
	}
	else
	{
		this->MouseLeave(screen_buffer, screen_buffer_info);
	}
}

void YButton::SetBoolOnClick(bool* bool_ptr)
{
	this->Set_True_On_Click = bool_ptr;
}

void YButton::SetWStringOnClick(std::wstring* wstring_ptr)
{
	this->Set_Name_On_Click = wstring_ptr;
}

void YButton::ChangeValueOnClick() const
{
	if (this->Set_True_On_Click != nullptr)
		*this->Set_True_On_Click = true;

	if (this->Set_Name_On_Click != nullptr)
		*this->Set_Name_On_Click = this->Name;
}
