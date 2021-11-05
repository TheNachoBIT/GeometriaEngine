#include "Application.h"

Application::State Application::_engineState;

bool Application::IsPlatform(Platform p)
{
	Platform currentPlatform;
#ifdef _WIN32
	currentPlatform = Platform::Windows;
#endif

#ifdef __linux__
	currentPlatform = Platform::Linux;
#endif

	return p == currentPlatform;
}

void Application::SetEditor()
{
	Application::_engineState = State::Editor;
}

void Application::SetGame()
{	
	Application::_engineState = State::Game;
}
