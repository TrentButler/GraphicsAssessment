#include "RenderApp.h"
#include <FPSCamera.h>
#include "Shader.h"
#include "Mesh.h"
#include <vec4.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>


RenderApp::RenderApp() {};
RenderApp::~RenderApp() {};

void RenderApp::genGrid(unsigned int rows, unsigned int cols)
{
	//Vertex* gridVerts = new Vertex[rows * cols];

	//for (unsigned int row = 0; row < rows; row++)
	//{
	//	for (unsigned int col = 0; col < cols; col++)
	//	{
	//		gridVerts[row * col + col].position = glm::vec4(float(col), 0, float(row), 1);
	//		gridVerts[row * col + col].color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//	}
	//}

	////RESEARCH THIS
	//unsigned int* gridIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	//unsigned int index = 0;
	//for (unsigned int row = 0; row < (rows - 1); row++)
	//{
	//	for (unsigned int col = 0; col < (cols - 1); col++)
	//	{
	//		gridIndices[index++] = row * cols + col;
	//		gridIndices[index++] = (row + 1) * cols + col;
	//		gridIndices[index++] = (row + 1) * cols + (col + 1);

	//		gridIndices[index++] = row * cols + col;
	//		gridIndices[index++] = (row + 1) * cols + (col + 1);
	//		gridIndices[index++] = row * cols + (col + 1);
	//	}
	//}
	

	//GIVE INFO TO THE VBO AND IBO	
	//glGenVertexArrays(1, &this->_vao); //GENRATE THE VERTEX ARRAY OBJECT BEFORE THE VBO AND IBO
	//
	//glGenBuffers(1, &this->_vbo); //GENERATE A VBO
	//glGenBuffers(1, &this->_ibo); //GENERATE A IBO
	//glBindVertexArray(this->_vao);

	//glBindBuffer(GL_ARRAY_BUFFER, this->_vbo); //BIND THE VBO
	//glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), gridVerts, GL_STATIC_DRAW); //SEND OPENGL THE VBO'S INFORMATION
	//
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ibo); //BIND THE IBO
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	//	(rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), gridIndices, GL_STATIC_DRAW); //SEND OPENGL THE IBO'S INFORMATION
	//
	//
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); //DEFINE THE SIZE OF THE VAO
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof( glm::vec4)); //DEFINE THE SIZE OF THE VAO

	//glBindVertexArray(0); //UNBIND THE VAO
	//glBindBuffer(GL_ARRAY_BUFFER, 0); //UNBIND THE VBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //UNBIND THE IBO
	//delete[] gridVerts;
	//delete[] gridIndices;

}

void RenderApp::startup()
{
	this->_camera = new FPSCamera();
	this->_camera->setLookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	this->_camera->setPerspective(3.14f / 4.0f, 16.0f / 9.0f, -0.1f, 1000.0f);

	/*this->_shader = new Shader();
	this->_shader->LoadShader("shader.vert", "shader.frag");
	this->_shader->attach();*/

	this->anotherShader = new Shader();
	this->anotherShader->LoadShader("triShader.vert", "triShader.frag");
	this->anotherShader->attach();

	Vertex a = { glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	Vertex b = { glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) };
	Vertex c = { glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) };

	std::vector<Vertex> triangle = { a,b,c };
	std::vector<unsigned int> triangleIndices = { 0, 1, 2 };

	this->_aObject = new Mesh();
	this->_aObject->initialize(triangle, triangleIndices);
	this->_aObject->create_buffers();

	//this->genGrid(10, 10);
}

void RenderApp::shutdown()
{
}

void RenderApp::update(float time)
{
}

void RenderApp::draw()
{
	this->anotherShader->bind();

	/*unsigned int projectionViewUniform =
		this->_shader->getUniform("ProjectionViewWorld");*/
	unsigned int projectionViewUniform =
		this->anotherShader->getUniform("wvProjection");	

	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(this->_camera->getProjectionView())); //SEND PROJECTIONVIEW TRANSFORM TO THE CURRENTLY ACTIVE SHADER PROGRAM
	
	this->_aObject->bind();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  
	glDrawElements(GL_TRIANGLES, this->_aObject->index_count, GL_UNSIGNED_INT, 0);
	
	this->_aObject->unbind();
	this->anotherShader->unbind();
}
