#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>

std::string getOutputFName(const std::string_view& strView);

int main(int /*argc*/, char** argv) {
	char* imageName = argv[1];

	cv::Mat img = cv::imread(imageName, cv::IMREAD_GRAYSCALE);

	cv::Size chessboardSize(7, 14);
	cv::Mat chessboardCorners;
	bool chessboardCornersFound = cv::findChessboardCornersSB(
		img, chessboardSize, chessboardCorners, cv::CALIB_CB_EXHAUSTIVE);

	std::cout << "Chessboard corners found: "
			  << (chessboardCornersFound ? "Yes" : "No") << std::endl;

	bool chessboardCornersSPFound =
		cv::find4QuadCornerSubpix(img, chessboardCorners, chessboardSize);
	std::cout << "Chessboard subpixel corners found: "
			  << (chessboardCornersSPFound ? "Yes" : "No") << std::endl;

	cv::drawChessboardCorners(img, chessboardSize, chessboardCorners,
							  chessboardCornersFound);

	std::string outputFName = getOutputFName(imageName);
	cv::imwrite(outputFName, img);
	// std::cout << chessboardCorners << std::endl;

	return 0;
	// useful functions

	// bool 	cv::checkChessboard (InputArray img, Size size) // not working,
	// many false positives

	// void 	cv::drawChessboardCorners (InputOutputArray image, Size
	// patternSize, InputArray corners, bool patternWasFound)

	// Scalar 	cv::estimateChessboardSharpness (InputArray image, Size
	// patternSize, InputArray corners, float rise_distance=0.8F, bool
	// vertical=false, OutputArray sharpness=noArray())

	// bool 	cv::find4QuadCornerSubpix (InputArray img, InputOutputArray
	// corners, Size region_size)

	// bool 	cv::findChessboardCorners (InputArray image, Size patternSize,
	// OutputArray corners, int
	// flags=CALIB_CB_ADAPTIVE_THRESH+CALIB_CB_NORMALIZE_IMAGE)

	// bool 	cv::findChessboardCornersSB (InputArray image, Size patternSize,
	// OutputArray corners, int flags, OutputArray meta)

	// bool 	cv::findChessboardCornersSB (InputArray image, Size patternSize,
	// OutputArray corners, int flags=0)
}

std::string getOutputFName(const std::string_view& strView) {
	std::string outputFName;
	outputFName.reserve(strView.size() + 4);
	outputFName.append(strView.substr(0, strView.find('.')));
	outputFName.append("_cbc.jpg");
	return outputFName;
}
