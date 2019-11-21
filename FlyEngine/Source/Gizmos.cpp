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

Gizmos::Gizmos(FlyObject* _objectAttached)
{
	objectAttached = _objectAttached; 

	objectBorderBox = new BoundingBox(objectAttached); 
	objectBorderBox->showCornerDots = false;
	SetBoxColor(float4(0.4f, 0.4f, 1.0f, 0.2f)); 
	FitBoxToObject(); 
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
				if (objectBorderBox->IsMouseOver())
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
		DrawMoveGizmo();
		break;
	}
}

void Gizmos::DrawSelectGizmo()
{
	objectBorderBox->Draw();
}

void Gizmos::CalculateGizmos()
{
	// Fit Transform gizmo 
	FitBoxToObject(); 

	// Fit Move Gizmo
}

void Gizmos::FitBoxToObject()
{
	if (objectAttached != nullptr) 
	{
		if (objectAttached->GetTool(AT_IMAGE) != nullptr)
		{
			float2 objectDimensions = objectAttached->GetObjectVisualDimensions(); 
			objectBorderBox->SetSize(objectDimensions.x, objectDimensions.y); 
		}
	}
}

void Gizmos::DrawMoveGizmo()
{
	glLineWidth(arrowWidth);

	// X Axis
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_LINES);
	glColor3f(0, 0, 255); glVertex3f(0, 0, 0.f);
	glColor3f(0, 0, 255); glVertex3f(arrowLenght, 0, 0.f);
	glEnd();

	float3 triangleBase = float3(arrowLenght, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x, triangleBase.y - 10.0f, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x + 10.0f, 0.0f, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x, triangleBase.y + 10.0f, 0.0f);

	glEnd();

	// Y Axis
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_LINES);
	glColor3f(0, 255, 0); glVertex3f(0, 0, 0.f);
	glColor3f(0, 255, 0); glVertex3f(0,-arrowLenght, 0.f);
	glEnd();

	triangleBase = float3(0, 0 - arrowLenght, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glColor3f(0, 255, 0); glVertex3f(triangleBase.x - 10.0f, triangleBase.y, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(0.0f, triangleBase.y - 10.0f, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(triangleBase.x + 10.0f, triangleBase.y, 0.0f);
	glEnd();

	// Center Square
	glBegin(GL_QUAD_STRIP);
	glColor3f(255, 0, 0); glVertex3f(-centerSquareSize, -centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(centerSquareSize, -centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(-centerSquareSize, centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(centerSquareSize, centerSquareSize, 0.f);
	glEnd();

	glLineWidth(1.0f);
	glColor3f(255, 255, 255);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Gizmos::SetArrowLenght(float& lineLenght)
{
	arrowLenght = lineLenght; 
}

void Gizmos::SetArrowWidth(float& _arrowWidth)
{
	arrowWidth = _arrowWidth; 
}

void Gizmos::SetBoxSize(float& boxSizeX, float& boxSizeY)
{
	objectBorderBox->SetSize(boxSizeX, boxSizeY); 
}

void Gizmos::SetBoxColor(float4 color)
{
	objectBorderBox->SetSquareColor(color); 
}

void Gizmos::SetMoveGizmoStyle(float centerSize, float lineLenght, float _arrowWidth)
{
	centerSquareSize = centerSize; 
	arrowLenght = lineLenght; 
	arrowWidth = _arrowWidth;
}

void Gizmos::SetCenterSquareSize(float& _centerSize)
{
	centerSquareSize = _centerSize;
}
