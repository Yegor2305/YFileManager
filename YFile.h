#pragma once
#include "Main.h"
#include "BaseButton.h"

class YFile : public BaseButton
{
public:
	YFile(unsigned short x_pos, unsigned short y_pos, const WIN32_FIND_DATA& file_data, unsigned short max_length,
		WORD attributes = 0xf0, WORD hover_attributes = 0x0f, WORD selected_attributes = 0x0f);
	void Draw(CHAR_INFO* screen_buffer, const CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info) override;
	std::wstring GetName() const;
	bool IsDirectory() const;
private:
	bool Is_Directory;
};

