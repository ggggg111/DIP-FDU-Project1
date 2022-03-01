#include "EditorToolSelector.h"

EditorToolSelector::EditorToolSelector()
	: current_tool(TOOLS::STANDARD_BRUSH), tool_size(1), color(ImVec4(0.0f, 0.0f, 0.0f, 1.0f))
{

}

EditorToolSelector::~EditorToolSelector()
{

}

ImVec4 EditorToolSelector::GetColor() const
{
	return ImVec4(
		this->color.x * 255.0f,
		this->color.y * 255.0f,
		this->color.z * 255.0f,
		this->color.w * 255.0f
	);
}

ImVec4& EditorToolSelector::GetColorReference()
{
	return this->color;
}

ImVec4 EditorToolSelector::GetColorNormalized() const
{
	return this->color;
}