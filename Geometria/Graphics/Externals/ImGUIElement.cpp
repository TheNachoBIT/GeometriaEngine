#include "ImGUIElement.h"
#include "../Cores/Renderer/RendererCore.h"
#include "../Externals/SceneAndDrawCall.h"
#include <experimental/filesystem>
#include "../Cores/MainAPI/Graphics.h"
#include "../../Input/Input.h"

void ImGUIElement::LoadElements()
{
	
}

void ImGUIElement::Move(Vector2 position)
{
	moveToPosition = position;
	_requestForceMove = true;
}

void ImGUIElement::OpenWithMouseButton(int input)
{
	mouseInput = input;
}

void ImGUIElement::Delete()
{
	onClickEvents.clear();
	onChangeEvents.clear();
	onFileChangeEvents.clear();
	onUIUpdateBeginEvents.clear();
	onUIUpdateEndEvents.clear();

	for (int i = 0; i < allElements.size(); i++)
	{
		allElements[i]->Delete();

		for (int h = 0; h < Hierarchy::allScripts.size();)
		{
			if (Hierarchy::allScripts[h]->scriptId == allElements[i]->scriptId)
			{
				Hierarchy::allScripts.erase(Hierarchy::allScripts.begin() + h);
			}
			else
			{
				h++;
			}
		}
	}
	//std::vector<ScriptBehaviour*>().swap(Hierarchy::allScripts);
	allElements.clear();
}

void ImGUIElement::OnBegin()
{

}

ImGUIElement::ImGUIElement() : guiType(GUIType::None) {}
ImGUIElement::ImGUIElement(GUIType gT) : guiType(gT) {}
ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner) : guiType(gT) 
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}
ImGUIElement::ImGUIElement(GUIType gT, std::string t) : guiType(gT), text(t) {}
ImGUIElement::ImGUIElement(GUIType gT, int value) : guiType(gT), val(value) {}
ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, int value) : guiType(gT), val(value)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}
ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, float* floatR) : guiType(gT), fRef(floatR)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}
ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, float* floatR, float min, float max) : guiType(gT), fRef(floatR), minVal(min), maxVal(max)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}
ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t) : guiType(gT), text(t) 
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, std::string t2) : guiType(gT), text(t), secondText(t2)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, Vector4* v4r) : guiType(gT), text(t), v4ref(v4r) 
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, std::string* targ) : guiType(gT), text(t), target(targ)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, Color* colRef) : guiType(gT), text(t), colorRef(colRef)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, ScriptBehaviour* sr) : guiType(gT), scriptRef(sr)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

