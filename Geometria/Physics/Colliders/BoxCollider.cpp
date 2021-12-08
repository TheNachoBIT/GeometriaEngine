#include "BoxCollider.h"
#include "../Rigidbody/Rigidbody.h"
#include "../../Editor/Editor.h"

VisualAccess(BoxCollider);

void BoxCollider::OnStart()
{
	RequireStaticScript(PhysicsManager);

	if (GetScript<Rigidbody>() == nullptr)
	{
		if (boxStatic == nullptr)
		{
			boxStatic = PhysicsManager::CreateStaticBox(GetTransform().position, GetTransform().scale);
			boxStatic->userData = this;
		}
	}
	else
	{
		if (boxDynamic == nullptr)
		{
			boxDynamic = PhysicsManager::CreateDynamicBox(GetTransform().position, GetTransform().scale);
			boxDynamic->userData = this;
			std::cout << "Dynamic Box Set!" << std::endl;
		}
	}
}

void BoxCollider::OnInspector()
{
	VisualAccess_Title(BoxCollider);
}

void BoxCollider::OnSave()
{
	SaveInclude(BoxCollider, "Physics/Colliders/BoxCollider.h");
	SaveNewScript(BoxCollider);

	SaveEnd();
}

void BoxCollider::OnUpdate()
{

}
