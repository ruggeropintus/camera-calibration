#include <exe_calibrate_camera/main_calibrate_camera.hpp>

int main(int argc, char** argv) {
	CalibrateCameraErrorHandler calibrateCameraErrorHandler(
		argc, const_cast<const char**>(argv));
	errorHandlerPtr = &calibrateCameraErrorHandler;

	CameraCalibrationData cameraCalibrationData;
	ChessboardViewsData chessboardViewsData;

	cameraCalibrationData.imageSize =
		cv::Size(std::atoi(argv[1]), std::atoi(argv[2]));
	chessboardViewsData.chessboardSize =
		cv::Size(std::atoi(argv[3]), std::atoi(argv[4]));
	chessboardViewsData.squareSize = std::atof(argv[5]);
	chessboardViewsData.viewsCount = argc - 6;
	chessboardViewsData.viewsFNames = const_cast<const char**>(&argv[6]);

	calibrateCamera(cameraCalibrationData, chessboardViewsData);

	exportIntrinsics(cameraCalibrationData);
	exportExtrinsics(cameraCalibrationData);
	exportStatistics(cameraCalibrationData);

	return 0;
}
