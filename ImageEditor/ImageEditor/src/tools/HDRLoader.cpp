#include <windows.h>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/photo.hpp"

#include "Application.h"
#include "modules/Editor.h"
#include "modules/Renderer.h"
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
	//cv::imwrite("image.hdr", hdr_debevec);

	cv::Mat ldr = HDRLoader::ApplyTonemap(tonemap_type, hdr_debevec);

	HDRLoader::SendImageToEditor(ldr);
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

cv::Mat HDRLoader::ApplyTonemap(const TONEMAP_TYPE& tonemap_type, const cv::Mat& hdr_debevec)
{
	cv::Mat ret;

	switch (tonemap_type)
	{
		case TONEMAP_TYPE::DRAGO:
		{
			ret = HDRLoader::ApplyDragoTonemap(hdr_debevec);

			break;
		}
		case TONEMAP_TYPE::REINHARD:
		{
			ret = HDRLoader::ApplyReinhardTonemap(hdr_debevec);

			break;
		}
		case TONEMAP_TYPE::MANTIUK:
		{
			ret = HDRLoader::ApplyMantiukTonemap(hdr_debevec);

			break;
		}
		default:
		{
			break;
		}
	}

	return ret;
}

cv::Mat HDRLoader::ApplyDragoTonemap(const cv::Mat& hdr_debevec)
{
	cv::Mat ldr_drago;
	cv::Ptr<cv::TonemapDrago> tonemap_drago = cv::createTonemapDrago(1.0f, 0.7f);
	tonemap_drago->process(hdr_debevec, ldr_drago);
	ldr_drago *= 3;
	//cv::imwrite("ldr-Drago.jpg", ldr_drago * 255);

	return ldr_drago;
}

cv::Mat HDRLoader::ApplyReinhardTonemap(const cv::Mat& hdr_debevec)
{
	cv::Mat ldr_reinhard;
	cv::Ptr<cv::TonemapReinhard> tonemap_reinhard = cv::createTonemapReinhard(1.5f, 0.0f, 0.0f, 0.0f);
	tonemap_reinhard->process(hdr_debevec, ldr_reinhard);
	//cv::imwrite("ldr-Reinhard.jpg", ldr_reinhard * 255);

	return ldr_reinhard;
}

cv::Mat HDRLoader::ApplyMantiukTonemap(const cv::Mat& hdr_debevec)
{
	cv::Mat ldr_mantiuk;
	cv::Ptr<cv::TonemapMantiuk> tonemap_mantiuk = cv::createTonemapMantiuk(2.2f, 0.85f, 1.2f);
	tonemap_mantiuk->process(hdr_debevec, ldr_mantiuk);
	ldr_mantiuk *= 3;
	//cv::imwrite("ldr-Mantiuk.jpg", ldr_mantiuk * 255);

	return ldr_mantiuk;
}

void HDRLoader::SendImageToEditor(const cv::Mat& ldr)
{
	std::string extension = ".jpg";

	std::string save_path;

	char temp_filename[MAX_PATH] = { 0 };
	tmpnam_s(temp_filename);

	save_path.append(temp_filename).append(extension);

	cv::imwrite(save_path, ldr * 255);

	App->editor->bg = App->editor->LoadImg(save_path);
	App->editor->RenderImg(App->editor->bg, App->renderer->texture_target);
}