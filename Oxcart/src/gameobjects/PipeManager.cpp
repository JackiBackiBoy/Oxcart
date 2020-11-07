#include "PipeManager.h"
#include "math/Math.h"

void PipeManager::Update(const float& aDeltaTime)
{
	mySpawnTimer += aDeltaTime;

	for (unsigned int i = 0; i < myPipes.size(); i++)
	{
		myPipes[i].Position().x -= myMoveSpeed * aDeltaTime;
	}

	if (mySpawnTimer >= mySpawnRate)
	{
		mySpawnTimer = 0;

		int tempRandomHeight = std::rand() % 4;

		Pipe tempBottomPipe = Pipe({ 10, (float)tempRandomHeight, 0 }, PipeModel);
		Pipe tempTopPipe = Pipe({ 10, (float)tempRandomHeight + myPipeRoomHeight, 0 }, PipeModel);

		// Flip the top pipe
		tempTopPipe.Rotation().x = -180;

		myPipes.push_back(tempBottomPipe);
		myPipes.push_back(tempTopPipe);
	}
}

void PipeManager::Render(Shader& aShader)
{
	Matrix4x4 tempModelMatrix;

	for (int i = 0; i < myPipes.size(); i++)
	{
		tempModelMatrix = Matrix4x4::Identity();
		tempModelMatrix = Matrix4x4::Scale(tempModelMatrix, { 3, 3, 3 });
		tempModelMatrix = Matrix4x4::Translate(tempModelMatrix, myPipes[i].Position());
		tempModelMatrix = Matrix4x4::Rotate(tempModelMatrix, Math::ToRadians(myPipes[i].Rotation().x), { 1.0f, 0.0f, 0.0f });
		aShader.SetUniformMatrix4x4("ModelMatrix", tempModelMatrix);

		myPipes[i].Render(aShader);
	}
}

Model PipeManager::PipeModel;
std::vector<Pipe> PipeManager::myPipes;
float PipeManager::myMoveSpeed = 5.0f;
float PipeManager::mySpawnRate = 3.0f;
float PipeManager::mySpawnTimer = 0.0f;
int PipeManager::myPipeRoomHeight = 5;