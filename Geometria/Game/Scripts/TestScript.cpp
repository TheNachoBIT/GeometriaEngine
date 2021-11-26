#include "TestScript.h"
#include <iostream>
#include "../../Editor/Editor.h"
#include "../../Physics/PhysicsManager.h"
#include "../../Physics/Rigidbody/Rigidbody.h"

VisualAccess(TestScript)

void TestScript::OnStart()
{
	std::cout << "Hello!" << std::endl;
}

void TestScript::OnUpdate()
{
	if (Input::GetKeyDown(GLFW_KEY_SPACE))
	{
		GetScript<Rigidbody>()->GetRigidbodyTransform().position = Vector3(0, 0, 0);
	}
}

void TestScript::OnInspector()
{
	VisualAccess_Title(TestScript);
	VisualAccess_AddValue(DragInt, Speed, &speed);
}

void TestScript::ChangeSpeed()
{
	GetScript<Model>()->color = Color(255, 255, 0, 1);
	std::cout << "Color Changed!" << std::endl;
}
