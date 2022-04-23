#ifndef __TORCH_LOADER_H__
#define __TORCH_LOADER_H__

#include <torch/torch.h>
#include <opencv2/opencv.hpp>

#include "Module.h"

class TorchLoader : public Module
{
public:
	TorchLoader();
	~TorchLoader();

	void Start() override;
	void CleanUp() override;

public:
	cv::Mat FastFlowInference(const std::string& path);

private:
	void LoadFastFlowModel();

	cv::Mat TensorToCVImage(at::Tensor tensor);

private:
	torch::jit::script::Module fastflow_model;
};

#endif /* __TORCH_LOADER_H__ */