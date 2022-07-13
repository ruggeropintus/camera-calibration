#include <exe_calibrate_camera/main_calibrate_camera.hpp>

/*
----------------
Usage:

calibrate_camera [1]<image_width> [2]<image_height>
[3]<chessboard_corners_width> [4]<chessboard_corners_height>
[5]<chessboard_square_size> [6]<calibration_flags> [7]<output_basename>
[8]<cbc_files>

Available Calibration Flags: "DEFAULT" (recommended), "RATIONAL", "THIN_PRISM",
"TILTED"

Example:

calibrate_camera 7380 4928 7 14 9.973 calibration *.cbc
----------------
Distortion coefficients

k1 k2 p1 p2 k3 - DEFAULT

k4 k5 k6 - CALIB_RATIONAL_MODEL

s1 s2 s3 s4 - CALIB_THIN_PRISM_MODEL

tx ty - CALIB_TILTED_MODEL

*/

int main(int argc, char** argv) {
	CalibrateCameraErrorHandler calibrateCameraErrorHandler(
		argc, const_cast<const char**>(argv));
	errorHandlerPtr = &calibrateCameraErrorHandler;

	CameraCalibration cameraCalibration;

	cameraCalibration.setImageSize(
		cv::Size(std::atoi(argv[1]), std::atoi(argv[2])));
	cameraCalibration.setChessboardPatternSize(
		cv::Size(std::atoi(argv[3]), std::atoi(argv[4])));
	cameraCalibration.setChessboardPatternSquareSize(std::atof(argv[5]));
	setCalibrationFlags(cameraCalibration, argv[6]);
	cameraCalibration.loadChessboardPatterns(const_cast<const char**>(&argv[8]),
											 argc - 8);

	cameraCalibration.run();
	cameraCalibration.exportCalibration(argv[7]);

	return 0;
}
