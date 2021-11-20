#include "GuisFirstScene.h"
#include "../geometria.h"

void GuisFirstScene::Init()
{
    SceneManager::MainScene().MainDrawCall().sort = DrawCall::Sorting::Update;
    SceneManager::MainScene().MainDrawCall().type = DrawCall::Type::Dynamic;
}