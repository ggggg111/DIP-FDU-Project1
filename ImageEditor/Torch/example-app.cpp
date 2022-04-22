#include <iostream>

#include <torch/torch.h>
#include <torch/script.h>

#include <opencv2/opencv.hpp>

int main()
{
	/*torch::Tensor tensor = torch::rand({2, 3});
	
	if (torch::cuda::is_available())
	{
		std::cout << "CUDA is available! Training on GPU" << std::endl;
		auto tensor_cuda = tensor.cuda();
		std::cout << tensor_cuda << std::endl;
	}
	else
	{
		std::cout << "CUDA is not available! Training on CPU" << std::endl;
		std::cout << tensor << std::endl;	  
	}

	std::cin.get();
	return 0;*/

	const char* encoder_path = "serialized_fastflow.zip";
	torch::jit::script::Module encoder;
	try
	{
		encoder = torch::jit::load(encoder_path);
		std::cout << "Model loaded correctly" << std::endl;
	}
	catch (const c10::Error& e)
	{
		std::cout << "Model loaded incorrectly" << std::endl;
		std::cerr << "error loading the model\n";
		return -1;
	}
	encoder.eval();

	cv::Mat img = cv::imread("002.png", cv::IMREAD_COLOR);
	std::cout << img.size << " x " << img.channels() << std::endl;

	at::Tensor tensor_image = torch::from_blob(img.data, { img.rows, img.cols, img.channels() }, at::kByte);
	std::cout << tensor_image.sizes() << std::endl;

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