#pragma once
#include "BaseWidget.h"
#include <memory>
#include <vector>
#include <windows.h>
#include <stdio.h>
#include "Main.h"
#include "BaseWidget.h"


class YApplication : public BaseWidget
{
public:
	YApplication(HANDLE& screen_buffer_handle, CHAR_INFO* screen_buffer, CONSOLE_SCREEN_BUFFER_INFO& screen_buffer_info);
	void AddWidget(BaseWidget* widget);
	void Draw();
	void Resize(CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info);
private:
	HANDLE Screen_Buffer_Handle;
	std::vector<BaseWidget*> Children;
};
