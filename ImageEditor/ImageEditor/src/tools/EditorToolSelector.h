#ifndef __EDITOR_TOOL_SELECTOR_H__
#define __EDITOR_TOOL_SELECTOR_H__

#include <vector>
#include <string>

#include "Imgui.h"

enum class TOOLS
{
	STANDARD_BRUSH = 0,
	RUBBER,
	CIRCLE_BRUSH,
	CIRCLE_BRUSH_FILL,
	RECTANGLE_BRUSH,
	RECTANGLE_BRUSH_FILL,
	LINE,
	ELLIPSE,
	ELLIPSE_FILL,
	RECTANGLE,
	RECTANGLE_FILL
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