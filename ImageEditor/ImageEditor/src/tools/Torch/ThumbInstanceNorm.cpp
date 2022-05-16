#include "ThumbInstanceNorm.h"
#include "modules/TorchLoader.h"

void InitThumbnailInstanceNorm(ThumbAdaptiveInstanceNorm& model, const bool& collection)
{
	model.collection = collection;
}