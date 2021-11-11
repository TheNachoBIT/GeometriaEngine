#pragma once
#include <vector>
#include <iostream>
#include "../Tools/Tools.h"
#include "../Files/Scene/SceneFile.h"

struct ScriptBehaviour;
class Hierarchy
{
public:
	static int highestScriptId;
	static std::vector<ScriptBehaviour*> deleteList;
	static std::vector<ScriptBehaviour*> allScripts;
	static std::vector<ScriptBehaviour*> allUpdateScripts, allUpdateEditorScripts;
	static bool _setEditor;
	static void EditorMode(bool e);

	static void StartScripts();
	static void UpdateScripts();
	static void AddObjectToDeleteList(ScriptBehaviour* id);
};

struct Transform
{
	Vector4 position, rotation, scale;
	Transform()
	{
		position = Vector4(0);
		rotation = Vector4(0);
		scale = Vector4(1);
	}

	Matrix GetTransform();

	void LoadTransform();
	void SaveTransform();
};

#define SaveExternalScripts(x) std::cout << "Saving " << #x << " with size of " << x.size() << std::endl; for(int i = 0; i < x.size(); i++) { x[i]->OnSave(); }

#define SaveNewPointer(x, ...) std::cout << "Saving New Pointer... " << std::endl; SceneSaveAndLoad::isSavePointer = true; SceneSaveAndLoad::objectName = new std::ostringstream(); *SceneSaveAndLoad::objectName << #x << scriptId; *SceneSaveAndLoad::allPointers << #x << "* " << SceneSaveAndLoad::objectName->str() << ";" << std::endl; SceneSaveAndLoad::sceneCppSave << std::endl << "SceneSaveAndLoad::StartLoadArray(\"" << SceneSaveAndLoad::objectName->str() << "\");" << std::endl << SceneSaveAndLoad::objectName->str() << " = new " << #x << "(" << SceneSaveAndLoad::ModifyCode(#__VA_ARGS__) << ");" << std::endl; if(this->objectClassName == "") { SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::objectName->str() << "->objectClassName = \"" << SceneSaveAndLoad::objectName->str() << "\";" << std::endl; } else {SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::objectName->str() << "->objectClassName = \"" << this->objectClassName << "\";" << std::endl; }

//Needs work
#define SaveResource(source, classType, item, group) SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::objectName->str() << "->" << #source << " = " << SceneSaveAndLoad::CreateOrAddIntoStructure(group, #classType, #item) << ";" << std::endl
#define SavePointerValue(source, value) SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::objectName->str() << "->" << #source << " = " << SceneSaveAndLoad::ModifyCode(#value) << ";" << std::endl
#define SaveObjectValue(source, value) SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::objectName->str() << "." << #source << " = " << SceneSaveAndLoad::ModifyCode(#value) << ";" << std::endl
#define SaveValue(source) SceneSaveAndLoad::SaveValueToTITS(#source, source)

#define SaveReadInt(name) SceneSaveAndLoad::LoadValueInt(#name)
#define SaveReadString(name) SceneSaveAndLoad::LoadValueString(#name)
#define SaveReadFloat(name) SceneSaveAndLoad::LoadValueFloat(#name)
#define SaveReadResourceString(name) SceneSaveAndLoad::LoadResourceString(#name)
#define SaveReadResourceInt(name) SceneSaveAndLoad::LoadResourceInt(#name)
#define SaveReadResourceFloat(name) SceneSaveAndLoad::LoadResourceFloat(#name)

#define SaveObjectOnlyInstruction(function, objectType) SceneSaveAndLoad::sceneCppSave << #function << "(" << #objectType << SceneSaveAndLoad::objectName->str() << ");" << std::endl
#define SaveObjectInstruction(function, objectType, ...) SceneSaveAndLoad::sceneCppSave << #function << "(" << #objectType << SceneSaveAndLoad::objectName->str() << ###__VA_ARGS__ << ");" << std::endl
#define SaveInstruction(function) SceneSaveAndLoad::sceneCppSave << SceneSaveAndLoad::ModifyCode(#function) << std::endl
#define SaveResourceInstructionAtEnd(function, group) SceneSaveAndLoad::CreateOrAddFunctionIntoStructureOnce(group, #function)

#define SaveEnd() SceneSaveAndLoad::isSavePointer = false; SceneSaveAndLoad::sceneCppSave << "SceneSaveAndLoad::EndLoadArray();" << std::endl

struct ScriptBehaviour
{
	bool isEditor = false;

	bool _dontStore = false;
	bool hasOwner = false;
	ScriptBehaviour* owner;
	std::vector<ScriptBehaviour*> scripts;
	std::vector<ScriptBehaviour*> objectsAndPointers;
	int scriptId = -1;
	Transform transform;
	std::string objectClassName;

	Transform& Transform()
	{
		if (ClassType == Class::Script)
		{
			if (owner != nullptr)
			{
				return owner->transform;
			}
		}
		
		return transform;
	}

	std::string& Name()
	{
		return objectClassName;
	}

	bool _start = true;
	bool isEnabled = true;

	enum class Class
	{
		Script,
		Object,
		Pointer
	};

	Class ClassType = Class::Script;

	ScriptBehaviour() 
	{
		objectClassName.clear();
		this->AddMyselfToHierarchy();
	}

	ScriptBehaviour(bool add)
	{
		objectClassName.clear();
		if (add) { this->AddMyselfToHierarchy(); }
	}

	virtual void OnCreate() { return; }
	virtual void OnDelete() { return; }

	virtual void OnLoad() { return; }
	virtual void OnSave() { return; }

	ScriptBehaviour* CreateNewObject(ScriptBehaviour s) { ScriptBehaviour* newScript = new ScriptBehaviour(s); newScript->OnLoad(); return newScript; }

	void StartScript();

	void AddMyselfToHierarchy();
	void AddChild(ScriptBehaviour& child);
	void RemoveMyselfFromHierarchy() { Hierarchy::allScripts.erase(Hierarchy::allScripts.begin() + scriptId); }

	virtual void OnStartup() { return; }
	virtual void OnStart() 
	{
		if (ClassType == Class::Script)
		{
			Hierarchy::allUpdateScripts.push_back(this);
		}

		return; 
	}
	virtual void OnEditorStart() { return; }
	virtual void OnUpdate() { return; }
	virtual void OnEditorUpdate() { return; }

	virtual void OnInspector() { return; }
};