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

	at::Tensor tensor_image = torch::from_blob(img.data, { img.rows, img.cols, img.channels() }, at::kByte);

	tensor_image = tensor_image.toType(torch::kFloat);
	tensor_image = tensor_image.permute({ 2, 0, 1 });
	tensor_image = tensor_image.div(255);

	tensor_image[0][0].sub_(0.485).div_(0.229);
	tensor_image[0][1].sub_(0.456).div_(0.224);
	tensor_image[0][2].sub_(0.406).div_(0.225);

	tensor_image = tensor_image.unsqueeze(0);
	tensor_image = tensor_image.to(torch::kCUDA);
	std::cout << tensor_image.sizes() << std::endl;

	auto anomaly = this->fastflow_model.forward({ tensor_image }).toGenericDict().at("anomaly_map");

	at::Tensor t = anomaly.toTensor().data();
	t = t.mul(-255).clamp(0, 255).to(torch::kU8).to(torch::kCPU).detach().squeeze(0);
	//t = t.repeat({ 3, 1, 1 });
	cv::Mat ret = TensorToCVImage(t);

	delete anomaly;
	img.release();

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
	int64_t width = tensor.size(1);
	int64_t height = tensor.size(2);
	
	std::cout << width << height << std::endl;
	cv::Mat mat = cv::Mat(cv::Size(width, height), CV_8UC1, tensor.data_ptr<uchar>());
	
	return mat;
}