#pragma once

#include <exe_calibrate_camera/calibrate_camera_error_handler.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

#define PRECISION() \
	std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10)

struct CameraCalibrationData {
	cv::Size imageSize;
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	cv::Mat rvecs;
	cv::Mat tvecs;
	cv::Mat stdDeviationsIntrinsics;
	cv::Mat stdDeviationsExtrinsics;
	cv::Mat perViewErrors;
	double rmsRProjError;
};

struct ChessboardViewsData {
	cv::Size chessboardSize;
	float squareSize;
	std::size_t viewsCount;
	const char** viewsFNames;
};

void calibrateCamera(CameraCalibrationData& cameraCalibrationData,
					 const ChessboardViewsData& chessboardViewsData);
std::vector<std::vector<cv::Vec3f>> computeObjectPoints(
	const ChessboardViewsData& chessboardViewsData);
std::vector<cv::Vec3f> computeChessboard3DPoints(
	const ChessboardViewsData& chessboardViewsData);
std::vector<std::vector<cv::Vec2f>> computeImage2DPoints(
	const ChessboardViewsData& chessboardViewsData);
std::vector<cv::Vec2f> loadChessboardCorners(
	const ChessboardViewsData& chessboardViewsData, const int viewIdx);
void exportIntrinsics(const CameraCalibrationData& cameraCalibrationData);
void exportExtrinsics(const CameraCalibrationData& cameraCalibrationData,
					  const ChessboardViewsData& chessboardViewsData);
void exportErrorStatistics(const CameraCalibrationData& cameraCalibrationData,
						   const ChessboardViewsData& chessboardViewsData);
void exportIntrinsicsErrStats(
	const CameraCalibrationData& cameraCalibrationData);
void exportExtrinsicsErrStats(
	const CameraCalibrationData& cameraCalibrationData,
	const ChessboardViewsData& chessboardViewsData);
std::string getOutputFName(const std::string_view& strView,
						   const std::string_view& suffixView,
						   const std::string_view& extView);

static const CalibrateCameraErrorHandler* errorHandlerPtr = nullptr;

void calibrateCamera(CameraCalibrationData& cameraCalibrationData,
					 const ChessboardViewsData& chessboardViewsData) {
	std::vector<std::vector<cv::Vec3f>> objectPoints =
		computeObjectPoints(chessboardViewsData);
	std::vector<std::vector<cv::Vec2f>> imagePoints =
		computeImage2DPoints(chessboardViewsData);

	std::cout << "Calibrating the camera ..." << std::flush;
	cameraCalibrationData.rmsRProjError = cv::calibrateCamera(
		objectPoints, imagePoints, cameraCalibrationData.imageSize,
		cameraCalibrationData.cameraMatrix, cameraCalibrationData.distCoeffs,
		cameraCalibrationData.rvecs, cameraCalibrationData.tvecs,
		cameraCalibrationData.stdDeviationsIntrinsics,
		cameraCalibrationData.stdDeviationsExtrinsics,
		cameraCalibrationData.perViewErrors);
	std::cout << "Done!" << std::endl;
}

std::vector<std::vector<cv::Vec3f>> computeObjectPoints(
	const ChessboardViewsData& chessboardViewsData) {
	std::vector<std::vector<cv::Vec3f>> objectPoints(
		chessboardViewsData.viewsCount);

	const std::vector<cv::Vec3f> chessboard3DPoints =
		computeChessboard3DPoints(chessboardViewsData);
	for (auto& elem : objectPoints) elem = chessboard3DPoints;

	return objectPoints;
}

std::vector<cv::Vec3f> computeChessboard3DPoints(
	const ChessboardViewsData& chessboardViewsData) {
	std::vector<cv::Vec3f> chessboard3DPoints;
	chessboard3DPoints.reserve(chessboardViewsData.chessboardSize.width *
							   chessboardViewsData.chessboardSize.height);
	for (int i = 0; i < chessboardViewsData.chessboardSize.height; i++)
		for (int j = 0; j < chessboardViewsData.chessboardSize.width; j++) {
			float x = j * chessboardViewsData.squareSize;
			float y = i * chessboardViewsData.squareSize;
			float z = 0.0f;
			chessboard3DPoints.push_back(cv::Vec3f(x, y, z));
		}

	return chessboard3DPoints;
}

std::vector<std::vector<cv::Vec2f>> computeImage2DPoints(
	const ChessboardViewsData& chessboardViewsData) {
	std::vector<std::vector<cv::Vec2f>> imagePoints(
		chessboardViewsData.viewsCount);

	for (int viewIdx = 0; auto& chessboardCorners : imagePoints)
		chessboardCorners =
			loadChessboardCorners(chessboardViewsData, viewIdx++);

	return imagePoints;
}

