#include "ScalarBoundingBox.h"
#include "Globals.h"

ScalarBoundingBox::ScalarBoundingBox(FlyObject* objectAttached) : BoundingBox(objectAttached)
{
	topLeftScaleBox = new BoundingBox(objectAttached); 
	topLeftScaleBox->SetMinPoint(float2(0, 0));
	topLeftScaleBox->SetMaxPoint(float2(0, 0));

	topRightScaleBox = new BoundingBox(objectAttached); 
	topRightScaleBox->SetMinPoint(float2(0, 0));
	topRightScaleBox->SetMaxPoint(float2(0, 0));

	bottomLeftScaleBox = new BoundingBox(objectAttached); 
	bottomLeftScaleBox->SetMinPoint(float2(0, 0));
	bottomLeftScaleBox->SetMaxPoint(float2(0, 0));

	bottomRightScaleBox = new BoundingBox(objectAttached); 
	bottomRightScaleBox->SetMinPoint(float2(0, 0));
	bottomRightScaleBox->SetMaxPoint(float2(0, 0));
}


ScalarBoundingBox::~ScalarBoundingBox()
{

}

void ScalarBoundingBox::Draw(bool fill, float4 color)
{
	BoundingBox::Draw(fill, color); 

	topLeftScaleBox->Draw(true, float4(255, 255, 255, 255)); 
	topRightScaleBox->Draw(true, float4(255, 255, 255, 255));
	bottomLeftScaleBox->Draw(true, float4(255, 255, 255, 255));
	bottomRightScaleBox->Draw(true, float4(255, 255, 255, 255));
}

void ScalarBoundingBox::Update()
{
	if (topLeftScaleBox->IsMouseOver())
	{

	}

	if (topRightScaleBox->IsMouseOver()) FLY_LOG("Mouse over top right"); 
	if (bottomLeftScaleBox->IsMouseOver()) FLY_LOG("Mouse over bottom left"); 
	if (bottomRightScaleBox->IsMouseOver()) FLY_LOG("Mouse over bottom right"); 
}

void ScalarBoundingBox::CleanUp()
{
}

void ScalarBoundingBox::SetCornerBoxSize(float newSize)
{
	if (newSize <= 0)
		return; 

	cornerBoxesSize = newSize; 

	float2 boxTopLeft = float2(minPoint.x, maxPoint.y); 
	topLeftScaleBox->SetMinPoint(float2(boxTopLeft.x - cornerBoxesSize, boxTopLeft.y + cornerBoxesSize));
	topLeftScaleBox->SetMaxPoint(float2(boxTopLeft.x + cornerBoxesSize, boxTopLeft.y - cornerBoxesSize));

	float2 boxTopRight = float2(maxPoint.x, maxPoint.y);
	topRightScaleBox->SetMinPoint(float2(boxTopRight.x - cornerBoxesSize, boxTopRight.y + cornerBoxesSize));
	topRightScaleBox->SetMaxPoint(float2(boxTopRight.x + cornerBoxesSize, boxTopRight.y - cornerBoxesSize));

	float2 boxBottomLeft = float2(minPoint.x, minPoint.y);
	bottomLeftScaleBox->SetMinPoint(float2(boxBottomLeft.x - cornerBoxesSize, boxBottomLeft.y + cornerBoxesSize));
	bottomLeftScaleBox->SetMaxPoint(float2(boxBottomLeft.x + cornerBoxesSize, boxBottomLeft.y - cornerBoxesSize));

	float2 boxBottomRight = float2(maxPoint.x, minPoint.y);
	bottomRightScaleBox->SetMinPoint(float2(boxBottomRight.x - cornerBoxesSize, boxBottomRight.y + cornerBoxesSize));
	bottomRightScaleBox->SetMaxPoint(float2(boxBottomRight.x + cornerBoxesSize, boxBottomRight.y - cornerBoxesSize));
}
