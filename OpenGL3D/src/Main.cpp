#include "core/Window.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

extern Window* BuildWindow();

int main(void)
{
	Window* tempWindow = BuildWindow();
	tempWindow->Run();
	delete tempWindow;
	return 0;

	glm::mat4 projection = glm::ortho()
}