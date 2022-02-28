#ifndef __EDITOR_TOOL_SELECTOR_H__
#define __EDITOR_TOOL_SELECTOR_H__

#include <vector>
#include <string>

#include "Imgui.h"

enum class TOOLS
{
	BRUSH = 0,
	RUBBER
};

class EditorToolSelector
{
public:
	EditorToolSelector();
	~EditorToolSelector();

	ImVec4 GetColor() const;
	ImVec4 GetColorNormalized() const;
	
	ImVec4& GetColorReference();

public:
	TOOLS current_tool;
	int tool_size;

private:
	ImVec4 color;
};

#endif /* __EDITOR_TOOL_SELECTOR_H__ */