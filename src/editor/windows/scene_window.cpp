#include "scene_window.h"

#include <imgui.h>

#include "../src/editor/editor_ui.h"
#include "../src/editor/ui_layout.h"
#include "../src/editor/ui_components.h"
#include "../src/editor/IconsFontAwesome6.h"
#include "../src/runtime/runtime.h"
#include "../src/rendering/postprocessing/post_processing.h"
#include "../src/input/input.h"
#include "../src/time/time.h"

SceneWindow::SceneWindow() : lastContentRegionAvail(glm::vec2(0.0f)),
sceneViewRightclicked(false),
movementSpeed(12.0f),
mouseSensitivity(0.08f),
scrollIncrementSpeed(2.0f),
keyAxis(glm::vec2(0.0f)),
keyAxisSmoothingFactor(5.0f),
mouseAxis(glm::vec2(0.0f))
{
	Runtime::getCamera().transform.position.y = 2.0f;
	Runtime::getCamera().transform.rotation.x = 22.0f;
}

void SceneWindow::prepare()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("  Scene  ", nullptr, EditorUI::getWindowFlags().standard);
	{

		bool focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
		bool hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

		UIComponents::space(0.0f, 14.0f);
		UIComponents::headline("Scene", ICON_FA_MAP, HeadlineJustification::CENTER);

		UILayout::beginFlex("toggles", FlexType::ROW, UILayout::FULL_WIDTH, 40.0f, Justification::CENTER, Alignment::CENTER, 1.0f);
		{
			UIComponents::toggleButton(ICON_FA_VECTOR_SQUARE, Runtime::wireframe, "Wireframe");
			UIComponents::toggleButton(ICON_FA_CUBE, Runtime::solidMode, "Solid Mode");
			UIComponents::toggleButton(ICON_FA_ECLIPSE, Runtime::shadows, "Shadows");
			UIComponents::toggleButton(ICON_FA_SPARKLES, Runtime::postProcessingEffects, "Post Processing");
		}
		UILayout::endFlex();

		UILayout::beginFlex("setup", FlexType::ROW, UILayout::FULL_WIDTH, 13.0f, Justification::START, Alignment::CENTER, 1.0f);
		{
			UIComponents::space(1.0f, 0.0f);
			UIComponents::label("Speed: " + std::to_string(static_cast<int>(movementSpeed)));
		}
		UILayout::endFlex();

		glm::vec2 contentRegionAvail = glm::vec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

		// Check if scene view has been resized
		if (contentRegionAvail != lastContentRegionAvail && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
			Runtime::resizeViewport(contentRegionAvail.x, contentRegionAvail.y);

			lastContentRegionAvail = contentRegionAvail;
		}

		ImGui::BeginChild("SceneView", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			ImGui::Image(
				Runtime::postProcessingPipeline.getOutput(),
				ImGui::GetContentRegionAvail(),
				ImVec2(0, 1),
				ImVec2(1, 0));

			sceneViewRightclicked = ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right);

		}
		ImGui::EndChild();

	}
	ImGui::End();
	ImGui::PopStyleVar();

	updateMovement(Runtime::getCamera());

	// Temporary, bad solution: Hide and lock cursor if scene view is rightclicked, otherwise use normal cursor
	if (sceneViewRightclicked) {
		Runtime::setCursor(0x00034003);
	}
	else {
		Runtime::setCursor(0x00034001);
	}
}

void SceneWindow::updateMovement(Camera& camera)
{
	// Get current inputs
	glm::vec2 currentKeyAxis = Input::getKeyAxis();
	if (!sceneViewRightclicked) {
		currentKeyAxis = glm::vec2(0.0f);
	}
	keyAxis = glm::mix(keyAxis, currentKeyAxis, keyAxisSmoothingFactor * Time::getDeltaTime());
	mouseAxis = Input::getMouseAxis();

	// Update scene view movement according to inputs
	glm::vec3 cam_forward = camera.transform.forward();
	glm::vec3 cam_right = camera.transform.right();
	glm::vec3 movement_direction = cam_forward * keyAxis.x + cam_right * keyAxis.y;
	camera.transform.position += movement_direction * movementSpeed * Time::fgetDeltaTime();

	// If scene viewport is rightclicked
	if (sceneViewRightclicked)
	{
		// Check for scrolling movement speed changes
		glm::vec2 currentScrollAxis = Input::getScrollAxis();
		movementSpeed = glm::clamp(movementSpeed + currentScrollAxis.y * scrollIncrementSpeed, 1.0f, 100.0f);

		// Check for camera rotation
		glm::vec3 rotate_direction = glm::vec3(-mouseAxis.y, mouseAxis.x, 0.0f);
		glm::vec3 new_rotation = camera.transform.rotation + (rotate_direction * mouseSensitivity);

		new_rotation = glm::vec3(glm::clamp(new_rotation.x, -90.0f, 90.0f), new_rotation.y, new_rotation.z);
		camera.transform.rotation = new_rotation;
	}
}
