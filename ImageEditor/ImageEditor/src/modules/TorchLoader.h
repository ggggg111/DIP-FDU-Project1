#ifndef __TORCH_LOADER_H__
#define __TORCH_LOADER_H__

#include <torch/torch.h>
#include <opencv2/opencv.hpp>

#include "Module.h"
#include "tools/Torch/ThumbInstanceNorm.h"

struct StyleTransferParams
{
	bool USE_URST;
	int RESIZE_H;
	int RESIZE_V;
	int IMAGE_WIDTH;
	int IMAGE_HEIGHT;
	int THUMB_SIZE;
	int PATCH_SIZE;
	int PADDING;
	int STYLE_SIZE;
	float ALPHA;
	bool HIGH_RES_MODE;
};

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

	cv::Mat TensorToCVImageFastFlow(at::Tensor& tensor);

	/* Style Transfer */
	void LoadStyleTransferModels();

	at::Tensor Mat2Tensor(const cv::Mat& input);

	at::Tensor Preprocess(const cv::Mat& content_image_mat, const int& padding, const int& patch_size);
	at::Tensor Unpadding(at::Tensor& tensor, const int& padding);

	cv::Mat StyleTransferThumbnail(at::Tensor& content, const at::Tensor& style_f, const float& alpha);
	cv::Mat StyleTransferHighResolution(at::Tensor patches, at::Tensor& style_f, const int& padding, const bool& collection, const float& alpha);

	at::Tensor StyleTransfer(const at::Tensor& content, const at::Tensor& style_f, const float& alpha);

	cv::Mat TensorToCVImageStyleTransfer(at::Tensor& tensor);

public:
	StyleTransferParams style_transfer_params;

private:
	/* FastFlow */
	torch::jit::script::Module fastflow_model;

	/* Style Transfer */
	ThumbAdaptiveInstanceNorm tain_model;
	torch::jit::script::Module vgg_model;
	torch::jit::script::Module decoder_model;
};

#endif /* __TORCH_LOADER_H__ */