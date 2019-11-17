#include "BoundingBox.h"
#include "OpenGL.h"
#include "FlyObject.h"

BoundingBox::BoundingBox(FlyObject* _objectAttached)
{
	size = float2(100.0f, 100.0f); 
	objectAttached = _objectAttached; 
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::Draw()
{
	float2 objectPosition = float2(objectAttached->transform->GetPosition().x, objectAttached->transform->GetPosition().y);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glLineWidth(2.0f);

	glBegin(GL_QUAD_STRIP);

	glVertex3f(objectPosition.x - size.x / 2, objectPosition.y - size.y / 2, 0.f);
	glVertex3f(objectPosition.x + size.x / 2, objectPosition.y - size.y / 2, 0.f);
	glVertex3f(objectPosition.x - size.x / 2, objectPosition.y + size.y / 2, 0.f);
	glVertex3f(objectPosition.x + size.x / 2, objectPosition.y + size.y / 2, 0.f);

	glEnd();

	//glLineWidth(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BoundingBox::FitToObject()
{
}

void BoundingBox::SetMinPoint(float newMinPointX, float newMinPointY)
{
	minPoint = float2(newMinPointX, newMinPointY);
}

float2& BoundingBox::GetMinPoint()
{
	return minPoint; 
}

void BoundingBox::SetMaxPoint(float newMaxPointX, float newMaxPointY)
{
	minPoint = float2(newMaxPointX, newMaxPointY);
}

float2& BoundingBox::GetMaxPoint()
{
	return maxPoint;
}

void BoundingBox::SetSize(float sizeX, float sizeY)
{
	size = float2(sizeX, sizeY);
}

float2& BoundingBox::GetSize()
{
	return size; 
}
