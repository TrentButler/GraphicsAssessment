#include <gl_core_4_4.h>
#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <string>

Mesh::Mesh() {};
Mesh::~Mesh() {};


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

void Mesh::loadOBJ(const char* fileName)
{
	//CHANGE THE WORKING DIR TO "..//[bin]//objects"
	//PARSE FILE
	//	- IF LINE STARTS WITH 'v' ADD TO THE 'VERTICES' ARRAY
	//	- IF LINE STARTS WITH 'f' ADD TO THE 'INDICES' ARRAY

	char prevDir[MAX_PATH];
	const char* newDir = "..//[bin]//objects";
	GetCurrentDirectory(MAX_PATH, prevDir);
	SetCurrentDirectory(newDir);

	std::string line = "";
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec4> textures;

	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<unsigned int> textureIndices;

	std::fstream objStream;
	objStream.open(fileName);
	while (objStream.eof() == false)
	{
		std::getline(objStream, line);

		if (line[0] == 'v' && line[1] == ' ')
		{
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

			glm::vec4 vertex = glm::vec4(X, Y, Z, 1.0f);
			vertices.push_back(vertex);
		}

		if (line[0] == 'f')
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
			}
		}

		if (line[0] == 'v' && line[1] == 'n')
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

		if (line[0] == 'v' && line[1] == 't')
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
			
			glm::vec4 vertexTextureCoord = glm::vec4(X, Y, Z, 1.0f);
			textures.push_back(vertexTextureCoord);
		}
	}

	SetCurrentDirectory(prevDir);

	std::vector<Vertex> allVerts;
	for (int i = 0; i < vertices.size(); i++)
	{
		Vertex vert = { vertices[i], glm::vec4(0.4f, 0.4f, 0.4f, 0.4f)};
		allVerts.push_back(vert);
	}

	//DUMP ARRAYS HERE
	initialize(allVerts, vertexIndices);
}