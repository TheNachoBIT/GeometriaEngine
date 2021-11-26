#include "Rigidbody.h"
#include "../../Editor/Editor.h"
#include "../Colliders/BoxCollider.h"

VisualAccess(Rigidbody);

void Rigidbody::OnStart()
{
	RequireStaticScript(PhysicsManager);
}

void Rigidbody::OnUpdate()
{
	auto* boxC = GetScript<BoxCollider>();
	if (boxC != nullptr)
	{
		if (boxC->boxDynamic != nullptr)
		{
			if (!forceChange)
			{
				physx::PxTransform t = boxC->boxDynamic->getGlobalPose();
				physx::PxVec3 pos = t.p;
				GetTransform().position = Vector3(t.p.x, t.p.y, t.p.z);
			}
			else
			{
				GetTransform().position = forcedTransform.position;
				boxC->boxDynamic->setGlobalPose(physx::PxTransform(forcedTransform.position.x, forcedTransform.position.y, forcedTransform.position.z));
				forceChange = false;
			}
		}
	}
	else
	{
		std::cout << "Box Collider not found" << std::endl;
	}
}

void Rigidbody::OnInspector()
{
	VisualAccess_Title(Rigidbody);
}
