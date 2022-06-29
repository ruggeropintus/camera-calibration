#include <exe_find_chessboard_corners/main_find_chessboard_corners.hpp>

int main(int argc, char** argv) {
	FindChessboardCornersErrorHandler findChessboardCornersErrorHandler(
		argc, const_cast<const char**>(argv));
	errorHandlerPtr = &findChessboardCornersErrorHandler;

	ChessboardPattern chessboardPattern;

	chessboardPattern.create(argv[1],
							 cv::Size(std::atoi(argv[2]), std::atoi(argv[3])));
	errorHandlerPtr->isChessboardPatternOK(chessboardPattern.isOk());

	exportChessboardCorners(argv[1], chessboardPattern);
	exportChessboardCornersImage(argv[1], chessboardPattern);

	return 0;
}
