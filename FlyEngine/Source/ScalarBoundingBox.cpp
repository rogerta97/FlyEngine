#include "ScalarBoundingBox.h"
#include "Globals.h"

#include "mmgr.h"

ScalarBoundingBox::ScalarBoundingBox(FlyObject* objectAttached, bool drawCornerPoints) : BoundingBox()
{
	this->drawCornerPoints = drawCornerPoints; 

	topLeftScaleBox = new BoundingBox(); 
	topLeftScaleBox->SetMinPoint(float2(0, 0));
	topLeftScaleBox->SetMaxPoint(float2(0, 0));
	topLeftScaleBox->EnableDrag(true); 

	topRightScaleBox = new BoundingBox(); 
	topRightScaleBox->SetMinPoint(float2(0, 0));
	topRightScaleBox->SetMaxPoint(float2(0, 0));
	topRightScaleBox->EnableDrag(true);

	bottomLeftScaleBox = new BoundingBox(); 
	bottomLeftScaleBox->SetMinPoint(float2(0, 0));
	bottomLeftScaleBox->SetMaxPoint(float2(0, 0));
	bottomLeftScaleBox->EnableDrag(true);

	bottomRightScaleBox = new BoundingBox(); 
	bottomRightScaleBox->SetMinPoint(float2(0, 0));
	bottomRightScaleBox->SetMaxPoint(float2(0, 0));
	bottomRightScaleBox->EnableDrag(true);
}


ScalarBoundingBox::~ScalarBoundingBox()
{

}

void ScalarBoundingBox::Draw(bool fill, float4 color)
{
	BoundingBox::Draw(false, float4(255, 255, 255, 255)); 

	if (drawCornerPoints)
	{
		topLeftScaleBox->Draw(true, float4(255, 0, 0, 255));
		topRightScaleBox->Draw(true, float4(255, 0, 0, 255));
		bottomLeftScaleBox->Draw(true, float4(255, 0, 0, 255));
		bottomRightScaleBox->Draw(true, float4(255, 0, 0, 255));
	}
}

void ScalarBoundingBox::Update()
{
	float2 topLeftDrag = topLeftScaleBox->HandleDrag(math::CardinalAxis::AxisNone);
	float2 topRighttDrag = topRightScaleBox->HandleDrag(math::CardinalAxis::AxisNone);
	float2 bottomLeftDrag = bottomLeftScaleBox->HandleDrag(math::CardinalAxis::AxisNone);
	float2 bottomRightDrag = bottomRightScaleBox->HandleDrag(math::CardinalAxis::AxisNone);

	if (topRighttDrag.x != 0 || topRighttDrag.y != 0)
	{
	  //Draw Function to get TopLeft TopRight...
		// assign the functions to the minpoin corresponly

		//If you draw 
	}

}

void ScalarBoundingBox::CleanUp()
{
	topLeftScaleBox->CleanUp();
	delete topLeftScaleBox;

	topRightScaleBox->CleanUp();
	delete topRightScaleBox;

	bottomLeftScaleBox->CleanUp();
	delete bottomLeftScaleBox;

	bottomRightScaleBox->CleanUp();
	delete bottomRightScaleBox;
}

void ScalarBoundingBox::SetCornerBoxSize(float newSize)
{
	if (newSize <= 0)
		return; 

	cornerBoxesSize = newSize; 

	float2 boxTopLeft = float2(minPoint.x, maxPoint.y); 
	topLeftScaleBox->SetMinPoint(float2(boxTopLeft.x, boxTopLeft.y + cornerBoxesSize * 2));
	topLeftScaleBox->SetMaxPoint(float2(boxTopLeft.x + cornerBoxesSize * 2, boxTopLeft.y));

	float2 boxTopRight = float2(maxPoint.x, maxPoint.y);
	topRightScaleBox->SetMinPoint(float2(boxTopRight.x - cornerBoxesSize * 2, boxTopRight.y + cornerBoxesSize * 2));
	topRightScaleBox->SetMaxPoint(float2(boxTopRight.x, boxTopRight.y));

	float2 boxBottomLeft = float2(minPoint.x, minPoint.y);
	bottomLeftScaleBox->SetMinPoint(float2(boxBottomLeft.x, boxBottomLeft.y));
	bottomLeftScaleBox->SetMaxPoint(float2(boxBottomLeft.x + cornerBoxesSize * 2, boxBottomLeft.y - cornerBoxesSize * 2));

	float2 boxBottomRight = float2(maxPoint.x, minPoint.y);
	bottomRightScaleBox->SetMinPoint(float2(boxBottomRight.x - cornerBoxesSize * 2, boxBottomRight.y));
	bottomRightScaleBox->SetMaxPoint(float2(boxBottomRight.x, boxBottomRight.y - cornerBoxesSize * 2));
}

void ScalarBoundingBox::EnableScaling(bool enableScaling)
{
	scalingEnabled = enableScaling; 

	topLeftScaleBox->EnableDrag(enableScaling);
	topRightScaleBox->EnableDrag(enableScaling);
	bottomLeftScaleBox->EnableDrag(enableScaling);
	bottomRightScaleBox->EnableDrag(enableScaling);
	
}
