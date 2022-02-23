#include <cstdlib>
#include <fstream>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

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

std::vector<std::vector<cv::Vec3f>> computeObjectPoints(
	const ChessboardViewsData& chessboardViewsData);
std::vector<cv::Vec3f> computeChessboard3DPoints(
	const ChessboardViewsData& chessboardViewsData);
std::vector<std::vector<cv::Vec2f>> computeImage2DPoints(
	const ChessboardViewsData& chessboardViewsData);
std::vector<cv::Vec2f> loadChessboardCorners(
	const ChessboardViewsData& chessboardViewsData, const int viewIdx);
void printObjectVSImagePoints(
	const std::vector<std::vector<cv::Vec3f>>& objectPoints,
	const std::vector<std::vector<cv::Vec2f>>& imagePoints);

int main(int argc, char** argv) {
	CameraCalibrationData cameraCalibrationData;
	ChessboardViewsData chessboardViewsData;

	cameraCalibrationData.imageSize =
		cv::Size(std::atoi(argv[1]), std::atoi(argv[2]));
	chessboardViewsData.chessboardSize =
		cv::Size(std::atoi(argv[3]), std::atoi(argv[4]));
	chessboardViewsData.squareSize = std::atof(argv[5]);
	chessboardViewsData.viewsCount = argc - 6;
	chessboardViewsData.viewsFNames = const_cast<const char**>(&argv[6]);

	std::cout << "Chessboard size - " << chessboardViewsData.chessboardSize
			  << std::endl;
	std::cout << "Chessboard square size - " << chessboardViewsData.squareSize
			  << std::endl;
	std::cout << "Chessboard view count - " << chessboardViewsData.viewsCount
			  << std::endl;

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

	std::cout << "Camera matrix:\n"
			  << cameraCalibrationData.cameraMatrix << std::endl;
	std::cout << "Dist coefficients:\n"
			  << cameraCalibrationData.distCoeffs << std::endl;
	std::cout << "Reprojection Error:\n"
			  << cameraCalibrationData.rmsRProjError << std::endl;
	std::cout << "STD Intrinsics:\n"
			  << cameraCalibrationData.stdDeviationsIntrinsics << std::endl;
	std::cout << "STD Extrinsics:\n"
			  << cameraCalibrationData.stdDeviationsExtrinsics << std::endl;
	std::cout << "Per view errors:\n"
			  << cameraCalibrationData.perViewErrors << std::endl;

	// printObjectVSImagePoints(objectPoints, imagePoints);

	return 0;
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
	ifs.ignore(1024, '\n');
	ifs.ignore(1024, '\n');

	for (auto& point2D : chessboardCorners) ifs >> point2D[0] >> point2D[1];

	ifs.close();

	return chessboardCorners;
}

void printObjectVSImagePoints(
	const std::vector<std::vector<cv::Vec3f>>& objectPoints,
	const std::vector<std::vector<cv::Vec2f>>& imagePoints) {
	for (int viewIdx = 0; const auto& chessboard3DPoints : objectPoints) {
		for (int pointIdx = 0; const auto& point3D : chessboard3DPoints) {
			std::cout << point3D << " - " << imagePoints[viewIdx][pointIdx++]
					  << std::endl;
		}
		viewIdx++;
	}
}

// double 	cv::calibrateCamera

// double 	cv::calibrateCamera

// double 	cv::calibrateCameraRO (

// void 	cv::calibrationMatrixValues

// void 	cv::drawFrameAxes

// void 	cv::projectPoints
