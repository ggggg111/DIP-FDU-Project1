#ifndef __HDR_LOADER_H__
#define __HDR_LOADER_H__

#include <list>
#include <string>

#include "opencv2/imgcodecs.hpp"

class HDRLoader
{
public:
	static void LoadHDRImage(const std::vector<std::string>& image_paths, const std::vector<float>& exposure_times);

private:
	static std::vector<cv::Mat> ReadImages(const std::vector<std::string>& image_paths);
	static std::vector<float> ReadTimes(const std::vector<float>& exposure_times);
};

#endif /* __HDR_LOADER_H__ */