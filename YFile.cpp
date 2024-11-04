#include "YFile.h"

YFile::YFile(unsigned short x_pos, unsigned short y_pos, const WIN32_FIND_DATA& file_data, unsigned short max_length,
	WORD attributes, WORD hover_attributes, WORD selected_attributes)
{
	this->Label_Info = {x_pos, y_pos, 0, attributes};
	this->Name = file_data.cFileName;
	this->Is_Directory = file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
	if (!this->IsDirectory())
		this->File_Size = ((static_cast<unsigned long long>(file_data.nFileSizeHigh) << 32) | static_cast<unsigned long long>(file_data.nFileSizeLow)) / 1.0;
	FileTimeToSystemTime(&file_data.ftLastWriteTime, &this->LastWriteTime);
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
	AsmFunctions::DrawLimitedLabel(screen_buffer, this->Label_Info, this->Name.c_str(), this->Max_Length);
}

std::wstring YFile::GetName() const
{
	return this->Name;
}

std::wstring YFile::GetSizeToStr() const
{
	if (this->IsDirectory()) return L"Folder";

	short unit_index = 0;
	double size = this->File_Size;
	while (size > 1000 && unit_index < this->Units_Number - 1)
	{
		size /= 1000;
		unit_index++;
	}

	std::wstringstream wss;
	wss << std::fixed << std::setprecision(2) << size << L' ' << this->Units[unit_index];
	return wss.str();
}

std::wstring YFile::GetLastWriteTimeToStr() const
{
	std::wstringstream wss;
	wss << std::setw(2) << std::setfill(L'0') << this->LastWriteTime.wDay << L'.'
		<< std::setw(2) << std::setfill(L'0') << this->LastWriteTime.wMonth << L'.'
		<< std::setw(2) << std::setfill(L'0') << this->LastWriteTime.wYear << L' '
		<< std::setw(2) << std::setfill(L'0') << this->LastWriteTime.wHour << L':'
		<< std::setw(2) << std::setfill(L'0') << this->LastWriteTime.wMinute;
	return wss.str();
}

bool YFile::IsDirectory() const
{
	return this->Is_Directory;
}
