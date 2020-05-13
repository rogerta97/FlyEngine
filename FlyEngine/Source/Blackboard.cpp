#include "Blackboard.h"
#include "RandomNumberGenerator.h"
#include "ModifyVariableAction.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "FlyVariable.h"
#include "ModuleImGui.h"
#include "MyFileSystem.h"
#include "Application.h"
#include "ModuleWorldManager.h"

#include "imgui.h"
#include "mmgr.h"

Blackboard::Blackboard(bool _isGlobal)
{
	isGlobal = _isGlobal;
	showGlobalsOnListPopup = true; 
}

Blackboard::~Blackboard()
{
}

void Blackboard::CleanUp()
{
	for (auto& currentVariable : blackboardVariablesList)
	{
		currentVariable->CleanUp();
		delete currentVariable; 
		currentVariable = nullptr;
	}

	blackboardVariablesList.clear(); 
}

void Blackboard::SaveData(std::string _fileName)
{
	std::string path = ""; 
	
	if (isGlobal || _fileName == "Global")
	{
		path = MyFileSystem::getInstance()->GetSavedDataDirectory() + "BlackboardsData\\GlobalBlackboard.json"; 
	}
	else
	{
		path = MyFileSystem::getInstance()->GetSavedDataDirectory() + "BlackboardsData\\" + _fileName.c_str() + "_Blackboard.json";
	}

	JSON_Value* blackboard_v = json_value_init_object();
	JSON_Object* blackboard_obj = json_value_get_object(blackboard_v);

	json_object_dotset_number(blackboard_obj, "VariablesAmount", blackboardVariablesList.size()); 

	int count = 0; 
	for (auto& currentVariable : blackboardVariablesList)
	{
		std::string baseObjectStr = "FlyVariable_" + to_string(count++) + "."; 
		currentVariable->Serialize(blackboard_obj, baseObjectStr);
	}

	json_serialize_to_file(blackboard_v, path.c_str());
}

void Blackboard::LoadData(std::string _fileName)
{
	std::string filePath; 
	if (isGlobal || _fileName == "Global")
	{
		filePath = MyFileSystem::getInstance()->GetSavedDataDirectory() + "BlackboardsData\\GlobalBlackboard.json";
	}
	else
	{
		filePath = MyFileSystem::getInstance()->GetSavedDataDirectory() + "BlackboardsData\\" + _fileName.c_str() + "_Blackboard.json";
	}

	JSON_Value* root = json_parse_file(filePath.c_str());
	JSON_Object* root_obj = json_value_get_object(root);

	int obj_ammount = json_object_dotget_number(root_obj, "VariablesAmount");

	int counter = 0;
	while (counter < obj_ammount)
	{
		string serializeObjectStr = "FlyVariable_" + to_string(counter) + string(".");
		FlyVariable* newVariable = new FlyVariable(); 

		std::string currentFieldObjStr = serializeObjectStr + "VariableName"; 
		newVariable->name = json_object_dotget_string(root_obj, currentFieldObjStr.c_str());

		currentFieldObjStr = serializeObjectStr + "VariableType";
		int varTypeTmp = json_object_dotget_number(root_obj, currentFieldObjStr.c_str());
		newVariable->varType = (VariableType)varTypeTmp; 

		currentFieldObjStr = serializeObjectStr + "IntegerValue";
		newVariable->varIntegerValue = json_object_dotget_number(root_obj, currentFieldObjStr.c_str());

		currentFieldObjStr = serializeObjectStr + "ToggleValue";
		newVariable->varToogleValue = json_object_dotget_boolean(root_obj, currentFieldObjStr.c_str());

		currentFieldObjStr = serializeObjectStr + "UID";
		newVariable->uniqueID = json_object_dotget_number(root_obj, currentFieldObjStr.c_str());

		blackboardVariablesList.push_back(newVariable); 

		counter++;
	}
}

void Blackboard::ModifyIntegerVariable(ModifyVariableEffect* modifyVariableEffect)
{
	// Sanity Checks 
	std::string targetVarName = modifyVariableEffect->targetVariable->name; 
	FlyVariable* targetVar = GetVariable(targetVarName);

	if (targetVar == nullptr)
		return; 

	modifyVariableEffect->ApplyEffect(); 
}

FlyVariable* Blackboard::DrawVariableListPopup(std::string popupID)
{
	if (ImGui::BeginPopup(popupID.c_str()))
	{
		static char searchVarBuffer[256];

		ImGui::SetNextItemWidth(200);
		ImGui::InputTextWithHint("", "Search...", searchVarBuffer, IM_ARRAYSIZE(searchVarBuffer)); 

		ImGui::SameLine();

		ImGui::Checkbox("Globals", &showGlobalsOnListPopup);

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

		if (showGlobalsOnListPopup)
		{
			for (auto& currentVariable : App->moduleWorldManager->globalBlackboard->blackboardVariablesList)
			{
				Texture* varTexture = 0;

				if (currentVariable->varType == Var_Integer)
					varTexture = (Texture*)ResourceManager::getInstance()->GetResource("NaturalNumberIcon");
				else if (currentVariable->varType == Var_Toggle)
					varTexture = (Texture*)ResourceManager::getInstance()->GetResource("ToggleIcon");

				ImGui::Image((ImTextureID)varTexture->GetTextureID(), ImVec2(20, 20));
				ImGui::SameLine();

				ImGui::PushFont(App->moduleImGui->rudaBlackMid);
				if (ImGui::Selectable(currentVariable->name.c_str()))
				{
					ImGui::PopFont();
					ImGui::EndPopup();
					return currentVariable;
				}
				ImGui::PopFont();
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
