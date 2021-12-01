#pragma once

#ifndef BEHAVIOUR_H
#include "Behaviour.h"
#endif

#ifndef GEOMETRIA_H
#include "geometria.h"
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
