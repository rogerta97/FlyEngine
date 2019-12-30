#include "Room.h"
#include "Globals.h"
#include "NodeGraph.h"
#include "ViewportManager.h"
#include "RandomNumberGenerator.h"
#include "Application.h"
#include "ModuleInput.h"
#include "FlyObject.h"
#include "GameViewportDockPanel.h"
#include "GameInventory.h"
#include "ModuleImGui.h"
#include "TextureMSAA.h"
#include "FlyVariable.h"
#include "ObjectPropertiesDockPanel.h"
#include "ModuleRoomManager.h"
#include "Blackboard.h"
#include "mmgr.h"

Room::Room(string roomName)
{
	this->roomName = roomName;
	roomID = RandomNumberGenerator::GenerateUID(); 
	roomBlackboard = new Blackboard(); 

	//roomBlackboard->AddDefaultVariable(); 

	//FlyVariable* boolVar = roomBlackboard->AddDefaultVariable(); 
	//boolVar->name = "VarDefault_Toggle"; 
	//boolVar->varIntegerValue = 0; 
	//boolVar->varType = Var_Toggle; 

	// Add The Room to the NodeGraph 
	static int placer = 50;
	NodeGraph::getInstance()->CreateNode(roomName, ImVec2(placer, 50), roomID);
	
	placer += 250; 
}

Room::~Room()
{
}


void Room::Update()
{
	if (App->moduleRoomManager->GetSelectedRoom() == this && !objectsInRoom.empty())
	{
		UpdateRoomObjects();

		if (App->moduleRoomManager->GetSelectedRoom()->objectsInRoom.empty() && App->isEngineInPlayMode)
			return;

		// Check for new Selected Objects -------
		if (App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_DOWN && App->moduleImGui->gameViewportDockPanel->IsMouseInViewport())
		{
			list<FlyObject*> objectCandidates = ViewportManager::getInstance()->RaycastMouseClick();

			if (!objectCandidates.empty())
				App->moduleManager->SetSelectedFlyObject(objectCandidates.back());
			else
				App->moduleManager->SetSelectedFlyObject(nullptr);
		}
	}

	if (App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_DOWN && GameInventory::getInstance()->droppingObject != nullptr)
	{
		// For now we will simply drop the object on click
		GameInventory::getInstance()->DropDroppingObjectToRoom();
	}

	// Check if SUPR is pressed to delete
	if (App->moduleInput->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		DeleteSelectedObject(); 
	}

	// Check if 1 is pressed to pick from inventory
	if (App->moduleInput->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		FlyObject* pickedObject = GameInventory::getInstance()->PickObjectFromInventory(0); 
	}
}

