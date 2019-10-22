#ifndef _QUAD_H_
#define _QUAD_H_

class Quad
{
public: 
	Quad(); 
	~Quad(); 

	void Create(const float desiredWidth, const float desiredHeight); 

	void LoadToMemory(); 
	void UnloadFromMemory(); 

private:
	void SetQuadData(const float desiredWidth, const float desiredHeight);

private: 

	float3* vertices; 
	int* indices;
	//float* uvs; 

	unsigned int numVertices = 0;
	unsigned int numIndices = 0;
	//uint numUvs = 0;

	unsigned int verticesID = -1;
	unsigned int indicesID = -1;
	//uint uvsID = -1;
};

#endif
