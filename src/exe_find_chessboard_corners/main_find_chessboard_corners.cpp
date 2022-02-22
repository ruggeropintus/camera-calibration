#include <exe_find_chessboard_corners/main_find_chessboard_corners.hpp>

int main(int argc, char** argv) {
	FindChessboardCornersErrorHandler findChessboardCornersErrorHandler(
		argc, const_cast<const char**>(argv));
	errorHandlerPtr = &findChessboardCornersErrorHandler;

	ChessboardData chessboardData;

	chessboardData.fName = argv[1];
	chessboardData.image = readImage(argv[1]);
	chessboardData.size = cv::Size(std::atoi(argv[2]), std::atoi(argv[3]));
	chessboardData.corners =
		findChessboardCorners(chessboardData.image, chessboardData.size);

	exportChessboardCorners(chessboardData);
	exportChessboardCornersImage(chessboardData);

	return 0;
}
