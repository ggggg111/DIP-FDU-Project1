#include <iostream>

#include <torch/torch.h>
#include <torch/script.h>

#include <opencv2/opencv.hpp>

cv::Mat TensorToCVImage(at::Tensor& tensor)
{
	/*int width = tensor.sizes()[1];
	int height = tensor.sizes()[2];
	std::cout << tensor.dtype();

	try
	{
		cv::Mat output_mat(cv::Size{ width, height }, CV_8UC1, tensor.data_ptr<float>());
		std::cout << output_mat << std::endl;
		std::cout << output_mat.size << std::endl;
		
		return output_mat.clone();
	}
	catch (const c10::Error& e)
	{
		std::cout << "an error has occured : " << e.msg() << std::endl;
	}

	return cv::Mat(width, height, CV_8UC3);*/
	tensor = tensor.mul(255).clamp(0, 255).to(torch::kU8);
	tensor = tensor.to(torch::kCPU);
	int64_t width = tensor.size(1);
	int64_t height = tensor.size(2);
	std::cout << width << height << std::endl;
	cv::Mat mat = cv::Mat(cv::Size(width, height), CV_8UC3, tensor.data_ptr<uchar>());
	return mat.clone();
}

int main()
{
	const char* model_path = "serialized_fastflow.zip";
	torch::jit::script::Module model;

	try
	{
		model = torch::jit::load(model_path);

		std::cout << "Model loaded correctly" << std::endl;
	}
	catch (const c10::Error& e)
	{
		std::cout << "Model loaded incorrectly" << std::endl;
		std::cerr << "Error loading the model" << std::endl;
		
		return -1;
	}

	model.to(torch::kCUDA);
	model.eval();

	cv::Mat img = cv::imread("002.png", cv::IMREAD_COLOR);
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

	c10::IValue output = model.forward({ tensor_image });
	c10::Dict<at::IValue, at::IValue> out = output.toGenericDict();
	at::Tensor anomaly_map = out.at("anomaly_map").toTensor().squeeze(0).detach();

	std::cout << anomaly_map.sizes() << std::endl;

	cv::Mat ret = TensorToCVImage(anomaly_map);
	std::cout << ret << std::endl;
	std::cout << ret.size << std::endl;
	
	return 0;
}