#ifndef _GIZMOS_H_
#define _GIZMOS_H_

#include "MathGeoLib.h"

class BoundingBox; 
class FlyObject;
class Gizmos
{
public:
	Gizmos(FlyObject* objectAttached);
	~Gizmos();

	void Update(); 
	void Draw();

	void DrawMoveGizmo();
	void DrawSelectGizmo(); 

	void FitBoxToObject(); 

	void SetCenterSquareSize(float& centerSize); 
	void SetArrowLenght(float& lineLenght);
	void SetArrowWidth(float& _arrowWidth);
	void SetBoxSize(float& boxSizeX, float& boxSizeY);
	void SetBoxColor(float4 color); 

	void SetMoveGizmoStyle(float centerSize, float lineLenght, float _arrowWidth); 

private:

	float arrowLenght; 
	float arrowWidth; 
	float centerSquareSize; 

	FlyObject* objectAttached;
	BoundingBox* objectBorderBox; 
};

#endif // ! _GIZMOS_H_


