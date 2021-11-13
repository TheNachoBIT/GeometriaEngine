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
		GetTransform().position += Vector3(0, 5 * Graphics::DeltaTime(), 0);
	}

	if (Input::GetKey(GLFW_KEY_DOWN))
	{
		GetTransform().position -= Vector3(0, 5 * Graphics::DeltaTime(), 0);
	}

	if (Input::GetKey(GLFW_KEY_LEFT))
	{
		GetTransform().position -= Vector3(5 * Graphics::DeltaTime(), 0, 0);
	}

	if (Input::GetKey(GLFW_KEY_RIGHT))
	{
		GetTransform().position += Vector3(5 * Graphics::DeltaTime(), 0, 0);
	}
}
