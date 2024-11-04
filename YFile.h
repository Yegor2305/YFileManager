#pragma once
#include <sstream>
#include <iomanip>
#include "Main.h"
#include "BaseButton.h"

class YFile : public BaseButton
{
public:
	YFile(unsigned short x_pos, unsigned short y_pos, const WIN32_FIND_DATA& file_data, unsigned short max_length,
		WORD attributes = 0xf0, WORD hover_attributes = 0x0f, WORD selected_attributes = 0x0f);
	std::wstring GetName() const;
	std::wstring GetSizeToStr() const;
	std::wstring GetLastWriteTimeToStr() const;
	bool IsDirectory() const;
private:
	bool Is_Directory;
	double File_Size;
	SYSTEMTIME LastWriteTime;
	const char Units_Number = 4;
	std::wstring Units[4] = { L"B", L"KB", L"MB", L"GB" };
};

