#include "Gizmos.h"
#include "OpenGL.h"
#include "FlyObject.h"
#include "ViewportManager.h"
#include "Application.h"
#include "DisplayImageAction.h"
#include "ScalarBoundingBox.h"
#include "Quad.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
#include "BoundingBox.h"
#include "ModuleWorldManager.h"
#include "Room.h"
#include "GameViewportDockPanel.h"
#include "Transform.h"
#include "mmgr.h"

Gizmos::Gizmos(FlyObject* _objectAttached)
{
	objectAttached = _objectAttached; 

	selectGizmo = new SelectGizmo();
	moveGizmo = new MoveGizmo();
	scaleGizmo = new ScaleGizmo();

	selectGizmo->objectBorderBox->SetSquareColor(float4(0.4f, 0.4f, 1.0f, 0.2f));
	SetMoveGizmoStyle(7.0f, 100.0f, 5.0f, 20, 20, 25);
	SetScaleGizmoStyle(7.0f, 90.0f, 5.0f, 20, 23, 25); 
}

Gizmos::Gizmos(BoundingBox* boxAttached)
{
	this->boxAttached = boxAttached;

	selectGizmo = new SelectGizmo();
	moveGizmo = new MoveGizmo();
	scaleGizmo = new ScaleGizmo();

	selectGizmo->objectBorderBox->SetSquareColor(float4(0.4f, 0.4f, 1.0f, 0.2f));
	SetMoveGizmoStyle(7.0f, 100.0f, 5.0f, 20, 20, 25);
	SetScaleGizmoStyle(7.0f, 90.0f, 5.0f, 20, 23, 25);
}

Gizmos::~Gizmos()
{

}

void Gizmos::CleanUp()
{
	selectGizmo->CleanUp();
	delete selectGizmo;

	moveGizmo->CleanUp();
	delete moveGizmo; 

	scaleGizmo->CleanUp();
	delete scaleGizmo;
}

bool Gizmos::Update()
{
	bool ret = false; 
	if (!App->moduleImGui->gameViewportDockPanel->IsMouseInViewport() || !ViewportManager::getInstance()->drawGizmos)
		return false;

	GizmoMode gizmoMode = App->moduleImGui->gameViewportDockPanel->GetGizmoMode();
	if (gizmoMode != GIZMO_null)
	{
		switch (gizmoMode)
		{
		case GIZMO_MOVE:
			ret = HandleMoveGizmo();
			break;

		case GIZMO_SCALE:
			HandleScaleGizmo();
			break;

		case GIZMO_null:
			break;
		}
	}

	return ret; 
}

void Gizmos::Draw()
{
	if (!ViewportManager::getInstance()->drawGizmos)
		return;

	switch (App->moduleImGui->gameViewportDockPanel->GetGizmoMode())
	{
	case GIZMO_SELECT:
		DrawSelectGizmo();
		break;

	case GIZMO_MOVE:
		DrawMoveGizmo();
		break;

	case GIZMO_SCALE:
		DrawScaleGizmo();
		break;
	}
}

