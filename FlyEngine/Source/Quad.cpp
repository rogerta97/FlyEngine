#include "Quad.h"
#include "OpenGL.h"
#include "mmgr.h"
#include "Globals.h"


Quad::Quad()
{
	vertices = nullptr;
	indices = nullptr;
	numVertices = numIndices = 0;
	verticesID = indicesID = 0;
}

Quad::~Quad()
{
}

void Quad::Create(const float desiredWidth, const float desiredHeight, bool topLeftCenter)
{
	float imageAspectRatio = desiredWidth / desiredHeight; 

	float imageWidth = 200.0f; 
	float imageHeight = imageWidth / imageAspectRatio; 

	SetQuadData(imageWidth, imageHeight, topLeftCenter);
	LoadToMemory(); 
}

void Quad::CreateLiteralSize(const float desiredWidth, const float desiredHeight, bool topLeftCenter)
{
	float imageWidth = desiredWidth;
	float imageHeight = desiredHeight;

	SetQuadData(imageWidth, imageHeight, topLeftCenter);
	LoadToMemory();
}

void Quad::SetQuadData(const float desiredWidth, const float desiredHeight, bool topLeftCenter)
{
	flog("SetData"); 

	numVertices = 4;
	vertices = new vec3[numVertices];

	vec2 half_size(desiredWidth / 2, desiredHeight / 2);

	if (topLeftCenter)
	{
		vertices[0] = { 0, desiredHeight, 0 };
		vertices[1] = { desiredWidth, desiredHeight, 0 };
		vertices[2] = { 0, 0, 0 };
		vertices[3] = { desiredWidth, 0, 0 };
	}
	else
	{
		vertices[0] = { -half_size.x, half_size.y, 0 };
		vertices[1] = { half_size.x, half_size.y, 0 };
		vertices[2] = { -half_size.x, -half_size.y, 0 };
		vertices[3] = { half_size.x, -half_size.y, 0 };
	}

	numIndices = 6;
	indices = new int[numIndices];

	//Front
	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;

	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	quadWidth = desiredWidth;
	quadHeigth = desiredHeight;

	numUvs = numVertices;
	uvs = new float[numUvs * 3];

	//Front 
	uvs[0] = 0.0f;
	uvs[1] = 1.0f;
	uvs[2] = 0.0f;

	uvs[3] = 1.0f;
	uvs[4] = 1.0f;
	uvs[5] = 0.0f;

	uvs[6] = 0.0f;
	uvs[7] = 0.0f;
	uvs[8] = 0.0f;

	uvs[9] = 1.0f;
	uvs[10] = 0.0f;
	uvs[11] = 0.0f;
}

void Quad::LoadToMemory()
{
	if (numVertices != 0)
	{
		glGenBuffers(1, &verticesID);
		glBindBuffer(GL_ARRAY_BUFFER, verticesID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (numIndices != 0)
	{
		glGenBuffers(1, &indicesID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numIndices, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if (numUvs != 0)
	{
		glGenBuffers(1, &uvsID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uvsID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * numUvs * 3, uvs, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Quad::UnloadFromMemory()
{
	if (numVertices != 0)
	{
		glDeleteBuffers(1, &verticesID);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (numIndices != 0)
	{
		glDeleteBuffers(1, &indicesID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if (numUvs != 0)
	{
		glDeleteBuffers(1, &uvsID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Quad::CleanUp()
{
	UnloadFromMemory(); 
	delete[] vertices;
	delete[] indices; 
	delete[] uvs; 
}

void Quad::SetWidth(float newWidth)
{
	// Update Vertices with the new width
	CleanUp();
	quadWidth = newWidth;
	SetQuadData(quadWidth, quadHeigth);
}

void Quad::SetHeight(float newHeigth)
{
	// Update Vertices with the new width
	CleanUp();
	quadHeigth = newHeigth;
	SetQuadData(quadWidth, quadHeigth);
}

void Quad::SetSize(float newWidth, float newHeigth)
{
	CleanUp();

	quadHeigth = newHeigth;
	quadWidth= newWidth;
	
	SetQuadData(quadWidth, quadHeigth);

	// Addapt Quad 

}
