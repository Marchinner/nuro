#pragma once

#include <string>

enum HeadlineJustification {
	HEADLINE_LEFT,
	HEADLINE_CENTER,
	HEADLINE_RIGHT
};

class UIComponents
{
public:
	static void headline(std::string title, const char* icon = "", HeadlineJustification justification = HEADLINE_LEFT, bool zeroMargin = false);
	static void tooltip(const char* tooltip);
	static void toggleButton(const char* label, bool& value, const char* tooltip = "");

	static void tryIcon(const char* icon);
	
	static void input(const char* label, int& value);
	static void input(const char* label, float& value);

	static bool extendableSettings(const char* label, bool& value, const char* icon = "");
};