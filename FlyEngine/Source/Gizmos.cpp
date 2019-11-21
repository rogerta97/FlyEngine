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

	CalculateGizmos(); 
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

			if (App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN))
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

			// Aixo es nomes un test, el algoritme de mousepicking no ha de funcionar aixi sino de la seguent manera:

			// el input s'agafa desde algun lloc on es tingui accés a la llista d'objectes, després s'itera per tots els objectes i es mira si la 
			// ratoli esta dintre de la bb, si está tel guardes, si despres un surt que falla seguir amb el que tens guardat. 
				 
			break;

		case GIZMO_MOVE:
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
		FitMinAndMaxPoints();	
	}
}

void Gizmos::FitMinAndMaxPoints()
{
	selectGizmo->objectBorderBox->CenterMinMaxPoints();
	float2 selectMaxPoint = selectGizmo->objectBorderBox->GetMaxPoint();
	float2 selectMinPoint = selectGizmo->objectBorderBox->GetMinPoint();

	// Position
	selectMaxPoint.x *= gizmoTransform->GetScale().x;
	selectMaxPoint.y *= gizmoTransform->GetScale().y;

	selectMinPoint.x *= gizmoTransform->GetScale().x;
	selectMinPoint.y *= gizmoTransform->GetScale().y;

	// Scale
	selectMaxPoint.x += gizmoTransform->GetPosition().x;
	selectMaxPoint.y += gizmoTransform->GetPosition().y;

	selectMinPoint.x += gizmoTransform->GetPosition().x;
	selectMinPoint.y += gizmoTransform->GetPosition().y;

	selectGizmo->objectBorderBox->SetMaxPoint(selectMaxPoint);
	selectGizmo->objectBorderBox->SetMinPoint(selectMinPoint);
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

MoveGizmo::MoveGizmo(FlyObject* parentObject)
{
	axisXBox = new BoundingBox(parentObject);
	axisYBox = new BoundingBox(parentObject);
}

MoveGizmo::~MoveGizmo()
{
}

void MoveGizmo::AddaptAxisBoxes()
{

}
