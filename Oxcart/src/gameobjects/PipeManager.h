#ifndef PIPE_MANAGER_HEADER
#define PIPE_MANAGER_HEADER

#include <vector>
#include "graphics/Shader.h"
#include "Pipe.h"

class OX_API PipeManager
{
public:
	static void Update(const float& aDeltaTime);
	static void Render(Shader& aShader);

	static Model PipeModel;

private:
	PipeManager() {};

	static std::vector<Pipe> myPipes;
	static float myMoveSpeed;
	static float mySpawnRate;
	static int myPipeRoomHeight;
	static float mySpawnTimer;
};
#endif