#include "ViewportManager.h"

ViewportManager* ViewportManager::instance = 0;

ViewportManager::ViewportManager()
{
}

ViewportManager* ViewportManager::getInstance()
{
	return instance;
}

ViewportManager::~ViewportManager()
{
}

void ViewportManager::Delete()
{
	delete instance; 
}

void ViewportManager::Draw()
{

}
