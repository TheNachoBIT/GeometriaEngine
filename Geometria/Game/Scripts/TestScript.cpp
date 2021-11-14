#include "TestScript.h"
#include "../../geometria.h"
#include <iostream>
#include "../../Editor/Editor.h"

VisualAccess(TestScript)

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

void TestScript::OnInspector()
{
	std::cout << "Accessing Inspector!" << std::endl;
	ImGUIElement* NewLine = new ImGUIElement(ImGUIElement::GUIType::Text, *Editor::Inspector, "");
	ImGUIElement* title = new ImGUIElement(ImGUIElement::GUIType::Text, *Editor::Inspector, "Test Script");
	ImGUIElement* speedInput = new ImGUIElement(ImGUIElement::GUIType::DragInt, *Editor::Inspector, "Speed", &speed);
	title->Alignment = ImGUIElement::AlignTo::Center;
}

void TestScript::ChangeSpeed()
{
	GetScript<Model>()->color = Color(255, 255, 0, 1);
	std::cout << "Color Changed!" << std::endl;
}
