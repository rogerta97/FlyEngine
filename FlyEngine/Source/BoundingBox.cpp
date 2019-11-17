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
	glLineWidth(2.0f);

	float2 topLeft = float2(objectPosition.x - size.x, objectPosition.y - size.y);
	float2 topRight = float2(objectPosition.x + size.x, objectPosition.y - size.y);
	float2 bottomLeft = float2(objectPosition.x - size.x, objectPosition.y + size.y);
	float2 bottomRight = float2(objectPosition.x + size.x, objectPosition.y + size.y);

	DrawSquare(topLeft, topRight, bottomLeft, bottomRight);

	DrawControlPoint(topLeft, 4);
	DrawControlPoint(topRight, 4);
	DrawControlPoint(bottomLeft, 4);
	DrawControlPoint(bottomRight, 4);

	glLineWidth(1.0f);
}

void BoundingBox::DrawSquare(math::float2& topLeft, math::float2& topRight, math::float2& bottomLeft, math::float2& bottomRight)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUAD_STRIP); 

	glColor3f(255, 255, 255); glVertex3f(topLeft.x, topLeft.y, 0.f);
	glColor3f(255, 255, 255); glVertex3f(topRight.x, topRight.y, 0.f);
	glColor3f(255, 255, 255); glVertex3f(bottomLeft.x, bottomLeft.y, 0.f);
	glColor3f(255, 255, 255); glVertex3f(bottomRight.x, bottomRight.y, 0.f);

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BoundingBox::DrawControlPoint(float2 pointPos, float pointSize)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUAD_STRIP);
	
	glColor3f(255, 0, 255); glVertex3f(pointPos.x - pointSize, pointPos.y - pointSize, 0.f);
	glColor3f(255, 0, 255); glVertex3f(pointPos.x + pointSize, pointPos.y - pointSize, 0.f);
	glColor3f(255, 0, 255); glVertex3f(pointPos.x - pointSize, pointPos.y + pointSize, 0.f);
	glColor3f(255, 0, 255); glVertex3f(pointPos.x + pointSize, pointPos.y + pointSize, 0.f);

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BoundingBox::FitToObject()
{
}

void BoundingBox::SetSize(float sizeX, float sizeY)
{
	size = float2(sizeX, sizeY);
}

float2& BoundingBox::GetSize()
{
	return size; 
}
