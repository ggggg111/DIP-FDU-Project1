#include <iostream>

#include <torch/torch.h>
#include <torch/script.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

	const char* encoder_path = "fastflow_encoder.pt";
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