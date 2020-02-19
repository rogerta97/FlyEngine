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
	drawTextBox = true;

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

	if (isSelected && drawTextBox)
		DrawTextBox(); 
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
	if (ImGui::CollapsingHeader("Display Text Attributes", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Action Happens On:");
		ImGui::PopFont();

		ImGui::PushFont(App->moduleImGui->rudaRegularMid);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 70));

		ImGui::SetCursorPos(ImVec2(5, 8));
		ImGui::Checkbox("On Mouse Over", &IsOccMouseOver());

		ImGui::SetCursorPos(ImVec2(5, 38));
		ImGui::Checkbox("Blackboard Value Condition", &IsOccCondition());

		ImGui::SameLine();
		static std::string showValueConditionButtonText = "Show Conditions";
		if (ImGui::Button(showValueConditionButtonText.c_str()))
		{
			if (showConditionsUI)
			{
				showConditionsUI = false;
				showValueConditionButtonText = "Show Conditions";
			}
			else
			{
				showConditionsUI = true;
				showValueConditionButtonText = "Hide Conditions";
			}
		}

		if (showConditionsUI)
			DrawActionConditionsList();

		ImGui::Spacing();
		ImGui::EndChild();

		ImGui::PopFont();
		ImGui::PopStyleColor();

		static char textBuffer[256];

		if (!GetText().empty())
			strcpy(textBuffer, GetText().c_str());

		IMGUI_SPACED_SEPARATOR;

		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Text Settings:");
		ImGui::PopFont();

		// Text Field -----------------------
		if (ImGui::InputText("Text##DisplayActionText", textBuffer, IM_ARRAYSIZE(textBuffer)))
		{
			SetText(textBuffer);
		}

		// Color Field -----------------------
		ImGui::ColorEdit4("Color", (float*)& GetTextColor());

		// Size Field -----------------------
		if (ImGui::InputInt("Size", &GetFont()->GetSize(), 1, 5))
		{
			GetFont()->SetSize(GetFont()->GetSize());
			UpdateTextQuadsSize();
		}

		Font* actionFont = GetFont();

		char actionFontNameBuffer[256];
		if (actionFont != nullptr)
		{
			strcpy(actionFontNameBuffer, actionFont->GetName().c_str());
		}
		else
		{
			FLY_ERROR("The action has no font assigned");
			assert(false);
		}

		// Font Field -----------------------
		string buttonString = "Find##FindFont" + to_string(actionFont->GetUID());
		if (ImGui::Button(buttonString.c_str()))
		{
			ImGui::OpenPopup("print_font_selection_popup");
		}

		Font* fontSelected = (Font*)ResourceManager::getInstance()->PrintFontSelectionPopup();
		if (fontSelected != nullptr)
		{
			SetFont(fontSelected);
		}

		ImGui::SameLine();
		if (ImGui::InputText("Font", actionFontNameBuffer, IM_ARRAYSIZE(actionFontNameBuffer), ImGuiInputTextFlags_ReadOnly))
		{
			GetFont()->SetSize(GetFont()->GetSize());
			UpdateTextQuadsSize();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("drag_resource"))
			{
				int* selectedResourceUID = (int*)payload->Data;
				Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

				if (resourceDropped->GetType() == RESOURCE_FONT)
				{
					Font* fontDropped = (Font*)resourceDropped;
					SetFont(fontDropped);
				}
			}

			ImGui::EndDragDropTarget();
		}

		// Text Box Settings ------------------
		IMGUI_SPACED_SEPARATOR;

		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Text Box Settings:");
		ImGui::PopFont();

		// Box Position 
		float2 boxPos = GetTextBox()->GetPosition();
		if (ImGui::DragFloat2("Position", (float*)& boxPos, 2))
		{
			GetTextBox()->SetPosition(float2(boxPos.x, boxPos.y));
			CalculateOriginTextPosition();
		}

		// Box Width & Heigth
		float2 boxSize = GetTextBox()->GetSize();
		if (ImGui::DragFloat2("Width & Heigth", (float*)& boxSize, 2))
		{
			GetTextBox()->SetSize(boxSize.x, boxSize.y);
			GetTextBox()->SetPosition(float2(boxPos.x, boxPos.y));
			CalculateOriginTextPosition();
		}

		ImGui::Checkbox("Draw Box", &GetDrawTextBox());
	}
}

