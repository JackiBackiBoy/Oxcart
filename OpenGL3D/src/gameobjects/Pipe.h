#ifndef PIPE_HEADER
#define PIPE_HEADER

#include "GameObject.h"
#include <data/Model.h>

class Pipe : public GameObject
{
public:
	Pipe() : GameObject({ 0, 0, 0 }) {};
	Pipe(const Vector3D& aPosition, const Model& aModel) : myPipeModel(aModel), GameObject(aPosition) {};

	void Render(Shader& aShader);

private:
	Model myPipeModel;
};
#endif