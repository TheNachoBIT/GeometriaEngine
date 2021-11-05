#include "SceneTest.h"
#include "../geometria.h"

void SceneTest::Init()
{
    SceneManager::MainScene().MainDrawCall().sort = DrawCall::Sorting::Update;
    SceneManager::MainScene().MainDrawCall().type = DrawCall::Type::Dynamic;

    DrawCall& d = SceneManager::MainScene().CreateDrawCall();
    d.sort = SceneManager::MainScene().MainDrawCall().sort;
    d.type = DrawCall::Type::UI;

    Model* model = new Model(Model::Square(), Vector3(0, 0, 0), Vector3(0), Vector3(1, 1, 1));
    //model->texture = &t;
    RendererCore::AddModel(*model);

    Model* model2 = new Model(Model::Square(), Vector3(1, 0, 0), Vector3(0), Vector3(1));
    //model2->texture = &t10;
    RendererCore::AddModel(*model2);

    ImGUIElement* window = new ImGUIElement(ImGUIElement::GUIType::Window, "Editor");
    ImGUIElement* text = new ImGUIElement(ImGUIElement::GUIType::Text, *window, Files::Read("ToshiSceneTest2.tits"));
    RendererCore::AddImGUIElement(*window, SceneManager::MainScene()._drawCalls[1]);
}