#include "EditorToolSelector.h"

EditorToolSelector::EditorToolSelector()
	: current_tool(TOOLS::PENCIL)
{

}

EditorToolSelector::~EditorToolSelector()
{

}

ImVec4 EditorToolSelector::GetColor()
{
	return ImVec4(
		this->color.x * 255.0f,
		this->color.y * 255.0f,
		this->color.z * 255.0f,
		this->color.w * 255.0f
	);
}

ImVec4 EditorToolSelector::GetColorNormalized()
{
	return this->color;
}

void EditorToolSelector::SetColor()
{

}