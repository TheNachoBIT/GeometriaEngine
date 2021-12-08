#pragma once

#ifndef BEHAVIOUR_H
#include "Behaviour.h"
#endif

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

#ifndef PHYSICSMANAGER_H
#include "../PhysicsManager.h"
#endif

#define BOXCOLLIDER_H
struct BoxCollider : public ScriptBehaviour
{
	AllowCloning(BoxCollider);

	physx::PxRigidStatic* boxStatic = nullptr;
	physx::PxRigidDynamic* boxDynamic = nullptr;

	physx::PxShape* boxShape = nullptr;

	void OnStart();
	void OnInspector();
	void OnSave();
	void OnUpdate();
};
