#include "Room.h"

#include <fstream>

#include "Globals.h"
#include "NodeGraph.h"
#include "ViewportManager.h"
#include "RandomNumberGenerator.h"
#include "Application.h"
#include "imnodes.h"
#include "ModuleInput.h"
#include "ResourceManager.h"
#include "ObjectCreatorDockPanel.h"
#include "FlyObject.h"
#include "Texture.h"
#include "GameViewportDockPanel.h"
#include "ObjectPropertiesDockPanel.h"
#include "GameInventory.h"
#include "ModuleImGui.h"
#include "MusicTrack.h"
#include "TextureMSAA.h"
#include "FlyObjectCharacter.h"
#include "FlyVariable.h"
#include "ObjectPropertiesDockPanel.h"
#include "ModuleWorldManager.h"
#include "MyFileSystem.h"
#include "Blackboard.h"
#include "RoomUIHandler.h"

#include "mmgr.h"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

Room::Room(string roomName)
{
	this->roomName = roomName;
	roomID = RandomNumberGenerator::GenerateUID(); 
	backgroundMusic = nullptr; 
	isStart = false; 

	roomThumbnail = 0; 
	roomBlackboard = new Blackboard(); 
	roomUIHandler = new RoomUIHandler(this); 	

	
	Texture* thumbnailTexture = (Texture*)ResourceManager::getInstance()->GetResource("EmptyObject");
	roomThumbnail = thumbnailTexture;

}

Room::~Room()
{
	roomUIHandler->CleanUp();
	delete roomUIHandler;
	roomUIHandler = nullptr;
}

void Room::Update()
{
	if (App->moduleWorldManager->GetSelectedRoom() != nullptr && App->moduleWorldManager->GetSelectedRoom()->GetUID() != GetUID())
		return; 

	if (ViewportManager::getInstance()->blockInputTick)
	{
		ViewportManager::getInstance()->blockInputTick = false;
		return;
	}

	if (ViewportManager::getInstance()->blockInput)
		return; 

	if (App->isEngineInPlayMode && App->moduleInput->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		flog("OPsd");
		GameInventory::getInstance()->ToggleVisibility();
	}

	if (App->moduleWorldManager->GetSelectedRoom() == this)
	{
		UpdateRoomObjects();
		roomUIHandler->Update(); 

		// Check for new Selected Objects -------
		if (CheckRaycastConditions())
		{
				switch (ViewportManager::getInstance()->editRoomMode)
				{

				case EditRoomMode::EDIT_ROOM_OBJECTS:
				{
					if (objectsInRoom.empty())
						return;

					list<FlyObject*> objectCandidates = ViewportManager::getInstance()->RaycastMouseClickObjects();

					if (!objectCandidates.empty())
						App->moduleManager->SetSelectedFlyObject(objectCandidates.back());
					else
					{
						App->moduleManager->SetSelectedFlyObject(nullptr);
					}
				}
				break;

				case EditRoomMode::EDIT_ROOM_UI:
				{
					if (roomUIHandler->uiElements.empty())
						return;

					list<UI_Element*> objectCandidates = ViewportManager::getInstance()->RaycastMouseClickUI();

					if (!objectCandidates.empty())
						App->moduleManager->SetSelectedUIElement(objectCandidates.back());
					else
						App->moduleManager->SetSelectedUIElement(nullptr);
				}
				break;
				}
			
		}
	}

	if (ImGui::IsMouseClicked(0) && GameInventory::getInstance()->droppingObject != nullptr)
	{
		// For now we will simply drop the object on click
		
	}

	// Check if SUPR is pressed to delete
	if (App->moduleInput->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)	
		DeleteSelectedObject(); 
	
	// Check if 1 is pressed to pick from inventory

	if (GameInventory::getInstance()->IsOpened())
	{
		if (App->moduleInput->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			FlyObject* pickedObject = GameInventory::getInstance()->PickObjectFromInventory(0); 
		}

		if (App->moduleInput->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
		
			FlyObject* pickedObject = GameInventory::getInstance()->PickObjectFromInventory(1);
		}

		if (App->moduleInput->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			FlyObject* pickedObject = GameInventory::getInstance()->PickObjectFromInventory(2);
		}
	}
}

void Room::PostUpdate()
{
	App->moduleManager->DeleteObjectsFromListNow();
}

void Room::CleanUp()
{

	if (GetOutputConnectionsAmount() > 0) {
		for (auto it : outLinks) {
			delete it; 
		}

		outLinks.clear();
	}

	for (auto& currentObject : objectsInRoom)
	{
		currentObject->CleanUp();
		delete currentObject; 
	}

	objectsInRoom.clear(); 
	selectedObject = nullptr; 

	roomBlackboard->CleanUp(); 
	delete roomBlackboard; 
	roomBlackboard = nullptr; 

	CleanRoomUI(); 
}

