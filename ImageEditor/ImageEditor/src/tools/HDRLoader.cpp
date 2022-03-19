#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/photo.hpp>

#include "HDRLoader.h"

void HDRLoader::LoadHDRImage(const std::vector<std::string>& image_paths, const std::vector<float>& exposure_times)
{
	std::vector<cv::Mat> images = HDRLoader::ReadImages(image_paths);
	std::vector<float> times = ReadTimes(exposure_times);

	cv::Ptr<cv::AlignMTB> align_mtb = cv::createAlignMTB();
	align_mtb->process(images, images);

	cv::Mat response_debevec;
	cv::Ptr<cv::CalibrateDebevec> calibrateDebevec = cv::createCalibrateDebevec();
	calibrateDebevec->process(images, response_debevec, times);

	cv::Mat hdr_debevec;
	cv::Ptr<cv::MergeDebevec> merge_debevec = cv::createMergeDebevec();
	merge_debevec->process(images, hdr_debevec, times, response_debevec);
	cv::imwrite("image.hdr", hdr_debevec);
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