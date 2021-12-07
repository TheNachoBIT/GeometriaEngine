#include "SampleScene.tits.h"
#include "geometria.h"
#include "../Physics/PhysicsManager.h"

#define RSPrint(x) std::cout << x << std::endl

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
	WebResponse response1;
	WebForm form1;

	form1.AddField("secret", "Wmfd2893gb7");
	form1.AddField("levelID", "1650666");
	form1.AddField("gdw", "0");
	form1.AddField("binaryVersion", "35");
	form1.AddField("gameVersion", "20");

	WebRequest::SendPrimitiveRequest(&response1, "http://www.boomlings.com/database/downloadGJLevel22.php", form1.Parse());

	RSPrint("-------------------------------------------");
	RSPrint("---------------- Request 1 ----------------");
	RSPrint("-------------------------------------------");
	RSPrint(form1.Parse());
	RSPrint("-------------------------------------------");
	RSPrint("URL: " << response1.url);
	RSPrint("Time Elapsed: " << response1.timeElapsed);
	RSPrint("Response Code: " << response1.code);
	RSPrint("MIME Type: " << response1.mime);
	RSPrint("Headers: " << response1.headers);
	RSPrint("Body: " << response1.body);
	RSPrint("-------------------------------------------");

	WebResponse response2 = WebResponse();
	auto lambda = [](WebResponse *response2) {
		WebRequest webRequest2 = WebRequest("https://robotoskunk.com/?test=asd");

		WebForm form2 = WebForm();
		form2.AddField("Hi", "I love you! :D");

		webRequest2.cookies = "auth_token=test";
		webRequest2.SetRequestHeader("Oauth-Token", "another-test");

		webRequest2.SendRequest(response2, form2);

		RSPrint("-------------------------------------------");
		RSPrint("---------------- Request 2 ----------------");
		RSPrint("-------------------------------------------");
		RSPrint(form2.Parse());
		RSPrint("-------------------------------------------");
		RSPrint("URL: " << response2->url);
		RSPrint("Time Elapsed: " << response2->timeElapsed);
		RSPrint("Response Code: " << response2->code);
		RSPrint("MIME Type: " << response2->mime);
		RSPrint("Headers: " << response2->headers);
		RSPrint("Body: " << response2->body);
		RSPrint("-------------------------------------------");

		std::cout << WebTools::EncodeURIComponent("¡Hola! Esta es una prueba para verificar que esto funcione correctamente. ()\"'+?¡._-*¨[}ñ") << std::endl;
	};

	std::thread th(lambda, &response2);
	th.join();

	/*auto thread = ThreadsManager::CreateThread(lambda, &response2);
	ThreadsManager::StartThread(thread);*/
}
