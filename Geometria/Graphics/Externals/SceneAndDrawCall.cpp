#include "SceneAndDrawCall.h"
#include "Model.h"
#include "../Cores/Renderer/RendererCore.h"

void DrawCall::SaveDrawCall()
{
	std::cout << "Saving Draw Call!" << std::endl;

	if (isMain)
	{
		SaveValue(SceneManager::MainScene().MainDrawCall().sort);
		SaveValue(SceneManager::MainScene().MainDrawCall().type);

		SaveInstruction(

			SceneManager::MainScene().MainDrawCall().sort = (DrawCall::Sorting)SaveReadInt(SceneManager::MainScene().MainDrawCall().sort);
			SceneManager::MainScene().MainDrawCall().type = (DrawCall::Type)SaveReadInt(SceneManager::MainScene().MainDrawCall().type);

		);
	}

	SaveExternalScripts(allModels);
	SaveExternalScripts(allImGUI);
}

DrawCall& Scene::CreateDrawCall(bool startEmpty)
{
	int size = _drawCalls.size();
	DrawCall draw;
	draw.id = size;
	draw.sceneId = id;
	draw.isMain = false;

	_drawCalls.push_back(draw);

	if (!startEmpty)
		RendererCore::OpenGL_Start_DrawCall(_drawCalls[size]);

	//_drawCalls[size].AddMyselfToHierarchy();
	return _drawCalls[size];
}

void DrawCall::Close()
{
	this->AddMyselfToHierarchy();
	SceneManager::MainScene()._drawCalls[SceneManager::MainScene()._drawCalls.size() - 1] = *this;
	Hierarchy::allScripts[Hierarchy::allScripts.size() - 1] = &SceneManager::MainScene()._drawCalls[SceneManager::MainScene()._drawCalls.size() - 1];
	RendererCore::OpenGL_Start_DrawCall(*this);
}

DrawCall& DrawCall::Target()
{
	return SceneManager::_allScenes[sceneId]._drawCalls[id];
}

void DrawCall::Refresh()
{
	refresh = true;
}

void OnSave()
{
}
