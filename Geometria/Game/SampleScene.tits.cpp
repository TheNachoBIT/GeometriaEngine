#include "SampleScene.tits.h"
#include "../geometria.h"

Model* Model126;

void SampleScene::Init()
{
SceneSaveAndLoad::StartSceneLoad("Game/SampleScene.tits");
TextureManager::UploadToGPU();

SceneManager::MainScene().MainDrawCall().sort = (DrawCall::Sorting)SaveReadInt(SceneManager::MainScene().MainDrawCall().sort); SceneManager::MainScene().MainDrawCall().type = (DrawCall::Type)SaveReadInt(SceneManager::MainScene().MainDrawCall().type);

SceneSaveAndLoad::StartLoadArray("Model126");
Model126 = new Model(Model::Square(), Vector4(SaveReadFloat(Model126->transform.position.x), SaveReadFloat(Model126->transform.position.y), SaveReadFloat(Model126->transform.position.z), SaveReadFloat(Model126->transform.position.w)), Vector4(SaveReadFloat(Model126->transform.rotation.x), SaveReadFloat(Model126->transform.rotation.y), SaveReadFloat(Model126->transform.rotation.z), SaveReadFloat(Model126->transform.rotation.w)), Vector4(SaveReadFloat(Model126->transform.scale.x), SaveReadFloat(Model126->transform.scale.y), SaveReadFloat(Model126->transform.scale.z), SaveReadFloat(Model126->transform.scale.w)));
Model126->objectClassName = "Model41";
Model126->color = Color(SaveReadFloat(Model126->color.r), SaveReadFloat(Model126->color.g), SaveReadFloat(Model126->color.b), SaveReadFloat(Model126->color.a));
RendererCore::AddModel(*Model126);
SceneSaveAndLoad::EndLoadArray();
}