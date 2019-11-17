#include "Gizmos.h"
#include "OpenGL.h"
#include "FlyObject.h"

Gizmos::Gizmos(FlyObject* _objectAttached)
{
	objectAttached = _objectAttached; 
	arrowLenght = 50.0f; 
	arrowWidth = 3.0f; 
}

Gizmos::~Gizmos()
{

}

void Gizmos::Draw()
{
	float3 objectCenter = objectAttached->transform->GetPosition(); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(arrowWidth); 


	// X Axis
	glBegin(GL_LINES);
	glColor3f(0, 0, 255); glVertex3f(objectCenter.x, objectCenter.y, 0.f);
	glColor3f(0, 0, 255); glVertex3f(objectCenter.x + arrowLenght, objectCenter.y, 0.f);
	glEnd();

	float3 triangleBase = float3(objectCenter.x + arrowLenght, 0, 0); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x, triangleBase.y - 10.0f, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x + 10.0f, 0.0f, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x, triangleBase.y + 10.0f, 0.0f);

	glEnd();

	// Y Axis
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_LINES);
	glColor3f(0, 255, 0); glVertex3f(objectCenter.x, objectCenter.y, 0.f);
	glColor3f(0, 255, 0); glVertex3f(objectCenter.x, objectCenter.y - arrowLenght, 0.f);
	glEnd();

	triangleBase = float3(0, objectCenter.y - arrowLenght, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glColor3f(0, 255, 0); glVertex3f(triangleBase.x - 10.0f, triangleBase.y, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(0.0f, triangleBase.y - 10.0f, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(triangleBase.x + 10.0f, triangleBase.y, 0.0f);
	glEnd();

	glLineWidth(1.0f); 
	glColor3f(255, 255, 255);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Gizmos::SetArrowLenght(float& lineLenght)
{
	arrowLenght = lineLenght; 
}

void Gizmos::SetArrowWidth(float& _arrowWidth)
{
	arrowWidth = _arrowWidth; 
}