void Room::CleanUp()
{
	if (GetOutputConnectionsAmount() > 0) {
		for (auto it : outConnections) {
			delete it; 
		}

		outConnections.clear();
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
}

int Room::GetObjectsInRoomAmount()
{
	return objectsInRoom.size();
}

void Room::AddItemToInventory(FlyObject* newObject_Inv)
{
#pragma region sanityChecks
	if (newObject_Inv == nullptr && IsObjectInRoom(newObject_Inv))
	{
		FLY_ERROR("Item Could Not Be Added To Inventory");
		return; 
	}
#pragma endregion

	// Delete the object from the room (now it will lay in the inventory) 
	GameInventory::getInstance()->AddObjectToInventoryList(newObject_Inv); 
}

bool Room::IsObjectInRoom(FlyObject* newObject_Inv)
{
	for (auto& currentObject : objectsInRoom)
	{
		if (newObject_Inv->GetUID() == currentObject->GetUID())
			return true; 
	}
}

void Room::SaveRoomData(JSON_Object* jsonObject)
{
	json_object_dotset_number(jsonObject, string(GetName() + ".ObjectsAmount").c_str(), GetObjectsInRoomAmount());

	int count = 0; 
	for (auto& it : objectsInRoom)
		it->SaveObjectData(jsonObject, count++);
}

RoomConnection* Room::ConnectToRoom(Room* destinationRoom)
{
	// Logic
	RoomConnection* newConnection = new RoomConnection(this, destinationRoom, "TestLink", false);
	outConnections.push_back(newConnection);
	destinationRoom->inRoomUIDs.push_back(GetRoomID());

	// Update Graph 
	NodeGraph::getInstance()->ConnectNodes(GetName(), "Out", destinationRoom->GetName(), "In", newConnection->connectionID);
	flog("Room %s connected the LOGIC succesfuly with %s", roomName.c_str(), destinationRoom->GetName().c_str()); 
	App->moduleRoomManager->connectionsInWorldAmount++; 
	return newConnection;
}

void Room::DeleteAllConnections()
{
	BreakOutputConnections();
	BreakEnterConnections(); 
}

// Output Connections
void Room::BreakOutputConnections()
{
	if (outConnections.size() <= 0)
		return; 

	for (auto it : outConnections) {
		it->destinationRoom->BreakEnterConnectionFromInRoomUIDs(roomID);
		it->DeleteOnGraph();

		delete it;
	}

	App->moduleRoomManager->connectionsInWorldAmount -= outConnections.size(); 
	outConnections.clear(); 
}

RoomConnection* Room::GetConnectionToRoom(UID dstRoomUID) const
{
	for (auto it : outConnections) {		
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
	for (auto& it : outConnections) {
		returnStr += (it)->connectionName; 
		returnStr += '\0';
	}

	return returnStr.c_str(); 
}


void Room::BreakOutputConnection(UID connectionToDelUID)
{
	for (auto it = outConnections.begin(); it != outConnections.end();) {
		
		if ((*it)->connectionID == connectionToDelUID) {

			(*it)->DeleteOnGraph();

			delete (*it);
			it = outConnections.erase(it);
			App->moduleRoomManager->connectionsInWorldAmount -= 1; 
			break; 
		}
		else
			it++; 
	}
}
void Room::BreakOutputConnection(Room* targetRoomConnected) 
{
	RoomConnection* roomConnection = GetConnectionToRoom(targetRoomConnected->GetRoomID()); 
	BreakOutputConnection(roomConnection->connectionID); 
}
// Input connections
void Room::BreakEnterConnections()
{
	if (inRoomUIDs.size() <= 0)
		return;

	for (auto it = inRoomUIDs.begin(); it != inRoomUIDs.end();) {
		BreakEnterConnection((*it)); 
		it = inRoomUIDs.erase(it);
	}

	inRoomUIDs.clear();	
}
void Room::BreakEnterConnection(UID roomToDelUID)
{
	Room* originRoom = App->moduleRoomManager->GetRoom(roomToDelUID);
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
	return outConnections.size();
}

void Room::DrawRoomObjects()
{
	for (auto& it : objectsInRoom) {
		(it)->Draw(); 
	}

	// Draw dropping inventory object 
	GameInventory::getInstance()->DrawDroppingObject();
	
}

void Room::UpdateRoomObjects()
{
	for (auto currentObject = objectsInRoom.begin(); currentObject != objectsInRoom.end(); currentObject++)
	{
		if ((*currentObject)->Update())
		{
			currentObject = objectsInRoom.erase(currentObject); 

			if (objectsInRoom.empty())
				return; 
		}
	}
}

int Room::GetEnterConnectionsAmount() const
{
	return inRoomUIDs.size();
}

FlyObject* Room::CreateFlyObject(std::string objectName, std::string description)
{
	FlyObject* newObject = new FlyObject(objectName, description); 
	objectsInRoom.push_back(newObject); 
	return newObject; 
}

FlyObject* Room::CreateInventoryItem(std::string objectName, std::string description)
{
	FlyObject* newObject = new FlyObject(objectName, description, INVENTORY_ITEM);
	objectsInRoom.push_back(newObject);
	return newObject;
}

void Room::AddFlyObject(FlyObject* newFlyObject)
{
	if (newFlyObject == nullptr) {
		FLY_ERROR("NULL object is trying to be added to the scene"); 
		return; 
	}

	objectsInRoom.push_back(newFlyObject);
}

void Room::DeleteFlyObject(FlyObject* objectToDelete)
{
	for (auto it = objectsInRoom.begin(); it != objectsInRoom.end(); it++) 
	{
		if ((*it)->GetName() == objectToDelete->GetName())
		{
			(*it)->CleanUp();
			delete (*it);
			(*it) = nullptr;

			it = objectsInRoom.erase(it); 

			if (objectsInRoom.empty())
				break; 
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

			if (objectsInRoom.empty())
				break;
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

UID Room::GetRoomID() const
{
	return roomID; 
}

void Room::SetRoomID(UID roomID)
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
	}

	selectedObject = newObject;

	ObjectPropertiesDockPanel* propertiesDockPanel = (ObjectPropertiesDockPanel*)App->moduleImGui->GetDockPanel(DOCK_OBJECT_PROPERTIES); 
	propertiesDockPanel->SetSelectedObject(selectedObject); 	
}

FlyObject* Room::GetSelectedObject() const
{
	return selectedObject;
}

Blackboard* Room::GetBlackboard()
{
	return roomBlackboard;
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
	NodeGraph::getInstance()->DeleteConnection(connectionID); 
	originRoom = nullptr; 
	destinationRoom = nullptr; 
}
