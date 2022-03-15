#ifndef __GUI_H__
#define __GUI_H__

#include <string>

#include "Module.h"

class GUI : public Module
{
public:
	GUI();
	~GUI();

	void Start() override;
	void CleanUp() override;

	void Draw();

	void HelpMarker(const std::string& desc);
};

#endif /* __GUI_H__ */