#include <exe_find_chessboard_corners/main_find_chessboard_corners.hpp>

int main(int argc, char** argv) {
	FindChessboardCornersErrorHandler findChessboardCornersErrorHandler(
		argc, const_cast<const char**>(argv));
	errorHandlerPtr = &findChessboardCornersErrorHandler;

	char* inputFName = argv[1];

	const cv::Mat chessboardCorners = findChessboardCorners(inputFName);

	exportChessboardCorners(inputFName, chessboardCorners);

	return 0;
}
