#include <cmath>

#include <torch/script.h>
#include <c10/cuda/CUDACachingAllocator.h>
#include <torch/data/transforms.h>

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
	this->LoadStyleTransferModels();

	this->tain_model.to(torch::kCUDA);
	this->tain_model.eval();

	this->style_transfer_params.USE_URST = true;
	this->style_transfer_params.HIGH_RES_MODE = true;
	this->style_transfer_params.RESIZE = 0;
	this->style_transfer_params.THUMB_SIZE = 1024;
	this->style_transfer_params.PATCH_SIZE = 1000;
	this->style_transfer_params.PADDING = 32;
	this->style_transfer_params.STYLE_SIZE = 1024;
	this->style_transfer_params.ALPHA = 1.0f;
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

	return TensorToCVImageFastFlow(t).clone();
}

cv::Mat TorchLoader::StyleTransferInference(const std::string& content_path, const std::string& style_path)
{
	cv::Mat image = cv::imread(content_path.c_str(), cv::IMREAD_COLOR);
	cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

	cv::Mat style = cv::imread(style_path.c_str(), cv::IMREAD_COLOR);
	cv::cvtColor(style, style, cv::COLOR_BGR2RGB);

	if(this->style_transfer_params.RESIZE != 0)
		cv::resize(image, image, cv::Size(this->style_transfer_params.RESIZE, this->style_transfer_params.RESIZE));

	cv::Size image_size = image.size();
	this->style_transfer_params.IMAGE_WIDTH = image_size.width;
	this->style_transfer_params.IMAGE_HEIGHT = image_size.height;

	cv::resize(
		style, style,
		cv::Size(this->style_transfer_params.STYLE_SIZE, this->style_transfer_params.STYLE_SIZE)
	);

	torch::cuda::synchronize();

	if (this->style_transfer_params.USE_URST)
	{
		int aspect_ratio = this->style_transfer_params.IMAGE_WIDTH / this->style_transfer_params.IMAGE_HEIGHT;

		cv::Mat thumbnail;
		cv::resize(
			image, thumbnail,
			cv::Size(aspect_ratio * this->style_transfer_params.THUMB_SIZE, this->style_transfer_params.THUMB_SIZE)
		);

		at::Tensor thumbnail_tensor = this->Mat2Tensor(thumbnail).to(torch::kCUDA);
		std::cout << "Thumbnail shape: " << thumbnail_tensor.sizes() << std::endl;

		thumbnail.release();
		
		at::Tensor patches_tensor = this->Preprocess(image, this->style_transfer_params.PADDING, this->style_transfer_params.PATCH_SIZE);
		std::cout << "Patches shape: " << patches_tensor.sizes() << std::endl;
		
		image.release();

		at::Tensor style_tensor = this->Mat2Tensor(style).unsqueeze(0).to(torch::kCUDA);
		std::cout << "Style shape: " << style_tensor.sizes() << std::endl;

		{
			c10::InferenceMode guard(true);
			torch::NoGradGuard no_grad;

			at::Tensor style_f_tensor = this->vgg_model.forward({ style_tensor }).toTensor();

			cv::Mat res_thumbnail;
			cv::Mat res_high_resolution;

			res_thumbnail = this->StyleTransferThumbnail(thumbnail_tensor, style_f_tensor, this->style_transfer_params.ALPHA);
			res_high_resolution = this->StyleTransferHighResolution(
				patches_tensor, style_f_tensor,
				this->style_transfer_params.PADDING, false, this->style_transfer_params.ALPHA
			);

			return this->style_transfer_params.HIGH_RES_MODE ? res_high_resolution : res_thumbnail;
		}
	}
	else
	{
		at::Tensor image_tensor = this->Mat2Tensor(image).to(torch::kCUDA);
		at::Tensor style_tensor = this->Mat2Tensor(style).unsqueeze(0).to(torch::kCUDA);

		std::cout << "Image shape: " << image_tensor.sizes() << std::endl;
		std::cout << "Style shape: " << style_tensor.sizes() << std::endl;

		{
			c10::InferenceMode guard(true);
			torch::NoGradGuard no_grad;

			at::Tensor style_f_tensor = this->vgg_model.forward({ style_tensor }).toTensor();

			cv::Mat res = this->StyleTransferThumbnail(image_tensor, style_f_tensor, this->style_transfer_params.ALPHA);
		
			return res;
		}
	}
}

