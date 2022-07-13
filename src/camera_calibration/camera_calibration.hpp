#pragma once

#include <camera_calibration/chessboard_pattern.hpp>
#include <opencv2/calib3d.hpp>

namespace camera_calibration {

class CameraCalibration {
   public:
	void run();
	void exportCalibration(const char* prefix) const;

   public:
	void setImageSize(const cv::Size& size) { m_ImageSize = size; }
	void setChessboardPatternSize(const cv::Size& size) {
		m_ChessboardPatternSize = size;
	}
	void setChessboardPatternSquareSize(const float& size) {
		m_ChessboardPatternSquareSize = size;
	}
	void loadChessboardPatterns(const char** viewsFNames,
								const std::size_t viewsCount);
	void setCalibrationFlags(const int flags) { m_CalibrationFlags = flags; };

   private:
	std::vector<std::vector<cv::Vec3f>> computeObjectPoints() const;
	std::vector<cv::Vec3f> computeChessboard3DPoints() const;
	std::vector<std::vector<cv::Vec2f>> computeImagePoints() const;
	std::vector<cv::Vec2f> loadChessboardCorners(
		const ChessboardPattern& chessboardPattern) const;
	void exportIntrinsics(const char* prefix) const;
	void exportExtrinsics(const char* prefix) const;
	void exportErrorStatistics(const char* prefix) const;
	void exportIntrinsicsErrStats(const char* prefix) const;
	void exportExtrinsicsErrStats(const char* prefix) const;

   private:
	cv::Size m_ImageSize;
	cv::Size m_ChessboardPatternSize;
	float m_ChessboardPatternSquareSize;
	std::vector<ChessboardPattern> m_ChessboardPatternVec;

	cv::Mat m_CameraMatrix;
	cv::Mat m_DistCoeffs;
	cv::Mat m_RVecs;
	cv::Mat m_TVecs;
	cv::Mat m_StdDeviationsIntrinsics;
	cv::Mat m_StdDeviationsExtrinsics;
	cv::Mat m_PerViewErrors;
	int m_CalibrationFlags = 0;
	double m_RmsRProjError;

	const char** m_ViewsFNames = nullptr;

   public:
	enum CalibrationFlag {
		DEFAULT = 0,
		RATIONAL = cv::CALIB_RATIONAL_MODEL,
		THIN_PRISM = cv::CALIB_RATIONAL_MODEL | cv::CALIB_THIN_PRISM_MODEL,
		TILTED = cv::CALIB_RATIONAL_MODEL | cv::CALIB_THIN_PRISM_MODEL |
				 cv::CALIB_TILTED_MODEL
	};
};

}  // namespace camera_calibration
