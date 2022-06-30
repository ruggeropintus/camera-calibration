#include <camera_calibration/camera_calibration.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

#define PRECISION() \
	std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10)

namespace camera_calibration {

void CameraCalibration::run() {
	std::vector<std::vector<cv::Vec3f>> objectPoints = computeObjectPoints();
	std::vector<std::vector<cv::Vec2f>> imagePoints = computeImagePoints();

	m_RmsRProjError = cv::calibrateCamera(
		objectPoints, imagePoints, m_ImageSize, m_CameraMatrix, m_DistCoeffs,
		m_RVecs, m_TVecs, m_StdDeviationsIntrinsics, m_StdDeviationsExtrinsics,
		m_PerViewErrors);  // FIXME: study proper flags
}

void CameraCalibration::loadChessboardPatterns(const char** viewsFNames,
											   const std::size_t viewsCount) {
	m_ChessboardPatternVec.resize(viewsCount);

	for (int i = 0; auto& chessboardPattern : m_ChessboardPatternVec)
		chessboardPattern.read(viewsFNames[i++]);

	m_ViewsFNames = viewsFNames;
}

std::vector<std::vector<cv::Vec3f>> CameraCalibration::computeObjectPoints()
	const {
	std::vector<std::vector<cv::Vec3f>> objectPoints(
		m_ChessboardPatternVec.size());

	const std::vector<cv::Vec3f> chessboard3DPoints =
		computeChessboard3DPoints();
	for (auto& elem : objectPoints) elem = chessboard3DPoints;

	return objectPoints;
}

std::vector<cv::Vec3f> CameraCalibration::computeChessboard3DPoints() const {
	std::vector<cv::Vec3f> chessboard3DPoints;
	chessboard3DPoints.reserve(m_ChessboardPatternSize.width *
							   m_ChessboardPatternSize.height);
	for (int i = 0; i < m_ChessboardPatternSize.height; i++)
		for (int j = 0; j < m_ChessboardPatternSize.width; j++) {
			float x = float(j) * m_ChessboardPatternSquareSize;
			float y = float(i) * m_ChessboardPatternSquareSize;
			float z = 0.0f;
			chessboard3DPoints.push_back(cv::Vec3f(x, y, z));
		}

	return chessboard3DPoints;
}

std::vector<std::vector<cv::Vec2f>> CameraCalibration::computeImagePoints()
	const {
	std::vector<std::vector<cv::Vec2f>> imagePoints(
		m_ChessboardPatternVec.size());

	for (int viewIdx = 0; auto& chessboardCorners : imagePoints)
		chessboardCorners =
			loadChessboardCorners(m_ChessboardPatternVec[viewIdx++]);

	return imagePoints;
}

std::vector<cv::Vec2f> CameraCalibration::loadChessboardCorners(
	const ChessboardPattern& chessboardPattern) const {
	std::vector<cv::Vec2f> chessboardCorners;

	const std::size_t chessboardCornersSize =
		m_ChessboardPatternSize.width * m_ChessboardPatternSize.height;
	chessboardCorners.resize(chessboardCornersSize);

	for (std::size_t i = 0; i < chessboardCornersSize; i++)
		chessboardCorners[i] = chessboardPattern.getCorner(i);

	return chessboardCorners;
}

void CameraCalibration::exportCalibration(const char* prefix) const {
	exportIntrinsics(prefix);
	exportExtrinsics(prefix);
	exportErrorStatistics(prefix);
}

void CameraCalibration::exportIntrinsics(const char* prefix) const {
	const std::string outputFName = std::string(prefix) + "_intrinsics.txt";

	std::fstream ofs(outputFName, std::fstream::out);
	ofs << PRECISION();

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			ofs << m_CameraMatrix.at<double>(i, j);
			ofs << ((j == 2) ? '\n' : ' ');
		}
	}

	for (int i = 0; i < m_DistCoeffs.size().width; ++i) {
		ofs << m_DistCoeffs.at<double>(0, i);
		ofs << ((i == m_DistCoeffs.size().width - 1) ? '\n' : ' ');
	}

	ofs << m_ImageSize.width << " " << m_ImageSize.height << std::endl;

	ofs.close();
}

void CameraCalibration::exportExtrinsics(const char* prefix) const {
	const std::string outputFName = std::string(prefix) + "_extrinsics.txt";

	std::fstream ofs(outputFName, std::fstream::out);
	ofs << PRECISION();

	ofs << m_ChessboardPatternVec.size() << std::endl;

	for (std::size_t viewIdx = 0; viewIdx < m_ChessboardPatternVec.size();
		 ++viewIdx) {
		ofs << m_ViewsFNames[viewIdx] << " ";
		for (int j = 0; j < 3; ++j)
			ofs << m_RVecs.ptr<double>(viewIdx)[j] << " ";
		for (int j = 0; j < 3; ++j)
			ofs << m_TVecs.ptr<double>(viewIdx)[j] << " ";
		ofs << std::endl;
	}

	ofs.close();
}

void CameraCalibration::exportErrorStatistics(const char* prefix) const {
	exportIntrinsicsErrStats(prefix);
	exportExtrinsicsErrStats(prefix);
}

void CameraCalibration::exportIntrinsicsErrStats(const char* prefix) const {
	const std::string outputFName =
		std::string(prefix) + "_intrinsics_stats.txt";

	std::fstream ofs(outputFName, std::fstream::out);
	ofs << PRECISION();

	cv::Mat cameraMatrixStd(3, 3, CV_64FC1);
	cameraMatrixStd.setTo(0.0);
	cameraMatrixStd.at<double>(0, 0) =
		m_StdDeviationsIntrinsics.at<double>(0, 0);
	cameraMatrixStd.at<double>(1, 1) =
		m_StdDeviationsIntrinsics.at<double>(1, 0);
	cameraMatrixStd.at<double>(0, 2) =
		m_StdDeviationsIntrinsics.at<double>(2, 0);
	cameraMatrixStd.at<double>(1, 2) =
		m_StdDeviationsIntrinsics.at<double>(3, 0);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			ofs << cameraMatrixStd.at<double>(i, j);
			ofs << ((j == 2) ? '\n' : ' ');
		}
	}

	for (int i = 0; i < m_DistCoeffs.size().width; ++i) {
		ofs << m_StdDeviationsIntrinsics.at<double>(i + 4, 0);
		ofs << ((i == m_DistCoeffs.size().width - 1) ? '\n' : ' ');
	}

	ofs.close();
}

void CameraCalibration::exportExtrinsicsErrStats(const char* prefix) const {
	const std::string outputFName =
		std::string(prefix) + "_extrinsics_stats.txt";

	std::fstream ofs(outputFName, std::fstream::out);
	ofs << PRECISION();

	ofs << "#viewCount totalReprojectionError" << std::endl;
	ofs << m_ChessboardPatternVec.size() << " " << m_RmsRProjError << std::endl;

	ofs << "#imageName perViewError stdRVec stdTVec" << std::endl;

	std::size_t count = 0;
	for (std::size_t viewIdx = 0; viewIdx < m_ChessboardPatternVec.size();
		 ++viewIdx) {
		ofs << m_ViewsFNames[viewIdx] << " ";
		ofs << m_PerViewErrors.at<double>(viewIdx, 0) << " ";
		for (int j = 0; j < 6; ++j)
			ofs << m_StdDeviationsExtrinsics.at<double>(count++, 0) << " ";
		ofs << std::endl;
	}

	ofs.close();
}

}  // namespace camera_calibration
