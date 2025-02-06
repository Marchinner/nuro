#include "entity_inspectable.h"

#include "../src/ui/editor_ui.h"
#include "../src/ui/components/im_components.h"
#include "../src/ui/windows/registry_window.h"
#include "../src/ui/windows/insight_panel_window.h"

EntityInspectable::EntityInspectable(HierarchyItem& item) : item(item)
{
}

std::string EntityInspectable::getName()
{
	return item.entity.name;
}

void EntityInspectable::renderStaticContent(ImDrawList& drawList)
{
	IMComponents::label(item.entity.name, EditorUI::getFonts().h3_bold);
	ImGui::Dummy(ImVec2(0.0f, 3.0f));
	IMComponents::buttonBig("Add Component");
}

void EntityInspectable::renderDynamicContent(ImDrawList& drawList)
{
	static float inputFloat = 0.0f;
	static bool inputBool = false;

	static bool enabled = true;
	static bool opened = false;

	auto drawContent = [&]() {
		IMComponents::input("Value 1", inputFloat);
		IMComponents::input("Value 2", inputFloat);
		IMComponents::input("Value 3", inputFloat);
		IMComponents::input("Value 4", inputBool);
	};

	IMComponents::componentWrapper(drawList, "Component", IconPool::get("component"), drawContent, enabled, opened);
}