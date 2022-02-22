#include <exe_find_chessboard_corners/main_find_chessboard_corners.hpp>

int main(int argc, char** argv) {
	FindChessboardCornersErrorHandler findChessboardCornersErrorHandler(
		argc, const_cast<const char**>(argv));
	errorHandlerPtr = &findChessboardCornersErrorHandler;

	const char* inputFName = argv[1];
	const cv::Size chessboardSize(std::atoi(argv[2]), std::atoi(argv[3]));

	const cv::Mat chessboardCorners =
		findChessboardCorners(inputFName, chessboardSize);

	exportChessboardCorners(inputFName, chessboardSize, chessboardCorners);
	exportChessboardCornersImage(inputFName, chessboardSize, chessboardCorners);

	return 0;
}
