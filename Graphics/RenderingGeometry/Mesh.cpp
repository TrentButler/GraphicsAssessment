#include <gl_core_4_4.h>
#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vec3.hpp>

Mesh::Mesh() {}
Mesh::~Mesh() {}

void Mesh::initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indices)
{
	for (auto v : verts)
	{
		m_vertices.push_back(v);
	}

	for (auto i : indices)
	{
		m_indices.push_back(i);
	}

	index_count = m_indices.size();
	vertex_count = m_vertices.size();

	create_buffers();
}

void Mesh::create_buffers()
{
	//GENERATE
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	//BIND
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	//GIVE INFO OF BUFFERS TO OPENGL
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)( 2 * sizeof(glm::vec4)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + (2 * sizeof(glm::vec4))));

	//CLEAN UP
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Mesh::bind()
{
	glBindVertexArray(m_vao);
}

void Mesh::unbind()
{
	glBindVertexArray(0);
}

void Mesh::loadOBJ(const char* path, const char* fileName)
{
	//CHANGE THE WORKING DIR TO "..//[bin]//objects"
	//PARSE FILE
	//	- IF LINE STARTS WITH 'v' ADD TO THE 'VERTICES' ARRAY
	//	- IF LINE STARTS WITH 'f' ADD TO THE 'INDICES' ARRAY

	char prevDir[MAX_PATH];
	const char* newDir = path;
	GetCurrentDirectory(MAX_PATH, prevDir);
	SetCurrentDirectory(newDir);

	std::string line = "";
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec3> textures;

	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<unsigned int> textureIndices;

	std::fstream objStream;
	objStream.open(fileName);
	while (objStream.eof() == false)
	{
		std::getline(objStream, line);

		if (line[0] == 'v' && line[1] == ' ') //VERTEX
		{
			int value = 0;
			std::string x = "";
			std::string y = "";
			std::string z = "";
			glm::vec4 vert = glm::vec4(0);
			for (int i = 2; i < line.size(); i++)
			{
				if (line[i] == ' ')
				{
					if (x == "") // CHECK IF THE (x) VALUE IS POPULATED, IF NOT CONTINUE
					{
						continue;
					}
					//NEXT VALUE
					value++;
				}

				switch (value)
				{
				case 0:
				{
					if (line[i] == ' ')
					{
						break;
					}
					x += line[i];
					break;
				}
				case 1:
				{
					if (line[i] == ' ')
					{
						break;
					}
					y += line[i];
					break;
				}
				case 2:
				{
					if (line[i] == ' ')
					{
						break;
					}
					z += line[i];
					break;
				}
				}
			}

			float X, Y, Z;
			if (x != "")
			{
				X = std::stof(x);
			}
			if (y != "")
			{
				Y = std::stof(y);
			}
			if (z != "")
			{
				Z = std::stof(z);
			}

			glm::vec4 vertex = glm::vec4(X, Y, Z, 1.0f);
			vertices.push_back(vertex);
		}

		if (line[0] == 'f') //FACE ELEMENT
		{
			int value = 0;
			int count = 0;
			std::string indexString = "";
			std::string textureString = "";
			std::string normalString = "";
			for (int i = 2; i < line.size(); i++)
			{
				if (count == 2)
				{
					value = 2; //POPULATE ONLY THE NORMAL INDEX ARRAY
				}

				if (line[i] == ' ')
				{
					//DUMP ALL VALUES TO ARRAYS
					if (indexString != "")
					{
						auto vertIndex = std::stoul(indexString) - 1;
						vertexIndices.push_back(vertIndex);
					}
					if (textureString != "")
					{
						auto textureCoordIndex = std::stoul(textureString) - 1;
						textureIndices.push_back(textureCoordIndex);
					}
					if (normalString != "")
					{
						auto normalIndex = std::stoul(normalString) - 1;
						normalIndices.push_back(normalIndex);
					}

					//RESET ALL VALUES
					indexString = "";
					textureString = "";
					normalString = "";
					value = 0;
					count = 0;
				}

				if (line[i] == '/')
				{
					//NEXT VALUE
					value++;
					count++; // 'f 1//2 2//3 4//5'
				}

				switch (value)
				{
				case 0:
				{
					if (line[i] == '/' || line[i] == ' ')
					{
						break;
					}
					indexString += line[i];					
					count = 0;
					break;
				}
				case 1:
				{
					if (line[i] == '/' || line[i] == ' ')
					{
						break;
					}
					textureString += line[i];
					count = 0;
					break;
				}
				case 2:
				{
					if (line[i] == '/' || line[i] == ' ')
					{
						break;
					}
					normalString += line[i];
					count = 0;
					break;
				}
				}

				if (i == line.size() - 1) // CONVERT THE FINAL VALUES AT THE END OF THE LOOP
				{
					//DUMP ALL VALUES TO ARRAYS
					if (indexString != "")
					{
						auto vertIndex = std::stoul(indexString) - 1;
						vertexIndices.push_back(vertIndex);
					}
					if (textureString != "")
					{
						auto textureCoordIndex = std::stoul(textureString) - 1;
						textureIndices.push_back(textureCoordIndex);
					}
					if (normalString != "")
					{
						auto normalIndex = std::stoul(normalString) - 1;
						normalIndices.push_back(normalIndex);
					}

					//RESET ALL VALUES
					indexString = "";
					textureString = "";
					normalString = "";
					value = 0;
					count = 0;
				}
			}
		}

		if (line[0] == 'v' && line[1] == 'n') //VERTEX NORMALS
		{
			//GET THE VERTEX NORMALS
			int value = 0;
			std::string x = "";
			std::string y = "";
			std::string z = "";
			glm::vec4 vert = glm::vec4(0);
			for (int i = 3; i < line.size(); i++)
			{
				if (line[i] == ' ')
				{
					//NEXT VALUE
					value++;
				}

				switch (value)
				{
				case 0:
				{
					if (line[i] == ' ')
					{
						break;
					}
					x += line[i];
					break;
				}

				case 1:
				{
					if (line[i] == ' ')
					{
						break;
					}
					y += line[i];
					break;
				}

				case 2:
				{
					if (line[i] == ' ')
					{
						break;
					}
					z += line[i];
					break;
				}
				}
			}

			float X, Y, Z;
			if (x != "")
			{
				X = std::stof(x);
			}
			if (y != "")
			{
				Y = std::stof(y);
			}
			if (z != "")
			{
				Z = std::stof(z);
			}

			glm::vec4 vertexNormal = glm::vec4(X, Y, Z, 1.0f);
			normals.push_back(vertexNormal);
		}

		if (line[0] == 'v' && line[1] == 't') //VERTEX TEXTURE COORDINATES
		{
			//GET THE VERTEX TEXTURE COORDINATES
			int value = 0;
			std::string x = "";
			std::string y = "";
			std::string z = "";
			glm::vec4 vert = glm::vec4(0);
			for (int i = 3; i < line.size(); i++)
			{
				if (line[i] == ' ')
				{
					//NEXT VALUE
					value++;
				}

				switch (value)
				{
				case 0:
				{
					if (line[i] == ' ')
					{
						break;
					}
					x += line[i];
					break;
				}

				case 1:
				{
					if (line[i] == ' ')
					{
						break;
					}
					y += line[i];
					break;
				}

				case 2:
				{
					if (line[i] == ' ')
					{
						break;
					}
					z += line[i];
					break;
				}
				}
			}			
			
			float X, Y, Z;
			if (x != "")
			{
				X = std::stof(x);
			}			
			if (y != "")
			{
				Y = std::stof(y);
			}			
			if (z != "")
			{
				Z = std::stof(z);
			}
			
			glm::vec3 vertexTextureCoord = glm::vec3(X, Y, Z);
			textures.push_back(vertexTextureCoord);
		}
	}

	SetCurrentDirectory(prevDir);

	int vertCount = vertices.size();
	int normCount = normals.size();
	int texCount = textures.size();

	std::vector<Vertex> allVerts;
	for (int i = 0; i < vertices.size(); i++)
	{
		glm::vec4 vert;
		glm::vec4 normal;
		glm::vec3 texture;

		if (i < vertCount && vertices.size() >= 1)
		{
			vert = vertices[i];
		}
		if (i < normCount && normals.size() >= 1)
		{
			normal = normals[i];
		}
		if (i < texCount && textures.size() >= 1)
		{
			texture = textures[i];
		}

		Vertex v = { vert, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), normal, texture };
		allVerts.push_back(v);
	}

	//DUMP ARRAYS HERE
	initialize(allVerts, vertexIndices);
}