#pragma once

#include <exe_find_chessboard_corners/find_chessboard_corners_error_handler.hpp>
#include <fstream>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

struct ChessboardData {
	const char* fName = nullptr;
	cv::Mat image;
	cv::Size size;
	cv::Mat corners;
};

cv::Mat readImage(const char* fname);
cv::Mat findChessboardCorners(const cv::Mat& chessboardImage,
							  const cv::Size& chessboardSize);
void exportChessboardCorners(const ChessboardData& chessboardData);
void exportChessboardCornersImage(const ChessboardData& chessboardData);
std::string getOutputFName(const std::string_view& strView,
						   const std::string_view& suffixView,
						   const std::string_view& extView);

static const FindChessboardCornersErrorHandler* errorHandlerPtr = nullptr;

cv::Mat readImage(const char* fname) {
	cv::Mat image = cv::imread(fname, cv::IMREAD_COLOR);
	errorHandlerPtr->isOpen(image);
	return image;
}

cv::Mat findChessboardCorners(const cv::Mat& chessboardImage,
							  const cv::Size& chessboardSize) {
	cv::Mat chessboardCorners;

	cv::Mat gray;
	cv::cvtColor(chessboardImage, gray, cv::COLOR_BGR2GRAY);

	bool chessboardCornersFound =
		cv::findChessboardCornersSB(chessboardImage, chessboardSize,
									chessboardCorners, cv::CALIB_CB_EXHAUSTIVE);
	chessboardCornersFound &=
		cv::find4QuadCornerSubpix(gray, chessboardCorners, chessboardSize);
	errorHandlerPtr->areCornersFound(chessboardCornersFound);

	return chessboardCorners;
}

void exportChessboardCorners(const ChessboardData& chessboardData) {
	const std::string outputFName =
		getOutputFName(chessboardData.fName, "", "cbc");
	const int cornersCount = chessboardData.corners.size().height;

	std::fstream ofs(outputFName, std::fstream::out);

	ofs << "#width height sharpness" << std::endl;
	ofs << chessboardData.size.width << " " << chessboardData.size.height
		<< " ";
	ofs << cv::estimateChessboardSharpness(chessboardData.image,
										   chessboardData.size,
										   chessboardData.corners)[0]
		<< std::endl;

	for (int i = 0; i < cornersCount; ++i) {
		ofs << chessboardData.corners.ptr<float>(i)[0] << " "
			<< chessboardData.corners.ptr<float>(i)[1] << std::endl;
	}

	ofs.close();
}

void exportChessboardCornersImage(ChessboardData& chessboardData) {
	cv::drawChessboardCorners(chessboardData.image, chessboardData.size,
							  chessboardData.corners, true);
	std::string outputFName =
		getOutputFName(chessboardData.fName, "cbc", "jpg");
	cv::imwrite(outputFName, chessboardData.image);
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
