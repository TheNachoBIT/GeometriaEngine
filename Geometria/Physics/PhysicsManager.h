#pragma once

#ifndef BEHAVIOUR_H
#include "Behaviour.h"
#endif

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

#include "PxConfig.h"
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"

#include <vector>

#define PHYSICSMANAGER_H
class PhysicsManager : public ScriptBehaviour
{
public:

	physx::PxDefaultAllocator gAllocator;
	physx::PxDefaultErrorCallback gErrorCallback;

	physx::PxFoundation* gFoundation = NULL;
	static physx::PxPhysics* gPhysics;

	physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
	static physx::PxScene* gScene;

	static physx::PxMaterial* gMaterial;

	static std::vector<physx::PxRigidDynamic*> allDynamics;
	static std::vector<physx::PxRigidStatic*> allStatics;

	static bool preUpdate;

	void OnStartup();
	void OnStart();
	void OnUpdate();

	static physx::PxRigidStatic* CreateStaticBox(Vector3 position, Vector3 scale);
	static physx::PxRigidDynamic* CreateDynamicBox(Vector3 position, Vector3 scale);
};