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

	float2 topLeft = float2(objectPosition.x - size.x, objectPosition.y - size.y);
	float2 topRight = float2(objectPosition.x + size.x, objectPosition.y - size.y);
	float2 bottomLeft = float2(objectPosition.x - size.x, objectPosition.y + size.y);
	float2 bottomRight = float2(objectPosition.x + size.x, objectPosition.y + size.y);

	DrawSquare(topLeft, topRight, bottomLeft, bottomRight, float4(1.0f, 1.0f, 1.0f, 1.0f));

	if (showCornerDots)
	{
		DrawControlPoint(topLeft, 4);
		DrawControlPoint(topRight, 4);
		DrawControlPoint(bottomLeft, 4);
		DrawControlPoint(bottomRight, 4);
	}
}

void BoundingBox::DrawSquare(math::float2& topLeft, math::float2& topRight, math::float2& bottomLeft, math::float2& bottomRight, float4 color = float4(1.0f, 1.0f, 1.0f, 0.4f))
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f);
	glBegin(GL_QUAD_STRIP); 

	glColor4f(color.x, color.y, color.z, color.w); glVertex3f(topLeft.x, topLeft.y, 0.f);
	glColor4f(color.x, color.y, color.z, color.w); glVertex3f(topRight.x, topRight.y, 0.f);
	glColor4f(color.x, color.y, color.z, color.w); glVertex3f(bottomLeft.x, bottomLeft.y, 0.f);
	glColor4f(color.x, color.y, color.z, color.w); glVertex3f(bottomRight.x, bottomRight.y, 0.f);

	glEnd();
	glLineWidth(1.0f);
	glColor4f(1,1,1,1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BoundingBox::DrawControlPoint(float2 pointPos, float pointSize)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUAD_STRIP);
	
	glColor3f(1.0f, 0, 1.0f); glVertex3f(pointPos.x - pointSize, pointPos.y - pointSize, 0.f);
	glColor3f(1.0f, 0, 1.0f); glVertex3f(pointPos.x + pointSize, pointPos.y - pointSize, 0.f);
	glColor3f(1.0f, 0, 1.0f); glVertex3f(pointPos.x - pointSize, pointPos.y + pointSize, 0.f);
	glColor3f(1.0f, 0, 1.0f); glVertex3f(pointPos.x + pointSize, pointPos.y + pointSize, 0.f);

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

void BoundingBox::SetSquareColor(float4& newSquareColor)
{
	squareColor = newSquareColor;
}

float4& BoundingBox::GetsquareColor()
{
	return squareColor; 
}

void BoundingBox::ShowCornerDots(bool show)
{
	showCornerDots = show; 
}
