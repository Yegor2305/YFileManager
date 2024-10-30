#pragma once
#include "Main.h"
#include "BaseWidget.h"
#include "YApplication.h"

class Panel : public BaseWidget
{
public:
	void Draw() override;
	Panel(unsigned short width, unsigned short height);
};