void Room::CleanRoomObjects()
{
	for (auto& it : objectsInRoom)
	{
		it->CleanUp(); 
	}

	objectsInRoom.clear(); 
	selectedObject = nullptr; 
}

void Room::CleanRoomUI()
{
	if(roomUIHandler != nullptr)
		roomUIHandler->CleanUp();
}

void Room::FitObjectUtils()
{
	for (auto& currentObject : objectsInRoom)
	{
		currentObject->FitObjectUtils();
	}
}

int Room::GetObjectsInRoomAmount()
{
	return objectsInRoom.size();
}

void Room::AddItemToInventory(FlyObject* newObject_Inv)
{
#pragma region sanityChecks
	if (newObject_Inv == nullptr && HasObject(newObject_Inv))
	{
		FLY_ERROR("Item Could Not Be Added To Inventory");
		return; 
	}
#pragma endregion

	// Delete the object from the room (now it will lay in the inventory) 
	GameInventory::getInstance()->AddObjectToInventory(newObject_Inv);
}

bool Room::HasObject(FlyObject* newObject_Inv)
{
	for (auto& currentObject : objectsInRoom)
	{
		if (newObject_Inv->GetUID() == currentObject->GetUID())
			return true; 
	}
}

FlyObject* Room::GetFlyObject(UID _uid)
{
	for (auto& currentObject : objectsInRoom)
	{
		if (_uid == currentObject->GetUID())
			return currentObject;
	}
	return nullptr; 
}

bool Room::CheckRaycastConditions()
{
	return App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_DOWN && 
		App->moduleImGui->gameViewportDockPanel->IsMouseInViewport() &&
		!App->moduleImGui->objectCreatorDockPanel->IsVisible() &&
		!ViewportManager::getInstance()->blockInputTick;
}

void Room::NotifyVariableChange(FlyVariable* currentVariableValue)
{
	for (auto& currentObject : objectsInRoom)
	{
		currentObject->DoVariableConditionActions(currentVariableValue);
	}
}

list<FlyObject*> Room::GetInventoryItemsList()
{
	list<FlyObject*> returnList = list<FlyObject*>(); 
	for (auto& currentObject : objectsInRoom)
	{
		if (currentObject->flyObjectType == INVENTORY_ITEM)
			returnList.push_back(currentObject);
	}

	return returnList;
}

void Room::SaveRoomData(JSON_Object* jsonObject)
{
	// Save Objects Data
	json_object_dotset_number(jsonObject, string("RoomData.ObjectsAmount").c_str(), GetObjectsInRoomAmount());
	json_object_dotset_string(jsonObject, string("RoomData.Name").c_str(), GetName().c_str());
	json_object_dotset_number(jsonObject, string("RoomData.UID").c_str(), GetUID());

	json_object_dotset_string(jsonObject, string("RoomData.ThumbnailName").c_str(), roomThumbnail->GetName().c_str());

	int count = 0; 
	for (auto& it : objectsInRoom)
		it->SaveObjectData(jsonObject, count++);

	// Save Blackboard 
	if(roomBlackboard != nullptr)
		roomBlackboard->SaveData(GetName()); 

	// Save UI 
	roomUIHandler->SaveRoomUI(jsonObject, string("RoomData.UserInterface"));

	// Thumbnail 
	SaveRoomThumbnail();
}

void Room::SaveRoomThumbnail()
{

	//if (MyFileSystem::getInstance()->IsFileInDirectory(MyFileSystem::GetThumbnilesDirectory(), to_string(GetUID())))
	//{
	//	ilEnable(IL_FILE_OVERWRITE);
	//}


	//string savePath = MyFileSystem::GetThumbnilesDirectory() + "\\" + to_string((int)GetUID()).c_str() + "_Thumbnail.dds";
	//uint savingTexture = 1; 

	////Create or open the file
	//ofstream stream;
	//stream.open(savePath.c_str(), ofstream::binary);
	//stream.clear();

	////Check the state of the Material to know what we should write (tex channels)
	////Save the diffuse channel 
	//ILuint size;
	//ILubyte* data;
	//ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use

	//size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer

	//if (size > 0) {

	//	data = new ILubyte[size]; // allocate data buffer

	//	if (savingTexture != 0)
	//	{
	//		glBindTexture(GL_TEXTURE_2D, savingTexture);

	//		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
	//		{	
	//			stream.write((const char*)data, size);
	//		}
	//	}
	//}

	//stream.close();

}

