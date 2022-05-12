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
};

namespace StyleTransfer
{
	at::Tensor ContentTransform(const cv::Mat& input);

	void Preprocess(const cv::Mat& content_image_mat, const int& padding, const int& patch_size);
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
	void LoadFastFlowModel();

	cv::Mat TensorToCVImage(at::Tensor& tensor);

private:
	torch::jit::script::Module fastflow_model;

	StyleTransferParams style_transfer_params;
};

#endif /* __TORCH_LOADER_H__ */