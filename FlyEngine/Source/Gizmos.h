#ifndef _GIZMOS_H_
#define _GIZMOS_H_

#include "MathGeoLib.h"

class BoundingBox; 
class FlyObject;
class Transform;

struct SelectGizmo
{
	BoundingBox* objectBorderBox;
};

struct MoveGizmo
{
	float arrowLenght;
	float arrowWidth;
	float centerSquareSize;
};

class Gizmos
{
public:
	Gizmos(FlyObject* objectAttached);
	~Gizmos();

	void Update(); 
	void Draw();

	void DrawMoveGizmo();
	void DrawSelectGizmo(); 
	void CalculateGizmos();
	void FitMinAndMaxPoints();


	void FitBoxToObject(); 

	void SetCenterSquareSize(float& centerSize); 
	void SetArrowLenght(float& lineLenght);
	void SetArrowWidth(float& _arrowWidth);
	void SetBoxSize(float& boxSizeX, float& boxSizeY);
	void SetBoxColor(float4 color); 

	void SetMoveGizmoStyle(float centerSize, float lineLenght, float _arrowWidth); 

public: 
	Transform* gizmoTransform; 

private:

	MoveGizmo moveGizmo;
	SelectGizmo selectGizmo; 

	FlyObject* objectAttached;
};

#endif // ! _GIZMOS_H_


