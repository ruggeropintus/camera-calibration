#pragma once

#include <exe_find_chessboard_corners/find_chessboard_corners_error_handler.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>

cv::Mat findChessboardCorners(const char* inputFName);
cv::Mat readImage(const char* fname);
void exportChessboardCorners(const char* inputFName,
							 const cv::Mat& chessboardCorners);
std::string getOutputFName(const std::string_view& strView);

static const FindChessboardCornersErrorHandler* errorHandlerPtr = nullptr;

cv::Mat findChessboardCorners(const char* /*inputFName*/) {
	// TODO: implement me
	return cv::Mat();
}

cv::Mat readImage(const char* fname) {
	cv::Mat image = cv::imread(fname, cv::IMREAD_ANYDEPTH | cv::IMREAD_COLOR);
	errorHandlerPtr->isOpen(image);
	return image;
}

void exportChessboardCorners(const char* /*inputFName*/,
							 const cv::Mat& /*chessboardCorners*/) {
	// TODO: implement me
}

std::string getOutputFName(const std::string_view& strView) {
	std::string outputFName;
	outputFName.reserve(strView.size() - 4 + 25);
	outputFName.append(strView.substr(0, strView.find('.')));
	outputFName.append("_color_aberration_error.m");
	return outputFName;
}
