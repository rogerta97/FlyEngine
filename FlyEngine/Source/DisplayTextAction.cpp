#include "DisplayTextAction.h"
#include "Quad.h"
#include "OpenGL.h"
#include "Font.h"
#include "FlyObject.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"
#include "ResourceManager.h"


DisplayTextAction::DisplayTextAction(FlyObject* _parentObject)
{
	actionType = ACTION_DISPLAY_TEXT;
	parentObject = _parentObject;
	isVisual = false;

	textBox = new BoundingBox();

	textBox->SetSize(300, 120); 
	CalculateOriginTextPosition(); 

	originTextPosition = float2(0, 0);

	textFont = (Font*)ResourceManager::GetResource("arial", RESOURCE_FONT);
	SetText("AB");
	textColor = float4(1.0f, 1.0f, 1.0f, 1.0f); 

	SetActionName("Display Text");
	SetToolDescription("This should be the description of display text");
}

DisplayTextAction::~DisplayTextAction()
{

}

void DisplayTextAction::Draw()
{
	if (!text.empty())
		RenderText(); 

	if(isSelected)
		textBox->Draw(false, float4(0, 1.0f, 0, 1.0f));
}

void DisplayTextAction::CleanUp()
{
}

void DisplayTextAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr)
{
	string toolsSerializeSection;

	if (!literalStr)
		toolsSerializeSection = serializeObjectString + string("Actions.DisplayText.");
	else
		toolsSerializeSection = serializeObjectString;

	Action::SaveAction(jsonObject, toolsSerializeSection);

	if (text.empty())	
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("Text")).c_str(), "None");	
	else	
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("Text")).c_str(), text.c_str());	

	if(textFont != nullptr)
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("FontName")).c_str(), textFont->GetName().c_str());
	else
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("FontName")).c_str(), "None");

}

void DisplayTextAction::DrawUISettings()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Display Text Settings: ");
	ImGui::PopFont();
}

void DisplayTextAction::RenderText()
{
	// Push Parent View Matrix ----------------
	if (parentObject->transform != nullptr)
	{
		float2 appliedArPos = parentObject->transform->GetPosition(true);
		parentObject->transform->SetPosition(appliedArPos);

		// Parent Object View Matrix 
		float4x4 objectViewMatrix = parentObject->transform->CalculateViewMatrix();

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)(objectViewMatrix.Transposed()).v);

		float2 unAppliedArPos = parentObject->transform->GetPosition() / App->moduleImGui->gameViewportDockPanel->GetAspectRatio();
		parentObject->transform->SetPosition(unAppliedArPos);
	}

	// Iterate through all characters ---------
	std::string::const_iterator currentLetter;
	int x = textBox->GetMinPoint().x; 
	int y = textBox->GetMaxPoint().y; 

	float2 cursorPos = originTextPosition;

	int currentLine = 0; 
	int letterCount = 0;
	for (currentLetter = text.begin(); currentLetter != text.end(); currentLetter++)
	{
		if (textFont == nullptr)
		{
			FLY_ERROR("A text with no font can not be rendered");
			assert(false);
		}

		// Get The Current Character 
		Character currentCharacter = textFont->GetCharacter(*currentLetter);

		// Get The Corresponding Quad 
		Quad* renderQuad = textQuads[letterCount];

		// Push Matrix to place the Corresponding quad in the correct position
		float4x4 characterTransformMatrix = float4x4::identity;
		cursorPos.y = y - currentCharacter.bearing.y + textFont->GetSize();
		characterTransformMatrix.SetTranslatePart(float3(cursorPos.x, cursorPos.y, 0));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)(characterTransformMatrix.Transposed()).v);

		cursorPos.x += currentCharacter.Advance;

		// Draw the quad with the correct texture 
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, renderQuad->verticesID);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (currentCharacter.textureID != 0) {

			glEnable(GL_TEXTURE_2D);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, renderQuad->uvsID);

			glBindTexture(GL_TEXTURE_2D, currentCharacter.textureID);

			glTexCoordPointer(3, GL_FLOAT, 0, NULL);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderQuad->indicesID);
		glColor4f(textColor.x, textColor.y, textColor.z, textColor.w);
		glDrawElements(GL_TRIANGLES, renderQuad->numIndices, GL_UNSIGNED_INT, NULL);

		if (currentCharacter.textureID != 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (currentCharacter.textureID != 0)
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		letterCount++;

	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DisplayTextAction::CalculateOriginTextPosition()
{
	if (textBox == nullptr)
	{
		FLY_ERROR("Text Position can no be calculated without a Text Box"); 
		assert(false); 
	}

	originTextPosition = float2(textBox->GetMinPoint().x, textBox->GetMaxPoint().y);
}

void DisplayTextAction::SetText(std::string newText)
{
	text = newText;
	
	AllocateTextQuads(newText.size()); 
	UpdateTextQuadsSize();
	CalculateOriginTextPosition(); 
}

std::string& DisplayTextAction::GetText()
{
	return text; 
}

void DisplayTextAction::SetFont(Font* newFont)
{
	if (newFont == nullptr)
	{
		FLY_ERROR("New Font Could Not Be Found"); 
		assert(false); 
	}

	textFont = newFont; 
}

Font* DisplayTextAction::GetFont()
{
	return textFont;
}

void DisplayTextAction::SetTextBoxSize(BoundingBox* newFont)
{
}

BoundingBox* DisplayTextAction::GetTextBox()
{
	return textBox;
}

void DisplayTextAction::SetTextColor(float4 newColor)
{
	textColor = newColor;
}

float4& DisplayTextAction::GetTextColor()
{
	return textColor; 
}

void DisplayTextAction::AllocateTextQuads(int amount, int position)
{
	if (position < 0) // Last Letter
		position = text.size();

	if (amount > quadsAllocated)
		amount -= quadsAllocated; 
	
	int counter = 0;
	while (counter < amount)
	{
		Quad* newQuad = new Quad();
		newQuad->Create(1, 1, true);

		textQuads.insert(std::pair<int, Quad*>(counter, newQuad));
		quadsAllocated++;
		counter++;
	}
}

void DisplayTextAction::UpdateTextQuadsSize()
{
	std::string::const_iterator currentLetter;

	int letterCount = 0;
	for (currentLetter = text.begin(); currentLetter != text.end(); currentLetter++)
	{
		Character currentCharacter = textFont->GetCharacter(*currentLetter);

		if (textQuads[letterCount] != nullptr)
		{
			textQuads[letterCount]->CleanUp();
			delete textQuads[letterCount];
		}

		textQuads[letterCount] = new Quad();

		if (textQuads[letterCount] != nullptr)
			textQuads[letterCount]->CreateLiteralSize(currentCharacter.size.x, currentCharacter.size.y, true);

		letterCount++;
	}
}


