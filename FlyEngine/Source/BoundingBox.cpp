#include "BoundingBox.h"
#include "OpenGL.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"
#include "SDL.h"
#include "FlyObject.h"
#include "ViewportManager.h"
#include "imgui.h"
#include "ModuleInput.h"
#include "Gizmos.h"
#include "mmgr.h"

BoundingBox::BoundingBox()
{
	size = float2(100.0f, 100.0f); 

	minPoint = float2(0, 0); 
	maxPoint = float2(0, 0); 

	displayGizmos = false; 
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::Draw(bool fill, float4 color)
{
	DrawSquare(color, fill);
}

void BoundingBox::CleanUp()
{

}

void BoundingBox::DrawSquare(float4 color, bool fill)
{
	if(fill) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glLineWidth(3.0f);
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

void BoundingBox::SetPosition(float2 newPositon)
{
	CenterMinMaxPointsToScreen(); 
	minPoint += newPositon; 
	maxPoint += newPositon; 
}

void BoundingBox::SetPositionInc(float2 newPositon)
{
	minPoint += newPositon;
	maxPoint += newPositon;
}

float2 BoundingBox::GetPosition()
{
	return GetCenter(); 
}

void BoundingBox::SetSize(float sizeX, float sizeY)
{
	size = float2(sizeX, sizeY);
	minPoint = float2(-sizeX / 2, sizeY / 2);
	maxPoint = float2(sizeX / 2, -sizeY / 2);
}

float2& BoundingBox::GetSize()
{
	return size; 
}

void BoundingBox::SetSquareColor(float4 newSquareColor)
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

	size.x = abs(maxPoint.x - minPoint.x);
	size.y = abs(maxPoint.y - minPoint.y);
}

float2& BoundingBox::GetMaxPoint()
{
	return maxPoint;
}

void BoundingBox::SetMaxPoint(float2 _maxPoint)
{
	maxPoint = _maxPoint; 

	size.x = abs(maxPoint.x - minPoint.x);
	size.y = abs(maxPoint.y - minPoint.y);
}

bool& BoundingBox::IsDragging()
{
	return isDragging; 
}

void BoundingBox::EnableDrag(bool enable)
{
	isDragEnabled = enable; 
}

void BoundingBox::CreateGizmos()
{
	boxGizmos = new Gizmos(this);

}

float2 BoundingBox::GetCenter()
{
	float2 relativeCenterPoint(size.x / 2, size.y / 2); 
	float2 worldCenterPoint(minPoint.x + relativeCenterPoint.x, maxPoint.y + relativeCenterPoint.y); 
	return worldCenterPoint;
}

void BoundingBox::CenterMinMaxPointsToScreen()
{
	minPoint = float2(-size.x / 2, size.y / 2);
	maxPoint = float2(size.x / 2, -size.y / 2);
}

bool BoundingBox::IsMouseOver()
{
	bool ret = false; 
	float2 mousePos = App->moduleImGui->gameViewportDockPanel->GetMouseRelativePosition();
	float2 mousePosGame = App->moduleImGui->gameViewportDockPanel->ScreenToWorld(mousePos.x, mousePos.y);

	mousePosGame.x *= ViewportManager::getInstance()->GetAspectRatio(); 
	mousePosGame.y *= ViewportManager::getInstance()->GetAspectRatio(); 

	// Calculate Final Position Values
	if (mousePosGame.x > (minPoint.x) && (mousePosGame.x < (maxPoint.x)))	
		ret = true;
	
	if (mousePosGame.y > (minPoint.y) || mousePosGame.y < (maxPoint.y))	
		ret = false;
	
	return ret;

}

bool BoundingBox::IsBoxClicked()
{
	if(App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_DOWN && IsMouseOver())
		return true;

	return false;
}

float2 BoundingBox::HandleDrag(CardinalAxis limitOnAxis)
{
	if (!isDragEnabled)
		return float2::zero; 

	if (!isDragging && App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_DOWN && IsMouseOver())
	{
		isDragging = true;	
		initDragPos = App->moduleImGui->gameViewportDockPanel->GetMouseGamePos(); 
		initDragMinPoint = minPoint; 
		initDragMaxPoint = maxPoint; 
	}

	if (isDragging)
	{	
		float2 dragIncrement = float2(initDragPos.x - App->moduleImGui->gameViewportDockPanel->GetMouseGamePos().x, initDragPos.y - App->moduleImGui->gameViewportDockPanel->GetMouseGamePos().y);

		switch (limitOnAxis)
		{
		case math::AxisX:
			minPoint = float2(initDragMinPoint.x - (dragIncrement.x * App->moduleImGui->gameViewportDockPanel->GetAspectRatio()), initDragMinPoint.y);
			maxPoint = float2(initDragMaxPoint.x - (dragIncrement.x * App->moduleImGui->gameViewportDockPanel->GetAspectRatio()), initDragMaxPoint.y);
			dragIncrement.y = 0; 
			break;

		case math::AxisY:
			minPoint = float2(initDragMinPoint.x, initDragMinPoint.y - (dragIncrement.y * App->moduleImGui->gameViewportDockPanel->GetAspectRatio()));
			maxPoint = float2(initDragMaxPoint.x, initDragMaxPoint.y - (dragIncrement.y * App->moduleImGui->gameViewportDockPanel->GetAspectRatio()));
			dragIncrement.x = 0; 
			break;

		case math::AxisNone:
			minPoint = initDragMinPoint - (dragIncrement * App->moduleImGui->gameViewportDockPanel->GetAspectRatio());
			maxPoint = initDragMaxPoint - (dragIncrement * App->moduleImGui->gameViewportDockPanel->GetAspectRatio());
			break;
		}

		if (App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_UP) {
			isDragging = false; 
		}

		return dragIncrement;
	}

	return float2::zero;
}

