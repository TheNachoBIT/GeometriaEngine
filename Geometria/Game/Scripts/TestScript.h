#pragma once

#include "../../Behaviours/Behaviour.h"

struct TestScript : public ScriptBehaviour
{
	void OnStart();
	void OnUpdate();

	int count = 0;
};