bool Gizmos::HandleScaleGizmo()
{
	// X axis -----
	if (scaleGizmo->axisXBox->IsBoxClicked())
	{
		scaleGizmo->initDragEndBoxXPos = scaleGizmo->endAxisBoxXPos;
	}

	float2 scaleIncX = scaleGizmo->axisXBox->HandleDrag(math::CardinalAxis::AxisX);
	if (scaleGizmo->axisXBox->IsDragging())
	{
		float2 nextBoxPos = scaleGizmo->initDragEndBoxXPos - (scaleIncX * App->moduleImGui->gameViewportDockPanel->GetAspectRatio());
		float2 increment = nextBoxPos - scaleGizmo->endAxisBoxXPos;
		scaleGizmo->endAxisBoxXPos = nextBoxPos;
		scaleGizmo->lineLengthX += increment.x;

		objectAttached->transform->SetScale(float2(objectAttached->transform->GetScale().x + increment.x * 0.01f, objectAttached->transform->GetScale().y));
	}

	// Y axis -----
	if (scaleGizmo->axisYBox->IsBoxClicked())
	{
		scaleGizmo->initDragEndBoxYPos = scaleGizmo->endAxisBoxYPos;
	}

	float2 scaleIncY = scaleGizmo->axisYBox->HandleDrag(math::CardinalAxis::AxisY);
	if (scaleGizmo->axisYBox->IsDragging())
	{
		float2 nextBoxPos = scaleGizmo->initDragEndBoxYPos - (scaleIncY * App->moduleImGui->gameViewportDockPanel->GetAspectRatio());
		float2 increment = nextBoxPos - scaleGizmo->endAxisBoxYPos;
		scaleGizmo->endAxisBoxYPos = nextBoxPos;
		scaleGizmo->lineLengthY -= increment.y;
		objectAttached->transform->SetScale(float2(objectAttached->transform->GetScale().x, objectAttached->transform->GetScale().y - increment.y * 0.01f));
		
	}

	//XY Box 
	static float2 prevScaleXY = float2::zero; 
	if (scaleGizmo->axisXYBox->IsBoxClicked())
	{
		scaleGizmo->initDragEndBoxXYPos = scaleGizmo->xySquarePos;
		prevScaleXY = float2(0,0); 
	}

	float2 scaleIncXY = scaleGizmo->axisXYBox->HandleDrag(math::CardinalAxis::AxisNone);
	if (scaleGizmo->axisXYBox->IsDragging())
	{
		float2 nextBoxPos = scaleGizmo->initDragEndBoxXYPos - (scaleIncXY * App->moduleImGui->gameViewportDockPanel->GetAspectRatio());

		float2 increment = scaleIncXY - prevScaleXY; 
		prevScaleXY = scaleIncXY; 

		float incrementLength = 0;
		if(increment.x < 0 && increment.y > 0)
			incrementLength = increment.Length() * 0.5f;
		else if (increment.x > 0 && increment.y < 0)
			incrementLength = -increment.Length() * 0.5f;

		scaleGizmo->lineLengthX += incrementLength; 
		scaleGizmo->lineLengthY += incrementLength; 
		scaleGizmo->endAxisBoxXPos.x += incrementLength;
		scaleGizmo->endAxisBoxYPos.y -= incrementLength;
	
		objectAttached->transform->SetScale(float2(objectAttached->transform->GetScale().x + incrementLength * 0.01f, objectAttached->transform->GetScale().y + incrementLength * 0.01f));

	}

	// Mouse Up -----
	if (App->moduleInput->GetMouseButton(MOUSE_BUTTON::LEFT_CLICK) == KEY_UP)
	{
		scaleGizmo->lineLengthX = scaleGizmo->lineLength; 
		scaleGizmo->endAxisBoxXPos = scaleGizmo->initDragEndBoxXPos; 

		scaleGizmo->lineLengthY = scaleGizmo->lineLength;
		scaleGizmo->endAxisBoxYPos = scaleGizmo->initDragEndBoxYPos;

		// Get AxisXY AABB back to center of the object
		scaleGizmo->axisXYBox->SetPosition(scaleGizmo->initDragEndBoxXYPos);
		scaleGizmo->axisXBox->CenterMinMaxPointsToScreen();
		scaleGizmo->axisYBox->CenterMinMaxPointsToScreen();

		objectAttached->FitObjectUtils();
	}

	return false; 
}

bool Gizmos::HandleMoveGizmo()
{
	// Get drag movement 
	float dragMovementIncX = moveGizmo->axisXBox->HandleDrag(CardinalAxis::AxisX).x;
	float dragMovementIncY = moveGizmo->axisYBox->HandleDrag(CardinalAxis::AxisY).y;
	float2 dragMovementIncXY = moveGizmo->axisXYBox->HandleDrag(CardinalAxis::AxisNone);

	static float prevPosX; 
	static float prevPosY; 
	static float2 prevPos; 

	float aspectRatio = App->moduleImGui->gameViewportDockPanel->GetAspectRatio();
	float2 inc = float2::zero; 

	if (App->moduleInput->GetMouseButton(RI_MOUSE_LEFT_BUTTON_DOWN) == KEY_DOWN && moveGizmo->axisXBox->IsMouseOver())
		prevPosX = dragMovementIncX;

	if (App->moduleInput->GetMouseButton(RI_MOUSE_LEFT_BUTTON_DOWN) == KEY_DOWN && moveGizmo->axisYBox->IsMouseOver())
		prevPosY = dragMovementIncY;

	if (App->moduleInput->GetMouseButton(RI_MOUSE_LEFT_BUTTON_DOWN) == KEY_DOWN && moveGizmo->axisXYBox->IsMouseOver())
		prevPos = dragMovementIncXY;
	
	HandleDrag(dragMovementIncX, inc, prevPosX, aspectRatio, dragMovementIncY, prevPosY, dragMovementIncXY, prevPos);

	if(dragMovementIncX != 0 || dragMovementIncY != 0)
		return true; 	

	return false; 
}

