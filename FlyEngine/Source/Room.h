#ifndef _ROOM_H_
#define _ROOM_H_

#include "Globals.h"
#include "JSON\parson.h"
#include "Blackboard.h"

#include <list>
#include <vector>
#include <string>

using namespace std; 

class MusicTrack; 
class Room; 
class RoomUIHandler; 

// Room Enter Pins on graph = roomUID - 1;
// Room Out Pins on graph = roomUID + 1;

struct RoomConnection 
{
public:
	RoomConnection(Room* _originRoom, Room* _roomConnected, string _connectionName, bool _isBidirectional);
	void DeleteOnGraph(); 

	Room* originRoom; 
	Room* destinationRoom;
	bool isBidirectional; 

	string connectionName;
	UID connectionID; 
};

class FlyObject; 
class UI_Element; 
class Texture; 
class Room {

public:

	Room(string roomName); 
	~Room(); 

	void Update(); 
	void PostUpdate(); 

	void CleanUp();
	void CleanRoomObjects(); 
	void CleanRoomUI(); 

	// Utility -------------------
	int GetObjectsInRoomAmount(); 
	void AddItemToInventory(FlyObject* newObject_Inv); 
	bool CheckRaycastConditions(); 
	void NotifyVariableChange(FlyVariable* currentVariableValue); 
	
	bool HasObject(FlyObject* newObject);
	FlyObject* GetFlyObject(UID _uid); 

	list<FlyObject*> GetInventoryItemsList(); 

	// Save & Load ---------------
	void SaveRoomData(JSON_Object* jsonObject);
	void SaveRoomThumbnail(); 

	// Connections ---------------
	RoomConnection* AddConnection(Room* destinationRoom);


	// ---

	RoomConnection* GetConnectionToRoom(UID dstRoomUID) const;

	void DeleteAllConnections();
	int GetTotalConnectionsAmount() const;
	const char* GetOutConnectionsAsCombo();

	// Output Connections ---------
	void BreakOutputConnections();
	void BreakOutputConnection(UID connectionUID);
	void BreakOutputConnection(Room* connectionUID);

	// Enter Connections ----------
	void BreakEnterConnections(); 
	void BreakEnterConnection(UID roomToDelUID);
	void BreakEnterConnectionFromInRoomUIDs(UID roomToDelUID);

	int GetEnterConnectionsAmount() const;
	int GetOutputConnectionsAmount() const;

	// Draw ---------------------------------------------------------------------------
	void DrawRoomObjects(); 
	void DrawRoomUserInterface(); 

	// Update -------------------------------------------------------------------------
	void UpdateRoomObjects(); 

	// Factory ------------------------------------------------------------------------
	FlyObject* CreateFlyObject(std::string objectName, std::string description = "");
	FlyObject* CreateInventoryItem(std::string objectName, std::string description = "");
	FlyObject* CreateFlyObjectSequencial(std::string objectName, std::string description = "");
	FlyObject* CreateCharacter(std::string objectName, std::string description = "", bool init = false);
	void AddFlyObject(FlyObject* newFlyObject);
	void DeleteFlyObject(FlyObject* objectToDelete);
	void DeleteFlyObjectFromList(FlyObject* objectToDelete);
	void DeleteSelectedObject(); 

	// Room Viewport Draw Control -----------------------------------------------------
	void SetEditUIEnabled(bool enable); 

	// Encapsulation ------------------------------------------------------------------
	string GetName() const; 
	void SetName(string newName); 

	UID GetUID() const;
	void SetUID(UID newName);

	void SetSelectedObject(FlyObject* newObject); 
	FlyObject* GetSelectedObject() const;

	Blackboard* GetBlackboard(); 
	FlyVariable* GetBlackboardVariable(string varName); 
	Blackboard* CreateBlackboard();

public: 
	
	// Save FrameBuffer ID
	Texture* roomThumbnail; 
	bool isSelected = false;

	// Link Management -----
	list<RoomConnection*> outLinks;
	list<UID> inRoomUIDs;

	// Room UI
	RoomUIHandler* roomUIHandler; 

	// Music ---------------
	MusicTrack* backgroundMusic; 

	// Object List ---------
	list<FlyObject*> objectsInRoom;

private: 

	Blackboard* roomBlackboard; 
	FlyObject* selectedObject; 
	UID roomID; 
	string roomName;
};



#endif // !_ROOM_H_

