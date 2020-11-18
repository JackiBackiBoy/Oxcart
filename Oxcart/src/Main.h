#ifndef MAIN_HEADER
#define MAIN_HEADER

#include "core/Window.h"

extern Window* BuildWindow();

int main(void)
{
	Window* tempWindow = BuildWindow();
	tempWindow->Run();
	delete tempWindow;
	return 0;
}

#endif