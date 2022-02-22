#include <cstdlib>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

struct ChessboardData {
	cv::Size size;
	float squareSize;
	std::size_t viewCount;
};

std::vector<std::vector<cv::Vec3f>> computeObjectPoints(
	const ChessboardData& chessboardData);

int main(int argc, char** argv) {
	ChessboardData chessboardData;

	chessboardData.size = cv::Size(std::atoi(argv[1]), std::atoi(argv[2]));
	chessboardData.squareSize = std::atof(argv[3]);
	chessboardData.viewCount = argc - 4;

	std::vector<std::vector<cv::Vec3f>> objectPoints =
		computeObjectPoints(chessboardData);

	// std::vector<std::string> chessboardCornersFNames(argc - );

	return 0;
}

std::vector<std::vector<cv::Vec3f>> computeObjectPoints(
	const ChessboardData& /*chessboardData*/) {
	// TODO: implement me
	return std::vector<std::vector<cv::Vec3f>>();
}

// double 	cv::calibrateCamera

// double 	cv::calibrateCamera

// double 	cv::calibrateCameraRO (

// void 	cv::calibrationMatrixValues

// void 	cv::drawFrameAxes

// void 	cv::projectPoints
