#ifndef MAIN_HEADER
#define MAIN_HEADER

#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "graphics/Shader.h"
#include "core/Window.h"
//#include "vendor/glm/glm.hpp"
//#include "vendor/glm/gtc/matrix_transform.hpp"
//
//extern Window* BuildWindow();
//
//int main(void)
//{
//	Window* tempWindow = BuildWindow();
//	tempWindow->Run();
//	delete tempWindow;
//	return 0;
//}

extern Window* BuildWindow();

int main(void)
{
	Window* tempWindow = BuildWindow();
	tempWindow->Run();
	delete tempWindow;
	return 0;
}

#endif // Main_h__