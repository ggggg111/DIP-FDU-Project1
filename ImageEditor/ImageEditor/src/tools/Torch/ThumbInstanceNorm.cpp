#include "ThumbInstanceNorm.h"
#include "modules/TorchLoader.h"

void InitThumbnailInstanceNorm(ThumbAdaptiveInstanceNorm& model, const bool& collection)
{
	/*for (auto& dict : model.named_modules())
	{
		std::cout << dict.key() << " " << dict.value() << std::endl;
		std::cout << "------" << dict.pair() << std::endl;
		if (IsInstanceOf<ThumbInstanceNorm*>(&dict.value()))
		{
			//dict.value() = collection;
		}
	}*/
}