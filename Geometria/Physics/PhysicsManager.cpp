#include "PhysicsManager.h"

physx::PxRigidDynamic* dynamicTest;

physx::PxPhysics* PhysicsManager::gPhysics = nullptr;
physx::PxScene* PhysicsManager::gScene = nullptr;
physx::PxMaterial* PhysicsManager::gMaterial = nullptr;

bool PhysicsManager::preUpdate = true;
bool PhysicsManager::foundationCreated = false;
bool PhysicsManager::physicsCreated = false;
bool PhysicsManager::sceneCreated = false;

Vector3 PhysicsManager::gravity(0, -9.81, 0);

std::vector<physx::PxRigidDynamic*> PhysicsManager::allDynamics;
std::vector<physx::PxRigidStatic*> PhysicsManager::allStatics;

void PhysicsManager::OnStartup()
{
	
}

void PhysicsManager::OnStart()
{
	if (PhysicsManager::sceneCreated == true && gScene != nullptr)
	{
		gScene->release();
	}

	if (!PhysicsManager::foundationCreated)
	{
		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	}

	if (!PhysicsManager::physicsCreated)
	{
		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true);
	}

	physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(gravity.x, gravity.y, gravity.z);
	gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = PhysicsManager::gPhysics->createMaterial(0.5f, 0.5f, 0);

	PhysicsManager::physicsCreated = true;
	PhysicsManager::foundationCreated = true;
	PhysicsManager::sceneCreated = true;
	PhysicsManager::preUpdate = true;
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

void PhysicsManager::OnDestroy()
{
	PhysicsManager::allDynamics.clear();
	std::vector<physx::PxRigidDynamic*>().swap(PhysicsManager::allDynamics);

	PhysicsManager::allStatics.clear();
	std::vector<physx::PxRigidStatic*>().swap(PhysicsManager::allStatics);
}

void PhysicsManager::SetGravity(Vector3 g)
{
	gravity = g;
	gScene->setGravity(physx::PxVec3(gravity.x, gravity.y, gravity.z));
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
