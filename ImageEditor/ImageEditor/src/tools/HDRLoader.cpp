#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/photo.hpp"

#include "HDRLoader.h"

void HDRLoader::LoadHDRImage(const std::vector<std::string>& image_paths, const std::vector<float>& exposure_times, const TONEMAP_TYPE& tonemap_type)
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

	HDRLoader::ApplyTonemap(tonemap_type, hdr_debevec);
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

void HDRLoader::ApplyTonemap(const TONEMAP_TYPE& tonemap_type, const cv::Mat& hdr_debevec)
{
	switch (tonemap_type)
	{
		case TONEMAP_TYPE::DRAGO:
		{
			HDRLoader::ApplyDragoTonemap(hdr_debevec);

			break;
		}
		case TONEMAP_TYPE::REINHARD:
		{
			HDRLoader::ApplyReinhardTonemap(hdr_debevec);

			break;
		}
		case TONEMAP_TYPE::MANTIUK:
		{
			HDRLoader::ApplyMantiukTonemap(hdr_debevec);

			break;
		}
		default:
		{
			break;
		}
	}
}

void HDRLoader::ApplyDragoTonemap(const cv::Mat& hdr_debevec)
{
	cv::Mat ldr_drago;
	cv::Ptr<cv::TonemapDrago> tonemap_drago = cv::createTonemapDrago(1.0f, 0.7f);
	tonemap_drago->process(hdr_debevec, ldr_drago);
	ldr_drago *= 3;
	cv::imwrite("ldr-Drago.jpg", ldr_drago * 255);
}

void HDRLoader::ApplyReinhardTonemap(const cv::Mat& hdr_debevec)
{
	cv::Mat ldr_reinhard;
	cv::Ptr<cv::TonemapReinhard> tonemap_reinhard = cv::createTonemapReinhard(1.5f, 0.0f, 0.0f, 0.0f);
	tonemap_reinhard->process(hdr_debevec, ldr_reinhard);
	cv::imwrite("ldr-Reinhard.jpg", ldr_reinhard * 255);
}

void HDRLoader::ApplyMantiukTonemap(const cv::Mat& hdr_debevec)
{
	cv::Mat ldr_mantiuk;
	cv::Ptr<cv::TonemapMantiuk> tonemap_mantiuk = cv::createTonemapMantiuk(2.2f, 0.85f, 1.2f);
	tonemap_mantiuk->process(hdr_debevec, ldr_mantiuk);
	ldr_mantiuk *= 3;
	cv::imwrite("ldr-Mantiuk.jpg", ldr_mantiuk * 255);
}