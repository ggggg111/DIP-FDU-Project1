#ifndef __TORCH_LOADER_H__
#define __TORCH_LOADER_H__

#include <torch/torch.h>

#include "Module.h"

class TorchLoader : public Module
{
public:
	TorchLoader();
	~TorchLoader();

	void Start() override;
	void CleanUp() override;

private:
	void LoadFastFlowModel();

private:
	torch::jit::script::Module fastflow_model;
};

#endif /* __TORCH_LOADER_H__ */