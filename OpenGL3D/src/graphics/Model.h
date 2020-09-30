#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <string>
#include <vector>
#include "math/Triangle.h"

class Model
{
public:
	Model(const std::string& aFilePath);
	~Model();

	std::vector<Triangle> triangles;
};
#endif