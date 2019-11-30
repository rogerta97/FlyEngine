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
#include "mmgr.h"

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

void BoundingBox::Draw(bool fill, float4 color)
{
	DrawSquare(color, fill);
}

void BoundingBox::CleanUp()
{
	objectAttached = nullptr; 
}

void BoundingBox::DrawSquare(float4 color, bool fill)
{
	if(fill) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
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

void BoundingBox::SetSize(float sizeX, float sizeY)
{
	size = float2(sizeX, sizeY);
	minPoint = objectAttached->transform->GetPosition(true) + float2(-sizeX, sizeY);
	maxPoint = objectAttached->transform->GetPosition(true) + float2(sizeX, -sizeY);
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

	size.x = abs(maxPoint.x - minPoint.x);
	size.y = abs(maxPoint.y - minPoint.y);
	size /= 2;
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
	size /= 2; 
}

void BoundingBox::CenterMinMaxPointsToScreen()
{
	minPoint = float2(-size.x, size.y);
	maxPoint = float2(size.x, -size.y);
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
	{
		ret = true;
	}

	if (mousePosGame.y > (minPoint.y) || mousePosGame.y < (maxPoint.y))
	{
		ret = false;
	}

	return ret;

}

void BoundingBox::HandleDrag()
{
	if (!isDragEnabled)
		return; 

	if (!isDragging && App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_DOWN)
	{
		isDragging = true;
		dragCenterOffset = App->moduleImGui->gameViewportDockPanel->GetMouseGamePos();
		dragCenterOffset -= (objectAttached->transform->GetPosition(false));
		dragCenterOffset = float2((int)dragCenterOffset.x, (int)dragCenterOffset.y);
	}

	if (isDragging)
	{
		float2 positionInDrag = float2(App->moduleImGui->gameViewportDockPanel->GetMouseGamePos().x, App->moduleImGui->gameViewportDockPanel->GetMouseGamePos().y);
		float2 positionInDragGame = float2(App->moduleImGui->gameViewportDockPanel->ScreenToWorld(positionInDrag.x, positionInDrag.y));
		positionInDragGame = float2((int)positionInDragGame.x, (int)positionInDragGame.y);

		minPoint += positionInDragGame;
		maxPoint += positionInDragGame;
	}
}

void BoundingBox::FitToObject()
{
	//minPoint += objectAttached->transform->GetPosition();
}

