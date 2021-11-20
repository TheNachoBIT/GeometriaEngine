#pragma once

#ifndef BEHAVIOUR_H
#include "../../Behaviours/Behaviour.h"
#endif

struct TestScript : public ScriptBehaviour
{
	AllowCloning(TestScript)

	void OnStart();
	void OnUpdate();
	void OnInspector();

	int speed = 5;

	void ChangeSpeed();
};
