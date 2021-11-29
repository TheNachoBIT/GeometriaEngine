#include "PhysicsManager.h"

physx::PxRigidDynamic* dynamicTest;

physx::PxPhysics* PhysicsManager::gPhysics = NULL;
physx::PxScene* PhysicsManager::gScene = nullptr;
physx::PxMaterial* PhysicsManager::gMaterial = NULL;

bool PhysicsManager::preUpdate = true;

std::vector<physx::PxRigidDynamic*> PhysicsManager::allDynamics;
std::vector<physx::PxRigidStatic*> PhysicsManager::allStatics;

void PhysicsManager::OnStartup()
{

}

void PhysicsManager::OnStart()
{
	if (gScene != nullptr)
	{
		gScene->release();
	}

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true);

	physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = PhysicsManager::gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	std::cout << "Physics Set Up!" << std::endl;

	preUpdate = true;
}

void PhysicsManager::OnUpdate()
{
	gScene->simulate(Graphics::DeltaTime());
	gScene->fetchResults(true);

	if (preUpdate)
	{
		for (auto i : PhysicsManager::allDynamics)
		{
			gScene->addActor(*i);
		}

		for (auto i : PhysicsManager::allStatics)
		{
			gScene->addActor(*i);
		}

		std::cout << PhysicsManager::allDynamics.size() << std::endl;

		preUpdate = false;
	}
}

physx::PxRigidStatic* PhysicsManager::CreateStaticBox(Vector3 position, Vector3 scale)
{
	physx::PxShape* boxShape = PhysicsManager::gPhysics->createShape(physx::PxBoxGeometry(scale.x / 2, scale.y / 2, scale.z / 2), *PhysicsManager::gMaterial);

	physx::PxRigidStatic* boxStatic = physx::PxCreateStatic(*PhysicsManager::gPhysics,
		physx::PxTransform(physx::PxVec3(position.x, position.y, position.z)),
		*boxShape);

	PhysicsManager::allStatics.push_back(boxStatic);

	return boxStatic;
}

physx::PxRigidDynamic* PhysicsManager::CreateDynamicBox(Vector3 position, Vector3 scale)
{
	physx::PxShape* boxShape = PhysicsManager::gPhysics->createShape(physx::PxBoxGeometry(scale.x / 2, scale.y / 2, scale.z / 2), *PhysicsManager::gMaterial);

	physx::PxRigidDynamic* boxDynamic = physx::PxCreateDynamic(*PhysicsManager::gPhysics, 
		physx::PxTransform(physx::PxVec3(position.x, position.y, position.z)), 
		*boxShape, 0);

	PhysicsManager::allDynamics.push_back(boxDynamic);

	return boxDynamic;
}
