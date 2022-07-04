#include <camera_calibration/chessboard_pattern.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

#define PRECISION() \
	std::fixed << std::setprecision(std::numeric_limits<float>::max_digits10)

namespace camera_calibration {

void ChessboardPattern::create(const char* imageFName, const cv::Size& size) {
	readImage(imageFName);
	m_Size = size;
	findChessboardCorners();
}

void ChessboardPattern::readImage(const char* imageFName) {
	m_Image = cv::imread(imageFName, cv::IMREAD_COLOR);
}

void ChessboardPattern::findChessboardCorners() {
	m_CornersFound = cv::findChessboardCornersSB(m_Image, m_Size, m_Corners,
												 cv::CALIB_CB_EXHAUSTIVE);

	cv::Mat gray = convertToGray(m_Image);
	m_CornersFound &= cv::find4QuadCornerSubpix(gray, m_Corners, m_Size);
}

cv::Mat ChessboardPattern::convertToGray(const cv::Mat& image) const {
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	return gray;
}

void ChessboardPattern::write(const char* outputFName) const {
	std::fstream ofs(outputFName, std::fstream::out);

	ofs << PRECISION();
	ofs << "#width height sharpness" << std::endl;
	ofs << m_Size.width << " " << m_Size.height << " ";
	ofs << cv::estimateChessboardSharpness(m_Image, m_Size, m_Corners)[0]
		<< std::endl;

	const int cornersCount = m_Corners.size().height;
	for (int i = 0; i < cornersCount; ++i)
		ofs << m_Corners.ptr<float>(i)[0] << " " << m_Corners.ptr<float>(i)[1]
			<< std::endl;

	ofs.close();
}

void ChessboardPattern::writeImage(const char* outputFName) const {
	if (m_Image.data == NULL) return;

	cv::Mat image;
	m_Image.copyTo(image);

	cv::drawChessboardCorners(image, m_Size, m_Corners, true);

	cv::imwrite(outputFName, image);
}

void ChessboardPattern::read(const char* fName) {
	clear();

	std::fstream ifs(fName, std::fstream::in);

	ifs.ignore(1024, '\n');
	ifs >> m_Size.width >> m_Size.height;
	ifs.ignore(1024, '\n');

	const int cornersCount = m_Size.width * m_Size.height;
	m_Corners.create(cornersCount, 1, CV_32FC2);
	for (int i = 0; i < cornersCount; ++i) {
		ifs >> m_Corners.ptr<float>(i)[0] >> m_Corners.ptr<float>(i)[1];
	}

	ifs.close();

	m_CornersFound = true;
}

void ChessboardPattern::clear() {
	m_Image.release();
	m_Size = cv::Size(0, 0);
	m_Corners.release();
	m_CornersFound = false;
}

cv::Vec2f ChessboardPattern::getCorner(const std::size_t idx) const {
	return cv::Vec2f(m_Corners.ptr<float>(idx)[0],
					 m_Corners.ptr<float>(idx)[1]);
}

}  // namespace camera_calibration
