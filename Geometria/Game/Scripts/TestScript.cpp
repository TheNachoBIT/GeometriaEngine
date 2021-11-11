#include "TestScript.h"
#include "../../geometria.h"
#include <iostream>

void TestScript::OnStart()
{
	std::cout << "Hello!" << std::endl;
}

void TestScript::OnUpdate()
{
	if (Input::GetKey(GLFW_KEY_UP))
	{
		Transform().position += Vector3(0, 5 * Graphics::DeltaTime(), 0);
	}

	if (Input::GetKey(GLFW_KEY_DOWN))
	{
		Transform().position -= Vector3(0, 5 * Graphics::DeltaTime(), 0);
	}

	if (Input::GetKey(GLFW_KEY_LEFT))
	{
		Transform().position -= Vector3(5 * Graphics::DeltaTime(), 0, 0);
	}

	if (Input::GetKey(GLFW_KEY_RIGHT))
	{
		Transform().position += Vector3(5 * Graphics::DeltaTime(), 0, 0);
	}
}