void Gizmos::HandleDrag(float dragMovementIncX, math::float2& inc, float& prevPosX, float aspectRatio, float dragMovementIncY, float& prevPosY, math::float2& dragMovementIncXY, math::float2& prevPos)
{
	// Set Object's new position from drag
	if (dragMovementIncX != 0)
	{
		//prevPosX = dragMovementIncX;
		inc.x = dragMovementIncX - prevPosX;
		prevPosX = dragMovementIncX;

		if (objectAttached != nullptr)
		{
			objectAttached->transform->SetPosition(objectAttached->transform->GetPosition().x - inc.x, objectAttached->transform->GetPosition().y);

			// Fit other Boxes
			moveGizmo->axisYBox->SetPosition(float2(objectAttached->transform->GetPosition().x * aspectRatio, moveGizmo->axisYBox->GetCenter().y));
			moveGizmo->axisXYBox->SetPosition(float2((objectAttached->transform->GetPosition().x * aspectRatio) + moveGizmo->xySquarePos.x, moveGizmo->axisXYBox->GetCenter().y));

			objectAttached->FitObjectUtils(); 
		}
		else if (boxAttached != nullptr)
		{
			boxAttached->SetPosition(float2(boxAttached->GetPosition().x - inc.x, boxAttached->GetPosition().y));

			// Fit other Boxes
			moveGizmo->axisYBox->SetPosition(float2(boxAttached->GetPosition().x, moveGizmo->axisYBox->GetCenter().y));
			moveGizmo->axisXYBox->SetPosition(float2((boxAttached->GetPosition().x ) + moveGizmo->xySquarePos.x, moveGizmo->axisXYBox->GetCenter().y));

			boxAttached->GetBoxGizmos()->CalculateMoveGizmo(boxAttached); 
		}
	}

	if (dragMovementIncY != 0)
	{
		inc.y = dragMovementIncY - prevPosY;
		prevPosY = dragMovementIncY;

		if (objectAttached != nullptr)
		{
			objectAttached->transform->SetPosition(objectAttached->transform->GetPosition().x * aspectRatio, objectAttached->transform->GetPosition().y * aspectRatio - inc.y);

			// Fit other Boxes
			moveGizmo->axisXBox->SetPosition(float2(moveGizmo->axisXBox->GetCenter().x, objectAttached->transform->GetPosition().y * aspectRatio));
			moveGizmo->axisXYBox->SetPosition(float2(moveGizmo->axisXYBox->GetCenter().x, (objectAttached->transform->GetPosition().y * aspectRatio) + moveGizmo->xySquarePos.y));

			objectAttached->FitObjectUtils();
		}
		else if (boxAttached != nullptr)
		{
			boxAttached->SetPosition(float2(boxAttached->GetPosition().x, boxAttached->GetPosition().y - inc.y));

			// Fit other Boxes
			moveGizmo->axisXBox->SetPosition(float2(moveGizmo->axisXBox->GetCenter().x, boxAttached->GetPosition().y ));
			moveGizmo->axisXYBox->SetPosition(float2(moveGizmo->axisXYBox->GetCenter().x, (boxAttached->GetPosition().y ) + moveGizmo->xySquarePos.y));

			boxAttached->GetBoxGizmos()->CalculateMoveGizmo(boxAttached);
		}
	}

	if (!dragMovementIncXY.Equals(float2(0, 0)))
	{
		float2 inc = dragMovementIncXY - prevPos;
		prevPos = dragMovementIncXY;

		if (objectAttached != nullptr)
		{
			objectAttached->transform->SetPosition(objectAttached->transform->GetPosition().x - inc.x, objectAttached->transform->GetPosition().y - inc.y);

			// Fit other Boxes
			moveGizmo->axisXBox->SetPosition(float2(moveGizmo->axisXBox->GetPosition().x - inc.x * aspectRatio, objectAttached->transform->GetPosition().y * aspectRatio));
			moveGizmo->axisYBox->SetPosition(float2(objectAttached->transform->GetPosition().x * aspectRatio, moveGizmo->axisYBox->GetCenter().y - inc.y * aspectRatio));

			objectAttached->FitObjectUtils();
		}
		else if (boxAttached != nullptr)
		{
			boxAttached->SetPosition(float2(boxAttached->GetPosition().x - inc.x, boxAttached->GetPosition().y - inc.y));

			// Fit other Boxes
			moveGizmo->axisXBox->SetPosition(float2(moveGizmo->axisXBox->GetPosition().x - inc.x , boxAttached->GetPosition().y));
			moveGizmo->axisYBox->SetPosition(float2(boxAttached->GetPosition().x , moveGizmo->axisYBox->GetCenter().y - inc.y ));

			boxAttached->GetBoxGizmos()->CalculateMoveGizmo(boxAttached);
		}
	}
}

bool Gizmos::IsMouseOver()
{
	return selectGizmo->objectBorderBox->IsMouseOver();
}

bool Gizmos::IsMouseOverClicked()
{
	return selectGizmo->objectBorderBox->IsBoxClicked();
}

