#ifndef __THUMB_INSTANCE_NORM_H__
#define __THUMB_INSTANCE_NORM_H__

#include <torch/torch.h>

#include "utils/Utils.h"

struct ThumbAdaptiveInstanceNorm : torch::nn::Module
{
public:
	ThumbAdaptiveInstanceNorm(const bool& affine = false)
	{

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

	torch::Tensor forward(torch::Tensor content_feat, torch::Tensor style_feat)
	{
		assert(content_feat.sizes()[0] == style_feat.sizes()[0]);
		assert(content_feat.sizes()[1] == style_feat.sizes()[1]);

		c10::IntArrayRef size = content_feat.sizes();

		torch::Tensor style_mean = this->CalcMean(style_feat);
		torch::Tensor style_std = this->CalcStd(style_feat);

		if (this->collection)
		{
			torch::Tensor thumb_mean = this->CalcMean(content_feat);
			torch::Tensor thumb_std = this->CalcStd(content_feat);

			this->thumb_mean = thumb_mean;
			this->thumb_std = thumb_std;
		}

		torch::Tensor normalized_feat = (content_feat - this->thumb_mean.expand(size)) / this->thumb_std.expand(size);
		return normalized_feat * style_std.expand(size) + style_mean.expand(size);
	}

	torch::Tensor thumb_mean;
	torch::Tensor thumb_std;
	bool collection = true;
};

void InitThumbnailInstanceNorm(ThumbAdaptiveInstanceNorm& model, const bool& collection);

#endif /* __THUMB_INSTANCE_NORM_H__ */