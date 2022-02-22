#pragma once

#include <exe_find_chessboard_corners/find_chessboard_corners_error_handler.hpp>
#include <fstream>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>

cv::Mat findChessboardCorners(const char* inputFName,
							  const cv::Size& chessboardSize);
cv::Mat readImage(const char* fname, const int flag);
void exportChessboardCorners(const char* inputFName,
							 const cv::Size& chessboardSize,
							 const cv::Mat& chessboardCorners);
void exportChessboardCornersImage(const char* inputFName,
								  const cv::Size& chessboardSize,
								  const cv::Mat& chessboardCorners);
std::string getOutputFName(const std::string_view& strView,
						   const std::string_view& suffixView,
						   const std::string_view& extView);

static const FindChessboardCornersErrorHandler* errorHandlerPtr = nullptr;

cv::Mat findChessboardCorners(const char* inputFName,
							  const cv::Size& chessboardSize) {
	cv::Mat chessboardCorners;

	cv::Mat img = readImage(inputFName, cv::IMREAD_GRAYSCALE);
	bool chessboardCornersFound = cv::findChessboardCornersSB(
		img, chessboardSize, chessboardCorners, cv::CALIB_CB_EXHAUSTIVE);
	chessboardCornersFound &=
		cv::find4QuadCornerSubpix(img, chessboardCorners, chessboardSize);
	errorHandlerPtr->areCornersFound(chessboardCornersFound);

	return (chessboardCornersFound ? chessboardCorners : cv::Mat());
}

cv::Mat readImage(const char* fname, const int flag) {
	cv::Mat image = cv::imread(fname, flag);
	errorHandlerPtr->isOpen(image);
	return image;
}

void exportChessboardCorners(const char* inputFName,
							 const cv::Size& chessboardSize,
							 const cv::Mat& chessboardCorners) {
	const std::string outputFName = getOutputFName(inputFName, "", "cbc");
	const int cornersCount = chessboardCorners.size().height;

	std::fstream ofs(outputFName, std::fstream::out);

	ofs << chessboardSize.width << " " << chessboardSize.height << std::endl;

	for (int i = 0; i < cornersCount; ++i) {
		ofs << chessboardCorners.ptr<float>(i)[0] << " "
			<< chessboardCorners.ptr<float>(i)[1] << std::endl;
	}

	ofs.close();
}

void exportChessboardCornersImage(const char* inputFName,
								  const cv::Size& chessboardSize,
								  const cv::Mat& chessboardCorners) {
	cv::Mat img = readImage(inputFName, cv::IMREAD_COLOR);
	cv::drawChessboardCorners(img, chessboardSize, chessboardCorners, true);
	std::string outputFName = getOutputFName(inputFName, "cbc", "jpg");
	cv::imwrite(outputFName, img);
}

std::string getOutputFName(const std::string_view& strView,
						   const std::string_view& suffixView,
						   const std::string_view& extView) {
	std::string outputFName;
	outputFName.reserve(strView.size() + 1 + suffixView.size() + 1 +
						extView.size());
	outputFName.append(strView.substr(0, strView.find('.')));
	if (suffixView.size() > 0) outputFName.append("_");
	outputFName.append(suffixView);
	outputFName.append(".");
	outputFName.append(extView);
	return outputFName;
}
