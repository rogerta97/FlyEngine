#include "ModuleRender.h"
#include "Application.h"

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleRender::~ModuleRender()
{
}

bool ModuleRender::Init()
{
	return true;
}

bool ModuleRender::CleanUp()
{
	return true;
}

void ModuleRender::SetTitle(const char* title)
{
}
