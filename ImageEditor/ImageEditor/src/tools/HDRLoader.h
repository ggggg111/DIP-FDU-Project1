#ifndef __HDR_LOADER_H__
#define __HDR_LOADER_H__

#include <list>
#include <string>

#include "opencv2/imgcodecs.hpp"

enum class TONEMAP_TYPE
{
	DRAGO = 0,
	REINHARD,
	MANTIUK
};

class HDRLoader
{
public:
	static void LoadHDRImage(const std::vector<std::string>& image_paths, const std::vector<float>& exposure_times, const TONEMAP_TYPE& tonemap_type);

private:
	static std::vector<cv::Mat> ReadImages(const std::vector<std::string>& image_paths);
	static std::vector<float> ReadTimes(const std::vector<float>& exposure_times);
	
	static void ApplyTonemap(const TONEMAP_TYPE& tonemap_type, const cv::Mat& hdr_debevec);
	static void ApplyDragoTonemap(const cv::Mat& hdr_debevec);
	static void ApplyReinhardTonemap(const cv::Mat& hdr_debevec);
	static void ApplyMantiukTonemap(const cv::Mat& hdr_debevec);
};

#endif /* __HDR_LOADER_H__ */