void DisplayTextAction::DrawUISettingsInButton()
{	
	if (ImGui::CollapsingHeader("Display Text Attributes", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static char textBuffer[256];

		if (!GetText().empty())
			strcpy(textBuffer, GetText().c_str());

		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Text Settings:");
		ImGui::PopFont();

		// Text Field -----------------------
		if (ImGui::InputText("Text##DisplayActionText", textBuffer, IM_ARRAYSIZE(textBuffer)))
		{
			SetText(textBuffer);
		}

		// Color Field -----------------------
		ImGui::ColorEdit4("Color", (float*)& GetTextColor());

		// Size Field -----------------------
		if (ImGui::InputInt("Size", &GetFont()->GetSize(), 1, 5))
		{
			GetFont()->SetSize(GetFont()->GetSize());
			UpdateTextQuadsSize();
		}

		Font* actionFont = GetFont();

		char actionFontNameBuffer[256];
		if (actionFont != nullptr)
		{
			strcpy(actionFontNameBuffer, actionFont->GetName().c_str());
		}
		else
		{
			FLY_ERROR("The action has no font assigned");
			assert(false);
		}

		// Font Field -----------------------
		string buttonString = "Find##FindFont" + to_string(actionFont->GetUID());
		if (ImGui::Button(buttonString.c_str()))
		{
			ImGui::OpenPopup("print_font_selection_popup");
		}

		Font* fontSelected = (Font*)ResourceManager::getInstance()->PrintFontSelectionPopup();
		if (fontSelected != nullptr)
		{
			SetFont(fontSelected);
		}

		ImGui::SameLine();
		if (ImGui::InputText("Font", actionFontNameBuffer, IM_ARRAYSIZE(actionFontNameBuffer), ImGuiInputTextFlags_ReadOnly))
		{
			GetFont()->SetSize(GetFont()->GetSize());
			UpdateTextQuadsSize();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("drag_resource"))
			{
				int* selectedResourceUID = (int*)payload->Data;
				Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

				if (resourceDropped->GetType() == RESOURCE_FONT)
				{
					Font* fontDropped = (Font*)resourceDropped;
					SetFont(fontDropped);
				}
			}

			ImGui::EndDragDropTarget();
		}

		// Text Box Settings ------------------
		IMGUI_SPACED_SEPARATOR;

		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Text Box Settings:");
		ImGui::PopFont();

		// Box Position 
		float2 boxPos = GetTextBox()->GetPosition();
		if (ImGui::DragFloat2("Position", (float*)& boxPos, 2))
		{
			GetTextBox()->SetPosition(float2(boxPos.x, boxPos.y));
			CalculateOriginTextPosition();
		}

		// Box Width & Heigth
		float2 boxSize = GetTextBox()->GetSize();
		if (ImGui::DragFloat2("Width & Heigth", (float*)& boxSize, 2))
		{
			GetTextBox()->SetSize(boxSize.x, boxSize.y);
			GetTextBox()->SetPosition(float2(boxPos.x, boxPos.y));
			CalculateOriginTextPosition();
		}

		ImGui::Checkbox("Draw Box", &GetDrawTextBox());
	}
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

void DisplayTextAction::DrawTextBox()
{
	if (textBox == nullptr)
	{
		FLY_ERROR("Cant draw text box"); 
		assert(false);
		return; 
	}

	textBox->Draw(false, float4(0, 1.0f, 0, 1.0f)); 
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
	SetText(text);
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

void DisplayTextAction::SetDrawTextBox(bool _draw)
{
	drawTextBox = _draw;
}

bool& DisplayTextAction::GetDrawTextBox()
{
	return drawTextBox;
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
	textQuads.clear();

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


