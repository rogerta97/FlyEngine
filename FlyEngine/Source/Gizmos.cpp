#include "Gizmos.h"
#include "OpenGL.h"
#include "FlyObject.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"

Gizmos::Gizmos(FlyObject* _objectAttached)
{
	objectAttached = _objectAttached; 
}

Gizmos::~Gizmos()
{

}

void Gizmos::Draw()
{
	switch (App->moduleImGui->gameViewportDockPanel->GetGizmoMode())
	{
	case GIZMO_SELECT:
		break;

	case GIZMO_MOVE:
		DrawMoveGizmo();
		break;
	}
}

void Gizmos::DrawMoveGizmo()
{
	float3 objectCenter = objectAttached->transform->GetPosition();
	glLineWidth(arrowWidth);

	// X Axis
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_LINES);
	glColor3f(0, 0, 255); glVertex3f(objectCenter.x, objectCenter.y, 0.f);
	glColor3f(0, 0, 255); glVertex3f(objectCenter.x + arrowLenght, objectCenter.y, 0.f);
	glEnd();

	float3 triangleBase = float3(objectCenter.x + arrowLenght, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x, triangleBase.y - 10.0f, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x + 10.0f, 0.0f, 0.0f);
	glColor3f(0, 0, 255); glVertex3f(triangleBase.x, triangleBase.y + 10.0f, 0.0f);

	glEnd();

	// Y Axis
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_LINES);
	glColor3f(0, 255, 0); glVertex3f(objectCenter.x, objectCenter.y, 0.f);
	glColor3f(0, 255, 0); glVertex3f(objectCenter.x, objectCenter.y - arrowLenght, 0.f);
	glEnd();

	triangleBase = float3(0, objectCenter.y - arrowLenght, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glColor3f(0, 255, 0); glVertex3f(triangleBase.x - 10.0f, triangleBase.y, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(0.0f, triangleBase.y - 10.0f, 0.0f);
	glColor3f(0, 255, 0); glVertex3f(triangleBase.x + 10.0f, triangleBase.y, 0.0f);
	glEnd();

	// Center Square
	glBegin(GL_QUAD_STRIP);
	glColor3f(255, 0, 0); glVertex3f(objectCenter.x - centerSquareSize, objectCenter.y - centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(objectCenter.x + centerSquareSize, objectCenter.y - centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(objectCenter.x - centerSquareSize, objectCenter.y + centerSquareSize, 0.f);
	glColor3f(255, 0, 0); glVertex3f(objectCenter.x + centerSquareSize, objectCenter.y + centerSquareSize, 0.f);
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

void Gizmos::SetGizmoStyle(float centerSize, float lineLenght, float _arrowWidth)
{
	centerSquareSize = centerSize; 
	arrowLenght = lineLenght; 
	arrowWidth = _arrowWidth;
}

void Gizmos::SetCenterSquareSize(float& _centerSize)
{
	centerSquareSize = _centerSize;
}
