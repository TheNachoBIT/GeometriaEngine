#pragma once

#ifndef BEHAVIOUR_H
#include "Behaviour.h"
#endif

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

#include "../PhysicsManager.h"

struct BoxCollider : public ScriptBehaviour
{
	AllowCloning(BoxCollider);

	physx::PxRigidStatic* boxStatic = nullptr;
	physx::PxRigidDynamic* boxDynamic;

	physx::PxShape* boxShape = nullptr;

	void OnStart();
	void OnInspector();
	void OnUpdate();
};
