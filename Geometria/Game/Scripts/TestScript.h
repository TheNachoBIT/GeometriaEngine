#pragma once

#ifndef BEHAVIOUR_H
#include "../../Behaviours/Behaviour.h"
#endif

struct TestScript : public ScriptBehaviour
{
	void OnStart();
	void OnUpdate();

	int speed = 5;

	void ChangeSpeed();
};
