#include <exe_calibrate_camera/calibrate_camera_error_handler.hpp>
#include <iostream>

#define RED_BOLD_TEXT(x) "\033[1;31m" << x << "\033[0m"

CalibrateCameraErrorHandler::CalibrateCameraErrorHandler(int argc,
														 const char** argv)
	: m_argc(argc), m_argv(argv) {
	checkArgCount();
	checkInputArgs();
}

void CalibrateCameraErrorHandler::checkArgCount() const {
	if (m_argc < 8) {
		std::cout << RED_BOLD_TEXT("ERROR: wrong number of arguments!!!")
				  << " (argc < 7)" << std::endl;
		showUsageAndExit();
	}
}

void CalibrateCameraErrorHandler::showUsageAndExit() const {
	std::cout << "\n\t----------------" << std::endl;
	std::cout << "\tUsage: " << m_argv[0]
			  << " <image_width> <image_height> <chessboard_corners_width> "
				 "<chessboard_corners_height> <chessboard_square_size> "
				 "<output_basename> <cbc_files>"
			  << std::endl;
	std::cout << "\tExample: " << m_argv[0]
			  << " 7380 4928 7 14 9.973 calibration *.cbc" << std::endl;
	std::cout << "\t----------------\n" << std::endl;
	exit(1);
}

void CalibrateCameraErrorHandler::checkInputArgs() const {
	for (int i = 7; i < m_argc; ++i) {
		if (!std::string_view(m_argv[i]).ends_with(".cbc")) {
			std::cout << RED_BOLD_TEXT(
							 "ERROR: unknown chessboard corner file type!!!")
					  << std::endl;
			showUsageAndExit();
		}
	}
}

void CalibrateCameraErrorHandler::isOpen(const std::fstream& ifs) const {
	if (!ifs.is_open()) {
		std::cout << RED_BOLD_TEXT("ERROR: unable to read cbc file!!!")
				  << std::endl;
		showUsageAndExit();
	}
}
