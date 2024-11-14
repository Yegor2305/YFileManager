#pragma once
#include "YObserver.h"
#include "BaseButton.h"

class YButton : public YObserver, public BaseButton
{
public:
	YButton(unsigned short x_pos, unsigned short y_pos, const std::wstring& name, unsigned short max_length,
			WORD attributes = 0xf0, WORD hover_attributes = 0x0f, WORD selected_attributes = 0x0f);
	void MouseEventHandler(CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info, MOUSE_EVENT_RECORD mouse_event) override;
	void SetBoolOnClick(bool* bool_ptr);
	void SetWStringOnClick(std::wstring* wstring_ptr);
private:
	void ChangeValueOnClick() const;
	bool* Set_True_On_Click = nullptr;
	std::wstring* Set_Name_On_Click = nullptr;
};

