#include "SampleScene.tits.h"
#include "geometria.h"
#include "../Physics/PhysicsManager.h"

Model* Model126;

void SampleScene::Init() {
	SceneSaveAndLoad::StartSceneLoad("Game/SampleScene.tits");
	TextureManager::UploadToGPU();

	SceneManager::MainScene().MainDrawCall()->sort = (DrawCall::Sorting)SaveReadInt(SceneManager::MainScene().MainDrawCall().sort); SceneManager::MainScene().MainDrawCall()->type = (DrawCall::Type)SaveReadInt(SceneManager::MainScene().MainDrawCall().type);

	SceneSaveAndLoad::StartLoadArray("Model126");
	Model126 = nullptr; 
	Model126 = new Model(Model::Square(), Vector4(SaveReadFloat(Model126->transform.position.x), SaveReadFloat(Model126->transform.position.y), SaveReadFloat(Model126->transform.position.z), SaveReadFloat(Model126->transform.position.w)), Vector4(SaveReadFloat(Model126->transform.rotation.x), SaveReadFloat(Model126->transform.rotation.y), SaveReadFloat(Model126->transform.rotation.z), SaveReadFloat(Model126->transform.rotation.w)), Vector4(SaveReadFloat(Model126->transform.scale.x), SaveReadFloat(Model126->transform.scale.y), SaveReadFloat(Model126->transform.scale.z), SaveReadFloat(Model126->transform.scale.w)));
	Model126->objectClassName = "Model41";
	Model126->color = Color(SaveReadFloat(Model126->color.r), SaveReadFloat(Model126->color.g), SaveReadFloat(Model126->color.b), SaveReadFloat(Model126->color.a));
	RendererCore::AddModel(*Model126);
	SceneSaveAndLoad::EndLoadArray();

	curl_global_init(CURL_GLOBAL_ALL);
	WebRequest request("http://www.boomlings.com/database/downloadGJLevel22.php", WebRequest::HttpMethod::CURL_DEFAULT);
	WebResponse response;
	WebForm form;

	request.maxRedirects = 0;

	form.AddField("secret", "Wmfd2893gb7");
	form.AddField("levelID", "1650666");
	form.AddField("gdw", "0");
	form.AddField("binaryVersion", "35");
	form.AddField("gameVersion", "20");

	request.SendWebRequest(&response, form);

	std::cout << "-------------------------------------------" << std::endl;
	std::cout << form.Parse() << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << response.url << std::endl;
	std::cout << response.headers << std::endl;
	std::cout << response.body << std::endl;
	std::cout << "-------------------------------------------" << std::endl;

	/*WebRequest webRequest = WebRequest("https://robotoskunk.com/?test=asd", WebRequest::HttpMethod::HTTP_GET);
	WebResponse response = WebResponse();
	WebForm form = WebForm();
	form.AddField("Hi", "I love you! :D");

	webRequest.cookies = "auth_token=test";
	webRequest.SetRequestHeader("Oauth-Token", "another-test");

	webRequest.SendWebRequest(&response, form);

	std::cout << "URL: " << response.url << std::endl;
	std::cout << "Time Elapsed: " << response.timeElapsed << std::endl;
	std::cout << "Response Code: " << response.code << std::endl;
	std::cout << "MIME Type: " << response.mime << std::endl;
	std::cout << "Headers Code: " << response.headers << std::endl;
	std::cout << "Body: " << response.body << std::endl;

	std::cout << WebTools::EncodeURIComponent("¡Hola! Esta es una prueba para verificar que esto funcione correctamente. ()\"'+?¡._-*¨[}ñ") << std::endl;*/
}
