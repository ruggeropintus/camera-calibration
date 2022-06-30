#include <exe_calibrate_camera/main_calibrate_camera.hpp>

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
	cameraCalibration.loadChessboardPatterns(const_cast<const char**>(&argv[7]),
											 argc - 7);

	cameraCalibration.run();
	cameraCalibration.exportCalibration(argv[6]);

	return 0;
}
