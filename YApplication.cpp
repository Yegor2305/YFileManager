#include "YApplication.h"

YApplication::YApplication(HANDLE& screen_buffer_handle, CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info)
{
	this->Screen_Buffer_Handle = screen_buffer_handle;
	this->Screen_Buffer = screen_buffer;
	this->Screen_Buffer_Info = screen_buffer_info;
	this->Width = Screen_Buffer_Info.dwSize.X;
	this->Height = Screen_Buffer_Info.dwSize.Y;
}

void YApplication::AddWidget(BaseWidget* widget)
{	
	widget->Screen_Buffer = this->Screen_Buffer;
	widget->Screen_Buffer_Info = this->Screen_Buffer_Info;
	this->Children.push_back(widget);
}

void YApplication::Draw()
{
	
	for (int i = 0; i < this->Children.size(); i++) {
		
		this->Children[i]->Width = this->Width;
		this->Children[i]->Height = this->Height;
		this->Children[i]->Screen_Buffer = this->Screen_Buffer;
		this->Children[i]->Screen_Buffer_Info = this->Screen_Buffer_Info;
		this->Children[i]->Draw();
	}

}

void YApplication::Resize(CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info)
{
	auto t = GetConsoleScreenBufferInfo(this->Screen_Buffer_Handle, &this->Screen_Buffer_Info);
	if (this->Screen_Buffer_Info.dwSize.X != this->Width) {
		this->Width = this->Screen_Buffer_Info.dwSize.X;
		this->Height = this->Screen_Buffer_Info.dwSize.Y;

		delete[] this->Screen_Buffer;
		this->Screen_Buffer = new CHAR_INFO[this->Width * this->Height];

		this->Draw();
		COORD screen_buffer_coord{};
		if (!WriteConsoleOutput(this->Screen_Buffer_Handle, this->Screen_Buffer,
			this->Screen_Buffer_Info.dwSize, screen_buffer_coord, &this->Screen_Buffer_Info.srWindow)) {
			printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
		}
	}
}
	