void Room::DeleteRoomFiles()
{
	string roomDataStr = MyFileSystem::getInstance()->GetSavedDataDirectory() + "RoomsData\\" + GetName() + ".json";
	remove(roomDataStr.c_str());

	string blackboardDataStr = MyFileSystem::getInstance()->GetSavedDataDirectory() + "BlackboardsData\\" + GetName() + "_Blackboard.json";
	remove(blackboardDataStr.c_str());
}

RoomConnection* Room::AddConnection(Room* destinationRoom)
{
	// Logic
	RoomConnection* newConnection = new RoomConnection(this, destinationRoom, "Link", false);
	outLinks.push_back(newConnection);
	destinationRoom->inRoomUIDs.push_back(GetUID());

	//flog("Room %s connected the LOGIC succesfuly with %s", roomName.c_str(), destinationRoom->GetName().c_str()); 

	// Update Graph 
	//NodeGraph::getInstance()->ConnectNodes(GetName(), "Out", destinationRoom->GetName(), "In", newConnection->connectionID);
	App->moduleWorldManager->connectionsInWorldAmount++; 
	return newConnection;
}

void Room::CleanGraphConnections()
{
	//outLinks.clear();
}

void Room::DeleteAllConnections()
{
	BreakOutputConnections();
	BreakEnterConnections(); 
}

// Output Connections
void Room::BreakOutputConnections()
{
	if (outLinks.size() <= 0)
		return; 

	for (auto it : outLinks) 
	{
		it->destinationRoom->BreakEnterConnectionFromInRoomUIDs(roomID);
		//it->DeleteOnGraph();

		delete it;
	}

	App->moduleWorldManager->connectionsInWorldAmount -= outLinks.size(); 
	outLinks.clear(); 
}

RoomConnection* Room::GetConnectionToRoom(UID dstRoomUID) const
{
	for (auto it : outLinks) {		
		if (it->destinationRoom->roomID == dstRoomUID) {
			return it; 
		}
	}
}

int Room::GetTotalConnectionsAmount() const
{
	return GetOutputConnectionsAmount() + GetEnterConnectionsAmount(); 
}

const char* Room::GetOutConnectionsAsCombo()
{
	string returnStr;
	for (auto& it : outLinks) {
		returnStr += (it)->connectionName; 
		returnStr += '\0';
	}

	return returnStr.c_str(); 
}


void Room::BreakOutputConnection(UID connectionToDelUID)
{
	for (auto it = outLinks.begin(); it != outLinks.end();) {
		
		if ((*it)->connectionID == connectionToDelUID) {

			(*it)->DeleteOnGraph();

			delete (*it);
			it = outLinks.erase(it);
			App->moduleWorldManager->connectionsInWorldAmount -= 1; 
			break; 
		}
		else
			it++; 
	}
}

void Room::BreakOutputConnection(Room* targetRoomConnected) 
{
	RoomConnection* roomConnection = nullptr; 
	roomConnection = GetConnectionToRoom(targetRoomConnected->GetUID());
	BreakOutputConnection(roomConnection->connectionID); 
}

// Input connections
void Room::BreakEnterConnections()
{
	if (inRoomUIDs.size() <= 0)
		return;

	for (auto it = inRoomUIDs.begin(); it != inRoomUIDs.end();) 
	{
		BreakEnterConnection((*it)); 
		it = inRoomUIDs.erase(it);
	}

	inRoomUIDs.clear();	
}
void Room::BreakEnterConnection(UID roomToDelUID)
{
	Room* originRoom = App->moduleWorldManager->GetRoom(roomToDelUID);
	originRoom->BreakOutputConnection(this); 
}
void Room::BreakEnterConnectionFromInRoomUIDs(UID roomToDelUID)
{
	for (auto it = inRoomUIDs.begin(); it != inRoomUIDs.end();) {
		if ((*it) == roomToDelUID)
			it = inRoomUIDs.erase(it);
		else
			it++;
	}
}

int Room::GetOutputConnectionsAmount() const
{
	return outLinks.size();
}

void Room::DrawRoomObjects()
{
	for (auto& it : objectsInRoom) 
	{
		(it)->DrawVisualLayer(); 
	}

	GameInventory::getInstance()->DrawInventory();

	for (auto& it : objectsInRoom)
	{
		(it)->DrawOverlaysLayer();
	}

	// Draw dropping inventory object 
	GameInventory::getInstance()->DrawDroppingObject();
	GameInventory::getInstance()->CheckReturnDroppingObject();


	if (ImGui::IsMouseClicked(0) && GameInventory::getInstance()->droppingObject != nullptr)
		GameInventory::getInstance()->DropObjectToRoom();
}

void Room::DrawRoomUserInterface()
{
	roomUIHandler->DrawUIElements(); 
}

