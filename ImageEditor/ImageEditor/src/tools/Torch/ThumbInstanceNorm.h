#ifndef __THUMB_INSTANCE_NORM_H__
#define __THUMB_INSTANCE_NORM_H__

#include <torch/torch.h>

struct ThumbInstanceNorm : torch::nn::Module
{
	ThumbInstanceNorm(int out_channels = 0, bool affine = false)
	{
		if (affine)
		{
			this->weight = register_parameter("weight", torch::ones({1, out_channels, 1, 1}).set_requires_grad(true));
			this->bias = register_parameter("bias", torch::zeros({ 1, out_channels, 1, 1 }).set_requires_grad(true));
		}
	}

	std::vector<torch::Tensor> forward(torch::Tensor& x, torch::Tensor& thumb = {})
	{
		if (this->is_training())
		{
			torch::Tensor thumb_mean = this->CalcMean(thumb);
			torch::Tensor thumb_std = this->CalcStd(thumb);

			x = (x - thumb_mean) / thumb_std * this->weight + this->bias;
			thumb = (thumb - thumb_mean) / thumb_std * this->weight + this->bias;

			return { x, thumb };
		}
		else
		{
			if (this->collection)
			{
				torch::Tensor thumb_mean = this->CalcMean(x);
				torch::Tensor thumb_std = this->CalcStd(x);

				this->thumb_mean = thumb_mean;
				this->thumb_std = thumb_std;
			}

			x = (x - this->thumb_mean) / this->thumb_std * this->weight + this->bias;

			return { x };
		}
	}

	torch::Tensor CalcMean(torch::Tensor feat)
	{
		c10::IntArrayRef size = feat.sizes();
		assert(size.size() == 4);
		
		int N = size[0];
		int C = size[1];

		return feat.view({ N, C, -1 }).mean(2).view({ N, C, 1, 1 });
	}

	torch::Tensor CalcStd(torch::Tensor feat, float eps = 0.00001f)
	{
		c10::IntArrayRef size = feat.sizes();
		assert(size.size() == 4);

		int N = size[0];
		int C = size[1];

		torch::Tensor feat_var = feat.view({ N, C, -1 }).var(2) + eps;
		return feat_var.sqrt().view({ N, C, 1, 1 });
	}

	torch::Tensor thumb_mean;
	torch::Tensor thumb_std;
	bool collection = true;

	torch::Tensor weight, bias;
};

#endif /* __THUMB_INSTANCE_NORM_H__ */