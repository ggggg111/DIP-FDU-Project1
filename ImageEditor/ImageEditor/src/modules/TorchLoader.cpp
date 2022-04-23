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
	cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
	cv::resize(img, img, cv::Size(256, 256));

	std::cout << img.size << " " << img.rows << " " << img.cols << " " << img.channels() << std::endl;
	at::Tensor tensor_image = torch::from_blob(img.data, { img.rows, img.cols, img.channels() }, at::kByte);
	tensor_image = tensor_image.toType(torch::kFloat);
	std::cout << tensor_image.sizes() << std::endl;
	tensor_image = tensor_image.permute({ 2, 1, 0 });
	std::cout << tensor_image.sizes() << std::endl;

	tensor_image = tensor_image.div(255);

	tensor_image[0][0].sub_(0.485).div_(0.229);
	tensor_image[0][1].sub_(0.456).div_(0.224);
	tensor_image[0][2].sub_(0.406).div_(0.225);
	
	tensor_image = tensor_image.unsqueeze(0);
	tensor_image = tensor_image.to(torch::kCUDA);

	c10::IValue output = this->fastflow_model.forward({ tensor_image });
	//std::cout << output << std::endl;
	c10::Dict<at::IValue, at::IValue> out = output.toGenericDict();
	at::Tensor anomaly_map = out.at("anomaly_map").toTensor().to(torch::kCPU).detach();
	anomaly_map = anomaly_map.div(255);
	std::cout << anomaly_map << std::endl;
	std::cout << anomaly_map.sizes() << std::endl;
	anomaly_map = anomaly_map.squeeze(0);
	std::cout << anomaly_map.sizes() << std::endl;

	std::cout << anomaly_map << std::endl;
	std::cout << anomaly_map.options() << std::endl;
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
	/*tensor = tensor.mul(255).clamp(0, 255).to(torch::kU8);
	tensor = tensor.to(torch::kCPU);
	
	int64_t width = tensor.size(1);
	int64_t height = tensor.size(2);
	
	std::cout << width << height << std::endl;
	cv::Mat mat = cv::Mat(cv::Size(width, height), CV_8UC3, tensor.data_ptr<uchar>());
	
	return mat.clone();*/

	std::cout << "tensor shape:" << tensor.sizes();
	//tensor = tensor.expand({ 3, -1, -1 });
	//std::cout << "tensor shape:" << tensor.sizes();
	//tensor = tensor.squeeze().detach().permute({1, 2, 0});
	tensor = tensor.detach();
	//std::cout << tensor << std::endl;
	//tensor = tensor.mul(255).clamp(0, 255).to(torch::kU8);
	tensor = tensor.mul(255).to(torch::kFloat32);
	std::cout << tensor << std::endl;
	tensor = tensor.to(torch::kCPU);
	std::cout << tensor.numel() << std::endl;
	cv::Mat resultImg(256, 256, CV_32F);
	std::memcpy((void*)resultImg.data, tensor.data_ptr(), sizeof(torch::kFloat32) * tensor.numel());
	std::cout << resultImg << std::endl;
	return resultImg;
	/*tensor = tensor.squeeze().detach().permute({1, 2, 0});
	tensor = tensor.mul(255).clamp(0, 255).to(torch::kU8);
	tensor = tensor.to(torch::kCPU);
	cv::Mat resultImg(512, 512, CV_8UC3);
	std::memcpy((void*)resultImg.data, tensor.data_ptr(), sizeof(torch::kU8) * tensor.numel());*/
	return cv::Mat();
}