void TorchLoader::LoadFastFlowModel()
{
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

void TorchLoader::LoadStyleTransferModels()
{
	const char* vgg_model_path = "models/style_transfer/vgg_model.pt";

	try
	{
		this->vgg_model = torch::jit::load(vgg_model_path);
		std::cout << "VGG model loaded correctly" << std::endl;
	}
	catch (const c10::Error& e)
	{
		std::cout << "VGG model loaded incorrectly: " << e.what() << std::endl;
	}

	this->vgg_model.to(torch::kCUDA);
	this->vgg_model.eval();

	const char* decoder_model_path = "models/style_transfer/decoder_model.pt";

	try
	{
		this->decoder_model = torch::jit::load(decoder_model_path);
		std::cout << "Decoder model loaded correctly" << std::endl;
	}
	catch (const c10::Error& e)
	{
		std::cout << "Decoder model loaded incorrectly: " << e.what() << std::endl;
	}

	this->decoder_model.to(torch::kCUDA);
	this->decoder_model.eval();
}

cv::Mat TorchLoader::TensorToCVImageFastFlow(at::Tensor& tensor)
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

at::Tensor TorchLoader::Preprocess(const cv::Mat& image, const int& padding, const int& patch_size)
{
	cv::Size image_size = image.size();

	int W = image_size.width;
	int H = image_size.height;
	int N = (int)std::ceil(sqrt((W * H) / (patch_size * patch_size)));
	
	int W_ = (int)std::ceil(W / N) * N + 2 * padding;
	int H_ = (int)std::ceil(H / N) * N + 2 * padding;

	int w = (int)std::ceil(W / N) + 2 * padding;
	int h = (int)std::ceil(H / N) + 2 * padding;

	at::Tensor image_tensor = this->Mat2Tensor(image).to(torch::kCPU);
	image_tensor = image_tensor.unsqueeze(0);

	int p_left = (W_ - W) / 2;
	int p_right = (W_ - W) - p_left;
	int p_top = (H_ - H) / 2;
	int p_bottom = (H_ - H) - p_top;

	image_tensor = F::pad(
		image_tensor,
		F::PadFuncOptions({ p_left, p_right, p_top, p_bottom }).mode(torch::kReflect));

	int c = image_tensor.sizes()[1];

	image_tensor = F::unfold(
		image_tensor,
		F::UnfoldFuncOptions({ h, w })
		.stride({ h - 2 * padding, w - 2 * padding }));

	int B = image_tensor.sizes()[0];
	int L = image_tensor.sizes()[2];

	image_tensor = image_tensor.permute({ 0, 2, 1 }).contiguous();
	image_tensor = image_tensor.view({B, L, c, h, w}).squeeze(0);

	return image_tensor;
}

at::Tensor TorchLoader::Unpadding(at::Tensor& tensor, const int& padding)
{
	using namespace torch::indexing;

	int h = tensor.sizes()[2];
	int w = tensor.sizes()[3];

	tensor = tensor.index({"...", Slice(padding, h - padding), Slice(padding, w - padding)});
	
	return tensor;
}

at::Tensor TorchLoader::Mat2Tensor(const cv::Mat& input)
{
	at::Tensor tensor = torch::from_blob(
		input.data,
		{ input.rows, input.cols, input.channels() },
		at::kByte
	);

	tensor = tensor.toType(torch::kFloat);
	tensor = tensor.permute({ 2, 0, 1 });
	tensor = tensor.div(255.0f);

	return tensor;
}

cv::Mat TorchLoader::StyleTransferThumbnail(at::Tensor& content, const at::Tensor& style_f, const float& alpha)
{
	content = content.unsqueeze(0);
	
	InitThumbnailInstanceNorm(this->tain_model, true);

	at::Tensor stylized_thumb_tensor = this->StyleTransfer(content, style_f, alpha);

	c10::cuda::CUDACachingAllocator::emptyCache();

	stylized_thumb_tensor = stylized_thumb_tensor.mul(255.0).clamp(0, 255).to(torch::kU8).to(torch::kCPU).detach().squeeze(0);

	return this->TensorToCVImageStyleTransfer(stylized_thumb_tensor).clone();
}

cv::Mat TorchLoader::StyleTransferHighResolution(at::Tensor patches, at::Tensor& style_f, const int& padding, const bool& collection, const float& alpha)
{
	std::vector<at::Tensor> stylized_patches(patches.sizes()[0]);
	
	InitThumbnailInstanceNorm(this->tain_model, collection);
	
	for (int d = 0; d < patches.sizes()[0]; ++d)
	{
		at::Tensor patch = patches[d];
		patch = patch.unsqueeze(0).to(torch::kCUDA);

		at::Tensor stylized_patch = this->StyleTransfer(patch, style_f, alpha);
		
		stylized_patch = F::interpolate(
			stylized_patch,
			F::InterpolateFuncOptions()
			.size(std::vector({ patch.sizes()[2], patch.sizes()[3] }))
			.mode(torch::kBilinear)
			.align_corners(true)
		);

		stylized_patch = Unpadding(stylized_patch, this->style_transfer_params.PADDING);		
		stylized_patches[d] = stylized_patch.to(torch::kCPU);
	}
	
	at::TensorList stylized_patches_tensorlist = at::TensorList(stylized_patches);
	
	at::Tensor stylized_patches_tensor = torch::stack(stylized_patches_tensorlist, 0).squeeze();

	c10::IntArrayRef stylized_patches_tensor_size = stylized_patches_tensor.sizes();
	int b = stylized_patches_tensor_size[0]; int c = stylized_patches_tensor_size[1];
	int h = stylized_patches_tensor_size[2]; int w = stylized_patches_tensor_size[3];

	stylized_patches_tensor = stylized_patches_tensor.unsqueeze(0);
	stylized_patches_tensor = stylized_patches_tensor.view({ 1, b, c * h * w }).permute({ 0, 2, 1 }).contiguous();

	int output_size_h = (int)sqrt(b) * h;
	int output_size_w = (int)sqrt(b) * w;

	at::Tensor stylized_image_tensor = F::fold(
		stylized_patches_tensor,
		F::FoldFuncOptions({ h, w }).output_size({ output_size_h, output_size_w }).stride({ h, w })
	);

	c10::cuda::CUDACachingAllocator::emptyCache();

	stylized_image_tensor = stylized_image_tensor.squeeze(0).mul(255.0).clamp(0, 255).to(torch::kU8).to(torch::kCPU).detach();

	return this->TensorToCVImageStyleTransfer(stylized_image_tensor).clone();
}

at::Tensor TorchLoader::StyleTransfer(const at::Tensor& content, const at::Tensor& style_f, const float& alpha)
{
	assert(0.0f <= alpha <= 1.0f);

	at::Tensor content_f = this->vgg_model.forward({ content }).toTensor();
	at::Tensor feat_tensor = this->tain_model.forward(content_f, style_f);
	
	feat_tensor = feat_tensor * alpha + content_f * (1.0f - alpha);

	return this->decoder_model.forward({ feat_tensor }).toTensor();
}

cv::Mat TorchLoader::TensorToCVImageStyleTransfer(at::Tensor& tensor)
{
	int64_t height = tensor.size(1);
	int64_t width = tensor.size(2);

	tensor = tensor.permute({ 1, 2, 0 });
	tensor = tensor.reshape({ width * height * 3 });

	cv::Mat res = cv::Mat(cv::Size(height, width), CV_8UC3, tensor.data_ptr());
	cv::cvtColor(res, res, cv::COLOR_RGB2BGR);

	return res.clone();
}
