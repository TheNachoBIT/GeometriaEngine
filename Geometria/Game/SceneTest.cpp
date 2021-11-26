#include "SceneTest.h"
#include "geometria.h"

void SceneTest::Init()
{
	DrawCall* gameDrawCall = new DrawCall(SceneManager::MainScene().CreateDrawCall());
	gameDrawCall->sort = DrawCall::Sorting::AtStartup;
	gameDrawCall->type = DrawCall::Type::UI;
	gameDrawCall->objectClassName = "Editor Draw Call";
	gameDrawCall->Close();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Editor/EditorResources/Fonts/Raleway-Regular.ttf", 20);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.12f, 0.51f, 0.8f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.14f, 0.12f, 0.51f, 0.8f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.32f, 0.61f, 1);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.34f, 0.42f, 0.71f, 1);

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.12f, 0.51f, 0.6f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14f, 0.12f, 0.51f, 1);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.32f, 0.61f, 1);

	/*ImGUIElement* demo = new ImGUIElement(ImGUIElement::ImGUIDemoWindow);
	RendererCore::AddImGUIElement(*demo, gameDrawCall->Target());*/

	/*ImGUIElement* window = new ImGUIElement(ImGUIElement::AppWindow, "Window Test");
	window->Move(Vector2(10, 10));
	window->Scale(Vector2(600, 150));
	ImGUIElement* text = new ImGUIElement(ImGUIElement::Text, *window, "Hello!");
	RendererCore::AddImGUIElement(*window, gameDrawCall->Target());*/

	ImGUIElement* window = new ImGUIElement(ImGUIElement::AppWindow, "Window Test");
	window->Move(Vector2(10, 10));
	window->Scale(Vector2(500, 150));
	ImGUIElement* text = new ImGUIElement(ImGUIElement::Text, *window, "Hello!");
	RendererCore::AddImGUIElement(*window, gameDrawCall->Target());

	ImGUIElement* window2 = new ImGUIElement(ImGUIElement::AppWindow, "Another Window");
	window2->Move(Vector2(1410, 790));
	window2->Scale(Vector2(500, 200));
	new ImGUIElement(ImGUIElement::Text, *window2, "This is the new App UI!");
	new ImGUIElement(ImGUIElement::Text, *window2, "With this \"App UI\" system, UI components");
	new ImGUIElement(ImGUIElement::Text, *window2, "can be used for general purposes rather than just being");
	new ImGUIElement(ImGUIElement::Text, *window2, "used for only editor UIs!");
	new ImGUIElement(ImGUIElement::Text, *window2, "");
	new ImGUIElement(ImGUIElement::Text, *window2, "All of this without any decrease in");
	new ImGUIElement(ImGUIElement::Text, *window2, "performance!");
	RendererCore::AddImGUIElement(*window2, gameDrawCall->Target());
}