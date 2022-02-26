#ifndef __EDITOR_TOOL_SELECTOR_H__
#define __EDITOR_TOOL_SELECTOR_H__

#include "Imgui.h"

enum class TOOLS
{
	PENCIL = 0,
	RUBBER
};

class EditorToolSelector
{
public:
	EditorToolSelector();
	~EditorToolSelector();

	ImVec4 GetColor();
	ImVec4 GetColorNormalized();
	
	ImVec4& GetColorReference();

public:
	TOOLS current_tool;

private:
	ImVec4 color;
};

#endif /* __EDITOR_TOOL_SELECTOR_H__ */