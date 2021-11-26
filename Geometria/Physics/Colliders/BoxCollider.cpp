#include "BoxCollider.h"
#include "../Rigidbody/Rigidbody.h"
#include "../../Editor/Editor.h"

VisualAccess(BoxCollider);

void BoxCollider::OnStart()
{
	RequireStaticScript(PhysicsManager);

	if (GetScript<Rigidbody>() == nullptr)
	{
		boxStatic = PhysicsManager::CreateStaticBox(GetTransform().position, GetTransform().scale);
	}
	else
	{
		boxDynamic = PhysicsManager::CreateDynamicBox(GetTransform().position, GetTransform().scale);
	}
}

void BoxCollider::OnInspector()
{
	VisualAccess_Title(BoxCollider);
}

void BoxCollider::OnUpdate()
{

}
