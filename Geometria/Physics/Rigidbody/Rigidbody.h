#pragma once

#ifndef BEHAVIOUR_H
#include "Behaviour.h"
#endif

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

#include "../PhysicsManager.h"

struct Rigidbody : public ScriptBehaviour
{
	AllowCloning(Rigidbody);

	void OnStart();
	void OnUpdate();
	void OnInspector();

	bool forceChange = false;
	Transform forcedTransform;

	Transform& GetRigidbodyTransform()
	{
		forceChange = true;
		return forcedTransform;
	}
};
