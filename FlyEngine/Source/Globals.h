#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>

enum consoleMessageType {
	CM_DEBUG,
	CM_WARNING,
	CM_ERROR
};

enum FlyEngineEvent
{
	WINDOW_RESIZED, 
	ENGINE_PLAY, 
	ENGINE_STOP, 
};

enum FileExtension
{
	// File
	FILE_JSON,

	// Image
	FILE_PNG,
	FILE_JPG,

	// Audio
	FILE_WAV,
	FILE_MP3,

	// Font
	FILE_TTF,
	FILE_OTF,
};

#define LOG(format, ...) log(__FILE__, __LINE__, CM_DEBUG, format, __VA_ARGS__)

#define flog(format, ...) flyLog(__FILE__, __LINE__, CM_DEBUG, format, __VA_ARGS__)
#define FLY_WARNING(format, ...) flyLog(__FILE__, __LINE__, CM_WARNING, format, __VA_ARGS__)
#define FLY_ERROR(format, ...) flyLog(__FILE__, __LINE__, CM_ERROR, format, __VA_ARGS__)

void log(const char file[], int line, consoleMessageType messageType, const char* format, ...);
void flyLog(const char file[], int line, consoleMessageType messageType, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI
#define IDENTITY_MAT 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1

typedef unsigned int uint;
typedef long double UID; 

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// ImGui -------------------
#define IMGUI_SPACE_SEPARATOR ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
#define PUSH_CHILD_BG_COLOR ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
#define SPACING ImGui::Spacing(); 
#define SEPARATOR ImGui::Separator(); 

#define INC_CURSOR_X_7 ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
#define INC_CURSOR_Y_7 ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
#define INC_CURSOR_7 ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 7,ImGui::GetCursorPosY() + 7));

#define INC_CURSOR_X_4 ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4);
#define INC_CURSOR_Y_4 ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
#define INC_CURSOR_4 ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 4,ImGui::GetCursorPosY() + 4));

// Configuration -----------
#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1200
#define SCREEN_SIZE 1
#define FLY_FULLSCREEN true
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Fly Engine V 0.01"