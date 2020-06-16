#include "DisplayTextAction.h"
#include "Quad.h"
#include "OpenGL.h"
#include "Room.h"
#include "Font.h"
#include "FlyObject.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "Quad.h"
#include "Gizmos.h"
#include "GameViewportDockPanel.h"
#include "FontImporter.h"
#include "ResourceManager.h"
#include "ViewportManager.h"

#include "mmgr.h"

DisplayTextAction::DisplayTextAction(FlyObject* _parentObject)
{
	actionType = ACTION_DISPLAY_TEXT;
	parentObject = _parentObject;
	isVisual = false;
	drawTextBox = true;
	pevBoxPos = float2::zero; 
	textBBMinPoint = float2(0, -0); 
	textBBMaxPoint = float2(-0, 0); 

	textQuads = new std::vector<Quad*>();

	textBox = new BoundingBox();
	textBox->CreateGizmos();

	textBoundingBox = new BoundingBox();
	textBoundingBox->SetSize(0, 0);
	textBoundingBoxColor = float4(1.0f, 0.0f, 0.0f, 1.0f); 

	textAlignment = ALIGN_CENTER; 

	textBox->SetSize(300, 120); 
	CalculateOriginTextPosition(); 

	displayTextBox = true; 
	displayTextBB = true; 

	originTextPosition = float2(0, 0);

	Font* tmptextFont = (Font*)ResourceManager::GetResource("arial", RESOURCE_FONT);

	textFont = new Font(); 
	textFont = FontImporter::getInstance()->LoadFont(tmptextFont->GetPath()); 

	SetText("");
	SetLineSpacing(textFont->GetSize()); 
	textColor = float4(1.0f, 1.0f, 1.0f, 1.0f); 

	SetActionName("Display Text");
	SetToolDescription("This should be the description of display text");
}

DisplayTextAction::~DisplayTextAction()
{

}

void DisplayTextAction::Draw()
{
	if (!App->isEngineInPlayMode && parentObject->isSelected && drawTextBox)
	{
		if(displayTextBox)
			DrawTextBox(); 

		/*if(displayTextBB)
			DrawTextBoundingBox(); */
	}

	if (!text.empty())
	{
		if (App->isEngineInPlayMode && IsOccMouseOver() && !parentObject->GetClickableArea()->IsMouseOver())
			return;

		RenderText(); 
	}
}

void DisplayTextAction::Update(float dt)
{
	if (textBox->IsBoxClicked() && parentObject->isSelected)
	{
		parentObject->SetSelectedAction(uid);
		ViewportManager::getInstance()->blockInputTick = true;
	}

	if (isSelected)
	{
		if (textBox->UpdateGizmos())
		{
			CalculateOriginTextPosition();
		}
	}
}

void DisplayTextAction::CleanUp()
{
	for (auto& currentQuad : *textQuads)
	{
		currentQuad->CleanUp(); 
		delete currentQuad;
	}

	textBox->CleanUp(); 
	delete textBox;

	textFont->CleanUp();
	delete textFont;

	textQuads->clear();
	delete textQuads;
}

void DisplayTextAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr, int actionPositionInObject)
{
	string toolsSerializeSection;

	if (!literalStr)
		toolsSerializeSection = serializeObjectString + string("Actions.Action_") + to_string(actionPositionInObject) + ".";
	else
		toolsSerializeSection = serializeObjectString;

	Action::SaveAction(jsonObject, toolsSerializeSection);
	json_object_dotset_boolean(jsonObject, string(toolsSerializeSection + "Occurrence.MouseOver").c_str(), IsOccMouseOver());

	Action::SaveOccurrence(jsonObject, toolsSerializeSection);

	// Save Bounding Box Position 
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("TextBox.MinPoint.x")).c_str(), textBox->GetMinPoint().x);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("TextBox.MinPoint.y")).c_str(), textBox->GetMinPoint().y);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("TextBox.MaxPoint.x")).c_str(), textBox->GetMaxPoint().x);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("TextBox.MaxPoint.y")).c_str(), textBox->GetMaxPoint().y);

	// Save Text String
	if (text.empty())	
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("Text")).c_str(), "None");	
	else	
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("Text")).c_str(), text.c_str());	

	// Save Text Font 
	if(textFont != nullptr)
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("FontName")).c_str(), textFont->GetName().c_str());
	else
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("FontName")).c_str(), "None");

	// Save Text Size 
	if (textFont != nullptr)
		json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("Size")).c_str(), textFont->GetSize());
	else
		json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("Size")).c_str(), 0);
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
		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 35));

		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::Checkbox("Draw when mouse is over", &IsOccMouseOver());

	/*	ImGui::SetCursorPos(ImVec2(5, 35));
		ImGui::Checkbox("Blackboard Value Condition", &IsOccCondition());*/

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
			UpdateTextQuads();
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
			UpdateTextQuads();
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
		float showPositionArr[2] = { GetTextBox()->GetPosition().x,  GetTextBox()->GetPosition().y };

		if (ImGui::DragFloat2("Position", showPositionArr, 2))
		{
			//float2 inc = boxPos - prevPos;
			//inc *= ViewportManager::getInstance()->GetAspectRatio();
			//boxPos *= ViewportManager::getInstance()->GetAspectRatio();
			GetTextBox()->SetPosition(float2(showPositionArr[0], showPositionArr[1]));
		}

		// Box Width & Heigth
		float2 boxSize = GetTextBox()->GetSize();
		if (ImGui::DragFloat2("Width & Heigth", (float*)& boxSize, 2))
		{
			GetTextBox()->SetSize(boxSize.x, boxSize.y);
			GetTextBox()->SetPosition(float2(showPositionArr[0], showPositionArr[1]));
			GetTextBox()->CalculateAllGizmos();
			CalculateOriginTextPosition();
		}
		
		if (!GetTextBox()->GetPosition().Equals(pevBoxPos))
		{
			pevBoxPos = GetTextBox()->GetPosition(); 
			GetTextBox()->CalculateAllGizmos();
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
			UpdateTextQuads();
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
			UpdateTextQuads();
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

void DisplayTextAction::RenderText(bool setup, float bbOffset)
{
	//textBox->DrawSquare(); 

	// Push Parent View Matrix ----------------
	if (parentObject != nullptr && parentObject->transform != nullptr)
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

	float2 pen = originTextPosition;

	int currentLine = 0; 
	int letterCount = 0;
	int cursorXInc = 0; 
	int cursorYInc = 0; 

	FT_Bool use_kerning = FT_HAS_KERNING(textFont->fontFace);
	FT_UInt previous = 0;

	textBBMinPoint = float2(1000, -1000);
	textBBMaxPoint = float2(-1000, 1000);

	for (currentLetter = text.begin(); currentLetter != text.end(); currentLetter++)
	{
		if (textFont == nullptr)
		{
			FLY_ERROR("A text with no font can not be rendered");
			assert(false);
		}

		// Get The Current Character 
		Character currentCharacter = textFont->GetCharacter(*currentLetter);

		if (cursorYInc > textBox->GetSize().y - 30)
		{
			if (pen.x < textBBMinPoint.x)
				textBBMinPoint.x = pen.x;

			if (pen.y + currentCharacter.size.y > textBBMinPoint.y)
				textBBMinPoint.y = pen.y + currentCharacter.size.y;

			continue;
		}

		/* convert character code to glyph index */
		FT_UInt glyph_index = FT_Get_Char_Index(textFont->fontFace, (*currentLetter));

		/* retrieve kerning distance and move pen position */
		if (use_kerning && previous && glyph_index)
		{
			FT_Vector  delta;

			FT_Get_Kerning(textFont->fontFace, previous, glyph_index,
				0, &delta);

			pen.x += delta.x >> 6;
		}

		previous = glyph_index;

		// Get The Corresponding Quad 
		Quad* renderQuad = textQuads->at(letterCount);

		// Push Matrix to place the Corresponding quad in the correct position
		float4x4 characterTransformMatrix = float4x4::identity;
		pen.y = y - currentCharacter.bearing.y + textFont->GetSize() + (currentLine * lineSpacing);
		characterTransformMatrix.SetTranslatePart(float3(pen.x, pen.y, 0));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)(characterTransformMatrix.Transposed()).v);

		float offsetBBLetter = 0; 

		if (*currentLetter == text.back())
			offsetBBLetter = currentCharacter.size.x;

		// Text Bounding Box
		if (pen.x + offsetBBLetter > textBBMaxPoint.x)
			textBBMaxPoint.x = pen.x + offsetBBLetter;

		if (pen.y < textBBMaxPoint.y)
			textBBMaxPoint.y = pen.y;

		if (pen.x < textBBMinPoint.x)
			textBBMinPoint.x = pen.x;

		if (pen.y + currentCharacter.size.y > textBBMinPoint.y)
			textBBMinPoint.y = pen.y + currentCharacter.size.y;

		pen.x += currentCharacter.Advance;

		// Supose Wrapping = true; 
		cursorXInc += currentCharacter.Advance;

		if (cursorXInc > textBox->GetSize().x - 30)
		{
			if (pen.x + offsetBBLetter > textBBMaxPoint.x)
				textBBMaxPoint.x = pen.x + offsetBBLetter;

			if (pen.y < textBBMaxPoint.y)
				textBBMaxPoint.y = pen.y;

			currentLine++;
			pen.x = textBox->GetMinPoint().x;
			cursorXInc = 0; 
			cursorYInc += lineSpacing; 
		}

		if (cursorYInc > textBox->GetSize().y - 30)
		{
			if (pen.x < textBBMinPoint.x)
				textBBMinPoint.x = pen.x;

			if (pen.y + currentCharacter.size.y > textBBMinPoint.y)
				textBBMinPoint.y = pen.y + currentCharacter.size.y;

			continue; 
		}


		if (setup)
			continue;

		// Draw the quad with the correct texture 
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, renderQuad->verticesID);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (currentCharacter.textureID != 0) 
		{
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
/*
		textBBMinPoint.y = pen.y + (lastChar.size.y / 
	Character lastChar = textFont->GetCharacter(text.back());

	if (pen.x + (lastChar.size.x / 64) > textBBMaxPoint.x)
		textBBMaxPoint.x = pen.x + (lastChar.size.x / 64);

	if (pen.y < textBBMaxPoint.y)
		textBBMaxPoint.y = pen.y;

	if (pen.x < textBBMinPoint.x)
		textBBMinPoint.x = pen.x;
	
	if (pen.y + (lastChar.size.y / 64) > textBBMinPoint.y)64);*/

	textBoundingBox->SetMaxPoint(textBBMaxPoint + float2(bbOffset, -bbOffset));
	textBoundingBox->SetMinPoint(textBBMinPoint + float2(-bbOffset, bbOffset));

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	SetActionCompleted(true); 
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

	if (isSelected)
		textBox->DrawBoxGizmos(); 
}

