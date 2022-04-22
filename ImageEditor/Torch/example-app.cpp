#include <iostream>

#include <torch/torch.h>
#include <torch/script.h>

#include <opencv2/opencv.hpp>

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
		std::cerr << "error loading the model" << std::endl;
		return -1;
	}
	model.to(torch::kCUDA);
	//model.eval();

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
	at::Tensor anomaly_map = out.at("anomaly_map").toTensor();
	std::cout << anomaly_map.sizes() << std::endl;
	
	std::cout << "Done!\n";

	//std::vector<torch::jit::IValue> inputs;
	//inputs.push_back(torch::ones({ 1, 3, 224, 224 }));

	//at::Tensor output = m.forward(inputs).toTensor();
	//std::cout << output.slice(/*dim=*/1, /*start=*/0, /*end=*/5) << '\n';

	/*const char* input_image = "test.png";
	at::Tensor input_tensor = torch::from_file(input_image);
	std::cout << input_tensor << std::endl;
	std::cout << input_tensor.sizes() << std::endl;
	std::vector<torch::jit::IValue> inputs;
	inputs.push_back(input_tensor);

	at::Tensor output = m.forward(inputs).toTensor();*/



	return 0;
}