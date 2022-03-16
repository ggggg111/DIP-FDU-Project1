#ifndef __GUI_H__
#define __GUI_H__

#include <string>

#include "Module.h"

enum class UI_STYLE
{
	DARK = 0,
	GREEN,
	DARK_BLUE
};

class GUI : public Module
{
public:
	GUI();
	~GUI();

	void Start() override;
	void CleanUp() override;

	void Draw();

	void HelpMarker(const std::string& desc);

	void SetUIStyle(const UI_STYLE& style);

public:
	UI_STYLE style;
};

#endif /* __GUI_H__ */