std::vector<cv::Vec2f> loadChessboardCorners(
	const ChessboardViewsData& chessboardViewsData, const int viewIdx) {
	std::vector<cv::Vec2f> chessboardCorners;
	chessboardCorners.resize(chessboardViewsData.chessboardSize.width *
							 chessboardViewsData.chessboardSize.height);

	std::fstream ifs(chessboardViewsData.viewsFNames[viewIdx],
					 std::fstream::in);
	errorHandlerPtr->isOpen(ifs);

	ifs.ignore(1024, '\n');
	ifs.ignore(1024, '\n');

	for (auto& point2D : chessboardCorners) ifs >> point2D[0] >> point2D[1];

	ifs.close();

	return chessboardCorners;
}

void exportIntrinsics(const CameraCalibrationData& cameraCalibrationData) {
	const std::string outputFName = "intrinsics.txt";

	std::fstream ofs(outputFName, std::fstream::out);
	ofs << PRECISION();

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			ofs << cameraCalibrationData.cameraMatrix.at<double>(i, j);
			ofs << ((j == 2) ? '\n' : ' ');
		}
	}

	for (int i = 0; i < cameraCalibrationData.distCoeffs.size().width; ++i) {
		ofs << cameraCalibrationData.distCoeffs.at<double>(0, i);
		ofs << ((i == cameraCalibrationData.distCoeffs.size().width - 1) ? '\n'
																		 : ' ');
	}

	ofs << cameraCalibrationData.imageSize.width << " "
		<< cameraCalibrationData.imageSize.height << std::endl;

	ofs.close();
}

void exportExtrinsics(const CameraCalibrationData& cameraCalibrationData,
					  const ChessboardViewsData& chessboardViewsData) {
	const std::string outputFName = "extrinsics.txt";

	std::fstream ofs(outputFName, std::fstream::out);
	ofs << PRECISION();

	ofs << chessboardViewsData.viewsCount << std::endl;

	for (std::size_t viewIdx = 0; viewIdx < chessboardViewsData.viewsCount;
		 ++viewIdx) {
		ofs << chessboardViewsData.viewsFNames[viewIdx] << " ";
		for (int j = 0; j < 3; ++j)
			ofs << cameraCalibrationData.rvecs.ptr<double>(viewIdx)[j] << " ";
		for (int j = 0; j < 3; ++j)
			ofs << cameraCalibrationData.tvecs.ptr<double>(viewIdx)[j] << " ";
		ofs << std::endl;
	}

	ofs.close();
}

void exportErrorStatistics(const CameraCalibrationData& cameraCalibrationData,
						   const ChessboardViewsData& chessboardViewsData) {
	exportIntrinsicsErrStats(cameraCalibrationData);
	exportExtrinsicsErrStats(cameraCalibrationData, chessboardViewsData);
}

void exportIntrinsicsErrStats(
	const CameraCalibrationData& cameraCalibrationData) {
	const std::string outputFName = "intrinsics_stats.txt";

	std::fstream ofs(outputFName, std::fstream::out);
	ofs << PRECISION();

	cv::Mat cameraMatrixStd(3, 3, CV_64FC1);
	cameraMatrixStd.setTo(0.0);
	cameraMatrixStd.at<double>(0, 0) =
		cameraCalibrationData.stdDeviationsIntrinsics.at<double>(0, 0);
	cameraMatrixStd.at<double>(1, 1) =
		cameraCalibrationData.stdDeviationsIntrinsics.at<double>(1, 0);
	cameraMatrixStd.at<double>(0, 2) =
		cameraCalibrationData.stdDeviationsIntrinsics.at<double>(2, 0);
	cameraMatrixStd.at<double>(1, 2) =
		cameraCalibrationData.stdDeviationsIntrinsics.at<double>(3, 0);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			ofs << cameraMatrixStd.at<double>(i, j);
			ofs << ((j == 2) ? '\n' : ' ');
		}
	}

	for (int i = 0; i < cameraCalibrationData.distCoeffs.size().width; ++i) {
		ofs << cameraCalibrationData.stdDeviationsIntrinsics.at<double>(i + 4,
																		0);
		ofs << ((i == cameraCalibrationData.distCoeffs.size().width - 1) ? '\n'
																		 : ' ');
	}

	ofs.close();
}

void exportExtrinsicsErrStats(
	const CameraCalibrationData& cameraCalibrationData,
	const ChessboardViewsData& chessboardViewsData) {
	const std::string outputFName = "extrinsics_stats.txt";

	std::fstream ofs(outputFName, std::fstream::out);
	ofs << PRECISION();

	ofs << "#viewCount totalReprojectionError" << std::endl;
	ofs << chessboardViewsData.viewsCount << " "
		<< cameraCalibrationData.rmsRProjError << std::endl;

	ofs << "#imageName perViewError stdRVec stdTVec" << std::endl;

	std::size_t count = 0;
	for (std::size_t viewIdx = 0; viewIdx < chessboardViewsData.viewsCount;
		 ++viewIdx) {
		ofs << chessboardViewsData.viewsFNames[viewIdx] << " ";
		ofs << cameraCalibrationData.perViewErrors.at<double>(viewIdx, 0)
			<< " ";
		for (int j = 0; j < 6; ++j)
			ofs << cameraCalibrationData.stdDeviationsExtrinsics.at<double>(
					   count++, 0)
				<< " ";
		ofs << std::endl;
	}

	ofs.close();
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