float vectorToFloat[3] = { 0, 0, 0 };
void ImGUIElement::OnUpdate()
{
	if (isEnabled)
	{
		if (mouseInput != -1)
		{
			if (Input::GetMouseButton(mouseInput))
			{
				isOpen = true;
			}
		}

		for (int i = 0; i < onUIUpdateBeginEvents.size(); i++)
		{
			onUIUpdateBeginEvents[i]();
		}

		switch (Alignment)
		{
		case Center:
			float font_size = ImGui::GetFontSize() * text.size() / 2;
			ImGui::NewLine();
			ImGui::SameLine
			(
				ImGui::GetWindowSize().x / 2 -
				font_size + (font_size / 2)
			);
		}

		ImGuiStyle& style = ImGui::GetStyle();
		ImColor styleCol = style.Colors[ImGuiCol_Text];

		if (isColored)
		{
			if(colorRef != nullptr)
				style.Colors[ImGuiCol_Text] = ImColor(colorRef->r, colorRef->g, colorRef->b, colorRef->a);
		}

		if (UITag == "")
			textFinal = text;
		else
			textFinal = text + "###" + UITag;

		switch (guiType)
		{
		case GUIType::Window:
			bool window;

			if (isOpen)
			{
				if (isOpen != _lastOpenState)
				{
					for (int i = 0; i < onOpenEvents.size(); i++)
					{
						onOpenEvents[i]();
					}
					_lastOpenState = isOpen;
				}

				if (_requestForceMove)
				{
					ImGui::SetNextWindowPos(ImVec2(moveToPosition.x, moveToPosition.y), ImGuiCond_Appearing);
					std::cout << moveToPosition.x << " | " << moveToPosition.y << std::endl;
					_requestForceMove = false;
				}

				if (!enableOpenAndClose)
					window = ImGui::Begin(textFinal.c_str());
				else
					window = ImGui::Begin(textFinal.c_str(), &isOpen);

				if (window)
				{
					for (int i = 0; i < allElements.size(); i++)
					{
						allElements[i]->OnUpdate();
					}

					ImGui::End();
				}
			}
			else
			{
				_lastOpenState = false;
			}
			break;

		case GUIType::MainMenuBar:
			if (ImGui::BeginMainMenuBar())
			{
				style.Colors[ImGuiCol_Text] = styleCol;
				for (int i = 0; i < allElements.size(); i++)
				{
					allElements[i]->OnUpdate();
				}

				ImGui::EndMainMenuBar();
			}
			break;

		case GUIType::Button:
			if (ImGui::Button(textFinal.c_str(), ImVec2(size.x, size.y)))
			{
				for (int i = 0; i < onClickEvents.size(); i++)
				{
					onClickEvents[i]();
				}
			}
			break;

		case GUIType::Menu:
			if (ImGui::BeginMenu(textFinal.c_str()))
			{
				style.Colors[ImGuiCol_Text] = styleCol;
				for (int i = 0; i < allElements.size(); i++)
				{
					allElements[i]->OnUpdate();
				}
				ImGui::EndMenu();
			}
			break;

		case GUIType::MenuItem:
			if (ImGui::MenuItem(textFinal.c_str()))
			{
				style.Colors[ImGuiCol_Text] = styleCol;
				for (int i = 0; i < allElements.size(); i++)
				{
					allElements[i]->isOpen = true;
				}
			}
			break;

		case GUIType::ListButton:
			if (allElements.size() == 0)
			{
				if (ImGui::Selectable(textFinal.c_str(), &clicked))
				{
					for (int i = 0; i < onClickEvents.size(); i++)
					{
						onClickEvents[i]();
					}
				}
			}
			else
			{
				ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_SpanAvailWidth;

				if (clicked)
				{
					base_flags |= ImGuiTreeNodeFlags_Selected;
				}

				if (ImGui::TreeNodeEx(&UITag, base_flags, text.c_str()))
				{
					if (ImGui::IsItemClicked())
					{
						clicked = true;
					}

					if (clicked)
					{
						for (int i = 0; i < onClickEvents.size(); i++)
						{
							onClickEvents[i]();
						}
					}

					for (int i = 0; i < allElements.size(); i++)
					{
						allElements[i]->OnUpdate();
					}
					ImGui::TreePop();
				}
			}
			break;

		case GUIType::DragVector3:
			ImGui::DragFloat3(textFinal.c_str(), (float*)v4ref, 0.01f);
			v4ref->SendChangeEvent();
			break;

		case GUIType::InputField:
			if (ImGUIElement::InputText(textFinal.c_str(), target, ImGuiInputTextFlags_CallbackResize, InputTextCallback, (void*)target))
			{
				for (int i = 0; i < onChangeEvents.size(); i++)
				{
					onChangeEvents[i]();
				}
			}
			break;

		case GUIType::Text:
			ImGui::Text(textFinal.c_str());
			break;

		case GUIType::ColorEditorButtonRGBA:
			ImGui::ColorEdit4(textFinal.c_str(), (float*)colorRef);
			break;

		case GUIType::ColorEditorButtonRGB:
			ImGui::ColorEdit3(textFinal.c_str(), (float*)colorRef);
			break;

		case GUIType::Columns:

			if (ColumnProperties.DynamicResizing)
			{
				if (ColumnProperties.size == nullptr)
					ColumnProperties.size = new float(256);

				float padding = 16.0f,
					cellSize = *ColumnProperties.size + padding,
					panelWidth = ImGui::GetContentRegionAvail().x;

				int columnCount = (int)(panelWidth / cellSize);

				if (columnCount < 1)
					columnCount = 1;

				val = columnCount;
			}

			ImGui::Columns(val, 0, ColumnProperties.CanResizeManually);
			break;

		case GUIType::EndColumns:
			ImGui::Columns(1);
			break;

		case GUIType::NextColumn:
			ImGui::NextColumn();
			break;

		case GUIType::ImGUIDemoWindow:
			ImGui::ShowDemoWindow();
			break;

		case GUIType::Style:
			if (StyleOptions.refresh)
			{
				ImGuiIO& io = ImGui::GetIO();
				io.Fonts->AddFontFromFileTTF(ImGUIElement::StyleOptions.font.c_str(), ImGUIElement::StyleOptions.fontSize);
				StyleOptions.refresh = false;
			}
			break;

		case GUIType::SliderFloat:
			if (ImGui::SliderFloat(textFinal.c_str(), fRef, minVal, maxVal))
			{
				for (int i = 0; i < onChangeEvents.size(); i++)
				{
					onChangeEvents[i]();
				}
			}
			break;

		case GUIType::FileSearch:
			if (ImGui::Button(textFinal.c_str(), ImVec2(size.x, size.y)))
			{
				for (int i = 0; i < onClickEvents.size(); i++)
				{
					onClickEvents[i]();
				}
			}

			ImGui::SameLine();

			if (storedPath != "")
			{
				ImGui::Text(std::experimental::filesystem::path(storedPath).filename().u8string().c_str());
			}
			else
			{
				ImGui::Text(secondText.c_str());
			}
			break;

		case GUIType::SameLine:
			ImGui::SameLine();
			break;

		case GUIType::HelpMarker:
			ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(text.c_str());
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
			break;

		case GUIType::Guizmo:
			if (scriptRef != 0)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				Matrix transform = scriptRef->transform.GetTransform();

				ImGuiIO& io = ImGui::GetIO();
				ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
				ImGuizmo::Manipulate(
					glm::value_ptr(Graphics::MainCamera()->GetViewProjection().coreMatrix),
					glm::value_ptr(Graphics::MainCamera()->_projectionMatrix.coreMatrix),
					ImGuizmo::TRANSLATE,
					ImGuizmo::LOCAL,
					glm::value_ptr(transform.coreMatrix)
				);
				break;
			}
		}

		if (isColored)
		{
			style.Colors[ImGuiCol_Text] = styleCol;
		}

		for (int i = 0; i < onUIUpdateEndEvents.size(); i++)
		{
			onUIUpdateEndEvents[i]();
		}
	}
}
