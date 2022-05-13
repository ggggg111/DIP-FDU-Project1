#ifndef __TORCH_LOADER_H__
#define __TORCH_LOADER_H__

#include <torch/torch.h>
#include <opencv2/opencv.hpp>

#include "Module.h"

struct StyleTransferParams
{
	bool USE_URST;
	int RESIZE;
	int IMAGE_WIDTH;
	int IMAGE_HEIGHT;
	int THUMB_SIZE;
	int PATCH_SIZE;
	int PADDING;
	int STYLE_SIZE;
	float ALPHA;
};

namespace StyleTransfer
{
	at::Tensor Mat2Tensor(const cv::Mat& input);

	at::Tensor Preprocess(const cv::Mat& content_image_mat, const int& padding, const int& patch_size);

	void StyleTransferThumbnail(at::Tensor& content, const at::Tensor& style_f, const float& alpha);
}

class TorchLoader : public Module
{
public:
	TorchLoader();
	~TorchLoader();

	void Start() override;
	void CleanUp() override;

public:
	cv::Mat FastFlowInference(const std::string& path);
	cv::Mat StyleTransferInference(const std::string& content_path, const std::string& style_path);

private:
	/* FastFlow */
	void LoadFastFlowModel();

	/* Style Transfer */
	void LoadStyleTransferModels();

	cv::Mat TensorToCVImage(at::Tensor& tensor);

private:
	/* FastFlow */
	torch::jit::script::Module fastflow_model;

	/* Style Transfer */
	torch::jit::script::Module vgg_model;

	StyleTransferParams style_transfer_params;
};

#endif /* __TORCH_LOADER_H__ */