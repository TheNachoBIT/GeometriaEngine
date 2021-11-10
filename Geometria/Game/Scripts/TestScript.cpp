#include "TestScript.h"
#include "../../geometria.h"
#include <iostream>

void TestScript::OnStart()
{
	std::cout << "Hello!" << std::endl;
}

void TestScript::OnUpdate()
{
	if (Input::GetKey(GLFW_KEY_M))
	{
		count++;
		std::cout << "Counting to: " << count << std::endl;
	}
}
