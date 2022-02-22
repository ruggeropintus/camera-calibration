#include <exe_find_chessboard_corners/find_chessboard_corners_error_handler.hpp>
#include <iostream>

#define RED_BOLD_TEXT(x) "\033[1;31m" << x << "\033[0m"

FindChessboardCornersErrorHandler::FindChessboardCornersErrorHandler(
	int argc, const char** argv)
	: m_argc(argc), m_argv(argv) {
	checkArgCount();
}

void FindChessboardCornersErrorHandler::checkArgCount() const {
	if (m_argc != 4) {
		std::cout << RED_BOLD_TEXT("ERROR: wrong number of arguments!!!")
				  << " (argc != 4)" << std::endl;
		showUsageAndExit();
	}
}

void FindChessboardCornersErrorHandler::showUsageAndExit() const {
	std::cout << "\n\t----------------" << std::endl;
	std::cout << "\tUsage: " << m_argv[0]
			  << " <image.xxx> <inner_corners_width> <inner_corners_height>"
			  << std::endl;
	std::cout << "\tExample: " << m_argv[0] << " DSC_1778.jpg 14 7"
			  << std::endl;
	std::cout << "\t----------------\n" << std::endl;
	exit(1);
}

void FindChessboardCornersErrorHandler::isOpen(const cv::Mat& image) const {
	if (image.data == NULL) {
		std::cout << RED_BOLD_TEXT("ERROR: unable to read image!!!")
				  << std::endl;
		showUsageAndExit();
	}
}

void FindChessboardCornersErrorHandler::areCornersFound(
	const bool cornersFound) const {
	if (!cornersFound) {
		std::cout << RED_BOLD_TEXT(
						 "ERROR: unable to find chessboard corners!!!")
				  << std::endl;
		showUsageAndExit();
	}
}
