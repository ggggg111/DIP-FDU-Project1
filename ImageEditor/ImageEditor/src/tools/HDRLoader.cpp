#include "HDRLoader.h"

void HDRLoader::LoadHDRImage(const std::vector<std::string>& image_paths, const std::vector<float>& exposure_times)
{
	std::vector<cv::Mat> images = HDRLoader::ReadImages(image_paths);
	std::vector<float> times = ReadTimes(exposure_times);


}

std::vector<cv::Mat> HDRLoader::ReadImages(const std::vector<std::string>& image_paths)
{
	std::vector<cv::Mat> images;
	for (const auto& image_path : image_paths)
	{
		images.push_back(cv::imread(image_path));
	}

	return images;
}

std::vector<float> HDRLoader::ReadTimes(const std::vector<float>& exposure_times)
{
	return std::vector<float>(exposure_times);
}