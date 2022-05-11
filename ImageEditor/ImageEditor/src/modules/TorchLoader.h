#ifndef __TORCH_LOADER_H__
#define __TORCH_LOADER_H__

#include <torch/torch.h>
#include <opencv2/opencv.hpp>

#include "Module.h"

namespace StyleTransfer
{
	void Preprocess(const at::Tensor& image);
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
};

#endif /* __TORCH_LOADER_H__ */