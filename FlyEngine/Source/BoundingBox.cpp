#include "BoundingBox.h"
#include "OpenGL.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"
#include "SDL.h"
#include "FlyObject.h"
#include "imgui.h"
#include "ModuleInput.h"

BoundingBox::BoundingBox(FlyObject* _objectAttached)
{
	size = float2(100.0f, 100.0f); 
	objectAttached = _objectAttached; 

	minPoint = float2(0, 0); 
	maxPoint = float2(0, 0); 
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::Draw()
{
	float2 objectPosition = float2(0, 0);

	DrawSquare(float4(1.0f, 1.0f, 1.0f, 1.0f));

	if (showCornerDots)
	{
		DrawControlPoint(float2(minPoint.x, maxPoint.y), 4);
		DrawControlPoint(float2(maxPoint.x, maxPoint.y), 4);
		DrawControlPoint(float2(minPoint.x, minPoint.y), 4);
		DrawControlPoint(float2(maxPoint.x, minPoint.y), 4);
	}
}

void BoundingBox::DrawSquare(float4 color = float4(1.0f, 1.0f, 1.0f, 0.4f))
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f);

	glBegin(GL_QUAD_STRIP); 

	glColor4f(color.x, color.y, color.z, color.w); glVertex3f(minPoint.x, maxPoint.y, 0.f);
	glColor4f(color.x, color.y, color.z, color.w); glVertex3f(maxPoint.x, maxPoint.y, 0.f);
	glColor4f(color.x, color.y, color.z, color.w); glVertex3f(minPoint.x, minPoint.y, 0.f);
	glColor4f(color.x, color.y, color.z, color.w); glVertex3f(maxPoint.x, minPoint.y, 0.f);

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

void BoundingBox::SetSize(float sizeX, float sizeY)
{
	size = float2(sizeX, sizeY);
	minPoint = float2(-sizeX, sizeY);
	maxPoint = float2(sizeX, -sizeY);
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

float2& BoundingBox::GetMinPoint()
{
	return minPoint; 
}

void BoundingBox::SetMinPoint(float2 _minPoint)
{
	minPoint = _minPoint; 
}

float2& BoundingBox::GetMaxPoint()
{
	return maxPoint;
}

void BoundingBox::SetMaxPoint(float2 _maxPoint)
{
	maxPoint = _maxPoint; 
}

void BoundingBox::Move(float2 newPosition)
{

}

void BoundingBox::CenterMinMaxPoints()
{
	minPoint = float2(-size.x, size.y);
	maxPoint = float2(size.x, -size.y);
}

void BoundingBox::Rotate(float2 newRotationEuler)
{

}

void BoundingBox::Scale(float2 newScale)
{
	
}

bool BoundingBox::IsMouseOver()
{
	float2 mousePos = App->moduleImGui->gameViewportDockPanel->GetMouseRelativePosition();
	float2 mousePosGame = App->moduleImGui->gameViewportDockPanel->ScreenToWorld(mousePos.x, mousePos.y);

	float aspectRatio = App->moduleImGui->gameViewportDockPanel->GetAspectRatio();
	mousePosGame.x *= aspectRatio;	

	// Calculate Final Position Values
	if (mousePosGame.x > (minPoint.x) && (mousePosGame.x < (maxPoint.x) &&
		mousePosGame.y < (minPoint.y) && mousePosGame.y >(maxPoint.y)))
		return true; 

	return false; 
}

void BoundingBox::FitToObject()
{
	//minPoint += objectAttached->transform->GetPosition();
}