void Room::UpdateRoomObjects()
{
	for (auto currentObject = objectsInRoom.begin(); currentObject != objectsInRoom.end(); currentObject++)
	{
		(*currentObject)->Update(App->GetDeltaTime());
	}
}

int Room::GetEnterConnectionsAmount() const
{
	return inRoomUIDs.size();
}

FlyObject* Room::CreateFlyObject(std::string objectName, std::string description)
{
	FlyObject* newObject = new FlyObject(objectName, description, ACTION_OBJECT, this); 
	objectsInRoom.push_back(newObject); 
	//flog("New Object");
	return newObject; 
}

FlyObject* Room::CreateCharacter(std::string objectName, std::string description, bool init)
{
	FlyObjectCharacter* newObject = new FlyObjectCharacter(objectName, description, OBJECT_CHARACTER, this);
	objectsInRoom.push_back(newObject);
	//flog("New Object");

	if (init)
		newObject->InitCharacter(); 

	return newObject;
}

FlyObject* Room::CreateInventoryItem(std::string objectName, std::string description)
{
	FlyObject* newObject = new FlyObject(objectName, description, INVENTORY_ITEM, this);
	newObject->AddDisplayImageAction("None"); 
	objectsInRoom.push_back(newObject);
	return newObject;
}

FlyObject* Room::CreateFlyObjectSequencial(std::string objectName, std::string description)
{
	FlyObject* newObject = new FlyObject(objectName, description, OBJECT_SEQUENTIAL, this);
	objectsInRoom.push_back(newObject);
	return newObject;
}

FlyObject* Room::AddFlyObject(FlyObject* newFlyObject)
{
	if (newFlyObject == nullptr) 
	{
		FLY_ERROR("NULL object is trying to be added to the scene"); 
		return nullptr; 
	}

	objectsInRoom.push_back(newFlyObject);
	return newFlyObject;
}

void Room::DeleteFlyObject(FlyObject* objectToDelete)
{
	if (objectToDelete == nullptr)
		return; 

	for (auto& currentObject : objectsInRoom) 
	{
		if (objectToDelete->GetUID() == currentObject->GetUID())
		{
			App->moduleManager->AddDeleteObject(currentObject);
			return; 
		}
	}
}

void Room::DeleteFlyObjectFromList(FlyObject* objectToDelete)
{
	for (auto it = objectsInRoom.begin(); it != objectsInRoom.end(); it++)
	{
		if ((*it)->GetName() == objectToDelete->GetName())
		{
			it = objectsInRoom.erase(it);
			return; 
		}
	}
}

void Room::DeleteSelectedObject()
{
	if (selectedObject != nullptr)
	{
		DeleteFlyObject(selectedObject);
		selectedObject = nullptr;
	}
}

string Room::GetName() const
{
	return roomName;
}

void Room::SetName(string newName)
{
	roomName = newName; 
}

UID Room::GetUID() const
{
	return roomID; 
}

void Room::SetUID(UID roomID)
{
	roomID = roomID;
}

void Room::SetSelectedObject(FlyObject* newObject)
{
	if (App->moduleManager->GetSelectedFlyObject() == newObject)
		return;

	for (auto& it : objectsInRoom)
	{
		if ((it) == newObject)
		{
			(it)->isSelected = true;
		}
		else
		{
			it->isSelected = false;
		}

		for (auto& currentAction : it->GetActionsList())
			currentAction->SetIsSelected(false);

		it->selectedAction = nullptr;
	}

	selectedObject = newObject;

	ObjectPropertiesDockPanel* propertiesDockPanel = nullptr;
	propertiesDockPanel = (ObjectPropertiesDockPanel*)App->moduleImGui->GetDockPanel(DOCK_OBJECT_PROPERTIES);
	propertiesDockPanel->SetSelectedObject(selectedObject); 
}

FlyObject* Room::GetSelectedObject() const
{
	return selectedObject;
}

Blackboard* Room::CreateBlackboard()
{
	delete roomBlackboard; 

	roomBlackboard = new Blackboard(); 
	return roomBlackboard;
}

Blackboard* Room::GetBlackboard()
{
	return roomBlackboard;
}

FlyVariable* Room::GetBlackboardVariable(string varName)
{
	return roomBlackboard->GetVariable(varName);
}

RoomConnection::RoomConnection(Room* _originRoom, Room* _roomConnected, string _connectionName, bool _isBidirectional)
{
	originRoom = _originRoom;
	destinationRoom = _roomConnected;
	connectionName = _connectionName;
	isBidirectional = _isBidirectional;
	connectionID = RandomNumberGenerator::getInstance()->GenerateUID();
}

void RoomConnection::DeleteOnGraph()
{
	//NodeGraph::getInstance()->DeleteConnection(connectionID); 
	originRoom = nullptr; 
	destinationRoom = nullptr; 
}
