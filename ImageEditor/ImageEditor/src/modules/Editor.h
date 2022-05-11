#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "tools/EditorToolSelector.h"
#include "tools/HDRLoader.h"
#include "Module.h"

#include "SDL.h"

class Editor : public Module
{
public:
	Editor();
	~Editor();

	void Start() override;
	void Update() override;
	void CleanUp() override;

public:
	void DrawGUI();

	SDL_Texture* LoadImg(const std::string& path) const;
	void SaveImg(SDL_Texture* texture, const std::string& path) const;

	void RenderImg(SDL_Texture* texture, SDL_Texture* target, const bool& assign_new_bg_rect=true);

private:
	void MainMenuBar();
	void ToolSelection();
	void Panels();
	void PopUps();

	void UseStandardBrush();
	void UseRubber(const int& button);
	void UseCirleBrush();
	void UseCirleBrushFill();
	void UseRectangleBrush();
	void UseRectangleBrushFill();
	void UseLine();
	void UseEllipse();
	void UseEllipseFill();
	void UseRectangle();
	void UseRectangleFill();

	void ApplySuperResolution();
	void ApplyLoadHDRImage(const std::vector<std::string>& image_paths, const std::vector<float>& exposure_times, const TONEMAP_TYPE& tonemap_type);
	void ApplyFastFlowInferenceLeather();
	void ApplyStyleTransferInference(const std::string& content_path, const std::string& style_path);

public:
	EditorToolSelector tools;
	SDL_Texture* bg;
	SDL_Texture* anomaly_viewer_texture;
	SDL_Rect bg_rect;

private:
	int mouse_position_x;
	int mouse_position_y;

	int last_frame_mouse_position_x;
	int last_frame_mouse_position_y;

	bool preferences_panel;
	bool anomaly_viewer_panel;
	bool load_hdr_image_popup;
	bool super_resolution_popup;
	bool style_transfer_popup;
};

#endif /* __EDITOR_H__ */