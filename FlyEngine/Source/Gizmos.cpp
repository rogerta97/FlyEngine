#include "Gizmos.h"
#include "OpenGL.h"
#include "FlyObject.h"
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
	SetMoveGizmoStyle(7.0f, 100.0f, 5.0f, 20, 20);
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
			break;

		case GIZMO_MOVE:
			if (App->moduleInput->GetMouseButton(LEFT_CLICK) == KEY_DOWN)
			{
				if (moveGizmo->axisXBox->IsMouseOver())
				{
					moveGizmo->canDrag = true; 
					moveGizmo->beginDragPos = objectAttached->transform->GetPosition(); 
				}

				if (moveGizmo->axisYBox->IsMouseOver())
				{
	
				}
			}

			if (moveGizmo->canDrag)
			{
				float2 positionInDrag = float2(moveGizmo->beginDragPos.x + ImGui::GetMouseDragDelta().x, moveGizmo->beginDragPos.y); 
				objectAttached->transform->SetPosition(positionInDrag);
				FLY_LOG("Accum Drag: %f", ImGui::GetMouseDragDelta().x);

				if (App->moduleInput->GetMouseButton(LEFT_CLICK) == KEY_UP)
				{
					moveGizmo->canDrag = false; 
					moveGizmo->beginDragPos = float2(10, 10); 
				}
			}

			break;

		case GIZMO_null:
			break;
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

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity(); 

		float4x4 moveGizmoViewMat = float4x4::identity;

		moveGizmoViewMat.RotateX(0);
		moveGizmoViewMat.RotateY(0);
	
		moveGizmoViewMat.SetTranslatePart(float3(objectAttached->transform->GetPosition().x, objectAttached->transform->GetPosition().y, 0));

		glLoadMatrixf((GLfloat*)moveGizmoViewMat.Transposed().v); 

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

void Gizmos::CalculateGizmos()
{
	if (objectAttached != nullptr)
	{
		gizmoTransform->SetPosition(objectAttached->transform->GetPosition());
		gizmoTransform->SetRotationEuler(objectAttached->transform->GetRotation());
		gizmoTransform->SetScale(objectAttached->transform->GetScale());
		gizmoTransform->CalculateViewMatrix();

		// Select Gizmo 
		selectGizmo->FitMinAndMaxPoints(objectAttached);

		// Move Tool 
		moveGizmo->AddaptAxisBoxes(objectAttached);
	}
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

	CalculateGizmos(); 
}

void Gizmos::DrawMoveGizmo()
{
	glLineWidth(moveGizmo->lineWidth);

	// X Axis
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_LINES);
	glColor3f(0, 0, 255); glVertex3f(0, 0, 0.f);
	glColor3f(0, 0, 255); glVertex3f(moveGizmo->lineLength, 0, 0.f);
	glEnd();

	float3 triangleBase = float3(moveGizmo->lineLength - moveGizmo->arrowLength, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x, triangleBase.y - moveGizmo->arrowWidth / 2, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x + moveGizmo->arrowLength, 0.0f, 0.0f);
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
	glColor3f(0, 255, 0); glVertex3f(0.0f, triangleBase.y - moveGizmo->arrowLength, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(triangleBase.x + moveGizmo->arrowWidth / 2, triangleBase.y, 0.0f);
	glEnd();

	// Center Square
	glBegin(GL_QUAD_STRIP);
	glColor3f(255, 0, 0); glVertex3f(-moveGizmo->centerSquareSize, -moveGizmo->centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(moveGizmo->centerSquareSize, -moveGizmo->centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(-moveGizmo->centerSquareSize, moveGizmo->centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(moveGizmo->centerSquareSize, moveGizmo->centerSquareSize, 0.f);
	glEnd();

	moveGizmo->axisXBox->Draw();
	moveGizmo->axisYBox->Draw();

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

void Gizmos::SetMoveGizmoStyle(float centerSize, float lineLength, float _lineWidth, float arrowLength, float arrowWidth)
{
	moveGizmo->centerSquareSize = centerSize;
	moveGizmo->lineLength = lineLength;
	moveGizmo->lineWidth = _lineWidth;

	moveGizmo->arrowLength = arrowLength;
	moveGizmo->arrowWidth = arrowWidth;

	moveGizmo->axisXBox->SetMinPoint(float2(0, arrowWidth / 2));
	moveGizmo->axisXBox->SetMaxPoint(float2(lineLength, - arrowWidth / 2));

	moveGizmo->axisYBox->SetMinPoint(float2(-arrowWidth / 2, 0));
	moveGizmo->axisYBox->SetMaxPoint(float2(arrowWidth / 2, -lineLength));
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

void SelectGizmo::FitMinAndMaxPoints(FlyObject* objectAttached)
{
	objectBorderBox->CenterMinMaxPoints();
	float2 selectMaxPoint = objectBorderBox->GetMaxPoint();
	float2 selectMinPoint = objectBorderBox->GetMinPoint();

	// Scale
	selectMaxPoint.x *= objectAttached->gizmos->gizmoTransform->GetScale().x;
	selectMaxPoint.y *= objectAttached->gizmos->gizmoTransform->GetScale().y;

	selectMinPoint.x *= objectAttached->gizmos->gizmoTransform->GetScale().x;
	selectMinPoint.y *= objectAttached->gizmos->gizmoTransform->GetScale().y;

	// Position
	selectMaxPoint.x += objectAttached->gizmos->gizmoTransform->GetPosition().x;
	selectMaxPoint.y += objectAttached->gizmos->gizmoTransform->GetPosition().y;

	selectMinPoint.x += objectAttached->gizmos->gizmoTransform->GetPosition().x;
	selectMinPoint.y += objectAttached->gizmos->gizmoTransform->GetPosition().y;

	objectBorderBox->SetMaxPoint(selectMaxPoint);
	objectBorderBox->SetMinPoint(selectMinPoint);
}

MoveGizmo::MoveGizmo(FlyObject* parentObject)
{
	axisXBox = new BoundingBox(parentObject);
	axisYBox = new BoundingBox(parentObject);
}

MoveGizmo::~MoveGizmo()
{
}

void MoveGizmo::AddaptAxisBoxes(FlyObject* objectAttached)
{
	//// X Axis 
	//float2 moveMaxPoint = axisXBox->GetMaxPoint();
	//float2 moveMinPoint = axisXBox->GetMinPoint();

	//moveMaxPoint.x += objectAttached->transform->GetPosition().x;
	//moveMinPoint.y += objectAttached->transform->GetPosition().y;
	//								
	//moveMaxPoint.x += objectAttached->transform->GetPosition().x;
	//moveMinPoint.y += objectAttached->transform->GetPosition().y;

	//axisXBox->SetMinPoint(moveMinPoint);
	//axisXBox->SetMaxPoint(moveMinPoint); 

	//// Y Axis 
	//moveMaxPoint = axisYBox->GetMaxPoint();
	//moveMinPoint = axisYBox->GetMinPoint();

	//moveMaxPoint.x += objectAttached->transform->GetPosition().x;
	//moveMinPoint.y += objectAttached->transform->GetPosition().y;

	//moveMaxPoint.x += objectAttached->transform->GetPosition().x;
	//moveMinPoint.y += objectAttached->transform->GetPosition().y;

	//axisYBox->SetMinPoint(moveMinPoint);
	//axisYBox->SetMaxPoint(moveMaxPoint);
}