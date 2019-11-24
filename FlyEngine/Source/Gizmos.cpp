#include "Gizmos.h"
#include "OpenGL.h"
#include "FlyObject.h"
#include "ViewportManager.h"
#include "Application.h"
#include "ImageTool.h"
#include "Quad.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
#include "BoundingBox.h"
#include "ModuleRoomManager.h"
#include "Room.h"
#include "GameViewportDockPanel.h"
#include "Transform.h"

Gizmos::Gizmos(FlyObject* _objectAttached)
{
	objectAttached = _objectAttached; 
	
	gizmoTransform = new Transform();

	selectGizmo = new SelectGizmo(_objectAttached);
	moveGizmo = new MoveGizmo(_objectAttached);

	SetMoveGizmoStyle(7.0f, 100.0f, 5.0f, 20, 20, 25);
	SetBoxColor(float4(0.4f, 0.4f, 1.0f, 0.2f)); 
}

Gizmos::~Gizmos()
{

}

void Gizmos::Update()
{
	if (!App->moduleImGui->gameViewportDockPanel->IsMouseInViewport())
		return; 

	GizmoMode gizmoMode = App->moduleImGui->gameViewportDockPanel->GetGizmoMode(); 
	if (gizmoMode != GIZMO_null)
	{
		switch (gizmoMode)
		{
		case GIZMO_SELECT:
			HandleSelectionGizmo();
			break;

		case GIZMO_MOVE:
			HandleMoveGizmo();
			break;

		case GIZMO_null:
			break;
		}
	}
}

void Gizmos::HandleMoveGizmo()
{
	if (App->moduleInput->GetMouseButton(LEFT_CLICK) == KEY_DOWN)
	{
		if (moveGizmo->axisXBox->IsMouseOver())
		{
			moveGizmo->dragAxis = DRAG_X;

			moveGizmo->dragCenterOffset = App->moduleImGui->gameViewportDockPanel->GetMouseGamePos(); 
			moveGizmo->dragCenterOffset -= (objectAttached->transform->GetPosition(false));
			moveGizmo->dragCenterOffset = float2((int)moveGizmo->dragCenterOffset.x, (int)moveGizmo->dragCenterOffset.y);
			
			moveGizmo->initDragPos = objectAttached->transform->GetPosition(false);
		}

		if (moveGizmo->axisYBox->IsMouseOver())
		{
			moveGizmo->dragAxis = DRAG_Y;

			moveGizmo->dragCenterOffset = App->moduleImGui->gameViewportDockPanel->GetMouseGamePos();
			moveGizmo->dragCenterOffset -= (objectAttached->transform->GetPosition(false));
			moveGizmo->dragCenterOffset = float2((int)moveGizmo->dragCenterOffset.x, (int)moveGizmo->dragCenterOffset.y);

			moveGizmo->initDragPos = objectAttached->transform->GetPosition(false);
		}

		if (moveGizmo->axisXYBox->IsMouseOver())
		{
			moveGizmo->dragAxis = DRAG_XY;

			moveGizmo->dragCenterOffset = App->moduleImGui->gameViewportDockPanel->GetMouseGamePos();
			moveGizmo->dragCenterOffset -= (objectAttached->transform->GetPosition(false));
			moveGizmo->dragCenterOffset = float2((int)moveGizmo->dragCenterOffset.x, (int)moveGizmo->dragCenterOffset.y);

			moveGizmo->initDragPos = objectAttached->transform->GetPosition(false);
		}
	}

	HandleDrag();
}

void Gizmos::HandleDrag()
{
	if (moveGizmo->dragAxis != NOT_DRAG)
	{
		FLY_LOG("Draggin");
		
		float2 positionInDrag = float2(App->moduleImGui->gameViewportDockPanel->GetMouseGamePos().x, App->moduleImGui->gameViewportDockPanel->GetMouseGamePos().y);
		float2 positionInDragGame = float2(App->moduleImGui->gameViewportDockPanel->ScreenToWorld(positionInDrag.x, positionInDrag.y));
		positionInDragGame = float2((int)positionInDragGame.x, (int)positionInDragGame.y);

		switch (moveGizmo->dragAxis)
		{

		case DRAG_X:
			objectAttached->transform->SetPosition(float2(positionInDrag.x - moveGizmo->dragCenterOffset.x, moveGizmo->initDragPos.y));
			objectAttached->CalculateCurrentGizmo(); 
			break;

		case DRAG_Y:
			objectAttached->transform->SetPosition(float2(moveGizmo->initDragPos.x, positionInDrag.y - moveGizmo->dragCenterOffset.y));
			objectAttached->CalculateCurrentGizmo();
			break;

		case DRAG_XY:
			objectAttached->transform->SetPosition(float2(positionInDrag.x - moveGizmo->dragCenterOffset.x, positionInDrag.y - moveGizmo->dragCenterOffset.y));
			objectAttached->CalculateCurrentGizmo();
			break;

		}

		if (App->moduleInput->GetMouseButton(LEFT_CLICK) == KEY_UP)
		{
			moveGizmo->dragAxis = NOT_DRAG;
		}
	}
}

