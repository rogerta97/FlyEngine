#include "DisplayTextAction.h"
#include "Quad.h"
#include "OpenGL.h"
#include "Font.h"
#include "FlyObject.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"
#include "ResourceManager.h"
#include "imgui.h"

#include "mmgr.h"

DisplayTextAction::DisplayTextAction(FlyObject* _parentObject)
{
	actionType = ACTION_DISPLAY_TEXT;
	parentObject = _parentObject;
	isVisual = false;

	textFont = (Font*)ResourceManager::GetResource("arial", RESOURCE_FONT);
	SetText("AAA");

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
	//glActiveTexture(GL_TEXTURE0);
	//glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator currentLetter;
	int x, y; 
	x = y = 0; 

	int letterCount = 0;
	for (currentLetter = text.begin(); currentLetter != text.end(); currentLetter++)
	{
		if (textFont == nullptr)
		{
			FLY_ERROR("A text with no font can not be rendered"); 
			assert(false);
		}

		Character currentCharacter = textFont->GetCharacter(*currentLetter);

		//GLfloat xpos = x + ch.Bearing.x * scale;
		//GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat xpos = x + currentCharacter.bearing.x;
		GLfloat ypos = y - (currentCharacter.size.y - currentCharacter.bearing.y);

		//GLfloat w = currentCharacter.size.x * scale;
		//GLfloat h = currentCharacter.size.y * scale;

		GLfloat w = currentCharacter.size.x;
		GLfloat h = currentCharacter.size.y;

		Quad* renderQuad = textQuads[letterCount];

	/*	renderQuad->SetWidth(w); 
		renderQuad->SetHeight(h); */

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, renderQuad->verticesID);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (parentObject->transform != nullptr)
		{
			float2 appliedArPos = parentObject->transform->GetPosition(true);
			parentObject->transform->SetPosition(appliedArPos);

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf((GLfloat*)((parentObject->transform->CalculateViewMatrix()).Transposed()).v);

			float2 unAppliedArPos = parentObject->transform->GetPosition() / App->moduleImGui->gameViewportDockPanel->GetAspectRatio();
			parentObject->transform->SetPosition(unAppliedArPos);
		}

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

		//// Update VBO for each character
		//GLfloat vertices[6][4] = {
		//	{ xpos,     ypos + h,   0.0, 0.0 },
		//	{ xpos,     ypos,       0.0, 1.0 },
		//	{ xpos + w, ypos,       1.0, 1.0 },

		//	{ xpos,     ypos + h,   0.0, 0.0 },
		//	{ xpos + w, ypos,       1.0, 1.0 },
		//	{ xpos + w, ypos + h,   1.0, 0.0 }
		//};

		//// Render glyph texture over quad
		//glBindTexture(GL_TEXTURE_2D, currentCharacter.textureID);

		//// Update content of VBO memory
		//glBindBuffer(GL_ARRAY_BUFFER, textQuads[letterCount]->verticesID);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		//// Render quad
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		//// x += (currentCharacter.Advance >> 6) * scale; Bitshift by 6 to get value in pixels (2^6 = 64)
		
		x += (currentCharacter.Advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
		letterCount++; 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DisplayTextAction::SetText(std::string newText)
{
	AllocateTextQuads(newText.size()); 
	text = newText;
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

void DisplayTextAction::AllocateTextQuads(int amount, int position)
{
	if (position < 0) // Last Letter
		position = text.size();
	
	int counter = 0;
	while (counter < amount)
	{
		Quad* newQuad = new Quad();
		newQuad->Create(1, 1);

		textQuads.insert(std::pair<int, Quad*>(position + counter, newQuad));
		quadsAllocated++;
		counter++;
	}
}


