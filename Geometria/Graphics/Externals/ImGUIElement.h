#pragma once
#include "../../Behaviours/Behaviour.h"

#include "../../ImGUI/imgui.h"
#include "../../ImGUI/imgui_impl_glfw.h"
#include "../../ImGUI/imgui_impl_opengl3.h"
#include "../../ImGUI/imgui_internal.h"
#include "../../ImGUI/ImGuizmo.h"

#include <functional>

class ImGUIElement : public ScriptBehaviour
{
public:
	struct InputTextCallback_UserData
	{
		std::string* Str;
		ImGuiInputTextCallback  ChainCallback;
		void* ChainCallbackUserData;
	};

	static int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
			std::string* str = user_data->Str;
			IM_ASSERT(data->Buf == str->c_str());
			str->resize(data->BufTextLen);
			data->Buf = (char*)str->c_str();
		}
		else if (user_data->ChainCallback)
		{
			// Forward to user callback, if any
			data->UserData = user_data->ChainCallbackUserData;
			return user_data->ChainCallback(data);
		}
		return 0;
	}

	bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;

		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
	}

	void OnLoad()
	{
		SceneSaveAndLoad::StartLoadArray("ImGUIElement");
		transform.LoadTransform();

		LoadElements();

		SceneSaveAndLoad::EndLoadArray();
	}

	void LoadElements();

	void OnStartup()
	{
		ClassType = Class::Pointer;
	}

	void OnSave()
	{
		SceneSaveAndLoad::StartSaveArray("ImGUIElement");
		transform.SaveTransform();

		SaveExternalScripts(allElements);

		SceneSaveAndLoad::EndSaveArray();
	}

	ImGUIElement* elementTrigger = nullptr;
	ImGUIElement* owner = nullptr;

	std::vector<ImGUIElement*> allElements {};

	enum GUIType
	{
		None,
		Window,
		Button,
		ListButton,
		DragVector3,
		Text,
		InputField,
		ColorEditorButtonRGBA,
		ColorEditorButtonRGB,
		Columns,
		EndColumns,
		NextColumn,
		ImGUIDemoWindow,
		SameLine,
		SliderFloat,
		Style,
		MainMenuBar,
		Menu,
		MenuItem,
		HelpMarker,
		FileSearch,
		Guizmo,
		DragInt
	};

	GUIType guiType = GUIType::None;

	int ownerSceneId = 0;
	int ownerDrawId = 0;

	int keyInput = -1;
	int mouseInput = -1;


	bool isOpen = true;
	bool isEditor = true;
	bool _lastOpenState = false, _requestForceMove = false;
	bool enableOpenAndClose = false;
	bool isColored = false;
	Vector2 moveToPosition;
	
	void Move(Vector2 position);

	std::string text = "", secondText = "", textFinal = "", storedPath = "";
	bool deleteMyself = false;

	Vector2 size;

	struct
	{
		bool CanResizeManually = true;
		bool DynamicResizing = false;
		float* size;

	} ColumnProperties;

	struct
	{
		std::string font = "Editor/EditorResources/Fonts/Raleway-Regular.ttf";
		int fontSize = 30;
		bool refresh = true;
	} StyleOptions;

	void RefreshStyle()
	{
		StyleOptions.refresh = true;
	}

	enum AlignTo
	{
		Right,
		Center
	};

	AlignTo Alignment = AlignTo::Right;

	void OpenWithMouseButton(int input);

	Vector4* v4ref = 0;
	std::string* target = 0;
	Color* colorRef = 0;
	float* fRef = 0, minVal = 0, maxVal = 100;
	int* iRef = 0;
	ScriptBehaviour* scriptRef = 0;
	int val = 0;
	bool clicked = false;
	std::string UITag;

	std::vector<std::function<void()>> onClickEvents {};

	std::vector<std::function<void()>> onChangeEvents {};

	std::vector<std::function<void()>> onFileChangeEvents {};

	std::vector<std::function<void()>> onOpenEvents {};

	std::vector<std::function<void()>> onUIUpdateBeginEvents {};

	std::vector<std::function<void()>> onUIUpdateEndEvents {};

	void OnOpen(std::function<void()> theEvent)
	{
		onOpenEvents.push_back(theEvent);
	}

	void OnClick(std::function<void()> theEvent)
	{
		onClickEvents.push_back(theEvent);
	}

	void OnChange(std::function<void()> theEvent)
	{
		onChangeEvents.push_back(theEvent);
	}

	void OnUIUpdateBegin(std::function<void()> theEvent)
	{
		onUIUpdateBeginEvents.push_back(theEvent);
	}

	void OnUIUpdateEnd(std::function<void()> theEvent)
	{
		onUIUpdateEndEvents.push_back(theEvent);
	}

	void OnFileChange(std::function<void()> theEvent)
	{
		onFileChangeEvents.push_back(theEvent);
	}

	void OnFileChange(std::string path)
	{
		storedPath = path;
		for (int i = 0; i < onFileChangeEvents.size(); i++)
		{
			onFileChangeEvents[i]();
		}
	}

	void ForceFocus()
	{
		if (guiType == GUIType::Window)
		{
			ImGui::SetWindowFocus(textFinal.c_str());
		}
	}

	void Delete();

	void OnBegin();

	bool isSelected;

	ImGUIElement();
	ImGUIElement(GUIType gT);
	ImGUIElement(GUIType gT, ImGUIElement& owner);
	ImGUIElement(GUIType gT, std::string t);
	ImGUIElement(GUIType gT, int value);
	ImGUIElement(GUIType gT, ImGUIElement& owner, int value);
	ImGUIElement(GUIType gT, ImGUIElement& owner, float* floatR);
	ImGUIElement(GUIType gT, ImGUIElement& owner, float* floatR, float min, float max);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, int* iR);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, std::string t2);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, Vector4* v4r);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, std::string* targ);
	ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, Color* colRef);
	ImGUIElement(GUIType gT, ImGUIElement& owner, ScriptBehaviour* sr);
	void OnUpdate();

	static ImGUIElement* AddColumns(int v, ImGUIElement& owner)
	{
		return AddColumns(v, owner, true, false);
	}

	static ImGUIElement* AddColumns(int v, ImGUIElement& owner, bool resize, bool dynamicResizing, float* getSize)
	{
		ImGUIElement* i = new ImGUIElement(ImGUIElement::GUIType::Columns, owner, v);
		i->ColumnProperties.CanResizeManually = resize;
		i->ColumnProperties.DynamicResizing = dynamicResizing;
		i->ColumnProperties.size = getSize;
		return i;
	}

	static ImGUIElement* AddColumns(int v, ImGUIElement& owner, bool resize, bool dynamicResizing)
	{
		ImGUIElement* i = new ImGUIElement(ImGUIElement::GUIType::Columns, owner, v);
		i->ColumnProperties.CanResizeManually = resize;
		i->ColumnProperties.DynamicResizing = dynamicResizing;
		return i;
	}

	static ImGUIElement* StopColumns(ImGUIElement& owner)
	{
		ImGUIElement* i = new ImGUIElement(ImGUIElement::GUIType::EndColumns, owner);
		return i;
	}

	static ImGUIElement* JumpToNextColumn(ImGUIElement& owner)
	{
		ImGUIElement* i = new ImGUIElement(ImGUIElement::GUIType::NextColumn, owner);
		return i;
	}

	static ImGUIElement* ContinueInSameLine(ImGUIElement& owner)
	{
		ImGUIElement* i = new ImGUIElement(ImGUIElement::GUIType::SameLine, owner);
		return i;
	}
};

