#include "Blackboard.h"
#include "RandomNumberGenerator.h"
#include "ModifyVariableAction.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "FlyVariable.h"

#include "imgui.h"

Blackboard::Blackboard()
{
}

Blackboard::~Blackboard()
{
}

FlyVariable* Blackboard::DrawVariableListPopup()
{
	if (ImGui::BeginPopup("search_variable_popup"))
	{
		static char searchVarBuffer[256];
		ImGui::InputTextWithHint("", "Search...", searchVarBuffer, IM_ARRAYSIZE(searchVarBuffer)); 

		ImGui::Separator(); 

		for (auto& currentVariable : this->blackboardVariablesList)
		{
			Texture* varTexture = 0; 

			if (currentVariable->varType == Var_Integer)
				varTexture = (Texture*)ResourceManager::getInstance()->GetResource("NaturalNumberIcon");
			else if (currentVariable->varType == Var_Toggle)
				varTexture = (Texture*)ResourceManager::getInstance()->GetResource("ToggleIcon");

			ImGui::Image((ImTextureID)varTexture->GetTextureID(), ImVec2(20, 20));
			ImGui::SameLine(); 

			if (ImGui::Selectable(currentVariable->name.c_str()))
			{
				ImGui::EndPopup(); 
				return currentVariable; 
			}
		}

		ImGui::EndPopup();
	}

	return nullptr; 
}

FlyVariable* Blackboard::AddDefaultVariable()
{
	FlyVariable* defaultVar = new FlyVariable(); 
	defaultVar->SetDefault();
	blackboardVariablesList.push_back(defaultVar); 
	return defaultVar;
}

FlyVariable* Blackboard::GetVariable(std::string name)
{
	for (auto& currentVariable : this->blackboardVariablesList)
	{
		if (currentVariable->name == name)
			return currentVariable;
	}

	return nullptr; 
}