void Gizmos::HandleSelectionGizmo()
{
	if (App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_DOWN)
	{
		if (selectGizmo->objectBorderBox->IsMouseOver())
		{
			App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(objectAttached);
		}
		else
		{
			App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(nullptr);
		}
	}
}

void Gizmos::Draw()
{
	switch (App->moduleImGui->gameViewportDockPanel->GetGizmoMode())
	{
	case GIZMO_SELECT:
		DrawSelectGizmo(); 
		break;

	case GIZMO_MOVE:
		DrawMoveGizmo();
		break;
	}
}

void Gizmos::DrawSelectGizmo()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	selectGizmo->objectBorderBox->Draw();
}

void Gizmos::CalculateSelectGizmo(FlyObject* objectAttached)
{			
	selectGizmo->AddaptSelectBox(objectAttached);
}

void Gizmos::CalculateMoveGizmo(FlyObject* objectAttached)
{
	moveGizmo->AddaptAxisBoxes(objectAttached);
}

void Gizmos::FitBoxToObject()
{
	if (objectAttached != nullptr) 
	{
		if (objectAttached->GetTool(AT_IMAGE) != nullptr)
		{
			float2 objectDimensions = objectAttached->GetObjectVisualDimensions(); 
			selectGizmo->objectBorderBox->SetSize(objectDimensions.x, objectDimensions.y); 
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
	float2 objectPosition = objectAttached->transform->GetPosition();
	moveGizmoViewMat.SetTranslatePart(float3(objectPosition.x * ViewportManager::getInstance()->GetAspectRatio(), objectPosition.y * ViewportManager::getInstance()->GetAspectRatio(), 0));
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

void Gizmos::SetArrowLenght(float& lineLenght)
{
	moveGizmo->lineLength = lineLenght;
}

void Gizmos::SetArrowWidth(float& _arrowWidth)
{
	moveGizmo->lineWidth = _arrowWidth;
}

void Gizmos::SetBoxSize(float& boxSizeX, float& boxSizeY)
{
	selectGizmo->objectBorderBox->SetSize(boxSizeX, boxSizeY);
}

void Gizmos::SetBoxColor(float4 color)
{
	selectGizmo->objectBorderBox->SetSquareColor(color); 
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

void Gizmos::SetCenterSquareSize(float& _centerSize)
{
	moveGizmo->centerSquareSize = _centerSize;
}

void Gizmos::SetLineLength(float& lineLenght)
{
	moveGizmo->lineLength = lineLenght;
}

void Gizmos::SetLineWidth(float& _arrowWidth)
{
	moveGizmo->arrowWidth = _arrowWidth; 
}

SelectGizmo::SelectGizmo(FlyObject* parentObject)
{
	objectBorderBox = new BoundingBox(parentObject); 
}

SelectGizmo::~SelectGizmo()
{
}

void SelectGizmo::AddaptSelectBox(FlyObject* objectAttached)
{
	objectBorderBox->CenterMinMaxPointsToScreen();
	float2 selectMaxPoint = objectBorderBox->GetMaxPoint();
	float2 selectMinPoint = objectBorderBox->GetMinPoint();

	// Scale							
	selectMaxPoint.x /= objectAttached->transform->GetScaleInc().x;
	selectMaxPoint.y /= objectAttached->transform->GetScaleInc().y;
								
	selectMinPoint.x /= objectAttached->transform->GetScaleInc().x;
	selectMinPoint.y /= objectAttached->transform->GetScaleInc().y;

	// Position
	selectMaxPoint.x += objectAttached->transform->GetPosition(true).x;
	selectMaxPoint.y += objectAttached->transform->GetPosition(true).y;
						
	selectMinPoint.x += objectAttached->transform->GetPosition(true).x; 
	selectMinPoint.y += objectAttached->transform->GetPosition(true).y; 
								

	objectBorderBox->SetMaxPoint(selectMaxPoint);
	objectBorderBox->SetMinPoint(selectMinPoint);
}

MoveGizmo::MoveGizmo(FlyObject* parentObject)
{
	axisXBox = new BoundingBox(parentObject);
	axisYBox = new BoundingBox(parentObject);
	axisXYBox = new BoundingBox(parentObject);

	dragAxis = NOT_DRAG; 
}

MoveGizmo::~MoveGizmo()
{
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