void Gizmos::DrawSelectGizmo()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	selectGizmo->objectBorderBox->Draw(false, float4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Gizmos::CalculateSelectGizmo(FlyObject* objectAttached)
{			
	FitSelectBoxSize(); 
	selectGizmo->AddaptSelectBox(objectAttached);
}

void Gizmos::CalculateMoveGizmo(FlyObject* objectAttached)
{
	if(objectAttached != nullptr)
		moveGizmo->AddaptAxisBoxes(objectAttached);
}

void Gizmos::CalculateMoveGizmo(BoundingBox* boxattached)
{
	if (boxattached != nullptr)
		moveGizmo->AddaptAxisBoxes(boxattached);
}

void Gizmos::CalculateScaleGizmo(FlyObject* objectAttached)
{
	FitScaleBoxSize();
	scaleGizmo->AddaptScaleBox(objectAttached);
	scaleGizmo->AddaptAxisBoxes(objectAttached); 
}

void Gizmos::FitSelectBoxSize()
{
	if (objectAttached != nullptr) 
	{
		if (objectAttached->GetAction(ACTION_DISPLAY_IMAGE) != nullptr)
		{
			float2 objectDimensions = objectAttached->GetObjectVisualDimensions(); 
			selectGizmo->objectBorderBox->SetSize(objectDimensions.x, objectDimensions.y); 
		}
	}
}

void Gizmos::FitScaleBoxSize()
{
	if (objectAttached != nullptr)
	{
		if (objectAttached->GetAction(ACTION_DISPLAY_IMAGE) != nullptr)
		{
			float2 objectDimensions = objectAttached->GetObjectVisualDimensions();
			scaleGizmo->borderBoundingBox->SetSize(objectDimensions.x, objectDimensions.y);
		}
	}
}

void Gizmos::DrawMoveGizmo()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float4x4 moveGizmoViewMat = float4x4::identity;
	moveGizmoViewMat.RotateX(0);
	moveGizmoViewMat.RotateY(0);

	float2 objectPosition = float2(0, 0); 
	if (objectAttached != nullptr)
	{
		objectPosition = objectAttached->transform->GetPosition();
		objectPosition *= ViewportManager::getInstance()->GetAspectRatio(); 
	}
	else if (boxAttached != nullptr)
		objectPosition = boxAttached->GetCenter();

	moveGizmoViewMat.SetTranslatePart(float3(objectPosition.x, objectPosition.y, 0));
	glLoadMatrixf((GLfloat*)moveGizmoViewMat.Transposed().v);

	glLineWidth(moveGizmo->lineWidth);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// X Axis
	glBegin(GL_LINES);
	glColor3f(0, 0, 255); glVertex3f(0, 0, 0.f);
	glColor3f(0, 0, 255); glVertex3f(moveGizmo->lineLength, 0, 0.f);
	glEnd();

	float3 triangleBase = float3(moveGizmo->lineLength - moveGizmo->arrowLength, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x, triangleBase.y - moveGizmo->arrowWidth / 2, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x + moveGizmo->arrowLength + 5, 0.0f, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x, triangleBase.y + moveGizmo->arrowWidth / 2, 0.0f);
	glEnd();

	// Y Axis
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_LINES);
	glColor3f(0, 255, 0); glVertex3f(0, 0, 0.f);
	glColor3f(0, 255, 0); glVertex3f(0,-moveGizmo->lineLength, 0.f);
	glEnd();

	triangleBase = float3(0, 0 - moveGizmo->lineLength + moveGizmo->arrowLength, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glColor3f(0, 255, 0); glVertex3f(triangleBase.x - moveGizmo->arrowWidth / 2, triangleBase.y, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(0.0f, triangleBase.y - moveGizmo->arrowLength - 5, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(triangleBase.x + moveGizmo->arrowWidth / 2, triangleBase.y, 0.0f);
	glEnd();

	// Yellow XY Square
	glBegin(GL_QUAD_STRIP);
	glColor3f(255, 255, 0); glVertex3f(moveGizmo->xySquarePos.x -moveGizmo->xySquareSize / 2, moveGizmo->xySquarePos.y -moveGizmo->xySquareSize / 2, 0.f);
	glColor3f(255, 255, 0); glVertex3f(moveGizmo->xySquarePos.x + moveGizmo->xySquareSize / 2, moveGizmo->xySquarePos.y  -moveGizmo->xySquareSize / 2, 0.f);
	glColor3f(255, 255, 0); glVertex3f(moveGizmo->xySquarePos.x -moveGizmo->xySquareSize / 2, moveGizmo->xySquarePos.y + moveGizmo->xySquareSize / 2, 0.f);
	glColor3f(255, 255, 0); glVertex3f(moveGizmo->xySquarePos.x + moveGizmo->xySquareSize / 2, moveGizmo->xySquarePos.y + moveGizmo->xySquareSize / 2, 0.f);
	glEnd();

	// Center Square
	glBegin(GL_QUAD_STRIP);
	glColor3f(255, 0, 0); glVertex3f(-moveGizmo->centerSquareSize, -moveGizmo->centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(moveGizmo->centerSquareSize, -moveGizmo->centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(-moveGizmo->centerSquareSize, moveGizmo->centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(moveGizmo->centerSquareSize, moveGizmo->centerSquareSize, 0.f);
	glEnd();

	glLineWidth(1.0f);
	glColor3f(255, 255, 255);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Gizmos::DrawScaleGizmo()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float4x4 moveGizmoViewMat = float4x4::identity;
	moveGizmoViewMat.RotateX(0);
	moveGizmoViewMat.RotateY(0);
	float2 objectPosition = objectAttached->transform->GetPosition();
	moveGizmoViewMat.SetTranslatePart(float3(objectPosition.x * ViewportManager::getInstance()->GetAspectRatio(), objectPosition.y * ViewportManager::getInstance()->GetAspectRatio(), 0));
	glLoadMatrixf((GLfloat*)moveGizmoViewMat.Transposed().v);

	glLineWidth(5.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Y Axis
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_LINES);
	glColor3f(0, 255, 0); glVertex3f(0, 0, 0.f);
	glColor3f(0, 255, 0); glVertex3f(0, -scaleGizmo->lineLengthY, 0.f);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glColor3f(0, 255, 0); glVertex3f(scaleGizmo->endAxisBoxYPos.x - scaleGizmo->lineSquareSize / 2, scaleGizmo->endAxisBoxYPos.y - scaleGizmo->lineSquareSize / 2, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(scaleGizmo->endAxisBoxYPos.x + scaleGizmo->lineSquareSize / 2, scaleGizmo->endAxisBoxYPos.y - scaleGizmo->lineSquareSize / 2, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(scaleGizmo->endAxisBoxYPos.x + scaleGizmo->lineSquareSize / 2, scaleGizmo->endAxisBoxYPos.y + scaleGizmo->lineSquareSize / 2, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(scaleGizmo->endAxisBoxYPos.x - scaleGizmo->lineSquareSize / 2, scaleGizmo->endAxisBoxYPos.y + scaleGizmo->lineSquareSize / 2, 0.0f);
	glEnd();

	// X Axis
	glBegin(GL_LINES);
	glColor3f(0, 0, 255); glVertex3f(0, 0, 0.f);
	glColor3f(0, 0, 255); glVertex3f(scaleGizmo->lineLengthX, 0, 0.f);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glColor3f(0, 0, 255); glVertex3f(scaleGizmo->endAxisBoxXPos.x - scaleGizmo->lineSquareSize / 2, scaleGizmo->endAxisBoxXPos.y - scaleGizmo->lineSquareSize / 2, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(scaleGizmo->endAxisBoxXPos.x + scaleGizmo->lineSquareSize / 2, scaleGizmo->endAxisBoxXPos.y - scaleGizmo->lineSquareSize / 2, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(scaleGizmo->endAxisBoxXPos.x + scaleGizmo->lineSquareSize / 2, scaleGizmo->endAxisBoxXPos.y + scaleGizmo->lineSquareSize / 2, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(scaleGizmo->endAxisBoxXPos.x - scaleGizmo->lineSquareSize / 2, scaleGizmo->endAxisBoxXPos.y + scaleGizmo->lineSquareSize / 2, 0.0f);
	glEnd();

	// Center Square
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUAD_STRIP);
	glColor3f(255, 0, 0); glVertex3f(-scaleGizmo->centerSquareSize, -scaleGizmo->centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(scaleGizmo->centerSquareSize, -scaleGizmo->centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(-scaleGizmo->centerSquareSize, scaleGizmo->centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(scaleGizmo->centerSquareSize, scaleGizmo->centerSquareSize, 0.f);
	glEnd();

	// Yellow XY Square
	glBegin(GL_QUAD_STRIP);
	glColor3f(255, 255, 0); glVertex3f(scaleGizmo->xySquarePos.x - scaleGizmo->xySquareSize / 2, scaleGizmo->xySquarePos.y - scaleGizmo->xySquareSize / 2, 0.f);
	glColor3f(255, 255, 0); glVertex3f(scaleGizmo->xySquarePos.x + scaleGizmo->xySquareSize / 2, scaleGizmo->xySquarePos.y - scaleGizmo->xySquareSize / 2, 0.f);
	glColor3f(255, 255, 0); glVertex3f(scaleGizmo->xySquarePos.x - scaleGizmo->xySquareSize / 2, scaleGizmo->xySquarePos.y + scaleGizmo->xySquareSize / 2, 0.f);
	glColor3f(255, 255, 0); glVertex3f(scaleGizmo->xySquarePos.x + scaleGizmo->xySquareSize / 2, scaleGizmo->xySquarePos.y + scaleGizmo->xySquareSize / 2, 0.f);
	glEnd();

	glLineWidth(1.0f);
	glColor3f(255, 255, 255);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Gizmos::SetMoveGizmoStyle(float centerSize, float lineLength, float _lineWidth, float arrowLength, float arrowWidth, float _xySquareSize)
{
	moveGizmo->centerSquareSize = centerSize;
	moveGizmo->lineLength = lineLength;
	moveGizmo->lineWidth = _lineWidth;

	moveGizmo->arrowLength = arrowLength;
	moveGizmo->arrowWidth = arrowWidth;

	moveGizmo->xySquareSize = _xySquareSize; 
	moveGizmo->xySquarePos = float2(moveGizmo->lineLength / 4, -moveGizmo->lineLength / 4);

	moveGizmo->axisXBox->SetMinPoint(float2(0, arrowWidth / 2));
	moveGizmo->axisXBox->SetMaxPoint(float2(lineLength, - arrowWidth / 2));

	moveGizmo->axisYBox->SetMinPoint(float2(-arrowWidth / 2, 0));
	moveGizmo->axisYBox->SetMaxPoint(float2(arrowWidth / 2, -lineLength));

	moveGizmo->axisXYBox->SetMinPoint(float2(moveGizmo->xySquarePos.x - moveGizmo->xySquareSize / 2, moveGizmo->xySquarePos.y + moveGizmo->xySquareSize / 2));
	moveGizmo->axisXYBox->SetMaxPoint(float2(moveGizmo->xySquarePos.x + moveGizmo->xySquareSize / 2, moveGizmo->xySquarePos.y - moveGizmo->xySquareSize / 2));
}

void Gizmos::SetScaleGizmoStyle(float centerSize, float lineLength, float _lineWidth, float arrowLength, float _lineSquareSize, float _xySquareSize)
{
	scaleGizmo->centerSquareSize = centerSize;
	scaleGizmo->lineLength = lineLength; 
	scaleGizmo->lineLengthX = lineLength; 
	scaleGizmo->lineLengthY = lineLength; 

	scaleGizmo->endAxisBoxXPos = float2(lineLength, 0);
	scaleGizmo->endAxisBoxYPos = float2(0, -lineLength);
	scaleGizmo->endAxisBoxXYPos = float2(lineLength, -lineLength);

	scaleGizmo->initDragEndBoxXPos = scaleGizmo->endAxisBoxXPos; 
	scaleGizmo->initDragEndBoxYPos = scaleGizmo->endAxisBoxYPos; 
	scaleGizmo->initDragEndBoxXYPos = scaleGizmo->endAxisBoxXYPos; 

	scaleGizmo->lineSquareSize = _lineSquareSize;

	scaleGizmo->xySquareSize = _xySquareSize;
	scaleGizmo->xySquarePos = float2(scaleGizmo->lineLength / 4, -scaleGizmo->lineLength / 4);

	scaleGizmo->axisXBox->SetMinPoint(float2(0, _lineSquareSize / 2));
	scaleGizmo->axisXBox->SetMaxPoint(float2(lineLength + _lineSquareSize / 2, -_lineSquareSize / 2));

	scaleGizmo->axisYBox->SetMinPoint(float2(-_lineSquareSize / 2, 0));
	scaleGizmo->axisYBox->SetMaxPoint(float2(_lineSquareSize / 2, -lineLength - _lineSquareSize / 2));

	scaleGizmo->axisXYBox->SetMinPoint(float2(scaleGizmo->xySquarePos.x - scaleGizmo->xySquareSize / 2, scaleGizmo->xySquarePos.y + scaleGizmo->xySquareSize / 2));
	scaleGizmo->axisXYBox->SetMaxPoint(float2(scaleGizmo->xySquarePos.x + scaleGizmo->xySquareSize / 2, scaleGizmo->xySquarePos.y - scaleGizmo->xySquareSize / 2));
}

void Gizmos::SetCenterSquareSize(float& _centerSize)
{
	moveGizmo->centerSquareSize = _centerSize;
}

SelectGizmo::SelectGizmo()
{
	objectBorderBox = new BoundingBox(); 
	objectBorderBox->EnableDrag(false); 
}

SelectGizmo::~SelectGizmo()
{
}

void SelectGizmo::CleanUp()
{
	objectBorderBox->CleanUp(); 
	delete objectBorderBox; 
}

void SelectGizmo::AddaptSelectBox(FlyObject* objectAttached)
{
	objectBorderBox->CenterMinMaxPointsToScreen();
	float2 selectMaxPoint = objectBorderBox->GetMaxPoint();
	float2 selectMinPoint = objectBorderBox->GetMinPoint();

	// Scale							
	selectMaxPoint.x *= objectAttached->transform->GetScale().x;
	selectMaxPoint.y *= objectAttached->transform->GetScale().y;
								
	selectMinPoint.x *= objectAttached->transform->GetScale().x;
	selectMinPoint.y *= objectAttached->transform->GetScale().y;

	// Position
	selectMaxPoint.x += objectAttached->transform->GetPosition().x * ViewportManager::getInstance()->GetAspectRatio(); 
	selectMaxPoint.y += objectAttached->transform->GetPosition().y * ViewportManager::getInstance()->GetAspectRatio(); 
						
	selectMinPoint.x += objectAttached->transform->GetPosition().x * ViewportManager::getInstance()->GetAspectRatio();
	selectMinPoint.y += objectAttached->transform->GetPosition().y * ViewportManager::getInstance()->GetAspectRatio();
								
	objectBorderBox->SetMaxPoint(selectMaxPoint);
	objectBorderBox->SetMinPoint(selectMinPoint);
}

MoveGizmo::MoveGizmo()
{
	axisXBox = new BoundingBox();
	axisYBox = new BoundingBox();
	axisXYBox = new BoundingBox();

	axisXBox->EnableDrag(true); 
	axisYBox->EnableDrag(true); 
	axisXYBox->EnableDrag(true); 
}

MoveGizmo::~MoveGizmo()
{
}

void MoveGizmo::CleanUp()
{
	axisXBox->CleanUp();
	delete axisXBox;
	axisXBox = nullptr; 

	axisYBox->CleanUp();
	delete axisYBox;
	axisXBox = nullptr;

	axisXYBox->CleanUp();
	delete axisXYBox;
	axisXBox = nullptr;
}

void MoveGizmo::AddaptAxisBoxes(FlyObject* objectAttached)
{
	// X Axis 
	axisXBox->CenterMinMaxPointsToScreen(); 
	float2 moveMaxPoint = float2(axisXBox->GetMaxPoint().x, axisXBox->GetMaxPoint().y);
	float2 moveMinPoint = float2(axisXBox->GetMinPoint().x, axisXBox->GetMinPoint().y);

	moveMaxPoint.x += lineLength / 2;
	moveMinPoint.x += lineLength / 2;

	moveMinPoint.x += objectAttached->transform->GetPosition(true).x;
	moveMinPoint.y += objectAttached->transform->GetPosition(true).y;
									
	moveMaxPoint.x += objectAttached->transform->GetPosition(true).x;
	moveMaxPoint.y += objectAttached->transform->GetPosition(true).y;

	axisXBox->SetMinPoint(moveMinPoint);
	axisXBox->SetMaxPoint(moveMaxPoint); 

	// Y Axis 
	axisYBox->CenterMinMaxPointsToScreen();
	moveMaxPoint = float2(axisYBox->GetMaxPoint().x, axisYBox->GetMaxPoint().y);
	moveMinPoint = float2(axisYBox->GetMinPoint().x, axisYBox->GetMinPoint().y);

	moveMaxPoint.y -= lineLength / 2;
	moveMinPoint.y -= lineLength / 2;

	moveMinPoint.x += objectAttached->transform->GetPosition(true).x;
	moveMinPoint.y += objectAttached->transform->GetPosition(true).y;

	moveMaxPoint.x += objectAttached->transform->GetPosition(true).x;
	moveMaxPoint.y += objectAttached->transform->GetPosition(true).y;

	axisYBox->SetMinPoint(moveMinPoint);
	axisYBox->SetMaxPoint(moveMaxPoint);

	// Center Box 
	axisXYBox->CenterMinMaxPointsToScreen();
	moveMaxPoint = float2(axisXYBox->GetMaxPoint().x, axisXYBox->GetMaxPoint().y);
	moveMinPoint = float2(axisXYBox->GetMinPoint().x, axisXYBox->GetMinPoint().y);

	moveMinPoint.x += objectAttached->transform->GetPosition(true).x + xySquarePos.x;
	moveMinPoint.y += objectAttached->transform->GetPosition(true).y + xySquarePos.y;

	moveMaxPoint.x += objectAttached->transform->GetPosition(true).x + xySquarePos.x;
	moveMaxPoint.y += objectAttached->transform->GetPosition(true).y + xySquarePos.y;

	axisXYBox->SetMinPoint(moveMinPoint);
	axisXYBox->SetMaxPoint(moveMaxPoint);
}

void MoveGizmo::AddaptAxisBoxes(BoundingBox* boxAttached)
{
	// X Axis 
	axisXBox->CenterMinMaxPointsToScreen();
	float2 moveMaxPoint = float2(axisXBox->GetMaxPoint().x, axisXBox->GetMaxPoint().y);
	float2 moveMinPoint = float2(axisXBox->GetMinPoint().x, axisXBox->GetMinPoint().y);

	moveMaxPoint.x += lineLength / 2;
	moveMinPoint.x += lineLength / 2;

	moveMinPoint.x += boxAttached->GetCenter().x ;
	moveMinPoint.y += boxAttached->GetCenter().y ;
												 
	moveMaxPoint.x += boxAttached->GetCenter().x;
	moveMaxPoint.y += boxAttached->GetCenter().y;

	axisXBox->SetMinPoint(moveMinPoint);
	axisXBox->SetMaxPoint(moveMaxPoint);

	// Y Axis 
	axisYBox->CenterMinMaxPointsToScreen();
	moveMaxPoint = float2(axisYBox->GetMaxPoint().x, axisYBox->GetMaxPoint().y);
	moveMinPoint = float2(axisYBox->GetMinPoint().x, axisYBox->GetMinPoint().y);

	moveMaxPoint.y -= lineLength / 2;
	moveMinPoint.y -= lineLength / 2;

	moveMinPoint.x += boxAttached->GetCenter().x;
	moveMinPoint.y += boxAttached->GetCenter().y;

	moveMaxPoint.x += boxAttached->GetCenter().x;
	moveMaxPoint.y += boxAttached->GetCenter().y;

	axisYBox->SetMinPoint(moveMinPoint);
	axisYBox->SetMaxPoint(moveMaxPoint);

	//// Center Box 
	axisXYBox->CenterMinMaxPointsToScreen();
	moveMaxPoint = float2(axisXYBox->GetMaxPoint().x, axisXYBox->GetMaxPoint().y);
	moveMinPoint = float2(axisXYBox->GetMinPoint().x, axisXYBox->GetMinPoint().y);

	moveMinPoint.x += boxAttached->GetCenter().x + xySquarePos.x;
	moveMinPoint.y += boxAttached->GetCenter().y + xySquarePos.y;

	moveMaxPoint.x += boxAttached->GetCenter().x + xySquarePos.x;
	moveMaxPoint.y += boxAttached->GetCenter().y + xySquarePos.y;

	axisXYBox->SetMinPoint(moveMinPoint);
	axisXYBox->SetMaxPoint(moveMaxPoint);
}
 
ScaleGizmo::ScaleGizmo()
{
	borderBoundingBox = new BoundingBox(); 

	axisXBox = new BoundingBox();
	axisYBox = new BoundingBox();
	axisXYBox = new BoundingBox();

	axisXBox->EnableDrag(true);
	axisYBox->EnableDrag(true);
	axisXYBox->EnableDrag(true);
}

ScaleGizmo::~ScaleGizmo()
{
}

void ScaleGizmo::CleanUp()
{
	axisXBox->CleanUp();
	delete axisXBox;
	axisXBox = nullptr;

	axisYBox->CleanUp();
	delete axisYBox;
	axisXBox = nullptr;

	axisXYBox->CleanUp();
	delete axisXYBox;
	axisXBox = nullptr;

	borderBoundingBox->CleanUp();
	delete borderBoundingBox;
	borderBoundingBox = nullptr;
}

void ScaleGizmo::AddaptScaleBox(FlyObject* objectAttached)
{
	borderBoundingBox->CenterMinMaxPointsToScreen();
	float2 selectMaxPoint = borderBoundingBox->GetMaxPoint();
	float2 selectMinPoint = borderBoundingBox->GetMinPoint();

	// Scale							
	selectMaxPoint.x *= objectAttached->transform->GetScale().x;
	selectMaxPoint.y *= objectAttached->transform->GetScale().y;

	selectMinPoint.x *= objectAttached->transform->GetScale().x;
	selectMinPoint.y *= objectAttached->transform->GetScale().y;

	// Position
	selectMaxPoint.x += objectAttached->transform->GetPosition(true).x;
	selectMaxPoint.y += objectAttached->transform->GetPosition(true).y;

	selectMinPoint.x += objectAttached->transform->GetPosition(true).x;
	selectMinPoint.y += objectAttached->transform->GetPosition(true).y;

	borderBoundingBox->SetMaxPoint(selectMaxPoint);
	borderBoundingBox->SetMinPoint(selectMinPoint);
}

void ScaleGizmo::AddaptAxisBoxes(FlyObject* objectAttached)
{
	// X Axis 
	axisXBox->CenterMinMaxPointsToScreen();
	float2 moveMaxPoint = float2(axisXBox->GetMaxPoint().x, axisXBox->GetMaxPoint().y);
	float2 moveMinPoint = float2(axisXBox->GetMinPoint().x, axisXBox->GetMinPoint().y);

	moveMaxPoint.x += lineLength / 2;
	moveMinPoint.x += lineLength / 2;

	moveMinPoint.x += objectAttached->transform->GetPosition(true).x;
	moveMinPoint.y += objectAttached->transform->GetPosition(true).y;

	moveMaxPoint.x += objectAttached->transform->GetPosition(true).x;
	moveMaxPoint.y += objectAttached->transform->GetPosition(true).y;

	axisXBox->SetMinPoint(moveMinPoint);
	axisXBox->SetMaxPoint(moveMaxPoint);

	// Y Axis 
	axisYBox->CenterMinMaxPointsToScreen();
	moveMaxPoint = float2(axisYBox->GetMaxPoint().x, axisYBox->GetMaxPoint().y);
	moveMinPoint = float2(axisYBox->GetMinPoint().x, axisYBox->GetMinPoint().y);

	moveMaxPoint.y -= lineLength / 2;
	moveMinPoint.y -= lineLength / 2;

	moveMinPoint.x += objectAttached->transform->GetPosition(true).x;
	moveMinPoint.y += objectAttached->transform->GetPosition(true).y;

	moveMaxPoint.x += objectAttached->transform->GetPosition(true).x;
	moveMaxPoint.y += objectAttached->transform->GetPosition(true).y;

	axisYBox->SetMinPoint(moveMinPoint);
	axisYBox->SetMaxPoint(moveMaxPoint);

	// Center Box 
	axisXYBox->CenterMinMaxPointsToScreen();
	moveMaxPoint = float2(axisXYBox->GetMaxPoint().x, axisXYBox->GetMaxPoint().y);
	moveMinPoint = float2(axisXYBox->GetMinPoint().x, axisXYBox->GetMinPoint().y);

	moveMinPoint.x += objectAttached->transform->GetPosition(true).x + xySquarePos.x;
	moveMinPoint.y += objectAttached->transform->GetPosition(true).y + xySquarePos.y;

	moveMaxPoint.x += objectAttached->transform->GetPosition(true).x + xySquarePos.x;
	moveMaxPoint.y += objectAttached->transform->GetPosition(true).y + xySquarePos.y;

	axisXYBox->SetMinPoint(moveMinPoint);
	axisXYBox->SetMaxPoint(moveMaxPoint);
}
