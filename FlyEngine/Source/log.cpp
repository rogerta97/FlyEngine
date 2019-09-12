#pragma once
#include "Globals.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "ConsoleDockPanel.h"
#include "mmgr.h"

void log(const char file[], int line, consoleMessageType messageType, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	
	OutputDebugStringA(tmp_string2);
}

void flyLog(const char file[], int line, consoleMessageType messageType, const char* format, ...)
{
	static char fly_tmp_string[4096];
	static char fly_tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(fly_tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(fly_tmp_string2, 4096, "\n%s(%d) : %s", file, line, fly_tmp_string);

	//assert(App != nullptr); 
	//assert(App->moduleImGui != nullptr);
	//assert(App->moduleImGui->consoleDockPanel != nullptr);

	if (App != nullptr && App->moduleImGui != nullptr && App->moduleImGui->consoleDockPanel) {
		App->moduleImGui->consoleDockPanel->AddLineToConsole(messageType, fly_tmp_string);
	}

}
