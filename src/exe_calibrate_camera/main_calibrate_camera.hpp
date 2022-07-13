#pragma once

#include <camera_calibration/camera_calibration.hpp>
#include <exe_calibrate_camera/calibrate_camera_error_handler.hpp>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

using CameraCalibration = camera_calibration::CameraCalibration;

static const CalibrateCameraErrorHandler* errorHandlerPtr = nullptr;

void setCalibrationFlags(CameraCalibration& cameraCalibration,
						 const char* calibrationFlag);

void setCalibrationFlags(CameraCalibration& cameraCalibration,
						 const char* calibrationFlag) {
	if (std::string_view(calibrationFlag) == "DEFAULT") {
		cameraCalibration.setCalibrationFlags(CameraCalibration::DEFAULT);
	} else if (std::string_view(calibrationFlag) == "CALIB_RATIONAL_MODEL") {
		cameraCalibration.setCalibrationFlags(CameraCalibration::RATIONAL);
	} else if (std::string_view(calibrationFlag) == "CALIB_THIN_PRISM_MODEL") {
		cameraCalibration.setCalibrationFlags(CameraCalibration::THIN_PRISM);
	} else if (std::string_view(calibrationFlag) == "CALIB_TILTED_MODEL") {
		cameraCalibration.setCalibrationFlags(CameraCalibration::TILTED);
	} else {
		cameraCalibration.setCalibrationFlags(CameraCalibration::DEFAULT);
	}
}
