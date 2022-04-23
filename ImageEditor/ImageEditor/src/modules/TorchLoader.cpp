#include <torch/script.h>

#include "TorchLoader.h"

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
	cv::Mat img = cv::imread(path.c_str(), cv::IMREAD_COLOR);
	std::cout << img.size << " x " << img.channels() << std::endl;

	cv::resize(img, img, cv::Size(256, 256));

	at::Tensor tensor_image = torch::from_blob(img.data, { img.rows, img.cols, img.channels() }, at::kByte);
	std::cout << tensor_image.sizes() << std::endl;

	tensor_image = tensor_image.to(at::kFloat).div(255).unsqueeze(0);
	std::cout << tensor_image.sizes() << std::endl;
	tensor_image = tensor_image.permute({ 0, 3, 1, 2 });
	std::cout << tensor_image.sizes() << std::endl;
	tensor_image.sub_(0.5).div_(0.5);
	tensor_image = tensor_image.to(torch::kCUDA);

	c10::IValue output = this->fastflow_model.forward({ tensor_image });
	c10::Dict<at::IValue, at::IValue> out = output.toGenericDict();
	at::Tensor anomaly_map = out.at("anomaly_map").toTensor().squeeze(0).detach();

	std::cout << anomaly_map.sizes() << std::endl;

	cv::Mat ret = TensorToCVImage(anomaly_map);
	//std::cout << ret << std::endl;
	std::cout << ret.size << std::endl;

	return ret.clone();
}

void TorchLoader::LoadFastFlowModel()
{
	const char* model_path = "models/serialized_fastflow.zip";
	
	try
	{
		this->fastflow_model = torch::jit::load(model_path);
		std::cout << "Model loaded correctly" << std::endl;
	}
	catch (const c10::Error& e)
	{
		std::cout << "Model loaded incorrectly" << std::endl;
		std::cerr << "Error loading the model" << std::endl;
	}

	this->fastflow_model.to(torch::kCUDA);
	this->fastflow_model.eval();
}

cv::Mat TorchLoader::TensorToCVImage(at::Tensor tensor)
{
	tensor = tensor.mul(255).clamp(0, 255).to(torch::kU8);
	tensor = tensor.to(torch::kCPU);
	
	int64_t width = tensor.size(1);
	int64_t height = tensor.size(2);
	
	std::cout << width << height << std::endl;
	cv::Mat mat = cv::Mat(cv::Size(width, height), CV_8UC3, tensor.data_ptr<uchar>());
	
	return mat.clone();
}