void DisplayTextAction::DrawTextBoundingBox(bool fill)
{
	textBoundingBox->Draw(fill, textBoundingBoxColor);
}

void DisplayTextAction::CleanQuads()
{
	for (auto& it : *textQuads)
	{
		it->CleanUp(); 
		delete it; 
	}

	textQuads->clear(); 
}

void DisplayTextAction::CalculateTextBB()
{
	std::string::const_iterator currentLetter;
	int x = textBox->GetMinPoint().x;
	int y = textBox->GetMaxPoint().y;

	float2 pen = originTextPosition;

	int currentLine = 0;
	int letterCount = 0;
	int cursorXInc = 0;
	int cursorYInc = 0;

	FT_Bool use_kerning = FT_HAS_KERNING(textFont->fontFace);
	FT_UInt previous = 0;

	for (currentLetter = text.begin(); currentLetter != text.end(); currentLetter++)
	{
		if (textFont == nullptr)
		{
			FLY_ERROR("A text with no font can not be rendered");
			assert(false);
		}

		// Get The Current Character 
		Character currentCharacter = textFont->GetCharacter(*currentLetter);

		/* convert character code to glyph index */
		FT_UInt glyph_index = FT_Get_Char_Index(textFont->fontFace, (*currentLetter));
	
		/* retrieve kerning distance and move pen position */
		if (use_kerning && previous && glyph_index)
		{
			FT_Vector  delta;

			FT_Get_Kerning(textFont->fontFace, previous, glyph_index,
				0, &delta);

			pen.x += delta.x >> 6;
		}

		previous = glyph_index;
	}
}

void DisplayTextAction::CalculateOriginTextPosition()
{
	if (textBox == nullptr)
	{
		FLY_ERROR("Text Position can no be calculated without a Text Box"); 
		assert(false); 
	}

	if(textAlignment == ALIGN_TOP_LEFT)
		originTextPosition = float2(textBox->GetMinPoint().x, textBox->GetMinPoint().y + (textBox->GetSize().y / 2) + (textBoundingBox->GetSize().y / 2));
	else
		originTextPosition = float2(textBox->GetMinPoint().x, textBox->GetMaxPoint().y);
}

void DisplayTextAction::SetText(std::string newText)
{
	text = newText;
	
	int quadsToAllocate = 0; 
	
	if (quadsAllocated <= newText.size())
		quadsToAllocate = newText.size() - quadsAllocated;

	UpdateTextQuads();
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

BoundingBox* DisplayTextAction::GetTextBB()
{
	return textBoundingBox;
}

void DisplayTextAction::SetTextColor(float4 newColor)
{
	textColor = newColor;
}

float4& DisplayTextAction::GetTextColor()
{
	return textColor; 
}

void DisplayTextAction::SetTextBBColor(float4 newColor)
{
	textBoundingBoxColor = newColor;
}

float4& DisplayTextAction::GetTextBBColor()
{
	return textBoundingBoxColor; 
}

void DisplayTextAction::SetDrawTextBox(bool _draw)
{
	drawTextBox = _draw;
}

bool& DisplayTextAction::GetDrawTextBox()
{
	return drawTextBox;
}

void DisplayTextAction::SetLineSpacing(float _lineSpacing)
{
	lineSpacing = _lineSpacing;
}

float& DisplayTextAction::GetLineSpacing()
{
	return lineSpacing; 
}

void DisplayTextAction::UpdateTextQuads()
{
	std::string::const_iterator currentLetter;
	CleanQuads(); 

	int letterCount = 0;
	for (currentLetter = text.begin(); currentLetter != text.end(); currentLetter++)
	{
		Character currentCharacter = textFont->GetCharacter(*currentLetter);

		Quad* newQuad = new Quad();
		newQuad->CreateLiteralSize(currentCharacter.size.x, currentCharacter.size.y, true);	
		textQuads->push_back(newQuad); 

		letterCount++;
	}
}


