#pragma once

#include <camera_calibration/camera_calibration.hpp>
#include <exe_calibrate_camera/calibrate_camera_error_handler.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

using CameraCalibration = camera_calibration::CameraCalibration;

static const CalibrateCameraErrorHandler* errorHandlerPtr = nullptr;

#if 0
	cameraCalibrationData.rmsRProjError = cv::calibrateCamera(
		objectPoints, imagePoints, cameraCalibrationData.imageSize,
		cameraCalibrationData.cameraMatrix, cameraCalibrationData.distCoeffs,
		cameraCalibrationData.rvecs, cameraCalibrationData.tvecs,
		cameraCalibrationData.stdDeviationsIntrinsics,
		cameraCalibrationData.stdDeviationsExtrinsics,
		cameraCalibrationData.perViewErrors,
		cv::CALIB_FIX_PRINCIPAL_POINT + cv::CALIB_FIX_ASPECT_RATIO);
	std::cout << "-------------------------" << std::endl;
	// std::cout << cameraCalibrationData.rmsRProjError << std::endl;
	// std::cout << cameraCalibrationData.cameraMatrix << std::endl;
	// std::cout << cameraCalibrationData.perViewErrors << std::endl;
	std::cout << cameraCalibrationData.tvecs << std::endl;
	// cameraCalibrationData.rmsRProjError = cv::calibrateCamera(
	// 	objectPoints, imagePoints, cameraCalibrationData.imageSize,
	// 	cameraCalibrationData.cameraMatrix, cameraCalibrationData.distCoeffs,
	// 	cameraCalibrationData.rvecs, cameraCalibrationData.tvecs,
	// 	cameraCalibrationData.stdDeviationsIntrinsics,
	// 	cameraCalibrationData.stdDeviationsExtrinsics,
	// 	cameraCalibrationData.perViewErrors, cv::CALIB_USE_INTRINSIC_GUESS);
	// std::cout << "-------------------------" << std::endl;
	// std::cout << cameraCalibrationData.rmsRProjError << std::endl;
	// std::cout << cameraCalibrationData.cameraMatrix << std::endl;
	// std::cout << cameraCalibrationData.perViewErrors << std::endl;
#endif
