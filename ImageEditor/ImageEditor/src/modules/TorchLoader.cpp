#include <torch/script.h>
#include <c10/cuda/CUDACachingAllocator.h>

#include "TorchLoader.h"

namespace F = torch::nn::functional;

TorchLoader::TorchLoader() : Module()
{

}

TorchLoader::~TorchLoader()
{

}

void TorchLoader::Start()
{
	this->LoadFastFlowModel();
}

void TorchLoader::CleanUp()
{

}

cv::Mat TorchLoader::FastFlowInference(const std::string& path)
{
	c10::InferenceMode guard(true);

	c10::cuda::CUDACachingAllocator::emptyCache();

	cv::Mat img = cv::imread(path.c_str(), cv::IMREAD_COLOR);
	cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
	cv::resize(img, img, cv::Size(256, 256));

	at::Tensor tensor_image = torch::from_blob(img.data, { img.rows, img.cols, img.channels() }, at::kByte);

	tensor_image = tensor_image.toType(torch::kFloat);
	tensor_image = tensor_image.permute({ 2, 0, 1 });
	tensor_image = tensor_image.div(255);

	tensor_image[0][0].sub_(0.485).div_(0.229);
	tensor_image[0][1].sub_(0.456).div_(0.224);
	tensor_image[0][2].sub_(0.406).div_(0.225);

	tensor_image = tensor_image.unsqueeze(0);
	tensor_image = tensor_image.to(torch::kCUDA);

	at::Tensor t = this->fastflow_model.forward({ tensor_image }).toGenericDict()
		.at("anomaly_map").toTensor().data();
	c10::cuda::CUDACachingAllocator::emptyCache();
	t = t.mul(-255).clamp(0, 255).to(torch::kU8).to(torch::kCPU).detach().squeeze(0);
	t = t.repeat({ 3, 1, 1 });

	img.release();

	return TensorToCVImage(t).clone();
}

cv::Mat TorchLoader::StyleTransferInference(const std::string& content_path, const std::string& style_path)
{
	cv::Mat content_image_mat = cv::imread(content_path.c_str(), cv::IMREAD_COLOR);
	cv::cvtColor(content_image_mat, content_image_mat, cv::COLOR_BGR2RGB);
	//cv::resize(content_image_mat, content_image_mat, cv::Size(256, 256));

	at::Tensor content_image_tensor = torch::from_blob(
		content_image_mat.data,
		{ content_image_mat.rows, content_image_mat.cols, content_image_mat.channels() },
		at::kByte
	);

	content_image_tensor = content_image_tensor.toType(torch::kFloat);
	content_image_tensor = content_image_tensor.permute({ 2, 0, 1 });

	StyleTransfer::Preprocess(content_image_tensor);

	return cv::Mat();
}

void TorchLoader::LoadFastFlowModel()
{
	c10::InferenceMode guard(true);

	const char* model_path = "models/serialized_fastflow.zip";
	
	try
	{
		this->fastflow_model = torch::jit::load(model_path);
		std::cout << "Fastflow model loaded correctly" << std::endl;
	}
	catch (const c10::Error& e)
	{
		std::cout << "Fastflow model loaded incorrectly: " << e.what() << std::endl;
	}

	this->fastflow_model.to(torch::kCUDA);
	this->fastflow_model.eval();
}

cv::Mat TorchLoader::TensorToCVImage(at::Tensor& tensor)
{
	int64_t width = tensor.size(1);
	int64_t height = tensor.size(2);

	tensor = tensor.permute({ 1, 2, 0 });
	tensor = tensor.reshape({ width * height * 3 });

	cv::Mat mat = cv::Mat(cv::Size(height, width), CV_8UC3, tensor.data_ptr());
	cv::convertScaleAbs(mat, mat, 1.5);
	cv::applyColorMap(mat, mat, cv::COLORMAP_AUTUMN);

	return mat;
}

void StyleTransfer::Preprocess(const at::Tensor& content_image)
{
	std::cout << content_image.sizes() << std::endl;
	std::cout << content_image << std::endl;

	F::PadFuncOptions opts({});
	//F::pad();
}