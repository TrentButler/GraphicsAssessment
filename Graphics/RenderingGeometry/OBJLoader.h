#pragma once
#include <vector>
struct Vertex;
class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	void loadOBJ(const char* fileName, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
};