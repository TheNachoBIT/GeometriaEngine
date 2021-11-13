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
		GetTransform().position += Vector3(0, speed * Graphics::DeltaTime(), 0);
	}

	if (Input::GetKey(GLFW_KEY_DOWN))
	{
		GetTransform().position -= Vector3(0, speed * Graphics::DeltaTime(), 0);
	}

	if (Input::GetKey(GLFW_KEY_LEFT))
	{
		GetTransform().position -= Vector3(speed * Graphics::DeltaTime(), 0, 0);
	}

	if (Input::GetKey(GLFW_KEY_RIGHT))
	{
		GetTransform().position += Vector3(speed * Graphics::DeltaTime(), 0, 0);
	}
}

void TestScript::ChangeSpeed()
{
	GetScript<Model>()->color = Color(255, 255, 0, 1);
	std::cout << "Color Changed!" << std::endl;
}
